#include "stdio.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "tty.h"
#include "func.h"
#include "proc.h"
#include "global.h"

PRIVATE void hd_handler();

PUBLIC void Init_Hd(){
	/* 从物理地址0x475处获取系统内硬盘数量 */
	u8 *pNrDrives = (u8*)(0x475);
	//printl("NrDrives: %d \n", *pNrDrives);
	//assert(*pNrDrives);
	
	put_irq_handler(NUM_AT_WINI_IRQ, hd_handler); //添加中断函数到 硬盘中断
	enable_irq(NUM_CASCADE_IRQ); //打开第二块8259A中端芯片
	enable_irq(NUM_AT_WINI_IRQ); //打开硬盘中断
}

PUBLIC void hd_identify(int drive){
	printf("hd_identify %d..", drive);
}

/**
 * <Ring 0> Interrupt handler 
 *
 *
 *
 */
PRIVATE void hd_handler(){
	u8 hd_status = in_port(REG_STATUS);
	
	inform_int(TASK_HD);
}