[bits 32]
section .text
global switch_to
switch_to:
    push esi
    push edi
    push ebp
    push ebx
    mov eax,[esp + 20]
    mov [eax],esp


    mov eax,[esp + 24]
    mov esp,[eax]
    pop ebx
    pop ebp
    pop edi
    pop esi
    ret


