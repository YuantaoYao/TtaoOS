#include "type.h"
#include "const.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "func.h"


PUBLIC char task_stack[STACK_SIZE_TOTAL];

PUBLIC PROCESS	proc_table[NR_TASKS];

PUBLIC irq_handler irq_table[NR_IRQ];

PUBLIC TASK task_table[NR_TASKS] = {{TestA, STACK_SIZE_TESTA, "TestA"},
									{TestB, STACK_SIZE_TESTB, "TestB"},
									{TestC, STACK_SIZE_TESTC, "TestC"}};
									
PUBLIC system_call sys_call_table[NR_SYS_CALL] = {sys_get_ticks};

PUBLIC TTY tty_table[NR_CONSOLE];

PUBLIC int cursor_table[NR_CONSOLE];

PUBLIC CONSOLE console_table[NR_CONSOLE];