#ifndef	_TTAO_STRING_H_
#define _TTAO_STRING_H_


PUBLIC int Addition(int a, int b);

//关闭开启中断
PUBLIC void disable_irq(int irq);
PUBLIC void enable_irq(int irq);

PUBLIC void exception_handler(int vec_no,int err_code,int eip,int cs,int eflags);

PUBLIC void init_protect();
PUBLIC void Init8259A();
PUBLIC void Init8253();

//读写端口
PUBLIC void out_port(u16 port, u8 value);
PUBLIC u8 in_port(u16 port);

PUBLIC void kernel_main();

PUBLIC void sleep(int time);

PUBLIC void restart();
//PUBLIC void restart_reenter();

//初始化中断描述符
PUBLIC void init_idt_desc(unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege);

PUBLIC void put_irq_handler(int irq, irq_handler handler);

PUBLIC u32 seg2phys(u16 seg);	

//中断处理程序
PUBLIC void clock_handler(int irq);
PUBLIC void keyboard_handler(int irq);
PUBLIC void disable_int();
PUBLIC void enable_int();

PUBLIC void inform_int(int task_nr);//通知进程发生了中断

//初始化中断
PUBLIC void Init_Keyboard();
PUBLIC void Init_Clock();
PUBLIC void Init_Hd();

//syscall
PUBLIC void task_tty();
PUBLIC void task_sys();
PUBLIC void task_hd();

PUBLIC void TestA();
PUBLIC void TestB();
PUBLIC void TestC();

PUBLIC void sys_call();

PUBLIC void milli_dalay(int milli_sec);

#define proc2pid(x) (x - proc_table)

#include "usercall.h"
#include "hd.h"

#endif