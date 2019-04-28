#include "stdio.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "func.h"

PUBLIC void clock_handler(int irq){
	ticks++;
	
	p_proc_ready->ticks--;
	
	if(k_reenter != 0){
		return ;
	}
	
	if(p_proc_ready->ticks > 0){
		return ;
	}
	
	int greatest_ticks = 0;
	PROCESS *p;
	while(!greatest_ticks){
		for(p=proc_table;p<(proc_table + NR_ALL_TASKS_PROC);p++){
			if(p->ticks > greatest_ticks){
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}
		if(!greatest_ticks){
			for(p=proc_table;p<(proc_table + NR_ALL_TASKS_PROC);p++){
				p->ticks = p->priority;
			}
		}
	}
	p_proc_ready->nr_tty = nr_current_console;
}