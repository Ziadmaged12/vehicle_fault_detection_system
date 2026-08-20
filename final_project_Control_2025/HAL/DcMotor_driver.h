/*
 * DcMotor_driver.h
 *
 *  Created on: Sep 28, 2025
 *      Author: ziad
 */

#ifndef HAL_DC_MOTOR_DRIVER_H_
#define HAL_DC_MOTOR_DRIVER_H_

#include "../MACROS/STD_types.h"
#include "../MCAL/GPIO_inteface.h"


#define MOTOR1_IN1_PORT      PORT_u8A
#define MOTOR1_IN1_PIN       PIN2

#define MOTOR1_IN2_PORT      PORT_u8A
#define MOTOR1_IN2_PIN       PIN3

#define MOTOR1_ENABLE_PORT   PORT_u8B
#define MOTOR1_ENABLE_PIN    PIN3

#define MOTOR2_IN1_PORT      PORT_u8A
#define MOTOR2_IN1_PIN       PIN6

#define MOTOR2_IN2_PORT      PORT_u8A
#define MOTOR2_IN2_PIN       PIN7

#define MOTOR2_ENABLE_PORT   PORT_u8B
#define MOTOR2_ENABLE_PIN    PIN3

typedef enum
{
    MOTOR_STOP,
    MOTOR_CW,
    MOTOR_ACW
} DcMotor_State;

typedef enum
{
	MOTOR_1ID=0,
	MOTOR_2ID=1
} DcMotor_ID;
void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_ID motorid ,DcMotor_State state, u8 speed);

#endif /* HAL_DC_MOTOR_DRIVER_H_ */
