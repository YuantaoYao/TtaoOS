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

void Clean();
void Init_Clock();

PUBLIC void kernel_main(){
	Clean();

	Init_Clock();
	Init_Keyboard();
	Init_Tty();
	Init_Hd();
	
	for(int i=0;i<NR_ALL_TASKS_PROC;i++){
		proc_table[i].ticks = proc_table[i].priority = 5;
		proc_table[i].nr_tty = 0;
	}
	
	k_reenter = 0;
	ticks = 0;
	
	TASK *  p_task;
	u8		privilege;
	u8		rpl;
	int 	eflags;
	PROCESS * p_proc		 = proc_table;
	char * p_task_stack 	 = task_stack + STACK_SIZE_TOTAL;
	u8 selector_ldt 		 = SELECTOR_LDT_FIRST;
	for(int i=0;i<NR_ALL_TASKS_PROC;i++){
		if(i<NR_SYS_TASKS_PROC){
			p_task = sys_task_table + i;
			privilege = PRIVILEGE_TASK;
			rpl = RPL_TASK;
			eflags = 0x1202;
		}else{
			p_task = user_task_table + (i-NR_SYS_TASKS_PROC);
			privilege = PRIVILEGE_USER;
			rpl = RPL_USER;
			eflags = 0x202;
		}
		strcpy(p_proc->name, p_task->name);
		p_proc->pid = i;
		p_proc->ldt_sel = selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | privilege << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | privilege << 5;
		p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | rpl;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | rpl;
		
		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		
		p_proc->regs.eflags = eflags;
		
		p_proc->p_flags = 0;
		p_proc->p_msg = 0;
		p_proc->p_recvfrom = NO_TASK;
		p_proc->p_sendto = NO_TASK;
		p_proc->has_int_msg = 0;
		p_proc->first_sending = 0;
		p_proc->next_sending = 0;
		
		p_task_stack -= p_task->stacksize;
		p_proc++;
		selector_ldt += 1 << 3;
	}
	p_proc_ready = proc_table;
	restart(); 
}

void Clean(){
	disp_pos=10;
	for(int i=0;i<80*25;i++){
		disp_str(" ");
	}
	disp_pos=0;
}
void TestA(){
	dev_open(0);
	while(1){
		/* printf("<Ticks : %d>.", get_ticks()); */
		milli_dalay(1000);
	}
}

void TestB(){
	while(1){
		//dev_open(2);
		/* printf("<Ticks : %d>.", get_ticks()); */
		milli_dalay(1000);
	}
}

void TestC(){ 
	while(1){
		
	}
}

