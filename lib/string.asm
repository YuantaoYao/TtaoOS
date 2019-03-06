[section .data]
disp_pos	dd 0
[section .text]

global  disp_str
global	memcpy
; ========================================================================
;                  void disp_str(char * info);
; ========================================================================
disp_str:
	push ebp
	mov ebp, esp

	mov esi, [ebp + 8]
	mov edi, [disp_pos]
	mov ah, 0Fh
.1:
	lodsb
	test al, al
	jz .2
	cmp al, 0Ah
	jnz .3
	push eax
	mov eax, edi
	mov bl, 160
	div bl
	and eax, 0FFh
	inc eax
	mov bl, 160
	mul bl
	mov edi, eax
	pop eax
	jmp .1
.3:
	mov [gs:edi], ax
	add edi, 2
	jmp .1
.2:
	mov [disp_pos], edi
	pop ebp
	ret
	
	
; ------------------------------------------------------------------------
; void* memcpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------

memcpy:
	push ebp
	mov ebp, esp
	
	push esi
	push edi
	push ecx
	
	mov edi, [ebp + 8]
	mov esi, [ebp + 12]
	mov ecx, [ebp + 16]
	
.1:
	cmp	ecx, 0
	jz .2
	mov al, [ds:esi]
	inc esi
	
	mov byte [ds:edi], al
	inc edi
	
	dec ecx
	jmp .1
	
	
.2:
	mov eax, [ebp + 8]
	
	pop ecx
	pop edi
	pop esi
	mov esp, ebp
	pop ebp
	ret