#include "stdio.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "keyboard.h"
#include "proc.h"
#include "func.h"

PRIVATE void set_cursor(unsigned int position);
PRIVATE void set_video_start_addr(u32 addr);
PRIVATE void flush(CONSOLE* p_con, int nr_console);
//初始化console 
PUBLIC void InitScreen(TTY* p_tty){
	
	int nr_tty = p_tty - tty_table;
	p_tty->p_console = console_table + nr_tty;
	int v_mem_size = V_MEM_SIZE >> 1;
	
	int con_v_mem_size	= SCREEN_WIDTH * (v_mem_size / NR_CONSOLE / SCREEN_WIDTH);  //除SCREEN_WIDTH又乘SCREEN_WIDTH是为了每一个tty都是SCREEN_WIDTH 的整倍数
	p_tty->p_console->original_addr 	 = nr_tty * con_v_mem_size;
	p_tty->p_console->v_mem_limit 		 = con_v_mem_size;
	//卷轴起始点开始的位置
	p_tty->p_console->current_start_addr = p_tty->p_console->original_addr; 
	//光标开始的位置
	p_tty->p_console->cursor 			 = p_tty->p_console->original_addr;
	//初始化全局变量cursor_table
	cursor_table[nr_tty] = p_tty->p_console->cursor;

	if(nr_tty == 0){
		/* 第一个控制台沿用原来的光标位置*/
		p_tty->p_console->cursor = disp_pos / 2;
		disp_pos = 0;
		set_cursor(p_tty->p_console->cursor);
	}else{
		//out_char(p_tty->p_console, nr_tty + '0');
		//out_char(p_tty->p_console, '#');
	}
	
}

//判断是不是当前控制台
PUBLIC int is_current_console(CONSOLE* p_con){
	return p_con == &console_table[nr_current_console];
}

//输出字符
PUBLIC void out_char(CONSOLE* p_con, char ch){
	int nr_console = p_con - console_table;
			
	u8* p_vmem = (u8*)(V_MEM_BASE + p_con->cursor * 2);
	switch(ch){
		case '\n':
		{
			if(p_con->cursor < (p_con->original_addr + p_con->v_mem_limit - SCREEN_WIDTH)){
				p_con->cursor = p_con->original_addr + SCREEN_WIDTH*((p_con->cursor - p_con->original_addr)/SCREEN_WIDTH+1);
			}
			break;
		}	
		case '\b':
		{
			if(p_con->cursor > p_con->original_addr){
				p_con->cursor--;
				*(p_vmem-2) = ' ';
				*(p_vmem-1) = DEFAULT_CHAR_COLOR ;
				cursor_table[nr_console] -= 1;
				p_con->cursor = cursor_table[nr_console];
			}
			break;
		}
		default:
		{
			if(p_con->cursor < (p_con->original_addr + p_con->v_mem_limit)){
				*p_vmem++ = ch;
				*p_vmem++ = DEFAULT_CHAR_COLOR ;
				cursor_table[nr_console] += 1;
				p_con->cursor = cursor_table[nr_console];
			}
			break;
		}
	}
	if(p_con->cursor < p_con->current_start_addr){
		scroll_screen(p_con, SCR_UP);		
	}
	if(p_con->cursor >= p_con->current_start_addr + SCREEN_SIZE){		
		scroll_screen(p_con, SCR_DN);
	}
	flush(p_con, nr_console);
}

PRIVATE void flush(CONSOLE* p_con, int nr_console){
	set_cursor(p_con->cursor); 
	cursor_table[nr_console] = p_con->cursor;
	if(p_con->cursor < p_con->current_start_addr){
		set_video_start_addr(SCREEN_WIDTH*(p_con->cursor/SCREEN_WIDTH));
	}else if(p_con->cursor > (p_con->current_start_addr + SCREEN_SIZE)){
		set_video_start_addr(SCREEN_WIDTH*(p_con->cursor/SCREEN_WIDTH+1) -SCREEN_SIZE);
	}else{
		set_video_start_addr(p_con->current_start_addr);
	}
}

//切换控制台
PUBLIC void select_console(int nr_console){
	if((nr_console < 0) || (nr_console >= NR_CONSOLE)){
		return ;
	}
	set_cursor(console_table[nr_console].cursor); 	
	set_video_start_addr(console_table[nr_console].current_start_addr);
}
//滚动屏幕
PUBLIC void scroll_screen(CONSOLE* p_con, int directhion){
	if(directhion == SCR_UP){
		if(p_con->current_start_addr > p_con->original_addr){
			p_con->current_start_addr -= SCREEN_WIDTH;
		}
	}else if(directhion == SCR_DN){
		if(p_con->current_start_addr + SCREEN_SIZE < (p_con->original_addr + p_con->v_mem_limit) && 
										 (p_con->cursor > SCREEN_WIDTH + p_con->original_addr)){
			p_con->current_start_addr += SCREEN_WIDTH;
		}
	}else{
		
	}
	set_video_start_addr(p_con->current_start_addr);
}

//切换光标
PRIVATE void set_cursor(unsigned int position){
	disable_int();
	out_port(CRTC_ADDR_REG, CURSOR_H);
	out_port(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_port(CRTC_ADDR_REG, CURSOR_L);
	out_port(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}
//切换卷轴位置
PRIVATE void set_video_start_addr(u32 addr){
	disable_int();
	out_port(CRTC_ADDR_REG, START_ADDR_H);
	out_port(CRTC_DATA_REG, (addr >> 8) & 0xFF);
	out_port(CRTC_ADDR_REG, START_ADDR_L);
	out_port(CRTC_DATA_REG, addr & 0xFF);
	enable_int();
}

