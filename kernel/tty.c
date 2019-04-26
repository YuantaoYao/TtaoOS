#include "type.h"
#include "const.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "keyboard.h"
#include "proc.h"
#include "func.h"

#define TTY_FIRST  (tty_table)
#define TTY_END    (tty_table + NR_CONSOLE)

PRIVATE void init_tty(TTY* p_tty);
PRIVATE void tty_do_write(TTY* p_tty);
PRIVATE void tty_do_read(TTY* p_tty);

//调用键盘
PUBLIC void task_tty(){
	TTY* p_tty;
	
	for(p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++){
		init_tty(p_tty);
	}
	nr_current_console = 0; //默认第一个TTY
	while(1){
		for(p_tty=TTY_FIRST;p_tty<TTY_END;p_tty++){
			tty_do_read(p_tty);
			tty_do_write(p_tty);
		}
	}
}
//初始化 tty
PRIVATE void init_tty(TTY* p_tty){
	InitScreen(p_tty);
	p_tty->inbuf_count = 0;
	p_tty->p_inbuf_head = p_tty->p_inbuf_tail = p_tty->in_buf;
	
	int nr_tty = p_tty - tty_table;
	p_tty->p_console = console_table + nr_tty;
}

PUBLIC void in_process(u32 key, TTY* p_tty){
	if(!(key & FLAG_EXT)){		
		if(p_tty->inbuf_count < TTY_IN_BYTES){
			*(p_tty->p_inbuf_head) = key;
			p_tty->p_inbuf_head++;
			if(p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES){
				p_tty->p_inbuf_head = p_tty->in_buf;
			}
			p_tty->inbuf_count++;
		}
	}else{
		int raw_code = key & MASK_RAW;
		
		switch(raw_code){
			case F1:
				if(((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R)) && ((key & FLAG_ALT_L) || (key & FLAG_ALT_R))){				
					nr_current_console = 0;			
					select_console(0);
				}
				break;
			case F2:
				if(((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R)) && ((key & FLAG_ALT_L) || (key & FLAG_ALT_R))){
					nr_current_console = 1;					
					select_console(1);
				}
				break;
			case F3:
				if(((key & FLAG_CTRL_L) || (key & FLAG_CTRL_R)) && ((key & FLAG_ALT_L) || (key & FLAG_ALT_R))){		
					nr_current_console = 2;
					select_console(2);
				}
				break;
			case F4:
				
				break;
			case F5:
				
				break;
			case F6:
				
				break;
			case F7:
				
				break;
			case UP:
				if((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)){
					scroll_screen(p_tty->p_console, SCR_UP);
				}
				break;
			case DOWN:
				if((key & FLAG_SHIFT_L) || (key & FLAG_SHIFT_R)){
					scroll_screen(p_tty->p_console, SCR_DN);
				}
				break;
			case LEFT:
				
				break;
			case RIGHT:
				
				break;
			case ENTER:
				put_a_key('\n', p_tty);
				break;
			case BACKSPACE:
				put_a_key('\b', p_tty);
				break;
			default:
				break;
		}
	}
}

PUBLIC void put_a_key(u32 key, TTY* p_tty){
	if(!(key & FLAG_EXT)){		
		if(p_tty->inbuf_count < TTY_IN_BYTES){
			*(p_tty->p_inbuf_head) = key;
			p_tty->p_inbuf_head++;
			if(p_tty->p_inbuf_head == p_tty->in_buf + TTY_IN_BYTES){
				p_tty->p_inbuf_head = p_tty->in_buf;
			}
			p_tty->inbuf_count++;
		}
	}
}

PRIVATE void tty_do_read(TTY* p_tty){
	if(is_current_console(p_tty->p_console)){
		keyboard_read(p_tty);
	}
}

PRIVATE void tty_do_write(TTY* p_tty){
	if(is_current_console(p_tty->p_console) && p_tty->inbuf_count){
		char ch = *(p_tty->p_inbuf_tail);
		p_tty->p_inbuf_tail++;
		if(p_tty->p_inbuf_tail == p_tty->in_buf + TTY_IN_BYTES){
			p_tty->p_inbuf_tail = p_tty->in_buf;
		}
		p_tty->inbuf_count--;
		out_char(p_tty->p_console, ch);
	}
}