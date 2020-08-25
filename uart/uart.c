#include "uart.h"
#include <string.h>
#include <z80.h>

volatile __sfr __at 0x80 SIOA_REGISTER;
volatile __sfr __at 0x81 SIOA_DATA; 
volatile __sfr __at 0x82 SIOB_REGISTER;
volatile __sfr __at 0x83 SIOB_DATA;
 

#define UART_RX_SIZE 128
uint8_t uart_rx_buffer[UART_RX_SIZE];
volatile uint8_t rxhead, rxtail;

void z80_rst_38h(void) __critical __interrupt(0)
{
  SIOA_REGISTER = 0x00; 
  if (!(SIOA_REGISTER & 0x01))
    return; 

  uart_rx_buffer[rxtail] = SIOA_DATA;
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

inline void uart_a_set_register(uint8_t reg, uint8_t data)
{
  SIOA_REGISTER = reg;
  SIOA_REGISTER = data;
}


void uart_init(void)
{
  SIOA_REGISTER = 0x18; // WR0, Channel Reset 
  z80_delay_ms(10);

  uart_a_set_register(0x04, 0xC4); // WR4, x64 Clock Devide
  uart_a_set_register(0x03, 0xC1); // WR3, RX Enabled
  uart_a_set_register(0x05, 0xE8); // WR5, 8bit Mode, TX Enabled (Bit 3)
  uart_a_set_register(0x01, 0x18); // WR1, INT on All Rx Characters (without Parity)
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
  SIOA_REGISTER = 0x01; 
  while (SIOA_REGISTER & 0x01 == 0)
  {
    SIOA_REGISTER = 0x01;
  }
  
	SIOA_DATA = tx_data;
}
