/*
 * GPIO_program.c
 *
 *  Created on: Sep 17, 2025
 *      Author: ziad
 */
#include<avr/io.h>

#include "../MACROS/macros.h"
#include "../MACROS/STD_types.h"
#include "GPIO_inteface.h"

void GPIO_voidSetPortDirection(u8 Copy_u8PortNum,u8 Copy_u8PortDirection)
{
	switch(Copy_u8PortNum)
	{
	case PORT_u8A:DDRA=Copy_u8PortDirection;
	break;
	case PORT_u8B:DDRB=Copy_u8PortDirection;
	break;
	case PORT_u8C:DDRC=Copy_u8PortDirection;
	break;
	case PORT_u8D:DDRD=Copy_u8PortDirection;
	break;
	}
}
void GPIO_voidSetPinDirection(u8 Copy_u8PortNum,u8 Copy_u8PinNum,u8
		Copy_u8PinDirection)
{
	if(Copy_u8PinDirection==PIN_OUT)
	{
		switch(Copy_u8PortNum)
		{
		case PORT_u8A:SET_BIT(DDRA,Copy_u8PinNum);
		break;
		case PORT_u8B:SET_BIT(DDRB,Copy_u8PinNum);
		break;
		case PORT_u8C:SET_BIT(DDRC,Copy_u8PinNum);
		break;
		case PORT_u8D:SET_BIT(DDRD,Copy_u8PinNum);
		break;
		}
	}
	else if(Copy_u8PinDirection==PIN_IN)
	{
		switch(Copy_u8PortNum)
		{
		case PORT_u8A:CLEAR_BIT(DDRA,Copy_u8PinNum);
		break;
		case PORT_u8B:CLEAR_BIT(DDRB,Copy_u8PinNum);
		break;
		case PORT_u8C:CLEAR_BIT(DDRC,Copy_u8PinNum);
		break;
		case PORT_u8D:CLEAR_BIT(DDRD,Copy_u8PinNum);
		break;
		}
	}
}

void GPIO_voidSetPortValue(u8 Copy_u8PortNum,u8 Copy_u8PortValue)
{
	switch(Copy_u8PortNum)
	{
	case PORT_u8A:PORTA=Copy_u8PortValue;
	break;
	case PORT_u8B:PORTB=Copy_u8PortValue;
	break;
	case PORT_u8C:PORTC=Copy_u8PortValue;
	break;
	case PORT_u8D:PORTD=Copy_u8PortValue;
	break;
	}
}
void GPIO_voidSetPinValue(u8 Copy_u8PortNum,u8 Copy_u8PinNum,u8 Copy_u8PinValue)
{
	if(Copy_u8PinValue==PIN_HIGH)
	{
		switch(Copy_u8PortNum)
		{
		case PORT_u8A:SET_BIT(PORTA,Copy_u8PinNum);
		break;
		case PORT_u8B:SET_BIT(PORTB,Copy_u8PinNum);
		break;
		case PORT_u8C:SET_BIT(PORTC,Copy_u8PinNum);
		break;
		case PORT_u8D:SET_BIT(PORTD,Copy_u8PinNum);
		break;
		}
	}
	else if(Copy_u8PinValue==PIN_LOW)
	{
		switch(Copy_u8PortNum)
		{
		case PORT_u8A:CLEAR_BIT(PORTA,Copy_u8PinNum);
		break;
		case PORT_u8B:CLEAR_BIT(PORTB,Copy_u8PinNum);
		break;
		case PORT_u8C:CLEAR_BIT(PORTC,Copy_u8PinNum);
		break;
		case PORT_u8D:CLEAR_BIT(PORTD,Copy_u8PinNum);
		break;
		}
	}
}

u8 GPIO_u8GetPinValue(u8 Copy_u8PortNum,u8 Copy_u8PinNum)
{
	u8 Local_u8PinState=0;
	switch(Copy_u8PortNum)
	{
	case PORT_u8A:Local_u8PinState=GET_BIT(PINA,Copy_u8PinNum);
	break;
	case PORT_u8B:Local_u8PinState=GET_BIT(PINB,Copy_u8PinNum);
	break;
	case PORT_u8C:Local_u8PinState=GET_BIT(PINC,Copy_u8PinNum);
	break;
	case PORT_u8D:Local_u8PinState=GET_BIT(PIND,Copy_u8PinNum);
	break;
	}

	return Local_u8PinState;
}

