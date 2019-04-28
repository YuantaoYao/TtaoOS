extern	disp_pos

global  disp_str
global  disp_color_str
global	memcpy
global  memset
global  strcpy
global  strlen
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
	
; ========================================================================
;                  void disp_color_str(char * info, int color);
; ========================================================================
disp_color_str:
	push ebp
	mov ebp, esp

	mov esi, [ebp + 8]
	mov edi, [disp_pos]
	mov ah,  [ebp + 8 + 4]
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
	
;-------------------------------------------------------------------------
; void memset(void* p_dst, char ch, int size); 格式化
;-------------------------------------------------------------------------	

memset:
	push ebp
	mov ebp, esp
	
	push esi
	push edi
	push ecx	
	
	mov edi, [ebp + 8]
	mov edx, [ebp + 12]
	mov ecx, [ebp + 16]

.1:
	cmp ecx, 0
	jz .2
	
	mov byte [edi], dl
	inc edi
	dec ecx
	jmp .1
	
.2:
	pop ecx
	pop edi
	pop esi
	mov esp, ebp
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
	
; ------------------------------------------------------------------------
; void * strcpy(char* p_dst, char* p_src);
; ------------------------------------------------------------------------
strcpy:
	push ebp
	mov ebp, esp
	
	push esi
	push edi
	
	mov edi, [ebp + 8]
	mov esi, [ebp + 12]
.1:	
	mov al, [esi]
	inc esi
	
	mov byte [edi], al
	inc edi
	
	cmp al, 0
	jnz .1
	
	mov eax, [ebp + 8]
	
	pop edi
	pop esi
	mov esp, ebp
	pop ebp
	ret
	
; ------------------------------------------------------------------------
;             int strlen(char * str)
; ------------------------------------------------------------------------
strlen:
	push ebp
	mov ebp, esp

	xor eax, eax
	mov esi, [ebp + 8]
	
.1:
	cmp byte [esi], 0
	jz .2
	inc esi
	inc eax
	jmp .1
.2:
	pop ebp
	ret