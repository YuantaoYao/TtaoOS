#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"

PRIVATE void init();

PUBLIC void cstart(){
	disp_str("------------------start cstart--------------------\n");
	memcpy(&gdt,
	(void*)(*((u32*)(&gdt_ptr[2]))),
	*((u16*)(&gdt_ptr[0])) + 1
	);
	disp_str("a\n");
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	disp_str("b\n");
	u32* p_gdt_base = (u32*)(&gdt_ptr[2]);
	disp_str("c\n");
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	disp_str("d\n");
	*p_gdt_base  = (u32)&gdt;
	disp_str("e\n");
	
	/* idt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sidt/lidt 的参数。*/
	u16* p_idt_limit = (u16*)(&idt_ptr[0]);
	u32* p_idt_base  = (u32*)(&idt_ptr[2]);
	*p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
	*p_idt_base  = (u32)&idt;
	disp_str("f\n");
	init_protect();
	disp_str("------------------end cstart--------------------\n");
}
