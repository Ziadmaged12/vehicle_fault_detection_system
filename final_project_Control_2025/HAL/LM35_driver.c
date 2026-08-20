/*
 * LM35_driver.c
 *
 *  Created on: Sep 27, 2025
 *      Author: Ziad Maged
 */

#include "LM35_driver.h"
#include "../MCAL/ADC_interface.h"

u8 LM35_getTemperature(void)
{
    u16 adc_value = ADC_readChannel(SENSOR_CHANNEL_ID);

    /* Temp = (ADC * Vref * 100) / 1024 */
    u8 temp_value = (u8)(((u32)adc_value * (ADC_REF_VOLT_VALUE) * 100) / ADC_MAXIMUM_VALUE);

    return temp_value;
}

