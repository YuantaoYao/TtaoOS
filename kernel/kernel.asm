SELECTOR_KERNEL_CS	equ	8

extern	cstart


extern	gdt_ptr
extern	idt_ptr
extern	disp_pos
[section .bss]
StackSpace		resb 	2 * 1024
StackTop:		
[section .text]	; 代码在此

global _start	; 导出 _start


_start:	; 跳到这里来的时候，我们假设 gs 指向显存
	mov esp, StackTop
	
	mov dword [disp_pos], 0
	
	sgdt	[gdt_ptr]
	
	call cstart
	
	lgdt	[gdt_ptr]
	
	lidt	[idt_ptr]

	jmp	SELECTOR_KERNEL_CS:csinit
csinit:
	ud2
	jmp 0x40:0

	; push	0
	; popfd
	
	hlt
	