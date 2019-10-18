#include "uart.h"
#include <string.h>

__sfr __at 0x80 ACIA_CONTROL;
__sfr __at 0x81 ACIA_DATA;

#define UART_RX_SIZE 128
uint8_t uart_rx_buffer[UART_RX_SIZE];
uint8_t rxhead, rxtail;

void z80_rst_38h(void) __critical __interrupt(0)
{
    uart_rx_buffer[rxtail] = uart_register_read();
    rxtail++;
    rxtail &= 127;
	
	return;
}

int uart_available()
{
    if (rxhead == rxtail)
        return 0;
     
    if (uart_rx_buffer[rxhead] == 0 && ((rxhead+1) & 127) == rxtail) 
        return 0;
        
    return 1;
}

uint8_t uart_read()
{
   uint8_t key;
   
   key = uart_rx_buffer[rxhead];
   rxhead++;
   rxhead &= 127;
     
   return key;
}

char uart_rx_filled()
{
    return (ACIA_CONTROL & 1);
}

char uart_register_read (void)
{
    // write to registers to tx the data. TX buffering, etc
    while ((ACIA_CONTROL & 1) == 0) {};
	return ACIA_DATA;
}

void uart_init(void)
{
	ACIA_CONTROL = 0x96; // interrupt 
    //ACIA_CONTROL = 0x16;  // no interrupts
}


void putstring_uart(uint8_t *text)
{
  int i; 
  for (i = 0; i<strlen(text);i++)
  {
    putchar_uart(text[i]);
  }
}

void putchar_uart (char tx_data)
{
    // write to registers to tx the data. TX buffering, etc
    while ((ACIA_CONTROL & 2) == 0) {};
	ACIA_DATA = tx_data;
}
