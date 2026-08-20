/*
 * LM35_driver.h
 *
 *  Created on: Sep 27, 2025
 *      Author: Ziad Maged
 */

#ifndef HAL_LM35_DRIVER_H_
#define HAL_LM35_DRIVER_H_

#include"../MACROS/STD_types.h"
#include"../MCAL/GPIO_inteface.h"
#include"../MCAL/ADC_interface.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define SENSOR_CHANNEL_ID         0
#define SENSOR_MAX_VOLT_VALUE     1.5
#define SENSOR_MAX_TEMPERATURE    150

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/

/*
 * Description :
 * Function responsible for calculate the temperature from the ADC digital value.
 */
u8 LM35_getTemperature(void);


#endif /* HAL_LM35_DRIVER_H_ */
