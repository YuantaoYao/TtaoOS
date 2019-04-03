##################################################
# Makefile of system
##################################################
# pragram of system
ASM = nasm
DASM = ndisasm
CC = gcc
LD = ld

# Director of system
d_boot:=boot/
d_include:=include/
d_kernel:=kernel/
d_lib:=lib/
d_floppy:=/mnt/floppy/

#KernelEntryPointPhyAddr
v_entrypoint = 0x30400
# Offset of entry point in kernel file
v_entryoffset = 0x400

f_kernel:=kernel.asm
f_img:=a.img

#parament of system
p_bootinclude = -I boot/include/
p_include = -I include/ -f elf
p_gccflags	  = -I include/ -c -m32  -fno-builtin -fno-stack-protector
p_ldflags	  = -m elf_i386	-s -Ttext $(v_entrypoint)

install : image cleanall

image : bootstrap buildimg

cleanall : 
	rm -f $(d_boot)*.o $(d_boot)*.bin \
			  $(d_kernel)*.o $(d_kernel)*.bin\
			  $(d_lib)*.o $(d_lib)*.bin
	
# This Program
buildimg : $(d_kernel)kernel.bin
	dd if=$(d_boot)boot.bin of=$(f_img) bs=512 count=1 conv=notrunc
	sudo mount -o loop $(f_img) $(d_floppy)
	sudo cp -fv $(d_boot)loader.bin $(d_floppy) 
	sudo cp -fv $(d_kernel)kernel.bin $(d_floppy) 
	sudo umount $(d_floppy)	
	
#boot/boot.bin : boot/boot.asm boot/include/loader.inc \
#				boot/include/safe.inc boot/include/fat12hdr.inc
#	$(ASM) $(p_bootinclude) -o $@ $<
	
#boot/loader.bin : boot/loader.asm boot/include/loader.inc \
#				  boot/include/fat12hdr.inc boot/include/safe.inc
#	$(ASM) $(p_bootinclude) -o $@ $<

bootstrap:
	(cd boot/; make)
	
$(d_kernel)kernel.bin : $(d_kernel)kernel.o $(d_lib)klib.o $(d_kernel)start.o $(d_lib)string.o $(d_lib)kliba.o \
						$(d_kernel)global.o $(d_kernel)protect.o $(d_kernel)protectint.o $(d_kernel)8259A.o	$(d_kernel)8259Aint.o $(d_kernel)main.o $(d_kernel)process.o
	$(LD) $(p_ldflags) -o $@ $(d_kernel)kernel.o $(d_lib)klib.o $(d_kernel)start.o $(d_lib)string.o $(d_lib)kliba.o \
							 $(d_kernel)global.o $(d_kernel)protect.o $(d_kernel)protectint.o $(d_kernel)8259A.o $(d_kernel)8259Aint.o $(d_kernel)main.o $(d_kernel)process.o
	
$(d_kernel)kernel.o : $(d_kernel)kernel.asm $(d_kernel)start.c
	$(ASM) $(p_include) -o $@ $<

$(d_kernel)start.o : $(d_kernel)start.c
	$(CC) $(p_gccflags) -o $@ $<
	
$(d_kernel)main.o : $(d_kernel)main.c
	$(CC) $(p_gccflags) -o $@ $<
	
$(d_kernel)global.o : $(d_kernel)global.c
	$(CC) $(p_gccflags) -o $@ $<
	
$(d_kernel)8259Aint.o : $(d_kernel)8259A.asm $(d_kernel)8259A.c
	$(ASM) $(p_include) -o $@ $<

$(d_kernel)8259A.o : $(d_kernel)8259A.c
	$(CC) $(p_gccflags) -o $@ $<

$(d_kernel)process.o : $(d_kernel)process.asm 
	$(ASM) $(p_include) -o $@ $<

$(d_kernel)protectint.o : $(d_kernel)protect.asm $(d_kernel)protect.c
	$(ASM) $(p_include) -o $@ $<

$(d_kernel)protect.o : $(d_kernel)protect.c
	$(CC) $(p_gccflags) -o $@ $<

$(d_lib)klib.o :$(d_lib)klib.c
	$(CC) $(p_gccflags) -o $@ $<

$(d_lib)string.o : $(d_lib)string.asm
	$(ASM) -f elf -o $@ $<

$(d_lib)kliba.o : $(d_lib)kliba.asm
	$(ASM) -f elf  -o $@ $<
	
# 需要链接的文件	
