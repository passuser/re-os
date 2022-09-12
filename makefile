build_dir = ./build
startaddr = 0xc0001500
lib = -I ./lib/ -I ./lib/kernel/  
c_filety = -c -m32 -fno-builtin -fno-stack-protector  -W\
-Wstrict-prototypes -Wmissing-prototypes
libk = ./lib/kernel

#####编译器
AS = nasm
CC = gcc
LD = ld

#####编译器参数
CFLAGS = -Wall $(lib) $(c_filety)
ASFLAGS = $(lib) -f elf 
LDFLAGS = -Ttext $(startaddr) -e main -melf_i386  -Map $(build_dir)/kernel.map

#########################         各模块文件        ###########

OBJS = $(build_dir)/main.o $(build_dir)/printf.o $(build_dir)/kernel.o $(build_dir)/init.o\
$(build_dir)/interrupt.o $(build_dir)/timer.o $(build_dir)/debug.o $(build_dir)/string.o\
$(build_dir)/bitmap.o $(build_dir)/memory.o $(build_dir)/thread.o $(build_dir)/list.o   \
$(build_dir)/switch.o $(build_dir)/sync.o $(build_dir)/console.o

###################             操作                 ############

all: $(build_dir)/kernel.bin	
	dd if=$(build_dir)/kernel.bin of=hd60.img bs=512 count=200 seek=9 conv=notrunc    
	@echo "hd finish"
#	rm build/*
	bochs -f bochsrc.disk

########               汇编编译                          #########

$(build_dir)/printf.o: $(libk)/printf.asm $(libk)/printf.h  
	$(AS) $(ASFLAGS) $< -o $@

$(build_dir)/kernel.o: kernel/kernel.asm
	$(AS) $(ASFLAGS) $< -o $@

$(build_dir)/switch.o: kernel/switch.asm
	$(AS) $(ASFLAGS) $< -o $@

#################        c语言编译                       #########

$(build_dir)/main.o: kernel/main.c  $(libk)/init.h $(libk)/printf.h\
	$(libk)/memory.h $(libk)/thread.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/init.o: kernel/init.c  $(libk)/init.h   \
	$(libk)/printf.h $(libk)/interrupt.h $(libk)/timer.h\
	$(libk)/memory.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/interrupt.o: kernel/interrupt.c $(libk)/interrupt.h\
	$(libk)/memory.h lib/stdint.h $(libk)/global.h $(libk)/io.h\
	$(libk)/printf.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/timer.o: device/timer.c  $(libk)/timer.h $(libk)/io.h\
	$(libk)/printf.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/debug.o: kernel/debug.c $(libk)/debug.h $(libk)/printf.h\
	$(libk)/interrupt.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/string.o: lib/string.c $(libk)/string.h $(libk)/debug.h\
	$(libk)/global.h 
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/bitmap.o: kernel/bitmap.c $(libk)/bitmap.h $(libk)/debug.h\
	$(libk)/interrupt.h $(libk)/printf.h $(libk)/string.h lib/stdint.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/memory.o: kernel/memory.c $(libk)/memory.h $(libk)/global.h\
	$(libk)/printf.h $(libk)/debug.h $(libk)/string.h lib/stdint.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/thread.o: kernel/thread.c $(libk)/thread.h $(libk)/global.h\
	$(libk)/memory.h $(libk)/string.h $(libk)/list.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/list.o: kernel/list.c   $(libk)/list.h $(libk)/interrupt.h 
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/sync.o: kernel/sync.c  $(libk)/sync.h $(libk)/list.h \
$(libk)/interrupt.h
	$(CC) $(CFLAGS) $< -o $@

$(build_dir)/console.o: device/console.c $(libk)/console.h $(libk)/sync.h\
$(libk)/thread.h
	$(CC) $(CFLAGS) $< -o $@

###################           链接目标文件                   ############

$(build_dir)/kernel.bin: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@
