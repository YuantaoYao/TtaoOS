#include "const.h"
#include "type.h"
#include "func.h"
#include "protect.h"
#include "proc.h"

void TestA();

PUBLIC void kernel_main(){
	disp_str("------------------start kernel main--------------------\n");
	disp_str("f");
	PROCESS * p_proc = proc_table;
	disp_str("g");
	p_proc->ldt_sel = SELECTOR_LDT_FIRST;
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
	p_proc->regs.eip = (u32)TestA;
	p_proc->regs.esp = (u32)task_stack + STACK_SIZE_TOTAL;
	p_proc->regs.eflags = 0x1202;
	p_proc_ready = proc_table;
	disp_str("h");
	restart(); 
	
}

void TestA(){
	int i = 0;
	while(1){
		disp_str("A");
		disp_str(".");
		disp_str("\n");
		sleep(1 * 1000);
	}
}