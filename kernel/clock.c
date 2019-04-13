#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"

PUBLIC void clock_handler(int irq){
	p_proc_ready++;
	
	/* disp_str("*"); */
	/* disp_int(k_reenter);
	if(k_reenter == 0){
		disp_str("!");
	}else{
		disp_str("?");
	} */
	if(p_proc_ready >= (proc_table + NR_TASKS)){
		p_proc_ready = proc_table;
	}
}