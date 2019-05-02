#include "stdio.h"

PUBLIC void panic(const char *fmt, ... ){
	int i;
	char buf[256];
	va_list arg = (va_list)((char *)(&fmt + 4));
	printf("%x")
}