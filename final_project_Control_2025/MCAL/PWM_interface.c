/*
 * PWM_interface.c
 *
 * Created on: Sep 28, 2025
 * Author:Ziad Maged
 */

#include <avr/io.h>
#include "PWM_inteface.h"
#include "GPIO_inteface.h"
#include "../MACROS/macros.h"

/**
 * @brief Initializes Timer0 for Fast PWM mode.
 */
void PWM_Timer0_Init(void)
{
	/* 1. Set OC0 (PB3) as an output pin */
	GPIO_voidSetPinDirection(PWM_OC0_PORT, PWM_OC0_PIN, PIN_OUT);

	/* 2. Configure Timer0 for Fast PWM mode */
	SET_BIT(TCCR0, WGM00);
	SET_BIT(TCCR0, WGM01);

	/* 3. Configure Compare Output Mode:
	 * Clear OC0 on compare match, set OC0 at TOP.
	 * (Non-inverting mode)
	 */
	SET_BIT(TCCR0, COM01);
	CLEAR_BIT(TCCR0, COM00);

	/* 4. Set Duty Cycle to 0 initially */
	OCR0 = 0;

	/* 5. Set Clock Prescaler (but don't start timer yet)
	 * The prescaler bits will be set by PWM_Timer0_Start()
	 */
	TCCR0 = (TCCR0 & 0xF8);
}


/**
 * @brief Starts the PWM signal or updates its duty cycle.
 */
void PWM_Timer0_Start(u8 duty_cycle)
{
	/* 1. Set the compare value (duty cycle) */
	/* Calculate (duty_cycle * 255) / 100 */
	OCR0 = (u8)(((u32)duty_cycle * 255) / 100);

	/* 2. Set the prescaler bits to start the timer */
	TCCR0 = (TCCR0 & 0xF8) | (0b011); /* Prescaler 64 */

    /* NOTE: This assumes PWM_PRESCALER_64.
     * If you want to use the #if block, make sure it is correct.
     * For 8MHz clock, 64 is a good choice. F_PWM = 8MHz / (64 * 256) = 488 Hz
     */
}

/**
 * @brief Stops the PWM timer clock.
 */
void PWM_Timer0_Stop(void)
{
	/* Clear clock select bits (CS00, CS01, CS02) */
	TCCR0 = (TCCR0 & 0xF8);
}
