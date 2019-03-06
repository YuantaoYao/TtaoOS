;%define	_BOOT_DEBUG_
%ifdef _BOOT_DEBUG_
	org 0100h
%else
	org 07c00h
%endif

%ifdef _BOOT_DEBUG_
	BaseOfStack equ 0100h
%else
	BaseOfStack equ 07c00h
%endif


	jmp short LABEL_START
	nop
	
	%include "loader.inc"
	%include "fat12hdr.inc"
	
LABEL_START:

	mov ax, cs
	mov ss, ax
	mov es, ax
	mov ds, ax
	mov sp, BaseOfStack
	call CLEARSCREEN

	mov	ax, BootMessage
	mov	bp, ax			; ┓
	mov	ax, cs			; ┣ ES:BP = 串地址
	mov	es, ax			; ┛
	mov	cx, MessageLength	; CX = 串长度
	call PRINTSTR
	
	xor ah, ah
	xor dl, dl 
	int 13h     ;软驱复位
	
	mov word [wSectorNom], SectorNoOfRootDirectory
	LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
		cmp word [wRootDirSizeForLoop], 0
		jz LABEL_NO_LOADERIN
		dec word [wRootDirSizeForLoop]
		mov ax, BaseOfLoader
		mov es, ax
		mov bx, OffsetOfLoader
		mov ax, [wSectorNom]
		mov cl, 1
		call RdSector
		mov si, LoaderFileName
		mov di, OffsetOfLoader 	
		cld 	
		mov dx, 10h
		
		LABEL_SEARCH_FOR_LOADERBIN:
			cmp dx, 0
			jz LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR
			dec dx
			mov cx, 11
			
			LABEL_CMP_FILENAME:
				cmp cx, 0
				jz LABEL_FILENAMW_FOUND
				dec cx
				lodsb     ;ds:si -> al
				cmp al, byte [es:di]
				jz LABEL_CMP_GOON
				jmp LABEL_DIFFERENT
				
					LABEL_CMP_GOON:
						inc di
						jmp LABEL_CMP_FILENAME
					LABEL_DIFFERENT:
						and di, 0FFE0h ;初始化线性地址
						add di, 20h
						mov si, LoaderFileName
						jmp LABEL_SEARCH_FOR_LOADERBIN
		LABEL_GOTO_NEXT_SECTOR_IN_ROOT_DIR:
			add word [wSectorNom], 1
			jmp LABEL_SEARCH_IN_ROOT_DIR_BEGIN

;变量
;----------------------------------------------------------------------------
wRootDirSizeForLoop		dw	RootDirSectors ;要读取的剩余扇区	
MessageLength		equ	9
wSectorNom		dw	0	;要读取的扇区号	
bOdd		db	0	; 奇数还是偶数	
;字符串
;----------------------------------------------------------------------------
LoaderFileName			db	"LOADER  BIN ",0	; LOADER.BIN 之文件名
BootMessage		db	"Booting  "	; LOADER.BIN 之文件名		
NoLoader	db	"No Loader" ;没有找到LOADER.BIN
Readyer		db	"Ready.   " ;加载完成
;----------------------------------------------------------------------------
LABEL_FILENAMW_FOUND:
	mov	ax, RootDirSectors
	and di, 0FFE0h
	add di, 01Ah
	mov cx, word [es:di]
	push cx
	add cx, DeltaSectorNo
	add cx, ax
	mov ax, BaseOfLoader
	mov es, ax
	mov bx, OffsetOfLoader
	mov ax, cx
	
	LABEL_GOON_LOADING_FILE:
		push ax
		push bx
		
		mov ah, 0Eh
		mov al, '.'
		mov bl, 06h
		int 10h
		
		pop bx
		pop ax
		
		mov cl, 1
		call RdSector
		
		pop cx
		mov ax, cx
		
		call GetFATEntry
		
		cmp ax, 0FFFh
		jz LABEL_FILE_LOADED
		
		push ax
		mov dx, RootDirSectors
		add ax, dx
		add ax, DeltaSectorNo
		add bx, [BPB_BytsPerSec]
		jmp LABEL_GOON_LOADING_FILE
		LABEL_FILE_LOADED:
			call CLEARSCREEN
			mov	ax, Readyer
			mov	bp, ax			; ┓
			mov	ax, cs			; ┣ ES:BP = 串地址
			mov	es, ax			; ┛
			mov	cx, MessageLength	; CX = 串长度
			call PRINTSTR   ; 显示Ready...
			
			; *****************************************************************************************************
			jmp	BaseOfLoader:OffsetOfLoader	; 这一句正式跳转到已加载到内存中的 LOADER.BIN 的开始处
								; 开始执行 LOADER.BIN 的代码
								; Boot Sector 的使命到此结束
			; *****************************************************************************************************


LABEL_NO_LOADERIN:
	mov	ax, NoLoader
	mov	bp, ax			; ┓
	mov	ax, cs			; ┣ ES:BP = 串地址
	mov	es, ax			; ┛
	mov	cx, MessageLength	; CX = 串长度
	call PRINTSTR
%ifdef _BOOT_DEBUG_
	mov ax, 4c00h
	int 21h
%else
	jmp $
%endif

;method GetFATEntry
; 作用:
;	找到序号为 ax 的 Sector 在 FAT 中的条目, 结果放在 ax 中
;	需要注意的是, 中间需要读 FAT 的扇区到 es:bx 处, 所以函数一开始保存了 es 和 bx
GetFATEntry:
	push es
	push bx
	push ax
	mov ax, BaseOfLoader
	sub ax, 0100h
	mov es, ax
	pop ax
	mov byte [bOdd], 0
	mov bx, 3
	mul bx
	mov bx, 2
	div bx
	cmp dx, 0
	jz LABEL_EVEN
	mov byte [bOdd], 1
	LABEL_EVEN: ;偶数
		xor dx, dx
		mov bx, [BPB_BytsPerSec]
		div bx
		
		push dx
		mov bx, 0
		add ax, SectorNoOfFAT1
		mov cl, 2
		call RdSector
		pop dx
		add bx, dx
		mov ax, [es:bx]
		cmp byte [bOdd], 1
		jnz LABEL_EVEN_2
		shr ax, 4
			LABEL_EVEN_2:
				and ax, 0FFFh
				LABEL_GET_FAT_ENRY_OK:
					pop bx
					pop es
					ret
;method CLEARSCREEN
;清屏 直接调用
CLEARSCREEN:
	; 清屏
	mov	ax, 0600h		; AH = 6,  AL = 0h
	mov	bx, 0700h		; 黑底白字(BL = 07h)
	mov	cx, 0			; 左上角: (0, 0)
	mov	dx, 0184fh		; 右下角: (80, 50)
	int	10h			; int 10h
	ret

;method PRINTSTR
;打印字符串 ES:BP = 串地址 CX = 串长度
PRINTSTR:
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov	dh, 2h
	mov	dl, 0h
	int	10h			; int 10h
	ret
GoBack:
	mov ax, 0601h
	mov cx, 0h
	mov dx, 0780h
	mov bx, 0006h
	int 10h
	ret
;method RdSector
;读取扇区 ES:BX = 缓冲地址 AX=扇区地址 CL=读取扇区个数
RdSector:
	push bp
	mov byte [bp - 2], cl
	push es
	push bx
	mov bl, [BPB_SecPerTrk]
	div bl
	inc ah
	mov cl, ah
	mov ch, al
	shr ch, 1
	mov dh, al
	and dh, 1	
	pop bx
	pop es
	mov dl, [BS_DrvNum]
	.GoOnReading:
		mov ah, 02h
		mov al, byte [bp - 2]
		int 13h
	jc .GoOnReading
	pop bp
	ret

		
times 510-($-$$) db 0
dw 0xaa55