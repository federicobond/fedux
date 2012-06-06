NASM = nasm -f aout

CC = gcc
CFLAGS = -m32 -march=i386 -fno-stack-protector -fno-builtin

SRC = src/
BIN = bin/

ASMSOURCES = libasm.asm kstart.asm loader.asm
CSOURCES = kernel.c ctype.c string.c stdio.c stdlib.c libc.c

LDFLAGS = -T src/link.ld -m elf_i386 

.PHONY: tests clean

all: tpe-asm tpe-c tpe-link
	sudo mount img/tpe.img /mnt/tpe
	sudo cp bin/kernel.bin /mnt/tpe/boot
	sudo umount /mnt/tpe

start-vm:
	qemu -fda img/tpe.img

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
	$(CC) $(CFLAGS) -c src/serial.c -o bin/serial.o
	
tpe-link:
	ld $(LDFLAGS) -o bin/kernel.bin bin/kernel.o bin/kstart.o bin/libasm.o bin/ctype.o bin/stdlib.o bin/string.o bin/stdio.o bin/sh.o bin/vgatext.o bin/bq.o bin/kbd.o bin/tty.o bin/ttybox.o bin/ttyman.o bin/kpanic.o bin/mm.o bin/serial.o

tests:
	$(CC) -g tests/string.c src/string.c -o tests/string.test && tests/string.test && rm tests/string.test
	$(CC) -g tests/ctype.c src/ctype.c -o tests/ctype.test && tests/ctype.test && rm tests/ctype.test

clean:
	rm -rf bin/*.o

