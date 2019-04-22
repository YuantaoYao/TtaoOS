#include "type.h"
#include "const.h"
#include "func.h"

PUBLIC void task_tty(){
	while(1){
		milli_dalay(1);
		keyboard_read();
	}
}
