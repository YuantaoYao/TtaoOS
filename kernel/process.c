#include "stdio.h"
#include "mem.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "func.h"
#include "syscall.h"

PUBLIC int sys_printx( PROCESS* p_proc, char* buf, int _unused2, int _unused1);
PUBLIC int sys_write(PROCESS * p_proc, char* buf, int len);
PUBLIC int sys_get_ticks();
PUBLIC int sys_send_rec(PROCESS * p_proc, int function, int src_dest, MESSAGE * msg);

PUBLIC void * va2la(int pid, void* va);
PUBLIC int ldt_seg_linear(PROCESS* p, int idx);
PUBLIC void reset_msg(MESSAGE *msg);
PRIVATE void block(PROCESS * p);
PRIVATE void unblock(PROCESS * p);
PRIVATE int msg_send(PROCESS * p_proc, int dest, MESSAGE * msg);
PRIVATE int msg_receive(PROCESS * p_proc, int src, MESSAGE * msg);

PUBLIC int sys_get_ticks(){
	return ticks;
}

PUBLIC void tty_write(TTY* p_tty, char* buf, int len){
	char* p = buf;
	int i = len;
	//disp_int(p_tty->p_console->v_mem_limit);
	while(i){
		//disp_int(p_tty->p_console->cursor);
		out_char(p_tty->p_console, *p++);
		i--;
	}
}

PUBLIC int sys_write( PROCESS* p_proc, char* buf, int len){
	tty_write(&tty_table[p_proc->nr_tty], buf, len);
	return 0;
} 

PUBLIC int sys_printx( PROCESS* p_proc, char* buf, int _unused2, int _unused1){
	const char * p;
	char ch;
	char reenter_err[] = "? k_reenter is incorrect for unknown reason";
	reenter_err[0] = MAG_CH_PANIC;
	if(k_reenter == 0){
		p = va2la(proc2pid(p_proc), buf);
	}else if(k_reenter >0){
		p = buf;
	}else{
		p = reenter_err;
	}

	if(*p == MAG_CH_PANIC || (*p == MAG_CH_ASSERT && p_proc_ready <= &proc_table[NR_SYS_TASKS_PROC])){//如果是系统内核级的异常 或 panic  系统完全停止运行
		disable_int();
		char *v = (char *)V_MEM_BASE;
		const char * q = p + 1;
		while(v < (char*)(V_MEM_BASE + V_MEM_SIZE)){
			*v++ = *q++;
			*v++ = RED_CHAR;
			if(!*q){
				while(((int)v - V_MEM_BASE) % (SCREEN_WIDTH * 16)){
					v++;
					*v++ = GRAY_CHAR;
				}
				q = p + 1;
			}
		}
		__asm__ __volatile__("hlt");
	}
	
	while((ch = *p++) != 0){
		if(ch == MAG_CH_PANIC || ch == MAG_CH_ASSERT){
			continue;
		}
		out_char(tty_table[p_proc->nr_tty].p_console, ch);
	}
	
}

PRIVATE void block(PROCESS * p){
	p->p_flags = 1;
	assert(p->p_flags);
}

PRIVATE void unblock(PROCESS * p){
	p->p_flags = 0;
	assert(p->p_flags == 0);
}
//判断消息的发送是不是安全
PRIVATE int deadlock(int src, int dest){
	PROCESS * p = proc_table + dest;
	while(1){
		if(p->p_flags & SENDING){
			if(p->p_sendto == src){
				p = proc_table + dest;
				printl("=_= %s", p->name);
				do{
					//assert(p->msg);
					p = proc_table + p->p_sendto;
					printl("-> %s", p->name);
				}while(p != proc_table + src);
				printl("=_=");
				
				return 1;
			}
			p = proc_table + p->p_sendto;
		}else{
			break;
		}
	}
	return 0;
}

PUBLIC int sys_send_rec(PROCESS * p_proc, int function, int src_dest, MESSAGE * msg){
	assert(k_reenter == 0);
	assert((src_dest >= 0 && src_dest < NR_ALL_TASKS_PROC) || src_dest == ANY || src_dest == INTERRUPT);
	
	int ret = 0;
	int caller = proc2pid(p_proc);
	
	MESSAGE* mla = (MESSAGE*)va2la(caller, msg);
	mla->source = caller;
	
	assert(mla->source != src_dest); //判断是不是发给自己的
	
	if(function == SEND){
		ret = msg_send(p_proc, src_dest, msg);
		if(ret != 0)
			return ret;
	}else if(function == RECEIVE){
		ret = msg_receive(p_proc, src_dest, msg);
		if(ret != 0)
			return ret;
	}else{
		panic("{sys_send_rec} invalid function %d (SEND: %d, RECEIVE: %d).",function, SEND, RECEIVE);
	}
	
	return 0;
}
//消息发送
PRIVATE int msg_send(PROCESS * p_proc, int dest, MESSAGE * msg){
	PROCESS* sender = p_proc;
	PROCESS* p_dest = proc_table + dest;
	
	assert(proc2pid(sender) != dest);
	
	if(deadlock(proc2pid(sender), dest)){
		panic(">>deadlock()<< %s, %s ", sender->name, p_dest->name);
	}
	
	if((p_dest->p_flags & RECEIVE) && (p_dest->p_recvfrom == proc2pid(sender) || p_dest->p_recvfrom == ANY)){
			assert(p_dest->p_msg);
			assert(msg);
			
			phys_copy(va2la(dest, p_dest->p_msg),
					  va2la(proc2pid(sender), msg),
					  sizeof(MESSAGE));
			p_dest->p_msg = 0;
			p_dest->p_flags &= ~RECEIVE;
			p_dest->p_recvfrom = NO_TASK;
			unblock(p_dest);
		}else{
			sender->p_flags |= SENDING;
			sender->p_sendto = dest;
			sender->p_msg = msg;
			
			PROCESS* p;
			if(p_dest->q_sending){
				p = p_dest->q_sending;
				while(p->next_sending)
					p = p->next_sending;
				p->next_sending = sender;
			}else{
				p_dest->q_sending = sender;
			}
			
			sender->next_sending = 0;
			
			block(sender);
		}

	return 0;
}
//消息接收
PRIVATE int msg_receive(PROCESS * p_proc, int src, MESSAGE * msg){
	PROCESS* receiver = p_proc;
	PROCESS* p_from = 0;
	PROCESS* prev = 0;
	int copyok = 0;
	
	assert(proc2pid(receiver) != src);

	if((receiver->has_int_msg) && ((src == ANY) || (src == INTERRUPT))){
		
		MESSAGE my_msg;
		reset_msg(&my_msg);
		
		my_msg.source = INTERRUPT;
		my_msg.type = HARD_INT;
		assert(msg);
		phys_copy(va2la(proc2pid(receiver), msg), &my_msg, sizeof(MESSAGE));
		
		receiver->has_int_msg = 0;
		
		return 0;
	}
	if(src == ANY){
		if(receiver->q_sending){
			p_from = receiver->q_sending;
			copyok = 1;
		}
	}else{
		p_from = &proc_table[src];
	}
	
	if(copyok){
		
	}else{

	}
	
	return 0;
}

PUBLIC void * va2la(int pid, void* va){
	PROCESS* p = &proc_table[pid];
	
	u32 seg_base = ldt_seg_linear(p, INDEX_LDT_RW);
	u32 la = seg_base + (u32)va;
	
		//disp_int(seg_base);
	if(pid < NR_SYS_TASKS_PROC + NR_USER_TASKS_PROC + NR_SYS_CALL){
		assert(la == (u32)va);
	}
	
	return (void*)la;
}

PUBLIC int ldt_seg_linear(PROCESS* p, int idx){
	DESCRIPTOR* d = &p->ldts[idx];
	return d->base_high << 24 | d->base_mid << 16 | d->base_low;
}

/* 格式化消息体 */
PUBLIC void reset_msg(MESSAGE *msg){
	memset(msg, 0, sizeof(MESSAGE));
}