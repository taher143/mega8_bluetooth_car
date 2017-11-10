/*
 * bluetooth.h
 *
 * Created: 10/14/2017 9:15:06 PM
 *  Author: B2
 */ 


#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

typedef struct
{
	unsigned char status;	//Bluetooth Status i.e. Command status.
	unsigned char mode;		//Bluetooth mode i.e. Connection or Command
	
	unsigned int timerDelay;
	
}BTCON;


//enum BTCONSTATUS { BT_null,BT_Connect,BT_Connect_wait,BT_rst,BT_rst_wait, BT_name,BT_name_wait, BT_pswd,BT_pswd_wait, BT_delay, BT_ready };
//enum BTCONMODE { BT_connection,BT_Command, BT_error };
enum BTCOMMANDS {BT_FW,BT_RV,BT_LF,BT_RT,BT_STOP,BT_READ};

void Bluetooth_cfg_connect (BTCON* connect);

//	Initialize UART and set Name and pair password for Bluetooth.
void Bluetooth_Init(void);

//	Change PIN i.e 1234
void BluetoothPINChange(unsigned char *bt_pin);

// Manage Command sequences.
void BluetoothManager(BTCON* connection);

// Change Bluetooth Name
void BluetoothNameChange(unsigned char *bt_name);

// Enter to Command Mode
void BluetoothCommandmode(void);

//	Bluetooth Communication mode to initiate communication with remote device.
void BluetoothCommunicationmode(void);

void BT_Manager(void);
#endif /* BLUETOOTH_H_ */