#ifndef	_TTAO_HD_H_
#define _TTAO_HD_H_

#define REG_STATUS	0x1F7 /* 0号硬盘状态寄存器（读时）、0号硬盘命令寄存器（写时） */



PUBLIC void hd_identify(int drive);


#endif