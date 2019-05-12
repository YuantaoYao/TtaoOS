#include "stdio.h"
#include "type.h"
#include "const.h"
#include "syscall.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "keyboard.h"
#include "proc.h"
#include "func.h"


PUBLIC void task_sys(){
	MESSAGE msg;
	while(1){
		sendrec(RECEIVE, ANY, &msg);
		int src = msg.source;
		switch(msg.type){
			case GET_TICKS:{
				msg.u.m1.mli1 = ticks;
				sendrec(SEND, src, &msg);
				break;
			}
			default:
				panic("unkown msg type!!");
				break;
		}
	}
}
