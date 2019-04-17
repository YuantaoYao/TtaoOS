#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"

void spurious_irq(int irq);

PUBLIC void Init8259A(){
	for(int i=0;i<NR_IRQ;i++){
		irq_table[i] = spurious_irq;
	}
	out_port(INT_M_CTL, 0x11);
	out_port(INT_S_CTL, 0x11);
	out_port(INT_M_CTLMASK, INT_VECTOR_IRQ0);
	out_port(INT_S_CTLMASK, INT_VECTOR_IRQ8);
	out_port(INT_M_CTLMASK, 0x4);
	out_port(INT_S_CTLMASK, 0x2);
	out_port(INT_M_CTLMASK, 0x1);
	out_port(INT_S_CTLMASK, 0x1);
	out_port(INT_M_CTLMASK, 0xFF);
	out_port(INT_S_CTLMASK, 0xFF);
}

// 输出中断序号
PUBLIC void spurious_irq(int irq){
	disp_str("spurious_irq: ");
	disp_int(irq);
	disp_str("\n");
}

PUBLIC void put_irq_handler(int irq, irq_handler handler){
	disable_irq(irq);
	irq_table[irq] = handler;
}

PUBLIC void disable_irq(int irq){ //关闭制定的中断
	
	u8 ocw1 = 1 << irq;
	u8 temp = 0;
	if(irq < 8){
		temp = in_port(INT_M_CTLMASK);
		ocw1 = ocw1 | temp;
		out_port(INT_M_CTLMASK, ocw1);
	}else{
		temp = in_port(INT_S_CTLMASK);
		ocw1 = ocw1 | temp;
		out_port(INT_S_CTLMASK, ocw1);
	}
}

PUBLIC void enable_irq(int irq){ //打开制定的中断
	
	u8 ocw1 = 1 << irq;
	u8 temp = 0;
	if(irq < 8){
		temp = in_port(INT_M_CTLMASK);
		ocw1 = (u8)~ocw1 & temp;
		out_port(INT_M_CTLMASK, ocw1);
	}else{
		temp = in_port(INT_S_CTLMASK);
		ocw1 = (u8)~ocw1 & temp;
		out_port(INT_S_CTLMASK, ocw1);
	}
}