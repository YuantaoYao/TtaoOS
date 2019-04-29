%include "kernel/include/proc.inc"

_NR_get_ticks equ 0
_NR_write	  equ 1
_NR_sendrec   equ 2
INT_VECTOR_SYS_CALL equ 0x90
INT_TTAO_TEXT equ 0xA0
extern disp_int

global get_ticks
global int_ttao
global write
global sendrec

bits 32
[section .text]

get_ticks:
	mov eax, _NR_get_ticks
	int INT_VECTOR_SYS_CALL
	ret
	
write:
	mov eax, _NR_write
	mov ebx, [esp + 4]
	mov ecx, [esp + 8]
	int INT_VECTOR_SYS_CALL
	ret
	
sendrec:
	mov eax, _NR_sendrec
	mov ebx, [esp + 4]
	mov ecx, [esp + 8]
	mov edx, [esp + 12]
	int INT_VECTOR_SYS_CALL
	ret