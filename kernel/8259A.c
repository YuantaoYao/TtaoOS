#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"

void spurious_irq(int irq);

PUBLIC void Init8259A(){
	out_port(INT_M_CTL, 0x11);
	out_port(INT_S_CTL, 0x11);
	out_port(INT_M_CTLMASK, INT_VECTOR_IRQ0);
	out_port(INT_S_CTLMASK, INT_VECTOR_IRQ8);
	out_port(INT_M_CTLMASK, 0x4);
	out_port(INT_S_CTLMASK, 0x2);
	out_port(INT_M_CTLMASK, 0x1);
	out_port(INT_S_CTLMASK, 0x1);
	out_port(INT_M_CTLMASK, 0xFC);
	out_port(INT_S_CTLMASK, 0xFC);
	for(int i=0;i<NR_IRQ;i++){
		irq_table[i] = spurious_irq;
	}
}

// 输出中断序号
PUBLIC void spurious_irq(int irq){
	disp_str("spurious_irq: ");
	disp_int(irq);
	disp_str("\n");
}

PUBLIC void put_irq_handler(int irq, irq_handler handler){
	irq_table[irq] = handler;
}