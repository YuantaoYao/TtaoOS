#include "const.h"
#include "type.h"
#include "func.h"
#include "protect.h"
#include "proc.h"
#include "global.h"

void initIRQ();

PUBLIC void kernel_main(){
	initIRQ();
	
	k_reenter = 0;
	ticks = 0;
	
	PROCESS * p_proc		 = proc_table;
	TASK * p_task 			 = task_table;
	char * p_task_stack 	 = task_stack + STACK_SIZE_TOTAL;
	u8 selector_ldt 		 = SELECTOR_LDT_FIRST;
	
	for(int i=0;i<NR_TASKS;i++){
		strcpy(p_proc->name, p_task->name);
		p_proc->pid = i;
		p_proc->ldt_sel = selector_ldt;
		memcpy(&p_proc->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
		memcpy(&p_proc->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(DESCRIPTOR));
		p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
		p_proc->regs.cs = ((8 * 0) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ds = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.es = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.fs = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.ss = ((8 * 1) & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		p_proc->regs.gs = (SELECTOR_KERNEL_GS & SA_RPL_MASK) | RPL_TASK;
		
		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		
		p_proc->regs.eflags = 0x1202;
		
		p_task_stack -= p_task->stacksize;
		p_proc++;
		p_task++;
		selector_ldt += 1 << 3;
	}
	p_proc_ready = proc_table;
	restart(); 
	
}

void initIRQ(){
	int CLOCK_IRQ = 0;
	put_irq_handler(CLOCK_IRQ, clock_handler);
	enable_irq(CLOCK_IRQ);
}

void TestA(){

	while(1){
		milli_dalay(1000);
		int t = get_ticks();
	 	disp_int(t);
		disp_str("\n");  
	}
}

void TestB(){

	while(1){
		sleep(10);
	}
}

void TestC(){
	 
	while(1){
		sleep(10);
	}
}
