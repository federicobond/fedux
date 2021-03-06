NASM = nasm -f aout

CC = gcc
CFLAGS = -m32 -march=i386 -fno-stack-protector -fno-builtin -Wall

SRC = src/
BIN = bin/

ASMSOURCES = libasm.asm kstart.asm loader.asm
CSOURCES = kernel.c ctype.c string.c stdio.c stdlib.c libc.c

LDFLAGS = -T src/link.ld -m elf_i386 

.PHONY: tests clean

all: tpe-asm tpe-c tpe-link
	sudo mount img/tpe.img /mnt/tpe -o loop 
	sudo cp bin/kernel.bin /mnt/tpe/boot
	sudo umount /mnt/tpe

start-vm:
	qemu -fda img/tpe.img

unetbootin:
	unetbootin method=diskimage imgfile="img/tpe.img" installtype=USB

tpe-asm:
	$(NASM) src/libasm.asm -o bin/libasm.o
	$(NASM) src/kstart.asm -o bin/kstart.o
	$(NASM) src/loader.asm -o bin/kstart.o

tpe-c:
	$(CC) $(CFLAGS) -c src/kernel.c -o bin/kernel.o
	$(CC) $(CFLAGS) -c src/ctype.c -o bin/ctype.o
	$(CC) $(CFLAGS) -c src/string.c -o bin/string.o
	$(CC) $(CFLAGS) -c src/stdio.c -o bin/stdio.o
	$(CC) $(CFLAGS) -c src/stdlib.c -o bin/stdlib.o
	$(CC) $(CFLAGS) -c src/sh.c -o bin/sh.o
	$(CC) $(CFLAGS) -c src/vgatext.c -o bin/vgatext.o
	$(CC) $(CFLAGS) -c src/bq.c -o bin/bq.o
	$(CC) $(CFLAGS) -c src/kbd.c -o bin/kbd.o
	$(CC) $(CFLAGS) -c src/tty.c -o bin/tty.o
	$(CC) $(CFLAGS) -c src/ttybox.c -o bin/ttybox.o
	$(CC) $(CFLAGS) -c src/ttyman.c -o bin/ttyman.o
	$(CC) $(CFLAGS) -c src/kpanic.c -o bin/kpanic.o
	$(CC) $(CFLAGS) -c src/mm.c -o bin/mm.o
	$(CC) $(CFLAGS) -c src/io.c -o bin/io.o
	$(CC) $(CFLAGS) -c src/syscall.c -o bin/syscall.o
	$(CC) $(CFLAGS) -c src/critical.c -o bin/critical.o
	$(CC) $(CFLAGS) -c src/serial.c -o bin/serial.o
	$(CC) $(CFLAGS) -c src/serialman.c -o bin/serialman.o
	$(CC) $(CFLAGS) -c src/time.c -o bin/time.o
	$(CC) $(CFLAGS) -c src/serialman.c -o bin/serialman.o
	$(CC) $(CFLAGS) -c src/ktime.c -o bin/ktime.o
	$(CC) $(CFLAGS) -c src/software/commons.c -o bin/software/commons.o
	$(CC) $(CFLAGS) -c src/software/laws.c -o bin/software/laws.o
	$(CC) $(CFLAGS) -c src/software/fortune.c -o bin/software/fortune.o
	$(CC) $(CFLAGS) -c src/software/echo.c -o bin/software/echo.o
	$(CC) $(CFLAGS) -c src/software/help.c -o bin/software/help.o
	$(CC) $(CFLAGS) -c src/software/chat.c -o bin/software/chat.o
	$(CC) $(CFLAGS) -c src/software/cowsay.c -o bin/software/cowsay.o
	
tpe-link:
	ld $(LDFLAGS) -o bin/kernel.bin bin/kernel.o bin/kstart.o bin/libasm.o bin/ctype.o bin/stdlib.o bin/string.o bin/stdio.o bin/sh.o bin/vgatext.o bin/bq.o bin/kbd.o bin/tty.o bin/ttybox.o bin/ttyman.o bin/kpanic.o bin/mm.o bin/io.o bin/syscall.o bin/critical.o bin/serial.o bin/serialman.o bin/software/laws.o bin/software/echo.o bin/software/fortune.o bin/software/commons.o bin/software/help.o bin/software/chat.o bin/software/cowsay.o bin/time.o bin/ktime.o

tests:
	$(CC) -g tests/string.c src/string.c -o tests/string.test && tests/string.test && rm tests/string.test
	$(CC) -g tests/ctype.c src/ctype.c -o tests/ctype.test && tests/ctype.test && rm tests/ctype.test

clean:
	rm -rf bin/*.o

