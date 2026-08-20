/******************************************************************************
 *
 * Module: KEYPAD
 *
 * File Name: keypad.c
 *
 * Description: Source file for the Keypad driver
 *
 * Author: BISHOY KAMEL
 *
 *******************************************************************************/
#include "keypad.h"
#include "../MCAL/GPIO_inteface.h"
#include <util/delay.h>

/*******************************************************************************
 *                      Functions Prototypes(Private)                          *
 *******************************************************************************/

#ifndef STANDARD_KEYPAD

#if (KEYPAD_NUM_COLS == 3)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x3 keypad
 */
static u8 KEYPAD_4x3_adjustKeyNumber(u8 button_number);
#elif (KEYPAD_NUM_COLS == 4)
/*
 * Function responsible for mapping the switch number in the keypad to
 * its corresponding functional number in the proteus for 4x4 keypad
 */
static u8 KEYPAD_4x4_adjustKeyNumber(u8 button_number);
#endif

#endif /* STANDARD_KEYPAD */

/*******************************************************************************
 * Functions Definitions                                  *
 *******************************************************************************/

void keypad_init(void)
{
	u8 i;

	/* Setup Row Pins as Inputs (default state) */
	for(i=0; i < KEYPAD_NUM_ROWS; i++)
	{
		GPIO_voidSetPinDirection(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID + i, PIN_IN);
	}

	/* Setup Column Pins as Inputs with Pull-up Resistors Enabled */
	for(i=0; i < KEYPAD_NUM_COLS; i++)
	{
		GPIO_voidSetPinDirection(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + i, PIN_IN);
		GPIO_voidSetPinValue(KEYPAD_COL_PORT_ID, KEYPAD_FIRST_COL_PIN_ID + i, PIN_HIGH); // Enable Pull-up
	}
}


u8 KEYPAD_getPressedKey(void)
{
	u8 col,row;

	/* * All pins are assumed to be initialized by KEYPAD_voidInit()
	 * Rows = IN
	 * Cols = IN (with pull-up)
	 */

	while(1)
	{
		for(row=0 ; row<KEYPAD_NUM_ROWS ; row++) /* loop for rows */
		{
			/* Set the current row as OUT and drive it LOW */
			GPIO_voidSetPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,PIN_OUT);
			GPIO_voidSetPinValue(KEYPAD_ROW_PORT_ID, KEYPAD_FIRST_ROW_PIN_ID+row, KEYPAD_BUTTON_PRESSED);

			for(col=0 ; col<KEYPAD_NUM_COLS ; col++) /* loop for columns */
			{
				/* Check if the switch is pressed (column is pulled LOW) */
				if(GPIO_u8GetPinValue(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED)
				{
					/* * --- DEBOUNCE ---
					 * Wait for the button press to stabilize
					 */
					_delay_ms(20);

					/* Check again to confirm the press */
					if(GPIO_u8GetPinValue(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED)
					{
						/* --- WAIT FOR RELEASE ---
						 * Wait until the user releases the key before returning
						 * This prevents multiple reads for one long press.
						 */
						while(GPIO_u8GetPinValue(KEYPAD_COL_PORT_ID,KEYPAD_FIRST_COL_PIN_ID+col) == KEYPAD_BUTTON_PRESSED);

						/* Calculate the button number */
						u8 button_number = ((row*KEYPAD_NUM_COLS)+col+1);

						#if (KEYPAD_NUM_COLS == 3)
							#ifdef STANDARD_KEYPAD
								return button_number;
							#else
								return KEYPAD_4x3_adjustKeyNumber(button_number);
							#endif
						#elif (KEYPAD_NUM_COLS == 4)
							#ifdef STANDARD_KEYPAD
								return button_number;
							#else
								return KEYPAD_4x4_adjustKeyNumber(button_number);
							#endif
						#endif
					}
				}
			}

			/* Set the row back to IN before checking the next row */
			GPIO_voidSetPinDirection(KEYPAD_ROW_PORT_ID,KEYPAD_FIRST_ROW_PIN_ID+row,PIN_IN);
			_delay_ms(5); /* Delay for Proteus simulation stability */
		}
	}	
}

#ifndef STANDARD_KEYPAD

#if (KEYPAD_NUM_COLS == 3)
/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x3 shape
 * (FIXED: Returns ASCII characters for numbers)
 */
static u8 KEYPAD_4x3_adjustKeyNumber(u8 button_number)
{
	u8 keypad_button = 0;
	switch(button_number)
	{
	case 1: keypad_button = '1';
					break;
			case 2: keypad_button = '2';
					break;
			case 3: keypad_button ='3';
					break;
			case 4: keypad_button = '4'; // ASCII Code of %
					break;
			case 5: keypad_button = '5';
					break;
			case 6: keypad_button ='6';
					break;
			case 7: keypad_button = '7';
					break;
			case 8: keypad_button = '8'; /* ASCII Code of '*' */
					break;
			case 9: keypad_button = '9';
					break;
			case 10: keypad_button = '10';
					break;
			case 11: keypad_button = '11';
					break;
			case 12: keypad_button = '12'; /* ASCII Code of '-' */
					break;
			case 13: keypad_button = '13';  /* ASCII of Enter */
					break;
			case 14: keypad_button = '14';
					break;
			case 15: keypad_button = '15'; /* ASCII Code of '=' */
					break;
			case 16: keypad_button = '16'; /* ASCII Code of '+' */
					break;
			default: keypad_button = button_number;
					break;
	}
	return keypad_button;
} 

#elif (KEYPAD_NUM_COLS == 4)

/*
 * Description :
 * Update the keypad pressed button value with the correct one in keypad 4x4 shape
 * (FIXED: Returns ASCII characters for numbers)
 */
static u8 KEYPAD_4x4_adjustKeyNumber(u8 button_number)
{
	u8 keypad_button = 0;
	switch(button_number)
	{
	/* Row 0 */
			case 1: keypad_button = '1';
					break;
			case 2: keypad_button = '2';
					break;
			case 3: keypad_button = '3';
					break;
			case 4: keypad_button = '4'; // '÷' (Division)
					break;

			/* Row 1 */
			case 5: keypad_button = '5';
					break;
			case 6: keypad_button = '6';
					break;
			case 7: keypad_button = '7';
					break;
			case 8: keypad_button = '8'; // 'x' (Multiplication)
					break;

			/* Row 2 */
			case 9: keypad_button = '9';
					break;
			case 10: keypad_button = '10';
					break;
			case 11: keypad_button = '11';
					break;
			case 12: keypad_button = '12';
					break;

			/* Row 3 */
			case 13: keypad_button = '13'; // 'ON/C' (Clear)
					break;
			case 14: keypad_button = '14';
					break;
			case 15: keypad_button = '15';
					break;
			case 16: keypad_button = '16';
					break;

			default: keypad_button = '100'; // Should not happen
					break;
	}
	return keypad_button;
} 

#endif

#endif /* STANDARD_KEYPAD */
