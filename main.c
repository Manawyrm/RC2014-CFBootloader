#include "main.h"
#include "cf/cf.h"
#include "fat/pff.h"
#include "uart/uart.h"
void print_memory(const void *addr, uint16_t size)
{
    uint16_t printed = 0;
    uint16_t i;
    const unsigned char* pc = addr;
    for (i=0; i<size; ++i)
    {
        int  g;
        g = (*(pc+i) >> 4) & 0xf;
        g += g >= 10 ? 'a'-10 : '0';
        putchar_uart(g);
        printed++;

        g = *(pc+i) & 0xf;
        g += g >= 10 ? 'a'-10 : '0';
        putchar_uart(g);
        printed++;
        if (printed % 32 == 0) putchar_uart('\n');
        else if (printed % 4 == 0) putchar_uart(' ');
    }
}


void die (FRESULT rc)
{
	//printf("Failed with rc=%u.\n", rc);
	putstring_uart("fat error :( \n");
	print_memory(&rc, sizeof(rc));
	for (;;) ;
}
void (*jump)(void) = 0x8000;

uint8_t linebuf[64];
uint8_t readline()
{
	uint8_t input = 0xFF; 
	uint8_t i = 0;
	// loop until we get a return or the buffer is full
	while (input != '\r' && i != 64) 
	{
		// wait for character
		while (!uart_available()) {}

		input = uart_read();
		
		// backspace
		if (input == 0x7F)
		{
			putchar_uart(0x08);
			putchar_uart(0x20);
			putchar_uart(0x08);
			i--;
		}
		else
		{
			// echo
			putchar_uart(input);

			// don't put newlines into the buffer
			if (!(input == '\r' || input == '\n'))
				linebuf[i++] = input;
		}

		
	}
	linebuf[i] = 0x00;

	//print_memory(linebuf, 64);
	return i;
}


void main()
{
	uart_init();

	FRESULT rc;
	FATFS fatfs;
	DIR dir;
	FILINFO fno;
	UINT bw, br, i;
	BYTE buff[64];

	putstring_uart("cfbootloader starting.\n");

	rc = pf_mount(&fatfs);
	if (rc) die(rc);

	rc = pf_opendir(&dir, "");
	if (rc) die(rc);

	for (;;) {
		rc = pf_readdir(&dir, &fno);
		if (rc || !fno.fname[0]) break;
		if (!(fno.fattrib & AM_DIR))
			
		putstring_uart(fno.fname);
		putchar_uart('\n');
	}
	if (rc) die(rc);

enterf:
	putstring_uart("Please enter filename to load: \n");
	while (!readline())
	{
		putstring_uart("Please enter filename to load: \n");
	}

	putstring_uart("Loading (");
	putstring_uart(linebuf);
	putstring_uart(").\n");

	rc = pf_open(linebuf);
	if (rc)
	{
		putstring_uart("File not found!\n");
		goto enterf;
	}

	rc = pf_read((uint8_t *) 0x8000, 0xFFFF, &br);

	putstring_uart("Will jump...\n");
	jump();


	while(1)
	{

	}
}