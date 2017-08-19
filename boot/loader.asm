%include "boot.inc"
org       LOADER_BASE_ADDR
LOADER_STACK_TOP   equ    LOADER_BASE_ADDR
jmp loader_start

;------------------------------------------构建GDT及其内部描述符------------------------------------------------------------------
GDT_BASE:           dd             0x00000000
                    dd             0x00000000
CODE_DESC:          dd             0x0000ffff
                    dd             DESC_CODE_HIGH4
DATA_STACK_DESC:    dd             0x0000ffff
                    dd             DESC_DATA_HIGH4
VIDEO_DESC:         dd             0x80000007;limit = (0xbffff ~ 0xb8000)/4k = 0x7
                    dd             DESC_VIDEO_HIGH4
 GDT_SIZE                equ           $-GDT_BASE
 GDT_LIMIT               equ           GDT_SIZE-1
       times       60 db     0       ;预留描述符空位
SELECTOR_CODE       equ     (0x0001 << 3) + TI_GDT + RPL0 
SELECTOR_DATA       equ     (0x0002 << 3) + TI_GDT + RPL0
SELECTOR_VIDEO      equ     (0x0003 << 3) + TI_GDT + RPL0


gdt_ptr              dw                GDT_LIMIT
                     dd                GDT_BASE
  DispStr     db  "Wecome to loader!"
 loader_start:
;--------------------循环实现字符串输出------------------------------------------------------------------------------------------
   mov ax,DispStr
   mov bx,ax
   mov ax,0
   mov ds,ax
   mov ax,0xb8a0
   mov gs,ax
   mov di,0
   mov cx,17
Str:
   mov byte dl,[ds:bx]
   mov byte [gs:di],dl
   mov byte [gs:di+1],0xa4
   inc bx
   inc di
   inc di
    loop Str
;-----------------------------------------------进入保护模式---------------------------------------------------------------------------
mov ax,0
in al,0x92
or al,00000010B
out 0x92,al                    ;打开A20

mov eax,0
mov eax,cr0
or  eax,00000001B
mov cr0,eax                    ;cro寄存器pe位置1

lgdt [gdt_ptr]                 ;加载GDT


     jmp   dword   SELECTOR_CODE:p_mode_start      ; 刷新流水线

[bits 32]
p_mode_start:
     mov ax,SELECTOR_DATA
     mov ds,ax
     mov es,ax
     mov ss,ax
     mov esp,LOADER_STACK_TOP
     mov ax,SELECTOR_VIDEO
     mov gs,ax

     mov byte [gs:160],'p' 
     mov byte [gs:161],0xa8

jmp $
