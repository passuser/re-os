object =build
startaddr = -Ttext 0xc0001500
lib = -I lib/ -I lib/kernel/ -g 
c_filety = -c -m32 -fno-builtin -fno-stack-protector  -o
ld_filety = -e main -melf_i386 -o
filety = -f elf -o
OBJS = $(object)/main.o $(object)/printf.o $(object)/kernel.o $(object)/init.o $(object)/interrupt.o $(object)/timer.o $(object)/debug.o \
			 $(object)/string.o $(object)/bitmap.o $(object)/memory.o

all: $(object)/kernel.bin	
	dd if=$(object)/kernel.bin of=hd60.img bs=512 count=200 seek=9 conv=notrunc    
	@echo "hd finish"
#	rm build/*
	bochs -f bochsrc.disk

$(object)/main.o: kernel/main.c  
		gcc $(lib)  $(c_filety) build/main.o kernel/main.c

$(object)/printf.o: lib/kernel/printf.asm lib/kernel/printf.h
		nasm $(lib) $(filety)  build/printf.o lib/kernel/printf.asm

$(object)/kernel.o: kernel/kernel.asm
		nasm $(lib) $(filety)  build/kernel.o kernel/kernel.asm

$(object)/init.o: kernel/init.c  lib/kernel/init.h
		gcc $(lib)  $(c_filety)  build/init.o kernel/init.c

$(object)/interrupt.o: kernel/interrupt.c lib/kernel/interrupt.h 
		gcc  $(lib) $(c_filety) build/interrupt.o kernel/interrupt.c

$(object)/timer.o: device/timer.c  lib/kernel/timer.h 
		gcc $(lib)  $(c_filety) build/timer.o device/timer.c

$(object)/debug.o: kernel/debug.c lib/kernel/debug.h
	  gcc $(lib)  $(c_filety) build/debug.o kernel/debug.c

$(object)/string.o: lib/string.c lib/kernel/string.h
		gcc $(lib)  $(c_filety) build/string.o lib/string.c 

$(object)/bitmap.o: kernel/bitmap.c lib/kernel/bitmap.h
		gcc $(lib)  $(c_filety) build/bitmap.o kernel/bitmap.c 

$(object)/memory.o: kernel/memory.c lib/kernel/memory.h
		gcc $(lib)  $(c_filety) build/memory.o kernel/memory.c

$(object)/kernel.bin: $(OBJS)
		ld  $(startaddr) $(ld_filety) build/kernel.bin $(OBJS) 
