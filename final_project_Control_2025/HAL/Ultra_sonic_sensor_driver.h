/*
 * Ultra_sonic_sensor_driver.h
 *
 *  Created on: Sep 30, 2025
 *      Author: Ziad Maged
 */

#ifndef HAL_ULTRA_SONIC_SENSOR_DRIVER_H_
#define HAL_ULTRA_SONIC_SENSOR_DRIVER_H_

#include"../MACROS/STD_types.h"

#define Ultrasonic_PORT        PORT_u8D

#define Ultrasonic_ECHO_PIN    PIN6
#define Ultrasonic_TRIGGER_PIN PIN7


// ultrasonic initiating function
void Ultrasonic_init(void);

// ultrasonic triggering function
void Ultrasonic_Trigger(void);

// ultrasonic read distance function
u16 Ultrasonic_readDistance(void);

// ultrasonic edge processing function
void Ultrasonic_edgeProcessing(void);
#endif /* HAL_ULTRA_SONIC_SENSOR_DRIVER_H_ */
