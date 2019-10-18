#pragma once
#include "stdint.h"

void putchar_uart (char tx_data);
void putstring_uart(uint8_t *text);
char uart_register_read (void);
void uart_init(void);
int _inbyte(unsigned short timeout);
void _outbyte(int c);
char uart_rx_filled();


int uart_available();
uint8_t uart_read();