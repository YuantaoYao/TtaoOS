#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"

PRIVATE KB_INPUT kb_in;
PRIVATE void init_keyboard();

PUBLIC void keyboard_handler(int irq){
	init_keyboard();
	
	u8 scan_code = in_port(RAED_KB_CODE);
	if(kb_in.count < KB_IN_BYTES){
		*(kb_in.p_head) = scan_code;
		kb_in.p_head++;
		if(kb_in.p_head == kb_in.buf + KB_IN_BYTES){
			kb_in.p_head = kb_in.buf;
		}
		kb_in.count++;
	}
}

PRIVATE void init_keyboard(){
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;
}