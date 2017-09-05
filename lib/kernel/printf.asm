RPL0        equ          00b
TI_GDT      equ          000b
SELECTOR_VIDEO  equ  (0x3 << 3) + TI_GDT + RPL0   ;  video selector
[bits 32]
section  .text:
global put_char
put_char:
         pushad
	 mov ax,SELECTOR_VIDEO
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
mov ecx,[esp+36]
cmp cl,0xd
jz  .is_carriage_return
cmp cl,0xa
jz .is_line_feed
cmp cl,0x8
jz  .is_backspace
jmp .put_other

.is_backspace:
              dec bx
	      shl bx,1
	      mov byte [gs:bx],0x20
	      inc bx
	      mov byte [gs:bx],0x7
	      shr bx,1
	      jmp .set_cursor

.put_other:
            shl bx,1
	    mov byte [gs:bx],cl
            inc bx
	    mov byte [gs:bx],0x7
	    shr bx,1
	    inc bx
	    cmp bx,2000
	    jl  .set_cursor

.is_line_feed:
.is_carriage_return:
                    xor dx,dx
		    mov ax,bx
		    mov si,80
		    div si
		    sub bx,dx


.is_carriage_return_end:
                        add bx,80
			cmp bx,2000

.is_line_feed_end:
                       jl .set_cursor


.roll_screen:
             cld
	     mov ecx,960
	     mov esi,0xc00b80a0
	     mov edi,0xc00b8000
	     rep movsd
	     mov ebx,3840
	     mov ecx,80


.cls:
         mov word [gs:ebx],0x720
	 add ebx,2
	 loop .cls
	 mov  bx,1920



.set_cursor:
             mov dx,0x3d4
	     mov al,0xe
	     out dx,al
	     mov dx,0x3d5
	     mov al,bh
	     out dx,al

	     mov dx,0x3d4
	     mov al,0xf
	     out dx,al
	     mov dx,0x3d5
	     mov al,bl
	     out dx,al


.put_char_done:
               popad
	       ret

	    

