object =build
startaddr = -Ttext 0xc0001500
lib = -I lib/ -I lib/kernel/ 
c_filety = -c -m32  -o
ld_filety = -e main -melf_i386 -o
filety = -f elf -o
OBJS = $(object)/main.o $(object)/printf.o $(object)/kernel.o $(object)/init.o $(object)/interrupt.o $(object)/timer.o 
all: $(object)/kernel.bin	
	dd if=$(object)/kernel.bin of=hd60.img bs=512 count=200 seek=9 conv=notrunc    
	@echo "hd finish"
#	rm build/*
	bochs -f bochsrc.disk
$(object)/interrupt.o: kernel/interrupt.c lib/kernel/interrupt.h 
		gcc  $(lib) $(c_filety) build/interrupt.o kernel/interrupt.c
$(object)/init.o: kernel/init.c  lib/kernel/init.h
		gcc $(lib)  $(c_filety)  build/init.o kernel/init.c
$(object)/main.o: kernel/main.c  
		gcc $(lib)  $(c_filety) build/main.o kernel/main.c
$(object)/timer.o: device/timer.c  lib/kernel/timer.h 
		gcc $(lib)  $(c_filety) build/timer.o device/timer.c
$(object)/printf.o: lib/kernel/printf.asm lib/kernel/printf.h
		nasm $(lib) $(filety)  build/printf.o lib/kernel/printf.asm
$(object)/kernel.o: kernel/kernel.asm
		nasm $(lib) $(filety)  build/kernel.o kernel/kernel.asm
$(object)/kernel.bin: $(OBJS)
		ld $(startaddr) $(ld_filety) build/kernel.bin $(OBJS) 
