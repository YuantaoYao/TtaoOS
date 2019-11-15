#ifndef	_TTAO_TTY_H_
#define _TTAO_TTY_H_
#define TTY_IN_BYTES	256 //tty 输入队列大小

struct s_console;

/*TTY*/
typedef struct s_tty{
	u32	 in_buf[TTY_IN_BYTES];    /*  TTY 输入缓冲区 */
	u32* p_inbuf_head;			  /* 指向缓冲区中下一个空闲位置 */
	u32* p_inbuf_tail;			  /* 指向键盘任务应处理的键值 */
	int inbuf_count;			  /* 缓冲区中已经填充了多少 */
	
	struct s_console * p_console;
}TTY;

/* CONSOLE */
typedef struct s_console{
	unsigned int current_start_addr; /* 当前显示到了什么位置 */
	unsigned int original_addr;		 /* 当前控制台对应显存位置 */
	unsigned int v_mem_limit;		 /* 当前控制台占的显存大小 */	
	unsigned int cursor;			 /* 当前光标位置 */	
}CONSOLE;

#define DEFAULT_CHAR_COLOR (MAKE_COLOR(BLACK, WHITE))
#define GRAY_CHAR (MAKE_COLOR(BLACK, BLACK) | BRIGHT)
#define RED_CHAR (MAKE_COLOR(BLUE, RED) | BRIGHT)

PUBLIC int is_current_console(CONSOLE* p_con);
PUBLIC void out_char(CONSOLE* p_con, char ch);
PUBLIC void in_process(u32 key, TTY* p_tty);
PUBLIC void InitScreen(TTY* p_tty);
PUBLIC void select_console(int nr_console);
PUBLIC void scroll_screen(CONSOLE* p_con, int directhion);
PUBLIC void put_a_key(u32 key, TTY* p_tty);
PUBLIC void Init_Tty();

#endif