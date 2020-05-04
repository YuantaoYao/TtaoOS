#include "stdio.h"

PUBLIC int printf(const char *fmt, ...){
	int i;
	char *buf;
	
	va_list args = (va_list) ((char*) (&fmt) + 4);/* 4是参数fmt所占堆栈中的大小 */
	i = vsprintf(buf, fmt, args);
	
	//write(buf, i);
	buf[i] = '\0';
	printx(buf);
	return i;
}
