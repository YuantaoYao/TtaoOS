#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"

PRIVATE void init();

PUBLIC void cstart(){
	memcpy((void*)&gdt,
	(void*)(*((u32*)(&gdt_ptr[2]))),
	*((u16*)(&gdt_ptr[0])) + 1
	);
	
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base = (u32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base = (u32)&gdt;
	
	/* idt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sidt/lidt 的参数。*/
	u16* p_idt_limit = (u16*)(&idt_ptr[0]);
	u32* p_idt_base  = (u32*)(&idt_ptr[2]);
	*p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
	*p_idt_base  = (u32)&idt;
	
	init_protect();
	
	/* disp_str("end;"); */

}
