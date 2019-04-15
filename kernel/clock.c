#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"

PUBLIC void clock_handler(int irq){
	p_proc_ready++;
	
	disp_int(irq);
	if(p_proc_ready >= (proc_table + NR_TASKS)){
		p_proc_ready = proc_table;
	}
}