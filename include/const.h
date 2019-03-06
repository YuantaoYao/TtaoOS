#ifndef	_TTAO_CONST_H_
#define _TTAO_CONST_H_

#define	PUBLIC
#define	PRIVATE	static

/*GDT和IDT中断描述的个数 */
#define	GDT_SIZE	128
#define	IDT_SIZE	256

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

#endif