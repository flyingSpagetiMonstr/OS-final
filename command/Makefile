# commands/Makefile

#ENTRYPOINT	= 0x1000
HD		= ../80m.img

ASM		= nasm
DASM		= objdump
CC		= gcc
LD		= ld
ASMFLAGS	= -I ../include/ -f elf32
CFLAGS		= -I ../include/ -c -fno-builtin -fno-stack-protector -Wall -m32
LDFLAGS		= -Ttext 0x1000 -m elf_i386
DASMFLAGS	= -D
LIB		= ../lib/orangescrt.a
BIN		= cat ticker mod stack target kill

# All Phony Targets
.PHONY : everything final clean realclean disasm all install

# Default starting position
everything : $(BIN) echo pwd lseektest ls

install : all clean
	cp ../kernel.bin ./ -v
	tar vcf inst.tar kernel.bin $(BIN)
	dd if=inst.tar of=$(HD) seek=`echo "obase=10;ibase=16;(\`egrep -e '^ROOT_BASE' ../boot/include/load.inc | sed -e 's/.*0x//g'\`+\`egrep -e '#define[[:space:]]*INSTALL_START_SECT' ../include/sys/config.h | sed -e 's/.*0x//g'\`)*200" | bc` bs=1 count=`ls -l inst.tar | awk -F " " '{print $$5}'` conv=notrunc

all : realclean everything

final : all clean

clean :
	rm -f *.o

realclean :
	rm -f $(BIN) *.o

kernel.bin :
	cp ../kernel.bin ./

start.o : start.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

echo.o: echo.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

echo : echo.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

pwd.o: pwd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pwd : pwd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

cat.o: cat.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

cat : cat.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

ticker.o: ticker.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

ticker : ticker.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

ls.o: ls.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

ls : ls.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

mod.o: mod.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

mod : mod.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $? 

stack.o: stack.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) $(CFLAGS) -o $@ $<

stack : stack.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

stack.asm: stack.c ../include/type.h ../include/stdio.h ../include/string.h
	$(CC) -I ../include/ -S -fno-builtin -fno-stack-protector -Wall -m32 -o $@ $<

# lseektest.o: lseektest.c ../include/type.h ../include/stdio.h ../include/string.h 
# 	$(CC) $(CFLAGS) -o $@ $<

# lseektest : lseektest.o start.o $(LIB)
# 	$(LD) $(LDFLAGS) -o $@ $? 

payload.o: payload.c
	$(CC) $(CFLAGS) -o $@ $<

payload : payload.o start.o
	$(LD) $(LDFLAGS) -o $@ $^

target.o: target.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

target : target.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

kill.o: kill.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

kill : kill.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?
