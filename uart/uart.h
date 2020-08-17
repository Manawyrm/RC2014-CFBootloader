#pragma once
#include "stdint.h"

void putchar_uart (char tx_data);
void putstring_uart(uint8_t *text);
char uart_register_read (void);
void uart_init(void);
char uart_rx_filled();
uint8_t uart_available();
uint8_t uart_read();