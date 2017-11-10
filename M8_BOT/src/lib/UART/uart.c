/*
 * uart.c
 *
 * Created: 21.02.2015 11:37:42
 *  Author: Taher Kawantwala
 */ 

#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <interrupt.h>
#include <string.h>
#include "uart_def.h"
#include "lib/common.h"


void uart_init(uint16_t BAUD_RATE);
char uart_getchar(void);
void uart_itoa (int value);
void uart_putchar(char c);
void uart_tx_buffer_copy (char* buffer, unsigned int start, unsigned int length);
void uart_printr (char* str);
void uart_printp (uint_farptr_t cmd);
void uart_printcrlf (void);
void uart_rx_buffer_flush (void);

unsigned int uart_tx_buffer_search (int16_t str, int str_length, int start);
void uart_tx_buffer_copy (char* buffer, unsigned int start, unsigned int length);

volatile char rx_buffer[RX_BUFFER_SIZE];
volatile unsigned int rx_wr_index=0;
volatile unsigned int rx_rd_index=0;
volatile unsigned int rx_counter=0;
// This flag is set on USART0 Receiver buffer overflow.
volatile char rx_buffer_overflow;

volatile char tx_buffer[TX_BUFFER_SIZE];
volatile unsigned int tx_wr_index=0;
volatile unsigned int tx_rd_index=0;
volatile unsigned int tx_counter=0;

// USART Receiver interrupt service routine.
ISR(USART_RXC_vect)
{
	char status,data;
	status=UCSRA;
	data=UDR;
	if ((status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN))==0)
	{
		rx_buffer[rx_wr_index++]=data;
		if (rx_wr_index == RX_BUFFER_SIZE) rx_wr_index=0;
		if (++rx_counter == RX_BUFFER_SIZE)
		{
			rx_counter=0;
			rx_buffer_overflow=1;
		}
	}

}

// Get a character from the USART Receiver buffer.
char uart_getchar(void)
{
	char data;
	while (rx_counter==0);
	data=rx_buffer[rx_rd_index++];
	if (rx_rd_index == RX_BUFFER_SIZE) rx_rd_index=0;
	cli();
	--rx_counter;
	sei();
	return data;
}

// Clear the Receive buffer.
void uart_rx_buffer_flush (void)
{
	unsigned int temp;
	cli();
	rx_wr_index = 0;
	rx_rd_index = 0;
	rx_counter = 0;
	rx_buffer_overflow=0;
	sei();
	for (temp=0;temp<RX_BUFFER_SIZE;temp++)
	{
		rx_buffer[temp]=0x00;
	}
}

// Search string in the Receive buffer. The function returns the element at the origin of the first match.
unsigned int uart_tx_buffer_search (int16_t str, int str_length, int start)
{
	int16_t tmp_str;
	char tmp;
	unsigned int index;
	unsigned int length;

	if (rx_counter<str_length)
	{
		return 0;
	}
	if (str_length<1)
	{
		return 0;
	}
	
	tmp_str = str;
	index = start;
	length = 0;

	tmp = (char) pgm_read_byte (tmp_str);
	while (index<rx_counter)
	{
		if(rx_buffer[index]==tmp)
		{
			while (rx_buffer[index+length]==tmp)
			{
				length++;
				tmp_str++;
				tmp = (char) pgm_read_byte (tmp_str);
			}
			if (length==str_length)
			{
				return (index+length);
			}
			else
			{
				tmp_str = str;
				length = 0;
				tmp = (char) pgm_read_byte (tmp_str);
			}		
		}
		index++;			
	};
	return 0;
}

// USART Transmitter interrupt service routine.
ISR(USART_TXC_vect)
{
	if (tx_counter)
	{
		--tx_counter;
		UDR=tx_buffer[tx_rd_index++];
		if (tx_rd_index == TX_BUFFER_SIZE) tx_rd_index=0;
	}	
}

// Write a character to the USART Transmitter buffer.
void uart_putchar(char c)
{
	//Wait For Transmitter to become ready
	while(!(UCSRA & (1<<UDRE)));

	//Now write
	UDR=c;
}

// Send one char to UART.
void uart_printr (char* str)
{
	char tmp;
	tmp = (char)*str;
	while (tmp!=0)
	{
		uart_putchar(tmp);
		str++;
		tmp = (char)*str;
	}
}

// Send one two chars (newline and carriage return) to UART.
void uart_printcrlf (void)
{
	uart_putchar(0x0D);
	uart_putchar(0x0A);
}

// Send to UART line from FLASH.
void uart_printp (uint_farptr_t cmd)
{
	char tmp;
	tmp = (char) pgm_read_byte (cmd);
	while (tmp!=0)
	{
		uart_putchar(tmp);
		cmd++;
		tmp = (char) pgm_read_byte (cmd);
	}
}

// Convert int to a string and send via UART.
void uart_itoa (int value)
{
	unsigned char i;
	unsigned char d;

	unsigned char mystr[5];

	for (i=0;i<5;i++)
	{
		mystr[i] = 0;
	}

	if(value)
	{
		for(i=4;value;i--)
		{
			d=value%10;
			if(d>9)
			{
				d+='7';
			}
			else
			{
				d+='0';
			}
			mystr[i]=d;
			value/=10;
		}
	}

	for(i=0;i<5;i++)
	{
		d=mystr[i];
		if (d != 0)
		{
			uart_putchar(d);
		}
	}
}

// Copy the string from the Receive buffer in the buffer.
void uart_tx_buffer_copy (char* buffer, unsigned int start, unsigned int length)
{
	unsigned int temp;
	//uart_putchar(0x00);
		for (temp=0;temp<length;temp++)
		{
			buffer[temp] = rx_buffer[start+temp];
			//uart_putchar(buffer[temp]);
		}
}

// Initialize UART function.
void uart_init (uint16_t BAUD_RATE)
{
	UBRRH = (((F_CPU/BAUD_RATE)/16)-1)>>8;		// set baud rate
  	UBRRL = (((F_CPU/BAUD_RATE)/16)-1);


	/*Set Frame Format
	
	Asynchronous mode
	No Parity
	1 StopBit
	char size 8

	*/

	UCSRB |= (1 << RXEN) | (1 << TXEN);   // Turn on the transmission and reception circuitry

   	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1); // Use 8-bit character sizes


	/*Enable Interrupts
	RXCIE- Receive complete
	UDRIE- Data register empty

	Enable The receiver and transmitter

	*/

	UCSRB |= (1 << RXCIE); // Enable the USART Receive Complete interrupt (USART_RXC)
   	sei(); // Enable the Global Interrupt Enable flag so that interrupts can be processed

}



