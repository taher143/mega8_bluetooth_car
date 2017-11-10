/*
 * uart.h
 *
 * Created: 21.02.2015 11:38:47
 *  Author: kab
 */ 


#ifndef UART_H_
#define UART_H_

#include "uart_def.h"

extern void uart_printr (char* str);
extern void uart_printp (uint_farptr_t cmd);
extern void uart_printcrlf (void);
extern void uart_rx_buffer_flush (void);

extern void uart_putchar(char c);
extern char uart_getchar(void);

extern void uart_init(uint16_t BAUD_RATE);

extern void uart_itoa(int value);
extern unsigned int uart_tx_buffer_search (int16_t str, unsigned int str_length, int start);
extern void uart_tx_buffer_copy (char* buffer, unsigned int start, unsigned int length);


#endif /* UART_H_ */