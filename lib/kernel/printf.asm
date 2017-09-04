TI_GDT  equ  0
RPL0    equ  0
SELECTOR_VIDEO  equ  (0x3 << 3) + TI_GDT + RPL0

[bits 32]
section  .text

global put_char
put_char:
         pushad
	 mox ax,SELECTOR_VIDEO
	 mov gs,ax

mov dx,0x3d4
mov al,0xe
out dx,al
mov dx,0x3d5
in al,dx
mov ah,al

mov dx,0x3d4
mov al,0xf
out dx,al
mov dx,0x3d5
in al,dx

mov bx,ax
mov ecx,[exp+36]
cmp cl,0xd
jz  .is_carriage_return
cmp cl,0xa
jz .is_line_feed
cmp cl,0x8
jz  .is_backspace
jmp .put_other
