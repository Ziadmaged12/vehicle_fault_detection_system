/******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC_interface.c
 *
 * Description: Source file for the AVR ADC driver (Polling Design)
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/

#include "ADC_interface.h"

void ADC_init(const ADC_ConfigType *Config_Ptr)
{
	/* 1. Select Voltage Reference */
	switch (Config_Ptr->ref_volt)
	{
	case AREF_REF:
		CLEAR_BIT(ADMUX, REFS0);
		CLEAR_BIT(ADMUX, REFS1);
		break;

	case AVCC_REF:
		SET_BIT(ADMUX, REFS0);
		CLEAR_BIT(ADMUX, REFS1);
		break;

	case INTERNAL_REF:
		SET_BIT(ADMUX, REFS0);
		SET_BIT(ADMUX, REFS1);
		break;
	}


	CLEAR_BIT(ADMUX, ADLAR);

	SET_BIT(ADCSRA, ADEN);

	ADCSRA &= 0xF8;
	ADCSRA |= (Config_Ptr->prescaler & 0x07);

	CLEAR_BIT(ADCSRA, ADIE);
}

u16 ADC_readChannel(u8 channel)
{
	channel &= 0x07;
	ADMUX = (ADMUX & 0xE0) | channel;

	SET_BIT(ADCSRA, ADSC);

	while (BIT_IS_SET(ADCSRA, ADSC));

	SET_BIT(ADCSRA, ADIF);

	return ADC;
}
