#ifndef	_TTAO_CONST_H_
#define _TTAO_CONST_H_

#define	PUBLIC
#define	PRIVATE	static

/*GDT和IDT中断描述的个数 */
#define	GDT_SIZE	128
#define	IDT_SIZE	256

#define LDT_SIZE	2  /* 每个进程中局部描述符的个数 */

#define NR_IRQ 		16 /* 定义中断数量 两片A8659A */
#define	NR_TASKS	3  /* 定义允许的最大进程数 */

#define STACK_SIZE_TESTA 0x8000
#define STACK_SIZE_TESTB 0x8000	
#define STACK_SIZE_TESTC 0x8000	

#define STACK_SIZE_TOTAL	STACK_SIZE_TESTA + STACK_SIZE_TESTB + STACK_SIZE_TESTC

/*权限 */
#define	PRIVILEGE_KRNL	0 /* */
#define	PRIVILEGE_TASK	1 /* */
#define PRIVILEGE_USER	3 /* */

/*8259A */
#define	INT_M_CTL		0x20 /*1.端口0x20：8259A主片的ICW1，OCW2以及OCW3。 */
#define	INT_M_CTLMASK	0x21 /*2.端口0x21：8259A主片的ICW2 ~ ICW4以及OCW1。 */
#define	INT_S_CTL		0xA0 /*3.端口0xa0：8259A从片的ICW1，OCW2以及OCW3。 */
#define	INT_S_CTLMASK	0xA1 /*4.端口0xa1：8259A从片的ICW2 ～ ICW4以及OCW1 */

#define	INT_VECTOR_IRQ0	0x20
#define	INT_VECTOR_IRQ8	0x28

/* GDT */
/* 描述符索引 */
#define	INDEX_DUMMY		0	// ┓
#define	INDEX_FLAT_C	1	// ┣ LOADER 里面已经确定了的.
#define	INDEX_FLAT_RW	2	// ┃
#define	INDEX_VIDEO		3	// ┛
#define	INDEX_TSS		4
#define	INDEX_LDT_FIRST	5	

/* 选择子 */
#define	SELECTOR_DUMMY		   0		// ┓
#define	SELECTOR_FLAT_C		0x08		// ┣ LOADER 里面已经确定了的.
#define	SELECTOR_FLAT_RW	0x10		// ┃
#define	SELECTOR_VIDEO		(0x18+3)	// ┛<-- RPL=3
#define	SELECTOR_TSS		0x20		/* TSS */
#define SELECTOR_LDT_FIRST	0x28


#define	SELECTOR_KERNEL_CS	SELECTOR_FLAT_C
#define	SELECTOR_KERNEL_DS	SELECTOR_FLAT_RW
#define SELECTOR_KERNEL_GS	SELECTOR_VIDEO	

/* 选择子类型值说明 */
#define	SA_RPL_MASK	0xFFFC
#define	SA_RPL0	0
#define	SA_RPL1	1
#define	SA_RPL2	2
#define	SA_RPL3	3

/* RPL */
#define	RPL_KRNL	SA_RPL0
#define	RPL_TASK	SA_RPL1
#define	RPL_USER	SA_RPL3

#define	SA_TI_MASK	0xFFFB
#define	SA_TIG	0
#define	SA_TIL	4

/* 描述符类型值说明 */
#define	DA_32			0x4000	/* 32 位段				*/
#define	DA_LIMIT_4K		0x8000	/* 段界限粒度为 4K 字节			*/
#define	DA_DPL0			0x00	/* DPL = 0				*/
#define	DA_DPL1			0x20	/* DPL = 1				*/
#define	DA_DPL2			0x40	/* DPL = 2				*/
#define	DA_DPL3			0x60	/* DPL = 3				*/
/* 存储段描述符类型值说明 */
#define	DA_DR			0x90	/* 存在的只读数据段类型值		*/
#define	DA_DRW			0x92	/* 存在的可读写数据段属性值		*/
#define	DA_DRWA			0x93	/* 存在的已访问可读写数据段类型值	*/
#define	DA_C			0x98	/* 存在的只执行代码段属性值		*/
#define	DA_CR			0x9A	/* 存在的可执行可读代码段属性值		*/
#define	DA_CCO			0x9C	/* 存在的只执行一致代码段属性值		*/
#define	DA_CCOR			0x9E	/* 存在的可执行可读一致代码段属性值	*/
/* 系统段描述符类型值说明 */
#define	DA_LDT			0x82	/* 局部描述符表段类型值			*/
#define	DA_TaskGate		0x85	/* 任务门类型值				*/
#define	DA_386TSS		0x89	/* 可用 386 任务状态段类型值		*/
#define	DA_386CGate		0x8C	/* 386 调用门类型值			*/
#define	DA_386IGate		0x8E	/* 386 中断门类型值			*/
#define	DA_386TGate		0x8F	/* 386 陷阱门类型值			*/

/* 中断向量 */
#define	INT_VECTOR_DIVIDE		0x0
#define	INT_VECTOR_DEBUG		0x1
#define	INT_VECTOR_NMI			0x2
#define	INT_VECTOR_BREAKPOINT		0x3
#define	INT_VECTOR_OVERFLOW		0x4
#define	INT_VECTOR_BOUNDS		0x5
#define	INT_VECTOR_INVAL_OP		0x6
#define	INT_VECTOR_COPROC_NOT		0x7
#define	INT_VECTOR_DOUBLE_FAULT		0x8
#define	INT_VECTOR_COPROC_SEG		0x9
#define	INT_VECTOR_INVAL_TSS		0xA
#define	INT_VECTOR_SEG_NOT		0xB
#define	INT_VECTOR_STACK_FAULT		0xC
#define	INT_VECTOR_PROTECTION		0xD
#define	INT_VECTOR_PAGE_FAULT		0xE
#define	INT_VECTOR_COPROC_ERR		0x10

/* 中断向量 */
#define	INT_VECTOR_IRQ0			0x20
#define	INT_VECTOR_IRQ8			0x28

/* 系统调用 */
#define	INT_VECTOR_SYS_CALL		0x90

/* 8253编程端口 */
#define TIMER0	0x40
#define TIMER1	0x41
#define	TIMER2	0x42
#define TIMER_MODE	0x43

#define TIMER_FREQ	1193182L
#define HZ	100

#define vir2phys(seg_base, vir) (u32)(((u32)seg_base) + (u32)(vir))

#define NR_SYS_CALL     1

#define INT_TTAO_TEXT	0xA0
#endif