#include "stdio.h"
#include "const.h"
#include "type.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "func.h"

PUBLIC int sys_write(char* buf, int len, PROCESS * p_proc);
PUBLIC int sys_get_ticks();

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

PUBLIC int sys_write(char* buf, int len, PROCESS* p_proc){
	tty_write(&tty_table[p_proc->nr_tty], buf, len);
	return 0;
} 