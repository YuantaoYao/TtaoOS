#include "stdio.h"
#include "const.h"

PUBLIC void panic(const char *fmt, ... ){
	int i;
	char *buf;
	va_list args = (va_list)((char *)(&fmt + 4));
	i = vsprintf(buf, fmt, args);
	
	printl("%c !!panic!! %s", MAG_CH_PANIC, buf);

	spin("panic()");
	
	/* should never arrive here */
	__asm__ __volatile__("ud2");
}