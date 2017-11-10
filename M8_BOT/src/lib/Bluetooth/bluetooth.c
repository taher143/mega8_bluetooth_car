/*
* bluetooth.c
*
* Created: 10/14/2017 9:15:21 PM
*  Author: Taher Kawantwala
*/
#include <avr/io.h>
#include <progmem.h>

#include "lib/Bluetooth/bluetooth.h"
#include "lib/UART/uart.h"
#include "lib/Scheduler/schedular.h"
#include "lib/MOTOR/motor.h"

// Delays can be optimized, it will accelerate the speed of sending data. 1 = 10ms, 90 = 900ms = 0,9s
#define DELAY_CFG_01	1
#define DELAY_CFG_02	185
#define DELAY_CFG_03	1
#define DELAY_CFG_04	1
#define DELAY_CFG_05	1
#define DELAY_CFG_06	100

// Constants for use with HC-05.

const char FW[] PROGMEM = {"FW"};	//Forward
const char RV[] PROGMEM = {"RV"};	//Reverse
const char ST[] PROGMEM = {"ST"};	//STOP
const char RT[] PROGMEM = {"RT"};	//Right
const char LT[] PROGMEM = {"LT"};	//Lefts

extern char rx_buffer[];
extern int rx_counter;

volatile BTCON BT_connection;

//////////////////////////////////////////////////////////////////////////
/*	function Declaration */
//////////////////////////////////////////////////////////////////////////

void Bluetooth_cfg_connect (BTCON* connect)
{
	connect->status = BT_READ;
}




void BluetoothManager (BTCON* connect)
{
	// Form a delay before proceeding to the next step.
	
	if (connect->timerDelay>0)
	{
		connect->timerDelay--;
	}
	// Wait for the end of the delay.
	if (connect->timerDelay == 0)
	{
		
		switch (connect->status)
		{
			case BT_READ:
			//uart_printr("*****\r\n");
			if(uart_tx_buffer_search((int16_t)FW,sizeof(FW)-1,0)>0)
			{
				// Clear the receive buffer UART.
				uart_rx_buffer_flush();
				PORTB ^= (1<<PB0);
				connect->status = BT_FW;
				connect->timerDelay = DELAY_CFG_02;
			}
			else if(uart_tx_buffer_search((int16_t)RV,sizeof(RV)-1,0)>0)
			{
				// Clear the receive buffer UART.
				uart_rx_buffer_flush();
				connect->status = BT_RV;
				connect->timerDelay = DELAY_CFG_02;
			}
			else if(uart_tx_buffer_search((int16_t)ST,sizeof(ST)-1,0)>0)
			{
				// Clear the receive buffer UART.
				uart_rx_buffer_flush();
				connect->status = BT_STOP;
				connect->timerDelay = DELAY_CFG_02;
			}
			else if(uart_tx_buffer_search((int16_t)RT,sizeof(RT)-1,0)>0)
			{
				// Clear the receive buffer UART.
				uart_rx_buffer_flush();
				connect->status = BT_RT;
				connect->timerDelay = DELAY_CFG_02;
			}
			else if(uart_tx_buffer_search((int16_t)LT,sizeof(LT)-1,0)>0)
			{
				// Clear the receive buffer UART.
				uart_rx_buffer_flush();
				connect->status = BT_LF;
				connect->timerDelay = DELAY_CFG_02;
			}
			else
			{
				connect->timerDelay = DELAY_CFG_01;
			}
			break;
			case BT_FW:
			uart_printr("bt_fw");
			Motor_forward();
			connect->status = BT_READ;
			connect->timerDelay = DELAY_CFG_02;
			break;
			case BT_RV:
			uart_printr("bt_RV");
			Motor_reverse();
			connect->status = BT_READ;
			connect->timerDelay = DELAY_CFG_02;
			break;
			case BT_STOP:
			uart_printr("bt_ST");
			Motor_stop();
			connect->status = BT_READ;
			connect->timerDelay = DELAY_CFG_02;
			break;
			case BT_RT:
			uart_printr("bt_RT");
			Motor_right();
			connect->status = BT_READ;
			connect->timerDelay = DELAY_CFG_02;
			break;
			case BT_LF:
			uart_printr("bt_LF");
			Motor_left();
			connect->status = BT_READ;
			connect->timerDelay = DELAY_CFG_02;
			break;
			default:
			
			break;
		}
	}
}

void BT_Manager(void)
{
	BluetoothManager((BTCON*)&BT_connection);
}

void Bluetooth_Init(void)
{
	uart_init(9600);
	Bluetooth_cfg_connect((BTCON*)&BT_connection);
	addTask(1,BT_Manager,10);
}
