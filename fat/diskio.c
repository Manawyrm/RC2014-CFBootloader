#include "diskio.h"
#include "../cf/cf.h"
#include "../uart/uart.h"
#include <string.h>

//#define DEBUG 1

DSTATUS disk_initialize (void)
{
    cf_init();
    return RES_OK;
}


void print_memory(const void *addr, uint16_t size);

void print_address(void* ptr)
{
    uint16_t printed = 0;
    int16_t i;
   
    for (i=sizeof(ptr) - 1; i >= 0; i--)
    {
        unsigned char pc = ((uint32_t) ptr >> (i*8)) & 0xFF;

        int  g;
        g = (pc >> 4) & 0xf;
        g += g >= 10 ? 'a'-10 : '0';
        putchar_uart(g);
        printed++;

        g = pc & 0xf;
        g += g >= 10 ? 'a'-10 : '0';
        putchar_uart(g);
        printed++;
        if (printed % 32 == 0) putchar_uart('\n');
        else if (printed % 4 == 0) putchar_uart(' ');
    }
}

DRESULT disk_readp (BYTE* buff, DWORD sector, WORD offset, WORD count)
{
    #ifdef DEBUG
        putstring_uart("disk_readp: ");
        putstring_uart("buff_a:");
        print_address(buff);
        putstring_uart("\n");
    #endif

    cf_read_chunk(sector, buff, offset, count);
    return RES_OK;
}

//uint8_t cf_sector_write[512];
uint32_t currentSector; 
DRESULT disk_writep (const BYTE* buff, DWORD sc)
{
   /* if (buff)
    {
        memcpy(cf_sector_write, buff, sc);
    }
    else
    {
        if (sc)
        {
            // init sector write transaction
            memset(cf_sector_write, 0x00, 512);
            currentSector = sc;
        }
        else
        {
            // write sector
            cf_write(currentSector, cf_sector_write);
        }
        
    }*/
    
    return RES_OK;
}