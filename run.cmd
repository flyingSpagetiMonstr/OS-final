ubuntu run make image

@REM cd command/
@REM ubuntu run make install 
@REM cd ..

set arg=%1
set root=D:\Program Files\Bochs-2.7\bochs
"%root%%arg%.exe" -f bochsrc.sh

@REM ================================================ 
@REM CV run.cmd bochsrc.sh and 
@REM modify kliba.asm, Makefile, bochsrc.sh 

@REM ================================================ 
@REM Makefile:
@REM ASMBFLAGS	= -I boot/include/
@REM ASMKFLAGS	= -f elf32 -I include/ -f elf
@REM CFLAGS		= -m32 -fno-builtin -fno-stack-protector -I include/ -c 
@REM LDFLAGS	= -m elf_i386 -s -Ttext $(ENTRYPOINT)
@REM DASMFLAGS	= -u -o $(ENTRYPOINT) -e $(ENTRYOFFSET)

@REM ================================================
@REM ; =============================
@REM 	push eax
@REM 	push ebx
@REM 	push edx

@REM 	mov eax, edi
@REM 	xor edx, edx
@REM 	mov ebx, 160 
@REM 	div ebx

@REM 	sub edi, edx
@REM 	add edi, 160

@REM 	pop edx
@REM 	pop ebx
@REM 	pop	eax
@REM 	jmp .1
@REM ; =============================
@REM ; pos = (pos - (pos%160)) + 160 
@REM 	; push	eax
@REM 	; mov	eax, edi
@REM 	; mov	bl, 160
@REM 	; div	bl

@REM 	; and	eax, 0FFh
@REM 	; inc	eax
@REM 	; mov	bl, 160
@REM 	; mul	bl
@REM 	; mov	edi, eax
@REM 	; pop	eax
@REM 	; jmp	.1
@REM ; =============================