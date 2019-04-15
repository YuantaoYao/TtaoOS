#include "type.h"
#include "const.h"
#include "func.h"

PUBLIC int Addition(int a, int b){
	return a + b;
}

PUBLIC void disp_int_hex(int n){
	char output[16];
	itoa(n, output, 16);
	disp_str(output);
} 

PUBLIC void disp_int(int n){
	char output[16];
	itoa(n, output, 10);
	disp_str(output);
}

//整形转字符串
PUBLIC char* itoa(int num, char* str,int radix)
{
	char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
	unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
	int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
	int u=0,v;
	char temp[12];
	//获取要转换的整数的绝对值
	if(radix==10&&num<0)//要转换成十进制数并且是负数
	{
		unum=(unsigned)-num;//将num的绝对值赋给unum
		str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
	}
	else unum=(unsigned)num;//若是num为正，直接赋值给unum

	//转换部分，注意转换后是逆序的
	do
	{
		str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
		unum /= radix;//unum去掉最后一位

	}while(unum);//直至unum为0退出循环
	
		
	if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
	else k=0;			//不是负数，全部都要调整

	str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。

	//将顺序调整过来
	
	if(k == 1){
		u = 1;
	};
	
	for(v = 0;v<i/2;v++){
		exchange(&str[v + k], &str[i-v-1]); //正
	}

	return str;//返回转换后的字符串
}
//交换字符
PUBLIC void * exchange(char * from, char * to){
	char temp;
	temp = *from;
	*from = *to;
	*to = temp;
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
