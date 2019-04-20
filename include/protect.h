#ifndef	_TTAO_PROTECT_H_
#define _TTAO_PROTECT_H_
#undef	EXTERN
#define	EXTERN

typedef struct s_descriptor{
	u16	limit_low;     //段界限  	（2字段）
	u16 base_low;      //段基址 1	（2字段）
	u8	base_mid;		//段基址 2	（1字段）
	u8	attr1;			// 属性 1
	u8	limit_high_attr2; //段界限 2 + 属性 2
	u8	base_high;		//段基址 3
}DESCRIPTOR;

typedef struct s_gate{
	u16 offset_low;
	u16 selector;
	u8	dcount;/* 该字段只在调用门描述符中有效。如果在利用
				   调用门调用子程序时引起特权级的转换和堆栈
				   的改变，需要将外层堆栈中的参数复制到内层
				   堆栈。该双字计数字段就是用于说明这种情况
				   发生时，要复制的双字参数的数量。*/
	u8	attr;
	u16	offset_high;
}GATE;


EXTERN int k_reenter;

EXTERN int disp_pos;

EXTERN	u8	gdt_ptr[6];
EXTERN	DESCRIPTOR	gdt[GDT_SIZE];
EXTERN	u8	idt_ptr[6];
EXTERN	GATE	idt[IDT_SIZE];

#endif
