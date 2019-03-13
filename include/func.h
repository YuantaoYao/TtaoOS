#ifndef	_TTAO_STRING_H_
#define _TTAO_STRING_H_

PUBLIC void* memcpy(void* pDst, void* pSrc, int iSize);
PUBLIC void  disp_str(char * pszInfo);

PUBLIC int Addition(int a, int b);
PUBLIC void disp_int(int n);
PUBLIC char * itoa(char * str, int num);
PUBLIC void disp_color_str(char * info, int color);
PUBLIC void exception_handler(int vec_no,int err_code,int eip,int cs,int eflags);

PUBLIC void init_protect();
#endif