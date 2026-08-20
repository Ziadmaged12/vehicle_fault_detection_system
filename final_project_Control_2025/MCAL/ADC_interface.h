/******************************************************************************
 *
 * Module: ADC
 *
 * File Name: ADC_interface.h
 *
 * Description: Header file for the AVR ADC driver (Polling Design)
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/

#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

#include "../MACROS/STD_types.h"
#include "../MACROS/macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

#define ADC_MAXIMUM_VALUE    1023
#define ADC_REF_VOLT_VALUE   2.56

typedef enum
{
	AREF_REF,
	AVCC_REF,
	INTERNAL_REF
} ADC_ReferenceVoltage;

typedef enum
{
	ADC_PRESCALER_2 = 1,
	ADC_PRESCALER_4,
	ADC_PRESCALER_8,
	ADC_PRESCALER_16,
	ADC_PRESCALER_32,
	ADC_PRESCALER_64,
	ADC_PRESCALER_128
} ADC_Prescaler;

typedef struct
{
	ADC_ReferenceVoltage ref_volt;
	ADC_Prescaler prescaler;
} ADC_ConfigType;

/*******************************************************************************
 *                              Function Prototypes                            *
 *******************************************************************************/


void ADC_init(const ADC_ConfigType *Config_Ptr);


u16 ADC_readChannel(u8 channel);

#endif /* ADC_INTERFACE_H_ */
