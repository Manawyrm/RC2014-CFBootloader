#include "cf.h"
#include <z80.h>
#include "../uart/uart.h"

uint16_t timeout = 0;

int cf_wait_for_ready()
{
	uint8_t state;

	state = 0xFF;
	while (bitRead(state, 7))
	{
		state = z80_inp(CF_STATUS);
		timeout++;
		z80_delay_ms(1);
		if (timeout == 60000)
		{
			return -1;
		}
	}
	timeout = 0;

	return 0;
}

int cf_wait_for_not_busy_and_ready()
{
	uint8_t state;

	state = 0xFF;
	while (bitRead(state, 7) || !bitRead(state, 6)) // wait for !busy 0x80 and ready 0x40
	{
		state = z80_inp(CF_STATUS);
		timeout++;
		z80_delay_ms(1);
		if (timeout == 60000)
		{
			return -1;
		}
	}
	timeout = 0;

	return 0;
}


void busy_sleep(uint32_t time)
{
	while (time--)
	{
		z80_delay_ms(1);
	}
}

void cf_init()
{
	//busy_sleep(1000);
	if (cf_wait_for_not_busy_and_ready())
	{
		putstring_uart("cf_init() timeout\n");
	}
	//busy_sleep(20);

	z80_outp(CF_FEATURE, CF_FEATURE_8BIT_MODE);
	//busy_sleep(20);

	z80_outp(CF_COMMAND, IDE_CMD_SETFEATURES);
	//busy_sleep(20);

	if (cf_wait_for_not_busy_and_ready())
	{
		putstring_uart("cf_init() timeout\n");
	}


	z80_outp(CF_FEATURE, CF_FEATURE_DISABLE_WRITE_CACHING);

	//busy_sleep(20);

	z80_outp(CF_COMMAND, IDE_CMD_SETFEATURES);
}

void cf_read(uint32_t sector, uint8_t* data)
{
	int i;
	uint8_t state;

	if (cf_wait_for_not_busy_and_ready())
	{
		putstring_uart("cf_read() !busy/ready timeout\n");
	}
	
	z80_outp(CF_NUMSECT, 1); // read only a single sector at a time
	cf_set_sector(sector);


	z80_outp(CF_COMMAND, IDE_CMD_READ);

	state = 0xFF;
	while (bitRead(state, 7) || !bitRead(state, 3)) // wait for !busy 0x80 and dry 0x08
	{
		state = z80_inp(CF_STATUS);
		timeout++;
		if (timeout == 60000)
		{
			putstring_uart("cf_read() !busy/dry timeout\n");
		}
	}
	timeout = 0;
	

	for (i = 0; i < SECTOR_SIZE; i++)
	{
		data[i] = z80_inp(CF_DATA);
	}
}

void cf_write(uint32_t sector, uint8_t* data)
{
	int i;
	uint8_t state;

	if (cf_wait_for_not_busy_and_ready())
	{
		putstring_uart("cf_write()-1 !busy/ready timeout\n");
	}
	
	z80_outp(CF_NUMSECT, 1); // read only a single sector at a time
	cf_set_sector(sector);

	z80_outp(CF_COMMAND, IDE_CMD_WRITE);

	state = 0xFF;
	while (bitRead(state, 7) || !bitRead(state, 3)) // wait for !busy 0x80 and dry 0x08
	{
		state = z80_inp(CF_STATUS);
	}
	
	for (i = 0; i < SECTOR_SIZE; i++)
	{
		z80_outp(CF_DATA, data[i]);
	}

	if (cf_wait_for_not_busy_and_ready())
	{
		putstring_uart("cf_write()-2 !busy/ready timeout\n");
	}
}

void cf_set_sector(uint32_t sector)
{
	z80_outp(CF_ADDR0, (sector >> 0) & 0xFF);
	z80_outp(CF_ADDR1, (sector >> 8) & 0xFF);
	z80_outp(CF_ADDR2, (sector >> 16) & 0xFF);
	z80_outp(CF_ADDR3, ((sector >> 24) & 0xFF) | CF_ADDR3_ADDITIONAL);
}

void cf_dump_sector(uint8_t* data)
{
	uint16_t i;
	for (i = 0; i < SECTOR_SIZE; i++)
	{
		if (i % 32 == 0)
		{
		//	putchar('\r');
		//	putchar('\n');
		}
		//printf("%02X", data[i]);
	}
}