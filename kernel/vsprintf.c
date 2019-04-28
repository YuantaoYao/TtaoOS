#include "stdio.h"
#include "mem.h"

PUBLIC int vsprintf(char *buf, const char *fmt, va_list args){
	char* p;
	char tmp[256];
	va_list p_next_args = args;
	for(p=buf;*fmt;fmt++){
		if(*fmt != '%'){
			*p++ = *fmt;
			continue;
		}
		fmt++;
		switch(*fmt){
			case 'x':
				itoa(tmp, *((int *)p_next_args), 16);
				strcpy(p, tmp);
				p_next_args += 4;
				p += strlen(tmp);
				break;
			case 'd':
				itoa(tmp, *((int *)p_next_args), 10);
				strcpy(p, tmp);
				p_next_args += 4;
				p += strlen(tmp);
				break;
			case 'h':
				itoa(tmp, *((int *)p_next_args), 2);
				strcpy(p, tmp);
				p_next_args += 4;
				p += strlen(tmp);
				break;
			default:
				break;
		}
	}
	return (p - buf);
}