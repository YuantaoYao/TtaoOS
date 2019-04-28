#ifndef	_TTAO_TYPE_H_
#define _TTAO_TYPE_H_

typedef void* system_call;

typedef	void (*int_handler)();
typedef void (*irq_handler)(int irq);
typedef void (*task_f)();
#endif