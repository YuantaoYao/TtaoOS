#ifndef	_TTAO_STDIO_H_
#define _TTAO_STDIO_H_

#define	PUBLIC
#define	PRIVATE	static

#undef	EXTERN
#define	EXTERN

#define FALSE	0
#define TRUE	1

typedef unsigned long long u64;
typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;
	
#endif

typedef char* va_list; //不定参数接收器


//基本输出
PUBLIC void disp_str(char * pszInfo);
PUBLIC void disp_int(int n);
PUBLIC void disp_int_hex(int n);
PUBLIC void disp_color_str(char * info, int color);

PUBLIC char* itoa(char* str, int num, int radix);
PUBLIC void * exchange(char * from, char * to);

PUBLIC int printf(const char * fmt, ...);

PUBLIC int vsprintf(char *buf, const char *fmt, va_list args);//字符处理器

PUBLIC void write(char *buf, int i);

PUBLIC void panic(const char *fmt, ... ); //c11标准 是当系统发现无法继续运行下去的故障时将调用它,会导致程序中止,然后由系统显示错误号

PUBLIC void spin(char * func_name); //输出函数名

#define	printl	printf 

#define ASSERT
#ifdef ASSERT
void assertion_failure(char *exp, char *file, char *base_file, int line);
#define assert(exp)  if (exp) ; \
        else assertion_failure(#exp, __FILE__, __BASE_FILE__, __LINE__)
#else
#define assert(exp)
#endif
