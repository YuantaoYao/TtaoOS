#include "stdio.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "syscall.h"
#include "tty.h"
#include "func.h"
#include "proc.h"
#include "global.h"


PUBLIC void task_fs(){
	printl("Task FS begins. \n");

	int ret = 0;

	MESSAGE driver_msg;
	driver_msg.type = DEV_OPEN;

	ret = sendrec(SEND, TASK_HD, &driver_msg);
	if (ret == 0)
		ret = sendrec(RECEIVE, TASK_HD, &driver_msg);
	spin("FS");

}