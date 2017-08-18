%include "boot.inc"
org       LOADER_BASE_ADDR
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
   mov byte [gs:di+1],0xb4
   inc bx
   inc di
   inc di
    loop Str
;-----------------------------------------------进入保护模式---------------------------------------------------------------------------
in al,0x92
or al,00000010B
out 0x92,al                    ;打开A20

mov eax,cr0
or  eax,00000001B
out cr0,eax                    ;cro寄存器pe位置1

lgdt [gdt_ptr]
  DispStr     db  "Wecome to loader!"
jmp $
