%include "kernel/include/proc.inc"
extern p_proc_ready
extern tss
extern StackTop
extern k_reenter

global save
global restart
global restart_reenter
;===============================================================
;                       saves
;===============================================================
	
save:
	pushad
	push ds
	push es
	push fs
	push gs
	mov dx, ss
	mov ds, dx
	mov es, dx
	
	mov eax, esp
	
	inc byte [gs:10]
	
	inc dword [k_reenter]
	cmp dword [k_reenter], 0
	jne .1
	mov esp, StackTop
	
	push restart
	jmp [eax + RETADR - P_STACKBASE]
	
.1:
	push restart_reenter	
	jmp [eax + RETADR - P_STACKBASE]

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
	lldt [esp + P_LDT_SEL]
	
	lea eax, [esp + P_STACKTOP]
	mov dword [tss + TSS3_S_SP0], eax
	
restart_reenter:
	dec dword [k_reenter]
	pop gs
	pop fs
	pop es
	pop ds
	popad
	add esp, 4
	iretd