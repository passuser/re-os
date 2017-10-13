object = ./build
startaddr = 0xc0001500
lib = -I lib/ -I lib/kernel/ -I device/
c_filety = -c -m32  -o
filety = -f elf -o
all: $(object)/kernel.bin	
	dd if=$(object)/kernel.bin of=hd60.img bs=512 count=200 seek=9 conv=notrunc    
	@echo "hd finish"
	rm build/*
	bochs -f bochsrc.disk
OBJS = $(object)/kernel.o $(object)/printf.o $(object)/main.o $(object)/init.o $(object)/interrupt.o 
$(object)/interrupt.o: kernel/interrupt.c 
		gcc  $(lib) $(c_filety) build/interrupt.o kernel/interrupt.c
$(object)/init.o: kernel/init.c  
		gcc $(lib)  $(c_filety)  build/init.o kernel/init.c
$(object)/main.o: kernel/main.c
		gcc $(lib)  $(c_filety) build/main.o kernel/main.c
$(object)/timer.o: device/timer.c
		gcc $(lib)  $(c_filety) build/timer.o device/timer.c
$(object)/printf.o: lib/kernel/printf.asm 
		nasm $(lib) $(filety)  build/printf.o lib/kernel/printf.asm
$(object)/kernel.o: kernel/kernel.asm
		nasm $(lib) $(filety)  build/kernel.o kernel/kernel.asm
$(object)/kernel.bin: $(OBJS)
		ld -Ttext $(startaddr) -e main -melf_i386 -o build/kernel.bin $(OBJS)
