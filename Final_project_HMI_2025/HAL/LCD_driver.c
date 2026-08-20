/*
 * LCD_driver.c
 *
 *  Created on: Sep 17, 2025
 *      Author: ziad
 */

/*******************************************************
 * *** THE FIX ***
 * Define F_CPU before including util/delay.h
 *******************************************************/
#ifndef F_CPU
#define F_CPU 8000000UL
#endif
/*******************************************************/

/*******************************************************************************
 * Functions Definitions                                  *
 *******************************************************************************/

/* ... (rest of the file is correct) ... */
#include <util/delay.h> /* For the delay functions */
#include "../MACROS/macros.h" /* For GET_BIT Macro */
#include "LCD_driver.h"
#include "../MCAL/GPIO_inteface.h"

/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/

/*
 * Description :
 * Initialize the LCD:
 * 1. Setup the LCD pins directions by use the GPIO driver.
 * 2. Setup the LCD Data Mode 8-bits.
 */
void LCD_init(void)
{
	/* Configure the direction for RS and E pins as OUT pins */
	GPIO_voidSetPinDirection(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_OUT);
	GPIO_voidSetPinDirection(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_OUT);

	/* Configure the data port as OUT port */
	GPIO_voidSetPortDirection(LCD_DATA_PORT_ID,PORT_OUT);

	_delay_ms(20);		/* LCD Power ON delay always > 15ms */

	/* use 2-lines LCD + 8-bits Data Mode + 5*7 dot display Mode */
	LCD_sendCommand(LCD_TWO_LINES_EIGHT_BITS_MODE);
	LCD_sendCommand(LCD_CURSOR_OFF); /* cursor off */
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* clear LCD at the beginning */
}

/*
 * Description :
 * Send the required command to the screen
 */
void LCD_sendCommand(u8 command)
{
	GPIO_voidSetPinValue(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_LOW); /* Instruction Mode RS=0 */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	GPIO_voidSetPinValue(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
	GPIO_voidSetPortValue(LCD_DATA_PORT_ID,command); /* out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_voidSetPinValue(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
}

/*
 * Description :
 * Display the required character on the screen
 */
void LCD_displayCharacter(u8 data)
{
	GPIO_voidSetPinValue(LCD_RS_PORT_ID,LCD_RS_PIN_ID,PIN_HIGH); /* Data Mode RS=1 */
	_delay_ms(1); /* delay for processing Tas = 50ns */
	GPIO_voidSetPinValue(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_HIGH); /* Enable LCD E=1 */
	_delay_ms(1); /* delay for processing Tpw - Tdws = 190ns */
	GPIO_voidSetPortValue(LCD_DATA_PORT_ID,data); /* out the required command to the data bus D0 --> D7 */
	_delay_ms(1); /* delay for processing Tdsw = 100ns */
	GPIO_voidSetPinValue(LCD_E_PORT_ID,LCD_E_PIN_ID,PIN_LOW); /* Disable LCD E=0 */
	_delay_ms(1); /* delay for processing Th = 13ns */
}

/*
 * Description :
 * Display the required string on the screen
 */
void LCD_displayString(const char *Str)
{
	u8 i = 0;
	while(Str[i] != '\0')
	{
		LCD_displayCharacter(Str[i]);
		i++;
	}
	/***************** Another Method ***********************
	while((*Str) != '\0')
	{
		LCD_displayCharacter(*Str);
		Str++;
	}
	*********************************************************/
}

/*
 * Description :
 * Move the cursor to a specified row and column index on the screen
 */
void LCD_moveCursor(u8 row,u8 col)
{
	u8 lcd_memory_address;

	/* Calculate the required address in the LCD DDRAM */
	switch(row)
	{
		case 0:
			lcd_memory_address=col;
				break;
		case 1:
			lcd_memory_address=col+0x40;
				break;
		case 2:
			lcd_memory_address=col+0x10;
				break;
		case 3:
			lcd_memory_address=col+0x50;
				break;
	}
	/* Move the LCD cursor to this specific address */
	LCD_sendCommand(lcd_memory_address | LCD_SET_CURSOR_LOCATION);
}

/*
 * Description :
 * Display the required string in a specified row and column index on the screen
 */
void LCD_displayStringRowColumn(u8 row,u8 col,const char *Str)
{
	LCD_moveCursor(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}

/*
 * Description :
 * Display the required decimal value on the screen
 */
void LCD_intgerToString(int data)
{
   char buff[16]; /* String to hold the ascii result */
   itoa(data,buff,10); /* Use itoa C function to convert the data to its corresponding ASCII value, 10 for decimal */
   LCD_displayString(buff); /* Display the string */
}

/*
 * Description :
 * Send the clear screen command
 */
void LCD_clearScreen(void)
{
	LCD_sendCommand(LCD_CLEAR_COMMAND); /* Send clear display command */
}
