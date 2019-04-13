#include "type.h"
#include "const.h"
#include "func.h"

PUBLIC int Addition(int a, int b){
	return a + b;
}

PUBLIC void disp_int_hex(int n){
	char output[16];
	hex(output, n);
	disp_str(output);
} 

PUBLIC void disp_int(int n){
	char output[16];
	dec(output, n);
	disp_str(output);
}
 
PUBLIC char * dec(char * str, int num){
	
	int  n = num, i = 0, w = 1;
    char tmp[20];
    
	
	while(n > 0){
		int t = n % 10; 
		str[i++] = t + '0';
		n = n / 10;
	}
	
	str[i] = '\0';
	
	return str;
}
 
PUBLIC char * hex(char * str, int num){
	char * p = str;
	char ch;
	int	i;
	int	flag = 0;

	*p++ = '0';
	*p++ = 'x';

	if(num == 0){
		*p++ = '0';
	}
	else{	
		for(i=28;i>=0;i-=4){
			ch = (num >> i) & 0xF;
			if(flag || (ch > 0)){
				flag = 1;
				ch += '0';
				if(ch > '9'){
					ch += 7;
				}
				*p++ = ch;
			}
		}
	}

	*p = 0;

	return str;
}

PUBLIC void sleep(int time){
	int i=0;
	int k=0;
	int j=0;
	for(i = 0;i < time;i++){
		for(k = 0;k < 30;k++){
			for(j = 0;j < 1000;j++){}
		}
	}
}
