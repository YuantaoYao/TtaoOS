SELECTOR_KERNEL_CS	equ	8

extern	cstart

extern	gdt_ptr

[section .bss]
StackSpace		resb 	2 * 1024
StackTop:		
[section .text]	; 代码在此

global _start	; 导出 _start

_start:	; 跳到这里来的时候，我们假设 gs 指向显存
	mov esp, StackTop
	sgdt	[gdt_ptr]
	call cstart
	lgdt	[gdt_ptr]
	jmp	SELECTOR_KERNEL_CS:csinit
csinit:
	push	0
	popfd
	
	hlt