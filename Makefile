SRCS=$(wildcard */*.c) $(wildcard *.c) 
CC = zcc
CFLAGS = +rc2014-manawyrm -subtype=none -vn -SO3 -O3  --opt-code-size -clib=sdcc_iy -pragma-define:__CRTCFG=-1 
#  --max-allocs-per-node200000

all:
	$(CC) $(CFLAGS) --list $(SRCS) -o main -create-app

.PHONY clean:
	rm -f *.bin *.lst *.ihx *.hex *.obj *.rom *.lis zcc_opt.def $(APP_NAME) *.reloc *.sym *.map disasm.txt

run: 
	cat main.ihx | python slowprint.py > /dev/ttyUSB0

build: 
	cp main.bin /home/tobias/Entwicklung/Code/RC2014/ROMs/rom/rc2014_0010.bin
	php /home/tobias/Entwicklung/Code/RC2014/ROMs/rom/build.php

runemu: 
	dd if=/dev/zero of=rc2014.bin bs=128k count=1
	dd if=main.bin of=rc2014.bin seek=0 bs=1 conv=notrunc
	cp rc2014.bin /home/tobias/Entwicklung/Code/rc2014-emulator
	/home/tobias/Entwicklung/Code/rc2014-emulator/rc2014 -a -m z80 -r rc2014.bin -e 0 -d 2 -i my.cf 2> stderr.log