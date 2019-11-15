#include "stdio.h"
#include "type.h"
#include "const.h"
#include "protect.h"
#include "global.h"
#include "tty.h"
#include "proc.h"
#include "keyboard.h"
#include "keymap.h"
#include "func.h"


PRIVATE int code_with_E0;
PRIVATE int shift_l;
PRIVATE int shift_r;
PRIVATE int ctrl_l;
PRIVATE int ctrl_r;
PRIVATE int alt_l;
PRIVATE int alt_r;
PRIVATE int caps_lock;
PRIVATE int num_lock;
PRIVATE int scroll_lock;
PRIVATE int column;

PRIVATE KB_INPUT kb_in;

PRIVATE int caps_lock;
PRIVATE int num_lock;
PRIVATE int scroll_lock;

PRIVATE void kb_wait();
PRIVATE void kb_ack();
PRIVATE void set_leds();

PRIVATE u8 get_byte_from_kbuf();
//初始化键盘参数
PUBLIC void Init_Keyboard(){
	
	code_with_E0 = 0;
	shift_l = shift_r = ctrl_l = ctrl_r = alt_l = alt_r = 0;
	kb_in.count = 0;
	kb_in.p_head = kb_in.p_tail = kb_in.buf;

	caps_lock = 1;
	num_lock = 1;
	scroll_lock = 0;
	
	set_leds();

	put_irq_handler(NUM_KEYBOAED_IRQ, keyboard_handler);
	enable_irq(NUM_KEYBOAED_IRQ);
}

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

//键盘处理
PUBLIC void keyboard_read(TTY* p_tty){
	u8 scan_code;
	int make;
	u32 key = 0;
	u32* keyrow;
	if(kb_in.count > 0){
		code_with_E0 = 0;
		scan_code = get_byte_from_kbuf();
		
		if(scan_code == 0xE1){
			
			u8 pausebre_scode[] = {0xE1,0x1D,0x45,
								   0xE1,0x9D,0xC5};
			int is_pausebreak = 1;
			for(int i=1;i<sizeof(pausebre_scode);i++){
				if(get_byte_from_kbuf() != pausebre_scode[i]){
					is_pausebreak = 0;
				}
			}
			if(is_pausebreak){
				key = PAUSEBREAK;
			}
		}else if(scan_code == 0xE0){
			
			scan_code = get_byte_from_kbuf();
		
			if(0x2A == scan_code){
				if(0xE0 == get_byte_from_kbuf()){
					if(0x37 == get_byte_from_kbuf()){
						key = PRINTSCREEN;
						make = 1;
					}
				}
			}
			if(0xB7 == scan_code){
				if(0xE0 == get_byte_from_kbuf()){
					if(0xAA == get_byte_from_kbuf()){
						key = PRINTSCREEN;
						make = 0;
					}
				}
			}
			if(key == 0){
				code_with_E0 = 1;
			}
		}
		
		if((key != PRINTSCREEN) && (key != PAUSEBREAK)){
			column = 0;
			make = (scan_code & FLAG_BREAK ? 0 : 1);//区分是不是Mark code *(Break code)
			keyrow = &keymap[(scan_code & 0x7F) * MAP_COLS];//从 map 中获取指定字符的 首地址 这里包括mark  和 break 
		
			if(shift_l || shift_r){
				column = 1;
			} 
			
		 	if(code_with_E0){
				column = 2;
				code_with_E0 = 0;
			}
			
			key = keyrow[column];
			
			switch(key){
				case SHIFT_L:
					shift_l = make;
					break;
				case SHIFT_R:
					shift_r = make;
					break;
				case CTRL_L:
					ctrl_l = make;
					break;
				case CTRL_R:
					ctrl_r = make;
					break;
				case ALT_L:
					alt_l = make;
					break;
				case ALT_R:
					alt_r = make;
					break;
				default:
					break;
			}
			if(make){
				key |= shift_l ? FLAG_SHIFT_L : 0;
				key |= shift_r ? FLAG_SHIFT_R : 0;
				key |= ctrl_l  ? FLAG_CTRL_L : 0;
				key |= ctrl_r  ? FLAG_CTRL_R : 0;
				key |= alt_l   ? FLAG_ALT_L : 0;
				key |= alt_r   ? FLAG_ALT_L : 0;
			
				in_process(key, p_tty);
			}  
		}
	}
}

//获取一个字符
PRIVATE u8 get_byte_from_kbuf(){
	u8 scan_code;
	while(kb_in.count < 1){}
	disable_int();
	scan_code = *(kb_in.p_tail);
	kb_in.p_tail++;
	if(kb_in.p_tail == kb_in.buf + KB_IN_BYTES){
		kb_in.p_tail = kb_in.buf;
	}
	kb_in.count--;
	
	enable_int();
	return scan_code;
}

PRIVATE void kb_wait(){ /* 等待8042 的输入缓冲区为空 */
	u8 kb_stat;
	
	do{
		kb_stat = in_port(KB_CMD);
	}while(kb_stat & 0x02);
}

PRIVATE void kb_ack(){
	u8 kb_read;
	
	do{
		kb_read = in_port(KB_DATA);
	}while(kb_read != KB_ACK);
}

PRIVATE void set_leds(){
	u8 leds = (caps_lock << 2) | (num_lock << 1) | scroll_lock;
	
	kb_wait();
	out_port(KB_DATA, LED_CODE);
	kb_ack();
	kb_wait();
	out_port(KB_DATA, leds);
	kb_ack();
}