%include "boot.inc"
org  0x7c00
mov ax,cs
mov ds,ax
mov sp,0x8c00
mov ah,0x06
mov bh,0x07
mov cx,0
mov dx,0x184f
int 0x10

;-----------------------------------------------------------------------------------------------------------------------
  mov ax,0xb800
  mov gs,ax
  mov ax,0
  mov es,ax
  mov ax,bootmessage
  mov bx,ax
  mov di,0                            ;初始化，将字符串地址传递DS，显存地址传递GS，si,di置零.
  mov cx,30

DispStr:
    push cx
	mov cx,0
	mov byte cl,[es:bx]
	mov byte [gs:di],cl
	mov byte [gs:di+1],0x4
	inc bx
	inc di
	inc di
	pop cx
    loop DispStr
       
mov eax,LOADER_START_SECTOR
mov bx,LOADER_BASE_ADDR
mov cx,10
call rd_disk_m_16
jmp  LOADER_BASE_ADDR


;十六位模式下读取硬盘
rd_disk_m_16:
mov esi,eax
mov di,cx
mov dx,0x1f2
mov al,cl
out dx,al

mov eax,esi

mov dx,0x1f3
out dx,al

mov cl,8
shr eax,cl
mov dx,0x1f4
out dx,al

shr eax,cl
mov dx,0x1f5
out dx,al

shr eax,cl
and al,0x0f
or al,0xe0
mov dx,0x1f6
out dx,al

mov dx,0x1f7
mov al,0x20
out dx,al

not_ready:
nop
in al,dx
and al,0x88

cmp al,0x08
jnz not_ready

mov ax,di
mov dx,256
mul dx
mov cx,ax

mov dx,0x1f0

go_on_read:
in ax,dx
mov [bx],ax
add bx,2
loop go_on_read
ret

bootmessage:  db   "booting system"
times  510-($-$$)  db       0
dw 0xaa55
