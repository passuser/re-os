org    0x7c00                                           ;引导加载
mov    ax,cs                                            ;这是第一版程序，采用bios中断功能来输出字符 
mov    ds,ax
mov    es,ax

   mov ah,0x06                                     ;ah 功能号 0x6,al上卷行数(0为全部)
   mov bh,0x07                                     ;  bh上卷行属性 
   mov cx,0                                        ; (cl,ch)=左上角(x,y)
   mov dx,0x184f                                   ;  (dl,dh)=右下角(x,y)
      int 0x10                                    ;清屏
   mov ah,3
   mov bh,0
      int 0x10
call    DispStr                                    ;调用字符函数
jmp    $                                               ;无限循环

DispStr:
       mov  ax,BootMessage
       mov bp,ax
       mov cx,12
       mov ax,0x1301
       mov bx,0x02
       mov dl,0
       int 0x10
  ret
    BootMessage:         db         "Hello,re-os!"
times     510-($-$$)          db         0
db   0x55
db   0xaa
