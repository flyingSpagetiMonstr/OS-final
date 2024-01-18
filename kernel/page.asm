; [SECTION .data]
BitMap:
    times 32 db 0xff
    times 32 db 0
BitMapLen equ $ - BitMap

alloc_addr dd 0x80000000


[SECTION .text]
global alloc_pages
global free_pages

; no args 
; returns: addr in eax
alloc_page:
    push ds
    push es

    xor eax, eax
.seek:
    bts [BitMap], eax
    jnc .find
    inc eax
    cmp eax, BitMapLen * 8
    jl .seek
    hlt
.find:
    shl eax, 12 ; eax is the physical addr now
    pop es
    pop ds
    ret

; ========================================== 
; args: eax = number of pages
; returns: ebx = allocated virtual addr
alloc_pages:
    push ecx
    push edx

    mov ecx, eax

    mov ebx, 4096
    mul ebx
    mov ebx, [alloc_addr] ; ebx = virtual addr of free page 
    add [alloc_addr], eax

    push ebx ; store return value
    ; for pde: 
    mov eax, ebx ; eax = virtual addr of free page 

    mov ebx, cr3 ; ebx = page dir base
    and ebx, 0xfffff000
    and eax, 0xffc00000 ; 12 + 20
    shr eax, 20 ; eax = offset by Byte of PDE in PDT
    add ebx, eax ; ebx = addr of that pde
    mov edx, ebx ; store 20 + 12 in edx 
    mov ebx, [ebx] ; ebx = content of that pde
    test ebx, 0x00000001

    jnz .pde_already

    mov ebx, cr3 ; ebx = page dir base
    mov ebx, [ebx] ; ebx = first entry of pdt
    and ebx, 0xff_ff_f0_00 ; 8+4=12, ebx = addr of (the first) pt << 12
    shl eax, 10
    add ebx, eax ;  
    or ebx, 0x00000007
    mov [edx], ebx

.pde_already:
    mov eax, [esp]
    and ebx, 0xfffff000
    and eax, 0x003ff000
    shr eax, 10
    add ebx, eax

.pte_loop:
    call alloc_page
    or eax, 0x00000007
    ; or eax, 111
    mov [ebx], eax
    add ebx, 4
    loop .pte_loop

    pop ebx
    ; pop ds
    pop edx
    pop ecx
    ret

; ========================================== 
; args: 
; ecx = number of pages 
; edx = start virtual addr of allocated page [4KB aligned] 
free_pages:
    push eax
    push ebx

    ;  args: edx = virtual addr (reserved)
    call get_pte_addr ; ebx = &pte

.free_one_page:
    call get_physical_addr ; eax = physical addr
    call reset_bitmap

    mov word [ebx], 0

    add edx, 0x1000
    add ebx, 4

    loop .free_one_page

    pop ebx
    pop eax
    ret
; ==================
; args: edx = virtual addr (reserved)
; returns: eax = physical
get_physical_addr:
    push ebx
    ; 10 10 12

    call get_pte_addr

    mov ebx, [ebx]
    and ebx, 0xfffff000

    mov eax, edx
    and eax, 0xfff
    add ebx, eax

    mov eax, ebx

    pop ebx
    ret
; =================================
; eax = physical addr of page (not reserved)
; no returns
reset_bitmap:
    push ebx
    push ecx
    push edx

    mov edx, 0
    mov ebx, 8
    div ebx 
    mov ecx, edx
    mov edx, 1 
    shl edx, cl
    mov ecx, 0xff ; ecx = bitmask
    sub ecx, edx
    add eax, BitMap
    and [eax], cl

    pop edx
    pop ecx
    pop ebx
    ret

; =================================
; args: edx = virtual addr (reserved)
; returns: ebx = &pte
get_pte_addr:
    push eax

    mov eax, edx
    shr eax, 10 + 12
    shl eax, 2

    mov ebx, cr3
    and ebx, 0xfffff000

    add ebx, eax ; ebx = &pde

    mov ebx, [ebx]
    and ebx, 0xfffff000

    mov eax, edx
    and eax, 0x003ff000
    shr eax, 10
    ; shr eax, 12
    ; shl eax, 2
    add ebx, eax ; ebx = &pte

    pop eax
    ret
