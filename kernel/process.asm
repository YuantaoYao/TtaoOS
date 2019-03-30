
P_STACKBASE	equ	0
GSREG		equ	P_STACKBASE
FSREG		equ	GSREG + 4
ESREG		equ	FSREG + 4
DSREG		equ	ESREG + 4
EDIREG		equ	DSREG + 4
ESIREG		equ	EDIREG + 4
EBPREG		equ	ESIREG + 4
KERNELESPREG	equ EBPREG + 4
EBXREG		equ	KERNELESPREG + 4
EDXREG		equ	EBPREG + 4
ECXREG		equ	EDXREG + 4
EAXREG		equ ECXREG + 4
RETADR		equ EAXREG + 4
EIPREG		equ RETADR + 4
CSREF		equ EIPREG + 4
EFLAGSREG	equ CSREF  + 4
ESPREG		equ EFLAGSREG + 4
SSREG		equ ESPREG + 4
P_STACKTOP	equ SSREG + 4
P_LDT_SEL	equ P_STACKTOP
P_LDT 		equ P_LDT_SEL + 4
TSS3_S_SP0	equ	4

SELECTOR_LDT_FIRST equ 0x28

extern p_proc_ready
extern tss
extern TestA
global restart
;===============================================================
;                       save
;===============================================================
; save:
	; pushad
	; push ds
	; push es
	; push fs
	; push gs
	; mov dx, ss
	; mov ds, dx
	; mov es, dx
	
	; mov esi, esp
	
	; inc dword [k_reenter]
	; cmp dword [k_reenter], 0
	; jne .1
	; mov esp, StackTop
	; push restart
	; jmp [esi + RETADR - P_STACKBASE]
	
; .1:
	; push restart_reenter
	; jmp [esi + RETADR - P_STACKBASE]
	
;===============================================================
;                       sys_call
;===============================================================
; sys_call:
	; call save
	; sti
	
	; call [sys_call_table + eax * 4]
	; mov [esi + EAXREG -P_STACKBASE], eax
	
	; cli
	
	; ret
;===============================================================
;                       restart
;===============================================================
restart:
	mov esp, [p_proc_ready]
	mov ax, SELECTOR_LDT_FIRST
	lldt	ax
	jmp TestA
	lea eax, [esp + P_STACKTOP]
	mov dword [tss + TSS3_S_SP0], eax
	pop gs
	pop gs
	pop es
	pop ds
	popad
	add esp, 4
	iretd