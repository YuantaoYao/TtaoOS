#include "type.h"
#include "const.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "keyboard.h"
#include "proc.h"
#include "func.h"

PRIVATE void set_cursor(unsigned int position);

PUBLIC int is_current_console(CONSOLE* p_con){
	return p_con == &console_table[nr_current_console];
}

PUBLIC void out_char(CONSOLE* p_con, char ch){
	u8* p_vmem = (u8*)(V_MEM_BASE + disp_pos);
	
	*p_vmem++ = ch;
	*p_vmem++ = DEFAULT_CHAR_COLOR;
	disp_pos += 2;
	
	set_cursor(disp_pos/2);
}

PRIVATE void set_cursor(unsigned int position){
	disable_int();
	out_port(CRTC_ADDR_REG, CURSOR_H);
	out_port(CRTC_DATA_REG, (position >> 8) & 0xFF);
	out_port(CRTC_ADDR_REG, CURSOR_L);
	out_port(CRTC_DATA_REG, position & 0xFF);
	enable_int();
}