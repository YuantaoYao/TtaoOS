#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"

void divide_error();
void divide_error();
void single_step_exception();
void nmi();
void breakpoint_exception();
void overflow();
void bounds_check();
void inval_opcode();
void copr_not_available();
void double_fault();
void copr_seg_overrun();
void inval_tss();
void segment_not_present();
void stack_exception();
void general_protection();
void page_fault();
void copr_error();

PRIVATE void init_idt_desc(unsigned char vector, u8 desc_type,
			  int_handler handler, unsigned char privilege);
			  
/*======================================================================*
                            init_prot
 *======================================================================*/
PUBLIC void init_protect(){
	init_idt_desc(INT_VECTOR_DIVIDE, DA_386IGate, divide_error, PRIVILEGE_KRNL);
}
/*======================================================================*
                             init_idt_desc
 *----------------------------------------------------------------------*
 初始化 386 中断门
 *======================================================================*/
PRIVATE void init_idt_desc(unsigned char vector, u8 desc_type,
			  int_handler handler, unsigned char privilege){
	GATE * p_gate = &idt[vector];
	u32 base = (u32)handler;
	disp_color_str("into interrupt", 0x74);
	 
 }
 /*======================================================================*
                            exception_handler
 *----------------------------------------------------------------------*
 异常处理
 *======================================================================*/
PUBLIC void exception_handler(int vec_no,int err_code,int eip,int cs,int eflags){

	int i;
	int text_color = 0x74;
	
	char * err_msg[] = {"#DE Divide Error",
			    "#DB RESERVED",
			    "--  NMI Interrupt",
			    "#BP Breakpoint",
			    "#OF Overflow",
			    "#BR BOUND Range Exceeded",
			    "#UD Invalid Opcode (Undefined Opcode)",
			    "#NM Device Not Available (No Math Coprocessor)",
			    "#DF Double Fault",
			    "    Coprocessor Segment Overrun (reserved)",
			    "#TS Invalid TSS",
			    "#NP Segment Not Present",
			    "#SS Stack-Segment Fault",
			    "#GP General Protection",
			    "#PF Page Fault",
			    "--  (Intel reserved. Do not use.)",
			    "#MF x87 FPU Floating-Point Error (Math Fault)",
			    "#AC Alignment Check",
			    "#MC Machine Check",
			    "#XF SIMD Floating-Point Exception"
	};
	/* 通过打印空格的方式清空屏幕的前五行，并把 disp_pos 清零 */
	disp_pos = 0;
	for(i=0;i<80*5;i++){
		disp_str("  ");
	}
	
	disp_pos = 0;

	disp_color_str("Exception! --> ", text_color);
	disp_color_str(err_msg[vec_no], text_color);
	disp_color_str("\n\n", text_color);
	disp_color_str("EFLAGS:", text_color);
	disp_int(eflags);
	disp_color_str("\n\nCS:", text_color);
	disp_int(cs);
	disp_color_str("\n\nEIP:", text_color);
	disp_int(eip);
	
	if(err_code != 0xFFFFFFFF){
		disp_color_str("\n\nError code:", text_color);
		disp_int(err_code);
	}
}