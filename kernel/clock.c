#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"
#include "global.h"

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
		for(p=proc_table;p<(proc_table + NR_TASKS);p++){
			if(p->ticks > greatest_ticks){
				greatest_ticks = p->ticks;
				p_proc_ready = p;
			}
		}
		if(!greatest_ticks){
			for(p=proc_table;p<(proc_table + NR_TASKS);p++){
				p->ticks = p->priority;
			}
		}
	}
}