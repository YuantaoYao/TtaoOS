#include "stdio.h"
#include "mem.h"

PUBLIC int vsprintf(char *buf, const char *fmt, va_list args){
	char* p;
	char *tmp;
	va_list p_next_args = args;
	for(p=buf;*fmt;fmt++){
		disp_str(buf);
		disp_str("\n");
		if(*fmt != '%'){
			*p++ = *fmt;
			continue;
		}
		fmt++;
		switch(*fmt){
			case 'c':
				*p++ = *((char*)p_next_args);
				p_next_args += 4;
				break;
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
			case 's':
				strcpy(tmp, (*((char**)p_next_args)));
				disp_str(tmp);
				//strcpy(p, tmp);
				p_next_args += 4;
				p += strlen(tmp);
				break;
			default:
				break;
		}
		disp_str("\n");
		disp_str("\n");
				disp_str(buf);
		
	}
	return (p - buf);
}