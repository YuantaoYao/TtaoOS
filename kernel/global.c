#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"


PUBLIC PROCESS	proc_table[NR_TASKS];

PUBLIC char 	task_stack[STACK_SIZE_TOTAL];