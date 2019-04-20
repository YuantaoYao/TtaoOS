extern spurious_irq
extern tss
extern StackTop
extern p_proc_ready
extern sleep
extern k_reenter
extern clock_handler
extern disp_int
extern disp_str
extern save
extern restart
extern restart_reenter
extern irq_table

global hwint00
global hwint01
global hwint02
global hwint03
global hwint04
global hwint05
global hwint06
global hwint07
global hwint08
global hwint09
global hwint0A
global hwint0B
global hwint0C
global hwint0D
global hwint0E
global hwint0F

%include "kernel/include/proc.inc"

%macro irq_master 1
	push %1
	call spurious_irq
	add esp, 4
	hlt
%endmacro

%macro irq_slave 1
	push %1
	call spurious_irq
	add esp, 4
	hlt
%endmacro 

%macro hwint_master 1
	call save ;保持寄存器状态
	
	in al, INT_M_CTLMASK
	or al, (1 << %1)					;关闭时钟中断
	out INT_M_CTLMASK, al
	
	mov al, EOI
	out INT_M_CTL, al
	sti ;默认中断是不打开的 为了在程序执行过程中能够接受其他中断
	
; 切换进程----start----

	push %1
	call [irq_table + %1 * 4]
	add esp, 4
	 
; 切换进程----end----
	cli ;关闭中断 此后不再接受其他中断请求
	
	in al, INT_M_CTLMASK
	and al, ~(1 << %1)		;打开时钟中断
	out INT_M_CTLMASK, al
	
	ret ;相当于 pop IP
%endmacro 

ALIGN 16
hwint00:
	hwint_master 0
	
ALIGN 16
hwint01:
	hwint_master 1
	
ALIGN 16
hwint02:
	irq_master 2
	
ALIGN 16
hwint03:
	irq_master 3
	
ALIGN 16
hwint04:
	irq_master 4
	
ALIGN 16
hwint05:
	irq_master 5
	
ALIGN 16
hwint06:
	irq_master 6
	
ALIGN 16
hwint07:
	irq_master 7
	
ALIGN 16
hwint08:
	irq_slave 8
	
ALIGN 16
hwint09:
	irq_slave 9

ALIGN 16
hwint0A:
	irq_slave 10

ALIGN 16
hwint0B:
	irq_slave 11

ALIGN 16
hwint0C:
	irq_slave 12

ALIGN 16
hwint0D:
	irq_slave 13

ALIGN 16
hwint0E:
	irq_slave 14

ALIGN 16
hwint0F:
	irq_slave 15