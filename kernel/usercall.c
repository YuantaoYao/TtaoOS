#include "stdio.h"
#include "mem.h"
#include "type.h"
#include "syscall.h"
#include "const.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "global.h"
#include "func.h"

PUBLIC int get_ticks(){
	int ret = 0;

	MESSAGE msg;
	reset_msg(&msg);
	msg.type = GET_TICKS;
	// send_rec(BOTH, TASK_SYS, &msg);
	ret = sendrec(SEND, TASK_SYS, &msg);
	if (ret == 0)
		ret = sendrec(RECEIVE, TASK_SYS, &msg);
	return msg.u.m1.mli1;
}

PUBLIC int dev_open(int drive){
	int ret = 0;

	MESSAGE msg;
	reset_msg(&msg);
	msg.type = DEV_OPEN;
	msg.u.m1.mli1 = drive;
	ret = sendrec(SEND, TASK_HD, &msg);
	return ret;
}