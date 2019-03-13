#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"

PRIVATE void init();

PUBLIC void cstart(){
	init();
	memcpy((void*)&gdt,
	(void*)(*((u32*)(&gdt_ptr[2]))),
	*((u16*)(&gdt_ptr[0])) + 1
	);
	
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base = (u32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base = (u32)&gdt;
	/* disp_pos = 0; */
	/* disp_str("\n"); */
	init_protect();
}
PRIVATE void init(){
	disp_pos = 0;
}