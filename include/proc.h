typedef struct s_tss{
	u32	backlink;
	u32	esp0;
	u32	ss0;
	u32	esp1;
	u32	ss1;
	u32	esp2;
	u32	ss2;
	u32	cr3;
	u32	eip;
	u32	flags;
	u32	eax;
	u32	ecx;
	u32	edx;
	u32	ebx;
	u32	esp;
	u32	ebp;
	u32	esi;
	u32	edi;
	u32	es;
	u32	cs;
	u32	ss;
	u32	ds;
	u32	fs;
	u32	gs;
	u32	ldt;
	u16	trap;
	u16	iobass;
}TSS;

typedef struct s_stackframe{
	u32	gs;
	u32	fs;
	u32	es;
	u32	ds;
	u32 edi;
	u32	esi;
	u32	ebp;
	u32	kernel_esp;
	u32	ebx;
	u32	edx;
	u32	ecx;
	u32	eax;
	u32	retaddr;
	u32	eip;   //保存被中断处理程序 正在运行的代码段的地址
	u32 cs;
	u32	eflags;
	u32	esp;   //保存被中断处理程序 正在运行的数据栈的地址
	u32 ss;
}STACKFRAME;

typedef struct s_process{
	STACKFRAME regs; //保存进程寄存器
	u16 ldt_sel;     //进程ldt地址
	DESCRIPTOR ldts[LDT_SIZE];  //局部变量表
	u32 pid;		 //进程号
	char name[16];	 //进程名称
}PROCESS;

typedef struct s_task{
	task_f initial_eip; //任务地址
	int stacksize;  //任务堆栈 
	char name[32];  //任务名
}TASK;

extern char task_stack[];
 
extern PROCESS proc_table[];

extern TASK task_table[];

extern irq_handler irq_table[];

extern system_call sys_call_table[];

EXTERN PROCESS* p_proc_ready;

EXTERN TSS	tss;

