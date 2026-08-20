/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/

#ifndef UART_H_
#define UART_H_

#include "../MACROS/STD_types.h"

typedef enum {
	UART_BAUD_2400 = 2400,
	UART_BAUD_4800 = 4800,
	UART_BAUD_9600 = 9600,
	UART_BAUD_14400 = 14400,
	UART_BAUD_19200 = 19200,
	UART_BAUD_38400 = 38400,
	UART_BAUD_57600 = 57600,
	UART_BAUD_115200 = 115200
} UART_BaudRateType;
typedef enum
{
	UART_5_BIT_DATA,
	UART_6_BIT_DATA,
	UART_7_BIT_DATA,
	UART_8_BIT_DATA,
	UART_9_BIT_DATA
} UART_BitDataType;

typedef enum
{
	UART_NO_PARITY,
	UART_EVEN_PARITY = 2,
	UART_ODD_PARITY = 3
} UART_ParityType;

typedef enum
{
	UART_1_STOP_BIT,
	UART_2_STOP_BIT
} UART_StopBitType;


typedef struct
{
	UART_BitDataType bit_data;
	UART_ParityType parity;
	UART_StopBitType stop_bit;
	UART_BaudRateType baud_rate;
} UART_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(const UART_ConfigType *Config_Ptr);

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const u8 data);

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
u8 UART_recieveByte(void);

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const u8 *Str);

/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(u8 *Str); // Receive until #

#endif /* UART_H_ */
