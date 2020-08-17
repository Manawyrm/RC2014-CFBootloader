#include "uart.h"
#include <string.h>

__sfr __at 0x80 ACIA_CONTROL;
__sfr __at 0x81 ACIA_DATA;

#define UART_RX_SIZE 128
uint8_t uart_rx_buffer[UART_RX_SIZE];
volatile uint8_t rxhead, rxtail;

char uart_register_read (void)
{
  while ((ACIA_CONTROL & 1) == 0) {};
  return ACIA_DATA;
}

void z80_rst_38h(void) __critical __interrupt(0)
{
  uart_rx_buffer[rxtail] = uart_register_read();
  rxtail++;
  rxtail &= 127;

  return;
}

uint8_t uart_available()
{
    return rxhead ^ rxtail;
}

uint8_t uart_read()
{
   uint8_t key;
   
   key = uart_rx_buffer[rxhead];
   rxhead++;
   rxhead &= 127;
     
   return key;
}

inline char uart_rx_filled()
{
    return (ACIA_CONTROL & 1);
}


void uart_init(void)
{
  ACIA_CONTROL = 0x96; // interrupt 
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
