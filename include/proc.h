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
	
	int ticks;		//进程优先级
	int priority;   //优先级初始化常量
	
	u32 pid;		 //进程号
	char name[16];	 //进程名称
	
	int p_flags;        /* 0 程序正在运行 或 准备运行
						   SENDING 进程处于发送消息状态（进程被阻塞）
						   RECEIVING 进程处于消息接收状态 （进程被阻塞） */
	
	struct msg * p_msg; //指向消息体的指针
	
	int p_recvfrom; //本进程想要接收消息 但当前并没有进程给他发送消息 本字段记录本进程想要从哪个进程接收消息
	int p_sendto;   //本进程想要发送消息 但当前并没有进程想要接收他的消息 本字段记录本进程想要把消息发送给哪个进程
	
	int has_int_msg;  //发送中断置位
	
	struct s_process * first_sending;//如果本进程没有准备好接收消息，则本进程的q_sending 指向希望 向本进程发送消息的进程消息队列的 首地址
	
	struct s_process * next_sending; // 进程队列的下一个进程
	
	int nr_tty;      //定位进程是哪个tty
}PROCESS;

typedef struct s_task{
	task_f initial_eip; //任务地址
	int stacksize;  //任务堆栈 
	char name[32];  //任务名
}TASK;

extern char task_stack[];
 
extern PROCESS proc_table[]; //进程表

extern TASK sys_task_table[]; //系统调用任务

extern TASK user_task_table[]; //用户调用任务

extern irq_handler irq_table[];

extern system_call sys_call_table[];

extern TTY tty_table[];

extern CONSOLE console_table[];

extern int cursor_table[];

EXTERN PROCESS* p_proc_ready; //可以指向进程的 进程数据结构指针

EXTERN TSS	tss;

