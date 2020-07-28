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
	putstring_uart("fat error :( \n");
	print_memory(&rc, sizeof(rc));
	for (;;) ;
}

uint8_t readline(uint8_t* buffer, uint16_t max_length)
{
	uint8_t input = 0xFF; 
	uint8_t i = 0;
	// loop until we get a return or the buffer is full
	while (input != '\r' && i != max_length) 
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
				buffer[i++] = input;
		}

		
	}
	buffer[i] = 0x00;

	return i;
}

FRESULT fat_load_and_execute(uint8_t* filename, void* load_address)
{
	FRESULT rc;
	UINT br; 
	rc = pf_open(filename);
	if (rc)
	{
		return rc;
	}

	rc = pf_read((uint8_t *) load_address, 0xFFFF, &br);

	void (*jump)(void) = load_address;
	jump();

	return 0;
}

void dummy()
{
	
}

uint8_t linebuf[64];

void main()
{
	uart_init();

	FRESULT rc;
	FATFS fatfs;
	DIR dir;
	FILINFO fno;

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
	while (!readline(linebuf, 64))
	{
		putstring_uart("Please enter filename to load: \n");
	}

	putstring_uart("Loading (");
	putstring_uart(linebuf);
	putstring_uart(").\n");

	if (fat_load_and_execute(linebuf, (void*) 0x5000))
	{
		putstring_uart("File not found!\n");
		goto enterf;
	}

	while(1)
	{

	}
}