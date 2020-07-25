SRCS=$(wildcard */*.c) $(wildcard *.c) romfunctions.asm
CC = zcc
#CFLAGS = +rc2014-manawyrm -subtype=none -vn -SO3 -O3  --opt-code-size -clib=sdcc_iy -pragma-define:__CRTCFG=-1
#CFLAGS = +rc2014 -subtype=basic -v -m -SOfast --c-code-in-asm  -clib=sdcc_ix
CFLAGS = +rc2014 -subtype=acia -v -m -SOs --c-code-in-asm  -clib=sdcc_ix -pragma-define:__CRTCFG=-1

#  --max-allocs-per-node200000

all:
	$(CC) $(CFLAGS) --list $(SRCS) -o main -create-app

.PHONY clean:
	rm -f *.bin *.lst *.ihx *.hex *.obj *.rom *.lis zcc_opt.def $(APP_NAME) *.reloc *.sym *.map disasm.txt

run: 
	cat main.ihx | python slowprint.py > /dev/ttyUSB0

build: 
	dd if=/dev/zero of=main_concat.bin seek=16k count=0 bs=1
	dd if=main.bin of=main_concat.bin conv=notrunc bs=1
	dd if=main_ROMFUNCTIONS.bin of=main_concat.bin conv=notrunc bs=1 seek=16320
	cp main_concat.bin /home/tobias/Entwicklung/RC2014/rom/rc2014_0010.bin
	php /home/tobias/Entwicklung/RC2014/rom/build.php

runemu: 
	dd if=/dev/zero of=rc2014.bin bs=128k count=1
	dd if=main.bin of=rc2014.bin seek=0 bs=1 conv=notrunc
	cp rc2014.bin /home/tobias/Entwicklung/Code/rc2014-emulator
	/home/tobias/Entwicklung/Code/rc2014-emulator/rc2014 -a -m z80 -r rc2014.bin -e 0 -f -i my.cf 2> stderr.log