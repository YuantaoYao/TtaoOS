#include "stdio.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "func.h"
#include "proc.h"
#include "global.h"

PUBLIC void Init8253(){
	out_port(TIMER_MODE, 0x36);
	out_port(TIMER0, (u8) (TIMER_FREQ/HZ));
	out_port(TIMER0, (u8) (TIMER_FREQ/HZ >> 8)); 
}

PUBLIC void Init_Clock(){
	put_irq_handler(NUM_CLOCK_IRQ, clock_handler);
	enable_irq(NUM_CLOCK_IRQ);	
}


PUBLIC void milli_dalay(int milli_sec){
	int t = get_ticks();
	while(((get_ticks()-t)*(1000/HZ)) < milli_sec){};
}