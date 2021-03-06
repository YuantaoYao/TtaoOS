#include "stdio.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "syscall.h"
#include "global.h"
#include "tty.h"
#include "func.h"
#include "proc.h"
#include "fs.h"

PUBLIC int sys_printx( PROCESS* p_proc, char* buf, int _unused2, int _unused1);

PUBLIC int sys_write(char* buf, int len, PROCESS * p_proc);

PUBLIC int sys_send_rec(MESSAGE * msg, int function, int src_desc, PROCESS * p_proc);

PUBLIC char task_stack[STACK_SIZE_TOTAL];

PUBLIC irq_handler irq_table[NR_IRQ];

PUBLIC TASK sys_task_table[NR_SYS_TASKS_PROC] = {{task_tty, STACK_SIZE_TTY_TASK, "task_tty"},				//用于tty的调用		
												 {task_sys, STACK_SIZE_SYS_TASK, "task_sys"},
												 {task_hd,  STACK_SIZE_HD_TASK,  "task_hd"}}; //系统任务   // 进程间通信引擎

PUBLIC TASK user_task_table[NR_USER_TASKS_PROC] = {{TestA, STACK_SIZE_TESTA, "TestA"},      
												   {TestB, STACK_SIZE_TESTB, "TestB"},
												   {TestC, STACK_SIZE_TESTC, "TestC"}}; //用户任务

PUBLIC PROCESS	proc_table[NR_ALL_TASKS_PROC]; //所有进程 （系统任务+用户任务）  通过时钟中断进行进程切换

PUBLIC system_call sys_call_table[NR_SYS_CALL] = {sys_printx, sys_write, sys_send_rec}; // 系统调用 （通过中断实现）

PUBLIC TTY tty_table[NR_CONSOLE];

PUBLIC int cursor_table[NR_CONSOLE];

PUBLIC CONSOLE console_table[NR_CONSOLE];

struct dev_drv_map dd_map[] = {
	{INVALID_DRIVER},
	{INVALID_DRIVER},
	{INVALID_DRIVER},
	{TASK_HD},
	{TASK_TTY},
	{INVALID_DRIVER}
};