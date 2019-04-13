extern spurious_irq
extern tss
extern StackTop
extern p_proc_ready
extern sleep
extern k_reenter
extern clock_handler

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

INT_M_CTL			equ	0x20 
INT_M_CTLMASK		equ	0x21 
INT_S_CTL			equ	0xA0 
INT_S_CTLMASK		equ	0xA1 

EOI					equ	0x20

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

save:
	sub esp, 4
	pushad
	push ds
	push es
	push fs
	push gs
	mov dx, ss
	mov ds, dx
	mov es, dx
	
	mov esi, esp
	
	inc dword [k_reenter]
	cmp dword [k_reenter], 0
	jne ignore
	mov esp, StackTop
	
	
ignore:
	
	
	iretd

%macro hwint_master 1
	sub esp, 4
	pushad
	push ds
	push es
	push fs
	push gs
	mov dx, ss
	mov ds, dx
	mov es, dx
	

	inc byte [gs:0]

	mov al, EOI
	out INT_M_CTL, al
		
	inc dword [k_reenter]
	cmp dword [k_reenter], 0
	jne	.1
	
	mov esp, StackTop ;将栈顶指向另一块空闲区域防止进程栈被破坏
	
	push .restart_v2
	jmp .2
.1:
	push .restart_reenter_v2
	ret
	
.2:	
	sti ;默认中断是不打开的 为了在程序执行过程中能够接受其他中断
	
; 切换进程----start----

	push 0	
	call clock_handler
	add esp, 4
	 
	 
; 切换进程----end----
	
	cli ;关闭中断 此后不再接受其他中断请求
	ret ;相当于 pop IP
	
.restart_v2:	
	mov esp, [p_proc_ready]
	lldt [esp + P_LDT_SEL]	
	lea eax, [esp + P_STACKTOP]
	mov dword [tss + TSS3_S_SP0], eax
	
.restart_reenter_v2:
	dec dword [k_reenter]
	
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp, 4
	iretd
%endmacro 

ALIGN 16
hwint00:
	hwint_master 0
	
ALIGN 16
hwint01:
	irq_master 1
	
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