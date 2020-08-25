#pragma once
#include "stdint.h"

void putchar_uart (char tx_data);
void putstring_uart(uint8_t *text);
void uart_init(void);
uint8_t uart_available();
uint8_t uart_read();