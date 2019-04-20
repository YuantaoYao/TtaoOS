#ifndef	_TTAO_STRING_H_
#define _TTAO_STRING_H_

//内存操作
PUBLIC void* memcpy(void* pDst, void* pSrc, int iSize);
PUBLIC void memset(void* p_dst, char ch, int size);
PUBLIC void * strcpy(char* p_dst, char* p_src);


PUBLIC int Addition(int a, int b);

//输出
PUBLIC void disp_str(char * pszInfo);
PUBLIC void disp_int(int n);
PUBLIC void disp_int_hex(int n);
PUBLIC void disp_color_str(char * info, int color);

PUBLIC char* itoa(int num, char* str,int radix);
PUBLIC void * exchange(char * from, char * to);

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

PUBLIC void TestA();

PUBLIC void TestB();

PUBLIC void TestC();

PUBLIC int sys_get_ticks();

PUBLIC void sys_call();

PUBLIC int  get_ticks();

PUBLIC void milli_dalay(int milli_sec);
#endif