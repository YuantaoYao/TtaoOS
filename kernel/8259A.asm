extern spurious_irq

global hwint00
global hwint01
global hwint02
global hwint03
global hwint04
global hwint05
global hwint06
global hwint07
global hwint08
global hwint09
global hwint0A
global hwint0B
global hwint0C
global hwint0D
global hwint0E
global hwint0F

%macro irq_master 1
	push %1
	call spurious_irq
	add esp, 4
	hlt
%endmacro

%macro irq_slave 1
	push %1
	call spurious_irq
	add esp, 4
	hlt
%endmacro

ALIGN 16
hwint00:
	irq_master 0
	
ALIGN 16
hwint01:
	irq_master 1
	
ALIGN 16
hwint02:
	irq_master 2
	
ALIGN 16
hwint03:
	irq_master 3
	
ALIGN 16
hwint04:
	irq_master 4
	
ALIGN 16
hwint05:
	irq_master 5
	
ALIGN 16
hwint06:
	irq_master 6
	
ALIGN 16
hwint07:
	irq_master 7
	
ALIGN 16
hwint08:
	irq_slave 8
	
ALIGN 16
hwint09:
	irq_slave 9

ALIGN 16
hwint0A:
	irq_slave 10

ALIGN 16
hwint0B:
	irq_slave 11

ALIGN 16
hwint0C:
	irq_slave 12

ALIGN 16
hwint0D:
	irq_slave 13

ALIGN 16
hwint0E:
	irq_slave 14

ALIGN 16
hwint0F:
	irq_slave 15