[BITS 32]

section .asm

global peachos_getkey:function

peachos_getkey:
    push ebp
    mov ebp, esp
    mov eax, 2 ; Command getkey
    int 0x80
    pop ebp
    ret
