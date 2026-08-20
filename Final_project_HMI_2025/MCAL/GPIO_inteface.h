/*
 * GPIO_inteface.h
 *
 *  Created on: Sep 17, 2025
 *      Author: ziad
 */
#include "../MACROS/STD_types.h"
#include "../MACROS/STD_types.h"
#ifndef GPIO_INTEFACE_H_
#define GPIO_INTEFACE_H_

void GPIO_voidSetPortDirection(u8 Copy_u8PortNum, u8 Copy_u8PortDirection);
void GPIO_voidSetPinDirection(u8 Copy_u8PortNum, u8 Copy_u8PinNum, u8 Copy_u8PinDirection);
void GPIO_voidSetPortValue(u8 Copy_u8PortNum, u8 Copy_u8PortValue);
void GPIO_voidSetPinValue(u8 Copy_u8PortNum, u8 Copy_u8PinNum, u8 Copy_u8PinValue);
u8 GPIO_u8GetPinValue(u8 Copy_u8PortNum, u8 Copy_u8PinNum);


#define PIN0       0
#define PIN1       1
#define PIN2       2
#define PIN3       3
#define PIN4       4
#define PIN5       5
#define PIN6       6
#define PIN7       7

#define PORT_u8A     0
#define PORT_u8B     1
#define PORT_u8C     2
#define PORT_u8D     3

#define PIN_OUT      1
#define PIN_IN       0

#define PIN_HIGH     1
#define PIN_LOW      0

#define PORT_HIGH   0XFF
#define PORT_LOW    0X00

#define PORT_OUT    0XFF
#define PORT_IN     0X00

#define LOW      0

#endif /* GPIO_INTEFACE_H_ */
