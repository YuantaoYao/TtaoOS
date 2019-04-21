#ifndef	_ORANGES_KEYBOARD_H_
#define _ORANGES_KEYBOARD_H_
#define RAED_KB_CODE 0x60 //读8042端口

#define	KB_IN_BYTES	32 //键盘缓存大小

/* Keyboard structure , 1 per console*/
typedef struct s_kb{
	char* p_head;
	char* p_tail;
	int count;
	char buf[KB_IN_BYTES];
}KB_INPUT;

#endif