object = build
startaddr = -Ttext 0xc0001500
lib = -I lib/ -I lib/kernel/ -g 
c_filety = -c -m32 -fno-builtin -fno-stack-protector  -o
ld_filety = -e main -melf_i386 -o
filety = -f elf -o
libk = lib/kernel
OBJS = $(object)/main.o $(object)/printf.o $(object)/kernel.o $(object)/init.o\
$(object)/interrupt.o $(object)/timer.o $(object)/debug.o $(object)/string.o\
$(object)/bitmap.o $(object)/memory.o $(object)/thread.o $(object)/list.o   \
$(object)/switch.o

all: $(object)/kernel.bin	
	dd if=$(object)/kernel.bin of=hd60.img bs=512 count=200 seek=9 conv=notrunc    
	@echo "hd finish"
#	rm build/*
	bochs -f bochsrc.disk

$(object)/main.o: kernel/main.c  $(libk)/init.h $(libk)/printf.h\
	$(libk)/memory.h $(libk)/thread.h
		gcc $(lib)  $(c_filety) build/main.o kernel/main.c

$(object)/printf.o: $(libk)/printf.asm $(libk)/printf.h  
		nasm $(lib) $(filety)  build/printf.o $(libk)/printf.asm

$(object)/kernel.o: kernel/kernel.asm
		nasm $(lib) $(filety)  build/kernel.o kernel/kernel.asm

$(object)/switch.o: kernel/switch.asm
		nasm $(lib) $(filety) build/switch.o kernel/switch.asm

$(object)/init.o: kernel/init.c  $(libk)/init.h   \
	$(libk)/printf.h $(libk)/interrupt.h $(libk)/timer.h\
	$(libk)/memory.h
		gcc $(lib)  $(c_filety)  build/init.o kernel/init.c

$(object)/interrupt.o: kernel/interrupt.c $(libk)/interrupt.h\
	$(libk)/memory.h lib/stdint.h $(libk)/global.h $(libk)/io.h\
	$(libk)/printf.h
		gcc  $(lib) $(c_filety) build/interrupt.o kernel/interrupt.c

$(object)/timer.o: device/timer.c  $(libk)/timer.h $(libk)/io.h\
	$(libk)/printf.h
		gcc $(lib)  $(c_filety) build/timer.o device/timer.c

$(object)/debug.o: kernel/debug.c $(libk)/debug.h $(libk)/printf.h\
	$(libk)/interrupt.h
	  gcc $(lib)  $(c_filety) build/debug.o kernel/debug.c

$(object)/string.o: lib/string.c $(libk)/string.h $(libk)/debug.h\
	$(libk)/global.h 
		gcc $(lib)  $(c_filety) build/string.o lib/string.c 

$(object)/bitmap.o: kernel/bitmap.c $(libk)/bitmap.h $(libk)/debug.h\
	$(libk)/interrupt.h $(libk)/printf.h $(libk)/string.h lib/stdint.h
		gcc $(lib)  $(c_filety) build/bitmap.o kernel/bitmap.c 

$(object)/memory.o: kernel/memory.c $(libk)/memory.h $(libk)/global.h\
	$(libk)/printf.h $(libk)/debug.h $(libk)/string.h lib/stdint.h
		gcc $(lib)  $(c_filety) build/memory.o kernel/memory.c

$(object)/thread.o: kernel/thread.c $(libk)/thread.h $(libk)/global.h\
	$(libk)/memory.h $(libk)/string.h $(libk)/list.h
		gcc $(lib)	$(c_filety) build/thread.o kernel/thread.c

$(object)/list.o: kernel/list.c   $(libk)/list.h $(libk)/interrupt.h 
	  gcc $(lib)  $(c_filety) $(object)/list.o kernel/list.c

$(object)/kernel.bin: $(OBJS)
		ld  $(startaddr) $(ld_filety) build/kernel.bin $(OBJS) 
