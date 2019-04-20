%include "kernel/include/proc.inc"

_NR_get_ticks equ 0
INT_VECTOR_SYS_CALL equ 0x90
INT_TTAO_TEXT equ 0xA0
extern disp_int

global get_ticks
global int_ttao

bits 32
[section .text]

get_ticks:
	mov eax, _NR_get_ticks
	int INT_VECTOR_SYS_CALL
	ret
	