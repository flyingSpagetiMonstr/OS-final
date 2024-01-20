bits 32

[section .text]

global _start

_start:
    push ebp
    mov ebp, esp
    sub esp, 0x100

    ; mov -0x14(ebp), 0x666e492d
    ; mov -0x10(ebp), 0x65746365
    ; mov -0xc(ebp), 0xa2d64

        mov eax, ebp
        sub eax, 0x14
        mov ebx, 0x666e492d
        mov [eax], ebx

        mov eax, ebp
        sub eax, 0x10
        mov ebx, 0x65746365
        mov [eax], ebx

        mov eax, ebp
        sub eax, 0xc
        mov ebx, 0xa2d64
        mov [eax], ebx
    
    mov eax, ebp
    sub eax, 0x14
    push eax  

    mov eax, 0x106e
    call eax

    mov eax, 0x17e4
    push 0x0
    call eax