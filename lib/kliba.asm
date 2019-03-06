[section .text]

global	out_port
global	in_port
; ========================================================================
;                  void out_port(u16 port, u8 value);
; ========================================================================	
out_port:
	mov edx, [esp + 4] ;port
	mov al, [esp + 4 + 4] ;value
	out dx, al
	nop
	nop
	nop
	ret
	
; ========================================================================
;                  void in_port(u16 port);
; ========================================================================	
in_port:
	mov edx, [esp + 4] ;port
	xor eax, eax
	in al, dx
	nop
	nop
	nop
	ret