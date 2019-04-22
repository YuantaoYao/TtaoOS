#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"
#include "global.h"
#include "keyboard.h"
#include "keymap.h"

PRIVATE KB_INPUT kb_in;

PUBLIC void keyboard_handler(int irq){
	//Break code = make code + 0x80
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

PUBLIC void Init_Keyboard(){
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;
	
	put_irq_handler(NUM_KEYBOAED_IRQ, keyboard_handler);
	enable_irq(NUM_KEYBOAED_IRQ);
}

PUBLIC void keyboard_read(){
	u8 scan_code;
	char output[2];
	int make;
	
	memset(output, 0, 2); 
	if(kb_in.count > 0){
		disable_int();
		scan_code = *(kb_in.p_tail);
		kb_in.p_tail++;
		if(kb_in.p_tail == kb_in.buf + KB_IN_BYTES){
			kb_in.p_tail = kb_in.buf;
		}
		kb_in.count--;
		
		enable_int();
		
			disp_int_hex(scan_code);
		if(scan_code == 0xE1){
			return;
		}else if(scan_code == 0xE0){
			disp_str("@");
			return ;
		}else{
			make = (scan_code & FLAG_BREAK ? FALSE : TRUE);//区分Mark code 和 Break code
			if(make){//Break Code
				output[0] = keymap[(scan_code & 0x7F) * MAP_COLS];
			}
		}
		
	}
}