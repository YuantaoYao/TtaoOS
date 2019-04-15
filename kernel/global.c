#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"

PUBLIC char task_stack[STACK_SIZE_TOTAL];

PUBLIC PROCESS	proc_table[NR_TASKS];

PUBLIC irq_handler irq_table[NR_IRQ];

PUBLIC TASK task_table[NR_TASKS] = {{TestA, STACK_SIZE_TESTA, "TestA"},
									{TestB, STACK_SIZE_TESTB, "TestB"},
									{TestC, STACK_SIZE_TESTC, "TestC"}};