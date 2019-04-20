#include "type.h"
#include "const.h"
#include "protect.h"
#include "func.h"
#include "proc.h"
#include "global.h"

PUBLIC void keyboard_handler(int irq){
	disp_str("*");
}