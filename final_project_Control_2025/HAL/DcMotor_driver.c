/*
 * DcMotor_driver.c
 *
 * Created on: Sep 28, 2025
 * Author: ziad
 */

#include "DcMotor_driver.h"
#include "../MCAL/GPIO_inteface.h"
#include "../MCAL/PWM_inteface.h"

void DcMotor_Init(void)
{
	GPIO_voidSetPinDirection(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, PIN_OUT);
	GPIO_voidSetPinDirection(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, PIN_OUT);
	GPIO_voidSetPinDirection(MOTOR1_ENABLE_PORT, MOTOR1_ENABLE_PIN, PIN_OUT); /* PB3 (OC0) */

	/* Stop motor initially */
	GPIO_voidSetPinValue(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, PIN_LOW);
	GPIO_voidSetPinValue(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, PIN_LOW);

	GPIO_voidSetPinDirection(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, PIN_OUT);
	GPIO_voidSetPinDirection(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, PIN_OUT);
	GPIO_voidSetPinDirection(MOTOR2_ENABLE_PORT, MOTOR2_ENABLE_PIN, PIN_OUT); /* PB6 (GPIO) */

	/* Stop motor initially */
	GPIO_voidSetPinValue(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, PIN_LOW);
	GPIO_voidSetPinValue(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, PIN_LOW);
	GPIO_voidSetPinValue(MOTOR2_ENABLE_PORT, MOTOR2_ENABLE_PIN, PIN_LOW); /* Turn off motor 2 */
}

void DcMotor_Rotate(DcMotor_ID motorid ,DcMotor_State state, u8 speed)
{
	/* Limit speed to maximum 100% */
	if(speed > 100)
		speed = 100;

	/*
	 * !!! CRITICAL FIX !!!
	 * Use an "if... else if..." structure to separate motor logic.
	 */

	if(motorid == MOTOR_1ID)
	{
		/* --- Logic for Motor 1 (Uses PWM) --- */
		switch(state)
		{
		case MOTOR_STOP:
			GPIO_voidSetPinValue(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, PIN_LOW);
			GPIO_voidSetPinValue(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, PIN_LOW);
			PWM_Timer0_Start(0); /* Speed = 0 */
			break;

		case MOTOR_CW:
			GPIO_voidSetPinValue(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, PIN_HIGH);
			GPIO_voidSetPinValue(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, PIN_LOW);
			PWM_Timer0_Start(speed); /* Set speed */
			break;

		case MOTOR_ACW:
			GPIO_voidSetPinValue(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, PIN_LOW);
			GPIO_voidSetPinValue(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, PIN_HIGH);
			PWM_Timer0_Start(speed); /* Set speed */
			break;

		default:
			GPIO_voidSetPinValue(MOTOR1_IN1_PORT, MOTOR1_IN1_PIN, PIN_LOW);
			GPIO_voidSetPinValue(MOTOR1_IN2_PORT, MOTOR1_IN2_PIN, PIN_LOW);
			PWM_Timer0_Start(0);
			break;
		}
	}
	else if (motorid == MOTOR_2ID)
	{
		/* --- Logic for Motor 2 (Uses GPIO Enable) --- */
		/* Note: The 'speed' parameter is ignored for this motor */
		switch(state)
		{
		case MOTOR_STOP:
			GPIO_voidSetPinValue(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, PIN_LOW);
			GPIO_voidSetPinValue(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, PIN_LOW);
			/* Turn off motor enable */
			GPIO_voidSetPinValue(MOTOR2_ENABLE_PORT, MOTOR2_ENABLE_PIN, PIN_LOW);
			break;

		case MOTOR_CW:
			GPIO_voidSetPinValue(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, PIN_HIGH);
			GPIO_voidSetPinValue(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, PIN_LOW);
			/* Turn on motor enable */
			GPIO_voidSetPinValue(MOTOR2_ENABLE_PORT, MOTOR2_ENABLE_PIN, PIN_HIGH);
			break;

		case MOTOR_ACW:
			GPIO_voidSetPinValue(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, PIN_LOW);
			GPIO_voidSetPinValue(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, PIN_HIGH);
			/* Turn on motor enable */
			GPIO_voidSetPinValue(MOTOR2_ENABLE_PORT, MOTOR2_ENABLE_PIN, PIN_HIGH);
			break;

		default:
			GPIO_voidSetPinValue(MOTOR2_IN1_PORT, MOTOR2_IN1_PIN, PIN_LOW);
			GPIO_voidSetPinValue(MOTOR2_IN2_PORT, MOTOR2_IN2_PIN, PIN_LOW);
			GPIO_voidSetPinValue(MOTOR2_ENABLE_PORT, MOTOR2_ENABLE_PIN, PIN_LOW);
			break;
		}
	}
}
