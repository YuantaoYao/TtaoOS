org 0100h

	jmp short INIT

%include "fat12hdr.inc"
%include "safe.inc"
%include "loader.inc"

BaseOfStack		equ	0100h
PageDirBase		equ	100000h ; 页目录开始的地址：1M
PageTblBase		equ	101000h ; 页表开始的地址：  1M + 4K


LABEL_GDT:			Descriptor  0,	             0,		0
LABEL_DESC_FLAT_C:	Descriptor	0,		   0FFFFFh,	    DA_CR|DA_32|DA_LIMIT_4K
LABEL_DESC_FLAT_RW:	Descriptor	0,		   0FFFFFh,	    DA_DRW|DA_32|DA_LIMIT_4K
LABEL_DESC_VIDEO:	Descriptor	0B8000h,	0FFFFh,     DA_DRW|DA_DPL3

GdtLen		equ	$ - LABEL_GDT
GdtPtr		dw  GdtLen - 1
			dd	BaseOfLoaderPhyAddr + LABEL_GDT	


; GDT 选择子
SelectorFlatC		equ	LABEL_DESC_FLAT_C  - LABEL_GDT
SelectorFlatRW		equ	LABEL_DESC_FLAT_RW - LABEL_GDT
SelectorVideo		equ	LABEL_DESC_VIDEO   - LABEL_GDT + SA_RPL3

INIT:
	call CLEARSCREEN
	
	call LABEL_START
LABEL_START:
	mov	ax, cs
	mov	es, ax
	mov	bp, BootMessage
	mov cx, BootMessageLen
	call PRINTSTR		; 调用显示字符串例程
	
	call _START_

_START_:
	mov ax, cs
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, BaseOfStack
	
	call CLEARSCREEN
	
	mov ebx, 0
	mov di, _MemChkBuf
.MemChkLoop:
	mov eax, 0E820h
	mov ecx, 20
	mov edx, 0534D4150h
	int 15h
	jc .MemChkFail
	add di, 20
	inc dword [_dwMCRNumber]
	cmp ebx, 0
	jne .MemChkLoop
	jmp .MemChkOK
.MemChkFail:
	mov dword [_dwMCRNumber], 0
.MemChkOK:
	mov word [wSectorNo], SectorNoOfRootDirectory
	xor ah, 0h
	mov dl, [BS_DrvNum]
	int 13h             ; 软驱复位
	
	mov bp, LoadMessage
	mov cx, MessageLength
	call PRINTSTR

	mov byte [wSectorNo], SectorNoOfRootDirectory
LABEL_SEARCH_IN_ROOT_DIR_BEGIN:
	cmp	byte [wRootDirSizeForLoop], 0
	jz	LABEL_NO_KERNELBIN
	dec byte [wRootDirSizeForLoop]
	mov ax, BaseOfKernelFile
	mov es, ax
	mov bx, OffsetOfKernelFile
	xor ah, ah
	mov al, byte [wSectorNo]
	mov cl, 1
	call RdSector
	
	mov si, KernelFileName
	mov di, OffsetOfKernelFile
	
	cld
	mov dx, 10h
LABEL_SEARCH_FOR_KERNELBIN:
	cmp dx, 0
	jz LABEL_GOTO_SECTOR_IN_ROOT_DIR
	dec dx
	mov cx, 11
	LABEL_CMP_FILENAME:
		cmp cx, 0
		jz LABEL_FILENAME_FOUND
		dec cx
		lodsb
		cmp al, byte [es:di]
		jz LABEL_GO_ON
		jmp LABEL_DIFFERENT
LABEL_GO_ON:
	inc di
	jmp LABEL_CMP_FILENAME
LABEL_DIFFERENT:
	and di, 0FFE0h
	add di, 20h
	mov si, KernelFileName
	jmp LABEL_SEARCH_FOR_KERNELBIN
			
LABEL_GOTO_SECTOR_IN_ROOT_DIR:
	inc byte [wSectorNo]
	jmp LABEL_SEARCH_IN_ROOT_DIR_BEGIN
	
;====================================
;method: LABEL_FILENAME_FOUND
;es kernel.bin ------ KERNEL.BIN 基地址
;bx kernel.bin ------ KERNEL.BIN 偏移地址
;di 找到kerner.bin目录的表项
LABEL_FILENAME_FOUND:		;找到KerNel.bin后到这里继续
	mov ax, RootDirSectors
	and di, 0FFE0h
	
	push eax
	mov eax, [es:di+01Ch]
	mov dword [dwKernelSize], eax,
	pop eax
	
	add di, 01Ah
	mov cx, word [es:di]
	push cx
	
	add cx, DeltaSectorNo
	add cx, ax
	
	mov ax, BaseOfKernelFile
	mov es, ax
	mov bx, OffsetOfKernelFile
	mov ax, cx
	
	;====================================
	;method: LABEL_GOON_LOADING_FILE
	;es kernel.bin ------ KERNEL.BIN 基地址
	;bx kernel.bin ------ KERNEL.BIN 偏移地址
	;ax 扇区开始位置
	LABEL_GOON_LOADING_FILE:
		push ax
		push bx
		;每读入一个扇区就打印一个点
		mov ah, 0Eh
		mov al, '.'
		mov bl, 07h
		int 10h
		
		pop bx
		pop ax  ;root 表项中的数据位置
		
		mov cl, 1
		call RdSector		              
		pop	ax
		
		call GetFATEntry
		cmp ax, 0FFFh
		jz LABEL_FILE_LOADER
		push ax
		mov dx, RootDirSectors
		add ax, dx
		add ax, DeltaSectorNo
		add bx, [BPB_BytsPerSec]
		jmp LABEL_GOON_LOADING_FILE
		
LABEL_FILE_LOADER:
	call KillMotor	; 关闭软驱马达
	
	call CLEARSCREEN
	
	
	;加载GDT
	lgdt [GdtPtr]
	
	;关闭中断
	cli
	
	; 打开地址线A20
	in al, 92h
	or al, 00000010b
	out 92h, al
	
	; 准备切换到保护模式
	mov eax, cr0
	or eax, 1
	mov cr0,eax
	jmp dword SelectorFlatC:(BaseOfLoaderPhyAddr + LABEL_PM_START)
	
	jmp $
	
%ifdef	_LOADER_DEBUG_
	mov ax, 4c00h
	int 21h
%else
	jmp $
%endif
;===================================================
;变量
wRootDirSizeForLoop	db	RootDirSectors ;剩余可读的根目录扇区数量
BootMessageLen		equ	13
MessageLength		equ 9
wSectorNo			db	0
dwKernelSize		dd	0
bOdd				db	0		; 奇数还是偶数
;字符串	
BootMessage:		db	"Start Loader!"
KernelFileName		db	"KERNEL  BIN", 0  ;KERNEL.BIN 之文件名
LoadMessage			db	"Loading.."
ReadyMsg			db	"Ready..  "
NoKernelMsg			db	"No KERNEL"
HasKernelMsg		db	"HasKerNel"
HadOkMsg			db	"Kernel ok"
;--------------------------------------------------------------------
;method RdSector
;读取扇区 ES:BX = 缓冲地址 AX=开始扇区地址 CL=读取扇区个数
RdSector:
	push bp
	mov bp, sp
	sub esp, 2
	
	mov byte [bp - 2], cl
	push bx
	mov bl, byte [BPB_SecPerTrk]
	div bl
	mov cl, ah
	inc cl
	mov ch, al
	shr ch, 1
	mov dh, al
	and dh, 1	
	mov dl, byte [BS_DrvNum]
	pop bx
	.ReReading:
		mov ah, 02h
		mov al, byte [bp - 2]
		int 13h
		jc .ReReading
	pop bp
	add esp, 2
	ret
	
;--------------------------------------------------------------------
;method LABEL_NO_KERNELBIN
;没有在根目录中找到Kernel.bin文件
LABEL_NO_KERNELBIN:
	mov bp, NoKernelMsg
	mov cx, MessageLength
	
	call PRINTSTR
%ifdef	_LOADER_DEBUG_
	mov ax, 4c00h
	int 21h
%else
	jmp $
%endif	
;--------------------------------------------------------------------
;method PRINTSTR
;打印字符串 ES:BP = 串地址 CX = 串长度
PRINTSTR:
	mov	ax, 01301h		; AH = 13,  AL = 01h
	mov	bx, 0007h		; 页号为0(BH = 0) 黑底白字(BL = 07h)
	mov	dh, 5h
	mov	dl, 20h
	int	10h			; int 10h
	ret
;--------------------------------------------------------------------
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
	
	
;----------------------------------------------------------------------------
; 函数名: GetFATEntry
;----------------------------------------------------------------------------
; 作用:
;	找到序号为 ax 的 Sector 在 FAT 中的条目, 结果放在 ax 中
;	需要注意的是, 中间需要读 FAT 的扇区到 es:bx 处, 所以函数一开始保存了 es 和 bx
GetFATEntry:
	push	es
	push	bx
	push	ax
	mov	ax, BaseOfKernelFile	; ┓
	sub	ax, 0100h		; ┣ 在 BaseOfKernelFile 后面留出 4K 空间用于存放 FAT
	mov	es, ax			; ┛
	pop	ax
	mov	byte [bOdd], 0
	mov	bx, 3
	mul	bx			; dx:ax = ax * 3
	mov	bx, 2
	div	bx			; dx:ax / 2  ==>  ax <- 商, dx <- 余数
	cmp	dx, 0
	jz	LABEL_EVEN
	mov	byte [bOdd], 1
LABEL_EVEN:;偶数
	xor	dx, dx			; 现在 ax 中是 FATEntry 在 FAT 中的偏移量. 下面来计算 FATEntry 在哪个扇区中(FAT占用不止一个扇区)
	mov	bx, [BPB_BytsPerSec]
	div	bx			; dx:ax / BPB_BytsPerSec  ==>	ax <- 商   (FATEntry 所在的扇区相对于 FAT 来说的扇区号)
					;				dx <- 余数 (FATEntry 在扇区内的偏移)。
	push	dx
	mov	bx, 0			; bx <- 0	于是, es:bx = (BaseOfKernelFile - 100):00 = (BaseOfKernelFile - 100) * 10h
	add	ax, SectorNoOfFAT1	; 此句执行之后的 ax 就是 FATEntry 所在的扇区号
	mov	cl, 2
	call RdSector; 读取 FATEntry 所在的扇区, 一次读两个, 避免在边界发生错误, 因为一个 FATEntry 可能跨越两个扇区
	pop	dx
	add	bx, dx
	mov	ax, [es:bx]
	cmp	byte [bOdd], 1
	jnz	LABEL_EVEN_2
	shr	ax, 4
LABEL_EVEN_2:
	and	ax, 0FFFh

LABEL_GET_FAT_ENRY_OK:

	pop	bx
	pop	es
	ret
;----------------------------------------------------------------------------
;----------------------------------------------------------------------------
; 函数名: KillMotor
;----------------------------------------------------------------------------
; 作用:
;	关闭软驱马达
KillMotor:
	push dx
	mov dx, 03F2h
	mov al, 0
	out dx, al
	pop dx
	ret
;----------------------------------------------------------------------------

; 从此以后的代码在保护模式下运行
; 32 位代码段。由实模式跳入
[SECTION	.s32]

ALIGN	32

[BITS	32]

LABEL_PM_START:
	mov ax, SelectorVideo
	mov gs, ax
	
	mov ax, SelectorFlatRW
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov ss, ax
	mov esp, TopOfStack
	
	call DispMemInfo
	call SetupPaging

	call	InitKernel
	;jmp $
	jmp	SelectorFlatC:KernelEntryPointPhyAddr	; 正式进入内核 *
;%include	"lib.inc"

; 显示内存信息 --------------------------------------------------------------
DispMemInfo:
	push esi
	push edi
	push ecx
	
	mov esi, MemChkBuf
	mov ecx, [dwMCRNumber]
.loop:				  ;{
	mov	edx, 5		  ;  for(int j=0;j<5;j++)//每次得到一个ARDS中的成员
	mov	edi, ARDStruct	  ;  {//依次显示:BaseAddrLow,BaseAddrHigh,LengthLow
.1:				  ;               LengthHigh,Type
	push	dword [esi]	  ;
	call	DispInt		  ;    DispInt(MemChkBuf[j*4]); // 显示一个成员
	pop	eax		  ;
	stosd			  ;    ARDStruct[j*4] = MemChkBuf[j*4];
	add	esi, 4		  ;
	dec	edx		  ;
	cmp	edx, 0		  ;
	jnz	.1		  ;  }
	call	DispReturn	  ;  printf("\n");
	cmp	dword [dwType], 1 ;  if(Type == AddressRangeMemory)
	jne	.2		  ;  {
	mov	eax, [dwBaseAddrLow];
	add	eax, [dwLengthLow];
	cmp	eax, [dwMemSize]  ;    if(BaseAddrLow + LengthLow > MemSize)
	jb	.2		  ;
	mov	[dwMemSize], eax  ;    MemSize = BaseAddrLow + LengthLow;
.2:				  ;  }
	loop	.loop		  ;}
				  ;
	call	DispReturn	  ;printf("\n");
	push	szRAMSize	  ;
	call	DispStr		  ;printf("RAM size:");
	add	esp, 4		  ;
				  ;
	push	dword [dwMemSize] ;
	call	DispInt		  ;DispInt(MemSize);
	add	esp, 4		  ;

	pop	ecx
	pop	edi
	pop	esi
	ret	
; ---------------------------------------------------------------------------

; 启动分页机制 --------------------------------------------------------------
SetupPaging:
	; 根据内存大小计算应初始化多少PDE以及多少页表
	xor	edx, edx
	mov eax, [dwMemSize]
	mov ebx, 400000h
	div ebx
	mov	ecx, eax	; 此时 ecx 为页表的个数，也即 PDE 应该的个数
	test	edx, edx
	jz	.no_remainder
	inc	ecx		; 如果余数不为 0 就需增加一个页表
.no_remainder:
	push	ecx		; 暂存页表个数

	; 为简化处理, 所有线性地址对应相等的物理地址. 并且不考虑内存空洞.

	; 首先初始化页目录
	mov	ax, SelectorFlatRW
	mov	es, ax
	mov	edi, PageDirBase	; 此段首地址为 PageDirBase
	xor	eax, eax
	mov	eax, PageTblBase | PG_P  | PG_USU | PG_RWW
.1:
	stosd
	add	eax, 4096		; 为了简化, 所有页表在内存中是连续的
	loop	.1

	; 再初始化所有页表
	pop	eax			; 页表个数
	mov	ebx, 1024		; 每个页表 1024 个 PTE
	mul	ebx
	mov	ecx, eax		; PTE个数 = 页表个数 * 1024
	mov	edi, PageTblBase	; 此段首地址为 PageTblBase
	xor	eax, eax
	mov	eax, PG_P  | PG_USU | PG_RWW
.2:
	stosd
	add	eax, 4096		; 每一页指向 4K 的空间
	loop	.2

	mov	eax, PageDirBase
	mov	cr3, eax
	mov	eax, cr0
	or	eax, 80000000h
	mov	cr0, eax
	jmp	short .3
.3:
	nop

	ret
; 分页机制启动完毕 ----------------------------------------------------------


; ------------------------------------------------------------------------
; 显示 AL 中的数字
; ------------------------------------------------------------------------
DispAL:
	push	ecx
	push	edx
	push	edi

	mov	edi, [dwDispPos]

	mov	ah, 0Fh			; 0000b: 黑底    1111b: 白字
	mov	dl, al
	shr	al, 4
	mov	ecx, 2
.begin:
	and	al, 01111b
	cmp	al, 9
	ja	.1
	add	al, '0'
	jmp	.2
.1:
	sub	al, 0Ah
	add	al, 'A'
.2:
	mov	[gs:edi], ax
	add	edi, 2

	mov	al, dl
	loop	.begin
	;add	edi, 2

	mov	[dwDispPos], edi

	pop	edi
	pop	edx
	pop	ecx

	ret
; DispAL 结束-------------------------------------------------------------


; ------------------------------------------------------------------------
; 显示一个整形数
; ------------------------------------------------------------------------
DispInt:
	mov	eax, [esp + 4]
	shr	eax, 24
	call	DispAL

	mov	eax, [esp + 4]
	shr	eax, 16
	call	DispAL

	mov	eax, [esp + 4]
	shr	eax, 8
	call	DispAL

	mov	eax, [esp + 4]
	call	DispAL

	mov	ah, 07h			; 0000b: 黑底    0111b: 灰字
	mov	al, 'h'
	push	edi
	mov	edi, [dwDispPos]
	mov	[gs:edi], ax
	add	edi, 4
	mov	[dwDispPos], edi
	pop	edi

	ret
; DispInt 结束------------------------------------------------------------

; ------------------------------------------------------------------------
; 显示一个字符串
; ------------------------------------------------------------------------
DispStr:
	push	ebp
	mov	ebp, esp
	push	ebx
	push	esi
	push	edi

	mov	esi, [ebp + 8]	; pszInfo
	mov	edi, [dwDispPos]
	mov	ah, 0Fh
.1:
	lodsb
	test	al, al
	jz	.2
	cmp	al, 0Ah	; 是回车吗?
	jnz	.3
	push	eax
	mov	eax, edi
	mov	bl, 160
	div	bl
	and	eax, 0FFh
	inc	eax
	mov	bl, 160
	mul	bl
	mov	edi, eax
	pop	eax
	jmp	.1
.3:
	mov	[gs:edi], ax
	add	edi, 2
	jmp	.1

.2:
	mov	[dwDispPos], edi

	pop	edi
	pop	esi
	pop	ebx
	pop	ebp
	ret
; DispStr 结束------------------------------------------------------------

; ------------------------------------------------------------------------
; 换行
; ------------------------------------------------------------------------
DispReturn:
	push	szReturn
	call	DispStr			;printf("\n");
	add	esp, 4

	ret
; DispReturn 结束---------------------------------------------------------


; ------------------------------------------------------------------------
; 内存拷贝，仿 memcpy
; ------------------------------------------------------------------------
; void* MemCpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
MemCpy:
	push	ebp
	mov	ebp, esp

	push	esi
	push	edi
	push	ecx

	mov	edi, [ebp + 8]	; Destination
	mov	esi, [ebp + 12]	; Source
	mov	ecx, [ebp + 16]	; Counter
.1:
	cmp	ecx, 0		; 判断计数器
	jz	.2		; 计数器为零时跳出

	mov	al, [ds:esi]		; ┓
	inc	esi			; ┃
					; ┣ 逐字节移动
	mov	byte [es:edi], al	; ┃
	inc	edi			; ┛

	dec	ecx		; 计数器减一
	jmp	.1		; 循环
.2:
	mov	eax, [ebp + 8]	; 返回值

	pop	ecx
	pop	edi
	pop	esi
	mov	esp, ebp
	pop	ebp

	ret			; 函数结束，返回
; MemCpy 结束-------------------------------------------------------------


; InitKernel ---------------------------------------------------------------------------------
; 将 KERNEL.BIN 的内容经过整理对齐后放到新的位置
; 遍历每一个 Program Header，根据 Program Header 中的信息来确定把什么放进内存，放到什么位置，以及放多少。
; --------------------------------------------------------------------------------------------
InitKernel:
    xor	esi, esi
	mov	cx, word [BaseOfKernelFilePhyAddr + 2Ch]; ┓ ecx <- pELFHdr->e_phnum
	movzx	ecx, cx					; ┛
	mov	esi, [BaseOfKernelFilePhyAddr + 1Ch]	; esi <- pELFHdr->e_phoff
	add	esi, BaseOfKernelFilePhyAddr		; esi <- OffsetOfKernel + pELFHdr->e_phoff
.Begin:
	mov	eax, [esi + 0]
	cmp	eax, 0				; PT_NULL
	jz	.NoAction
	push	dword [esi + 010h]		; size	┓
	mov	eax, [esi + 04h]		;	┃
	add	eax, BaseOfKernelFilePhyAddr	;	┣ ::memcpy(	(void*)(pPHdr->p_vaddr),
	push	eax				; src	┃		uchCode + pPHdr->p_offset,
	push	dword [esi + 08h]		; dst	┃		pPHdr->p_filesz;
	call	MemCpy				;	┃
	add	esp, 12				;	┛
.NoAction:
	add	esi, 020h			; esi += pELFHdr->e_phentsize
	dec	ecx
	jnz	.Begin

	ret
; InitKernel ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

; SECTION .data1 之开始 ---------------------------------------------------------------------------------------------
[SECTION .data1]
ALIGN 32
LABEL_DATA:
; 实模式下使用这些符号
; 字符串
_szMemChkTitle:	db "BaseAddrL BaseAddrH LengthLow LengthHigh   Type", 0Ah, 0
_szRAMSize:	db "RAM size:", 0
_szReturn:	db 0Ah, 0
;; 变量
_dwMCRNumber:	dd 0	; Memory Check Result
_dwDispPos:	dd (80 * 6 + 0) * 2	; 屏幕第 6 行, 第 0 列
_dwMemSize:	dd 0
_ARDStruct:	; Address Range Descriptor Structure
  _dwBaseAddrLow:		dd	0
  _dwBaseAddrHigh:		dd	0
  _dwLengthLow:			dd	0
  _dwLengthHigh:		dd	0
  _dwType:			dd	0
_MemChkBuf:	times	256	db	0
;; 保护模式下使用这些符号
szMemChkTitle		equ	BaseOfLoaderPhyAddr + _szMemChkTitle
szRAMSize		equ	BaseOfLoaderPhyAddr + _szRAMSize
szReturn		equ	BaseOfLoaderPhyAddr + _szReturn
dwDispPos		equ	BaseOfLoaderPhyAddr + _dwDispPos
dwMemSize		equ	BaseOfLoaderPhyAddr + _dwMemSize
dwMCRNumber		equ	BaseOfLoaderPhyAddr + _dwMCRNumber
ARDStruct		equ	BaseOfLoaderPhyAddr + _ARDStruct
	dwBaseAddrLow	equ	BaseOfLoaderPhyAddr + _dwBaseAddrLow
	dwBaseAddrHigh	equ	BaseOfLoaderPhyAddr + _dwBaseAddrHigh
	dwLengthLow	equ	BaseOfLoaderPhyAddr + _dwLengthLow
	dwLengthHigh	equ	BaseOfLoaderPhyAddr + _dwLengthHigh
	dwType		equ	BaseOfLoaderPhyAddr + _dwType
MemChkBuf		equ	BaseOfLoaderPhyAddr + _MemChkBuf

	
StackSpace: 	times	1024	db 0
TopOfStack		equ		BaseOfLoaderPhyAddr + $ ; 栈顶	