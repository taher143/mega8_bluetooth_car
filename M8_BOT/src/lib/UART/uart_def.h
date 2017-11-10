/*
 * uart_def.h
 *
 * Created: 21.02.2015 11:39:17
 *  Author: kab
 */ 

#ifndef UART_DEF_H_
#define UART_DEF_H_

#define DATA_REGISTER_EMPTY (1<<UDRE)
#define RX_COMPLETE (1<<RXC)
#define FRAMING_ERROR (1<<FE)
#define PARITY_ERROR (1<<PE)
#define DATA_OVERRUN (1<<DOR)

// USART Receiver buffer
#define RX_BUFFER_SIZE 260
// USART Transmitter buffer
#define TX_BUFFER_SIZE 8

#endif /* UART_DEF_H_ */