/*
 * PWM_driver.h
 *
 * Created on: Sep 28, 2025
 * Author: Ziad Maged
 */

#ifndef HAL_PWM_DRIVER_H_
#define HAL_PWM_DRIVER_H_

#include "../MACROS/STD_types.h"
#include "../MCAL/GPIO_inteface.h"

#define PWM_OC0_PORT        PORT_u8B
#define PWM_OC0_PIN         PIN3

#define PWM_PRESCALER_1         1
#define PWM_PRESCALER_8         8
#define PWM_PRESCALER_64        64
#define PWM_PRESCALER_256       256
#define PWM_PRESCALER_1024      1024


#define PWM_PRESCALER  PWM_PRESCALER_64 /* Use 64 for a good balance */


/**
 * @brief Initializes Timer0 for Fast PWM mode.
 * Sets the OC0 (PB3) pin as an output.
 * Must be called once at setup.
 */
void PWM_Timer0_Init(void);

/**
 * @brief Starts the PWM signal with a given duty cycle.
 * @param duty_cycle Speed from 0 (stop) to 100 (full).
 */
void PWM_Timer0_Start(u8 duty_cycle);

/**
 * @brief Stops the PWM timer clock.
 */
void PWM_Timer0_Stop(void);

#endif /* HAL_PWM_DRIVER_H_ */
