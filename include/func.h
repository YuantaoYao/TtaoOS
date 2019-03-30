#ifndef	_TTAO_STRING_H_
#define _TTAO_STRING_H_

PUBLIC void* memcpy(void* pDst, void* pSrc, int iSize);
PUBLIC void memset(void* p_dst, char ch, int size);

PUBLIC void  disp_str(char * pszInfo);

PUBLIC int Addition(int a, int b);
PUBLIC char * itoa(char * str, int num);
PUBLIC void exception_handler(int vec_no,int err_code,int eip,int cs,int eflags);
PUBLIC void disp_int(int n);
PUBLIC void disp_color_str(char * info, int color);

PUBLIC void init_protect();
PUBLIC void Init8259A();

PUBLIC void out_port(u16 port, u8 value);

PUBLIC void kernel_main();

PUBLIC void sleep(int time);

PUBLIC void restart();

PUBLIC void init_idt_desc(unsigned char vector, u8 desc_type, int_handler handler, unsigned char privilege);

PUBLIC u32 seg2phys(u16 seg);	

#endif