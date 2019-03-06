# Makefile fo boot

# Attribute 
BOOT:=boot.asm
LOADER:=loader.asm
include:=include/
FAT12HDR_INC:=fat12hdr.inc
LOADER_INC:=loader.inc
SAFE_INC:=safe.inc

# Programs, flags, etc
ASM = nasm
ASMFLAGS = -I include/

# this Programs
TARGET = $(subst .asm,.bin,$(BOOT)) $(subst .asm,.bin,$(LOADER))

#ALL phony Targets
.PHONY	: everyting clean all

# Default starting position
everyting : $(TARGET)
	
clean : 
	rm -f $(TARGET)
	
all : clean everyting

$(subst .asm,.bin,$(BOOT)) : $(BOOT) $(include)$(FAT12HDR_INC) $(include)$(LOADER_INC)
	$(ASM) $(ASMFLAGS) -o $@  $<
$(subst .asm,.bin,$(LOADER)) : $(LOADER) $(include)$(FAT12HDR_INC) $(include)$(LOADER_INC) $(include)$(SAFE_INC)
	$(ASM) $(ASMFLAGS) -o $@  $<