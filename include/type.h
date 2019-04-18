#ifndef	_TTAO_TYPE_H_
#define _TTAO_TYPE_H_

typedef unsigned int   u32;
typedef unsigned short u16;
typedef unsigned char  u8;

typedef void* system_call;

typedef	void (*int_handler)();
typedef void (*irq_handler)(int irq);
typedef void (*task_f)();
#endif