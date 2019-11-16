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

PUBLIC void task_hd(){
	MESSAGE msg;
	while(1){
		sendrec(RECEIVE, ANY, &msg);
		int src = msg.source;
		int drive = msg.u.m1.mli1;
		switch(msg.type){
			case DEV_OPEN:{
				hd_identify(drive);
				break;
			}
			default:
				dump_msg("HD driver:unKnown msg", &msg);
				panic("unkown msg type!!");
				break;
		}
	}

}