%include "kernel/include/proc.inc"

_NR_printx	  equ 0
_NR_write	  equ 1
_NR_sendrec   equ 2
INT_VECTOR_SYS_CALL equ 0x90

global printx
global write
global sendrec

bits 32
[section .text]

printx:
	mov eax, _NR_printx
	mov ebx, [esp + 4]
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