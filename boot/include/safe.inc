DA_32		EQU	4000h
DA_LIMIT_4K	EQU	8000h

DA_DPL0		EQU	00h  ; DPL = 0
DA_DPL1		EQU	20h  ; DPL = 1
DA_DPL2		EQU	40h  ; DPL = 2
DA_DPL3		EQU	60h  ; DPL = 3

DA_DR		EQU	90h
DA_DRW		EQU	92h
DA_DRWA		EQU	93h
DA_C		EQU	98h
DA_CR		EQU	9Ah
DA_CCO		EQU	9Ch
DA_CCOR		EQU	9Eh
;----------------------------------------------------------------------------
; 系统段描述符类型值说明
;----------------------------------------------------------------------------
DA_LDT		EQU	  82h	; 局部描述符表段类型值
DA_TaskGate	EQU	  85h	; 任务门类型值
DA_386TSS	EQU	  89h	; 可用 386 任务状态段类型值
DA_386CGate	EQU	  8Ch	; 386 调用门类型值
DA_386IGate	EQU	  8Eh	; 386 中断门类型值
DA_386TGate	EQU	  8Fh	; 386 陷阱门类型值
;----------------------------------------------------------------------------


; 选择子图示:
;         ┏━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┳━━┓
;         ┃ 15 ┃ 14 ┃ 13 ┃ 12 ┃ 11 ┃ 10 ┃ 9  ┃ 8  ┃ 7  ┃ 6  ┃ 5  ┃ 4  ┃ 3  ┃ 2  ┃ 1  ┃ 0  ┃
;         ┣━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━┻━━╋━━╋━━┻━━┫
;         ┃                                 描述符索引                                 ┃ TI ┃   RPL    ┃
;         ┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┻━━┻━━━━━┛
;
; RPL(Requested Privilege Level): 请求特权级，用于特权检查。
;
; TI(Table Indicator): 引用描述符表指示位
;	TI=0 指示从全局描述符表GDT中读取描述符；
;	TI=1 指示从局部描述符表LDT中读取描述符。
;

;----------------------------------------------------------------------------
; 选择子类型值说明
; 其中:
;       SA_  : Selector Attribute

SA_RPL0		EQU	0	; ┓
SA_RPL1		EQU	1	; ┣ RPL
SA_RPL2		EQU	2	; ┃
SA_RPL3		EQU	3	; ┛

SA_TIG		EQU	0	; ┓TI
SA_TIL		EQU	4	; ┛
;----------------------------------------------------------------------------


;----------------------------------------------------------------------------
; 分页机制使用的常量说明
;----------------------------------------------------------------------------
PG_P		EQU	1	; 页存在属性位
PG_RWR		EQU	0	; R/W 属性位值, 读/执行
PG_RWW		EQU	2	; R/W 属性位值, 读/写/执行
PG_USS		EQU	0	; U/S 属性位值, 系统级
PG_USU		EQU	4	; U/S 属性位值, 用户级
;----------------------------------------------------------------------------

; 宏 ------------------------------------------------------------------------------------------------------
;
; 描述符
; usage: Descriptor Base, Limit, Attr
;        Base:  dd
;        Limit: dd (low 20 bits available)
;        Attr:  dw (lower 4 bits of higher byte are always 0)
%macro	Descriptor	3
	dw	%2	&	0FFFFh
	dw	%1	&	0FFFFh
	db	(%1 >> 16)	& 0FFh
	dw	((%2 >> 8)  & 0F00h) | (%3 & 0F0FFh)
	db	(%1 >> 24)	&	0FFh
%endmacro

;
; 门
; usage: Gate Selector, Offset, DCount, Attr
;        Selector:  dw
;        Offset:    dd
;        DCount:    db
;        Attr:      db
%macro	Gate 4
	dw	(%2	& 0FFFFh)
	dw	%1
	dw	(%3	& 1Fh) | ((%4 << 8) & 0FF00h)
	dw	((%2 >> 16) & 0FFFFh)
%endmacro

