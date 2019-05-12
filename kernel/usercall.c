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
PUBLIC int send_rec(int function, int src_dest, MESSAGE* msg);


PUBLIC int send_rec(int function, int src_dest, MESSAGE* msg){
	int ret = 0;

	if (function == RECEIVE)
		memset(msg, 0, sizeof(MESSAGE));

	switch (function) {
	case BOTH:
		ret = sendrec(SEND, src_dest, msg);
		if (ret == 0)
			ret = sendrec(RECEIVE, src_dest, msg);
		break;
	case SEND:
	case RECEIVE:
		ret = sendrec(function, src_dest, msg);
		break;
	default:
		assert((function == BOTH) ||
		       (function == SEND) || (function == RECEIVE));
		break;
	}

	return ret;
}