#include "const.h"
#include "type.h"
#include "func.h"

PUBLIC void Init8259A(){
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