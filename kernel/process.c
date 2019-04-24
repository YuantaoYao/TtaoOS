#include "const.h"
#include "type.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "func.h"

PUBLIC int sys_get_ticks(){
	return ticks;
}
