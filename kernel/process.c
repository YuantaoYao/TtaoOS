#include "stdio.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "func.h"
#include "syscall.h"

PUBLIC int sys_write(PROCESS * p_proc, char* buf, int len);
PUBLIC int sys_get_ticks();
PUBLIC int sys_send_rec(PROCESS * p_proc, int function, int src_desc, MESSAGE * msg);

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

PUBLIC int sys_send_rec(PROCESS * p_proc, int function, int src_desc, MESSAGE * msg){
	printf("%d", msg->type);
	return 0;
}