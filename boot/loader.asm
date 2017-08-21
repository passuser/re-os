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

total_mem_bytes  dd    0
ards_buf     times     244     db      0
ards_nr      dw        0
 
DispStr     db  "Wecome to loader!"
 loader_start:
;----------------------------mem----------------------------------------------------------------------------------------------------
    xor ebx,ebx                      ;ebx = 0
    mov edx,0x534d4150
    mov di,ards_buf
    
E820_mem_get:
     mov eax,0xe820
     mov ecx,20
     int 0x15
     jc E801_mem_get
     add di,cx
     inc word [ards_nr]
     cmp ebx,0
     jnz E820_mem_get

     mov cx,[ards_nr]
     mov ebx,ards_buf
     xor edx,edx                        ;edx = 0

find_max_mem_area:
        mov eax,[edx]                   ;base add low
	add eax,[edx+8]                 ; length low
	add ebx,20                      ;next ards
	cmp edx,eax
;bubble sort
    jge next_ards
    mov edx,eax                  ;edx store mem summ
    
next_ards:
    loop  find_max_mem_area
    jmp   mem_get_ok
;-----------------------------------------E820 finish------------------------
E801_mem_get:
   mov ax,0xe801
   int 0x15
   jc  .88_mem_get
   mov cx,0x400
   mul cx
   shl edx,16
   and eax,0xffff
   or  edx,eax
   add edx,0x100000
   mov esi,edx

   xor eax,eax
   mov ax,bx
   mov ecx,0x100000
   mul ecx

   add esi,eax
   mov edx,esi
   jmp mem_get_ok
;---------------------------------------E801 finish--------------------
.88_mem_get:
   mov ah,0x88
   int 0x15
;   jc  error_hlt           ; Not need it for now
   and eax,0xffff
   mov cx,0x400
   mul cx
   shl edx,16
   or edx,eax
   add edx,0x100000
;------------------------------------88 finish-----------------------------
mem_get_ok:
   mov [total_mem_bytes],edx




;--------------------循环实现字符串输出------------------------------------------------------------------------------------------
   mov ax,DispStr
   mov bx,ax
   mov ax,0
   mov ds,ax
   mov ax,0xb800
   mov gs,ax
   mov di,0
   mov cx,17
Str:
   mov byte dl,[ds:bx]
   mov byte [gs:di+0xa0],dl
   mov byte [gs:di+0xa1],0xa4
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
     mov ax,bootstr
     mov bx,ax
     mov edi,0
     mov ecx,26
display:
     mov byte dl,[ds:bx]
     mov byte [gs:edi+0x140],dl 
     inc bx
     inc edi
     inc edi
      loop  display
;------------------------------------turn on page table--------------------------------------------------------------
call  setup_page
sgdt  [gdt_ptr]                  ; update  GDT
mov   ebx,[gdt_ptr + 2]
or dword [ebx + 0x18 + 4],0xc0000000
add dword [gdt_ptr + 2],0xc0000000

add esp,0xc0000000

mov eax,PAGE_DIR_TABLE_POS
mov cr3,eax

mov eax,cr0
or eax,0x80000000
mov cr0,eax

lgdt [gdt_ptr]                           ; update gdt finish

mov byte [gs:480],'V'

;------------------------------------------create page table---------------------------------------------------------
setup_page:
   mov ecx,4096
   mov esi,0
clear_page_dir:
   mov byte [PAGE_DIR_TABLE_POS + esi],0
   inc esi
   loop clear_page_dir

create_pde:
   mov eax,PAGE_DIR_TABLE_POS
   add eax,0x1000
   mov ebx,eax
   
   or eax,PG_US_U | PG_RW_W | PG_P

   mov [PAGE_DIR_TABLE_POS + 0x0],eax
   mov [PAGE_DIR_TABLE_POS + 0xc00],eax
   sub eax,0x1000
   mov [PAGE_DIR_TABLE_POS + 4092],eax

;create pte
  mov ecx,256
  mov esi,0
  mov edx,PG_US_U | PG_RW_W | PG_P

create_pte:
   mov [ebx+esi*4],edx
   add edx,4096
   inc esi
   loop create_pte

    mov eax,PAGE_DIR_TABLE_POS
    add eax,0x200
    or eax,PG_US_U | PG_RW_W | PG_P
    mov  ebx,PAGE_DIR_TABLE_POS
    mov  ecx,254
    mov  esi,769
    
create_kernel_pde:
    mov [ebx + esi*4],eax
    inc esi
    add eax,0x1000
    loop create_kernel_pde
    ret
jmp $
  bootstr:  db  "you turned on protect-mode"
