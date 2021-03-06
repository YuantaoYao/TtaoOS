#ifndef	_TTAO_CONST_H_
#define _TTAO_CONST_H_


/*GDT和IDT中断描述的个数 */
#define	GDT_SIZE	128
#define	IDT_SIZE	256

#define LDT_SIZE	2  /* 每个进程中局部描述符的个数 */
/* descrpitor indices in LDT */
#define INDEX_LDT_C		0
#define INDEX_LDT_RW	1

#define NR_IRQ 		16 /* 定义中断数量 两片A8659A */

/* 中断码 */
#define NUM_CLOCK_IRQ	0
#define NUM_KEYBOAED_IRQ 1
#define NUM_CASCADE_IRQ 2 /* 开启改中断是为了使用第二块8259A 中断控制芯片*/
#define NUM_TTY2_IRQ	3 /* tty2 */
#define NUM_TTY3_IRQ	4 /* tty1 */
#define NUM_XT_WINI_IRQ	5 /* XT Whechester*/
#define NUM_FLOPPY_IRQ	6 /* 软驱 */
#define NUM_AT_WINI_IRQ	14 /* AT Winchester*/

#define	NR_SYS_TASKS_PROC	3  /* 定义允许的最大系统进程数 */
#define	NR_USER_TASKS_PROC	3  /* 定义允许的最大用户进程数 */

#define NR_ALL_TASKS_PROC	NR_SYS_TASKS_PROC + NR_USER_TASKS_PROC

#define STACK_SIZE_TTY_TASK	0x8000
#define STACK_SIZE_SYS_TASK 0x8000
#define STACK_SIZE_HD_TASK  0x8000

#define STACK_SIZE_TESTA 0x8000
#define STACK_SIZE_TESTB 0x8000	
#define STACK_SIZE_TESTC 0x8000	

#define STACK_SIZE_TOTAL	STACK_SIZE_TTY_TASK + STACK_SIZE_SYS_TASK + STACK_SIZE_HD_TASK + STACK_SIZE_TESTA + STACK_SIZE_TESTB + STACK_SIZE_TESTC

#define INVALID_DRIVER	-20
#define INTERRUPT	-10

#define TASK_TTY	0
#define TASK_SYS	1
#define TASK_HD		2

#define TASK_TESTA	3
#define TASK_TESTB	4
#define TASK_TESTC	5

#define ANY (NR_ALL_TASKS_PROC + 10)
#define NO_TASK (NR_ALL_TASKS_PROC + 20)


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

/* 825芯片 */
#define TIMER_FREQ	1193182L
#define HZ	100

/* VGA */
#define CRTC_ADDR_REG	0x3D4
#define CRTC_DATA_REG	0x3D5
#define START_ADDR_H	0xC
#define START_ADDR_L	0xD
#define CURSOR_H		0xE
#define CURSOR_L		0xF
#define V_MEM_BASE		0xB8000 //显卡内存开始的位置
#define V_MEM_SIZE		0x8000

/* magic chars used by `printx' */
#define MAG_CH_PANIC	'\002'
#define MAG_CH_ASSERT	'\003'

/* Process msg status */
#define SENDING   	0x02
#define RECEIVING 	0x04

/* ipc */
#define SEND		1
#define RECEIVE		2
#define BOTH		3 /*BOTH = (SEND | RECEIVE)*/

/* TTY */
#define NR_CONSOLE	3

/* SCREEN */
#define SCREEN_WIDTH	80
#define SCR_UP			1
#define SCR_DN			-1
#define SCREEN_SIZE		(25 * 80)



/* Color */
/*
 * e.g. MAKE_COLOR(BLUE, RED)
 *      MAKE_COLOR(BLACK, RED) | BRIGHT
 *      MAKE_COLOR(BLACK, RED) | BRIGHT | FLASH
 */
#define BLACK   0x0     /* 0000 */
#define WHITE   0x7     /* 0111 */
#define RED     0x4     /* 0100 */
#define GREEN   0x2     /* 0010 */
#define BLUE    0x1     /* 0001 */
#define FLASH   0x80    /* 1000 0000 */
#define BRIGHT  0x08    /* 0000 1000 */
#define	MAKE_COLOR(x,y)	((x<<4) | y) /* MAKE_COLOR(Background,Foreground) */

#define vir2phys(seg_base, vir) (u32)(((u32)seg_base) + (u32)(vir))

#define NR_SYS_CALL    4

/**
*major device number （主设备号）
*
*/
#define NO_DEV			0
#define DEV_FLOPPY		1
#define	DEV_CDROM		2
#define DEV_HD			3
#define	DEV_CHAR_TTY	4
#define DEV_SCSI		5

/***定义设备号**/
#define MAJOR_SHIFT		8
#define MAKE_DEV(a, b)	((a << MAJOR_SHIFT) | b)
/**获取主次设备号*/
#define	MAJOR(x)		((x >> MAJOR_SHIFT) & 0xFF)
#define MINOR(x)		(x & 0xFF)

#define INT_TTAO_TEXT	0xA0
#endif