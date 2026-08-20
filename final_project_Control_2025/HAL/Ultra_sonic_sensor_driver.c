/*
 * Ultra_sonic_sensor_driver.c
 *
 *  Created on: Sep 30, 2025
 *      Author:Ziad Maged
 */
#include<avr/io.h>
#include<util/delay.h>

#include"../MACROS/STD_types.h"
#include"../MACROS/macros.h"
#include"Ultra_sonic_sensor_driver.h"
#include"../MCAL/GPIO_inteface.h"
#include"../MCAL/ICU_interface.h"

static volatile u8 g_edgeCount = 0;

static volatile u16 g_timeHigh = 0;

void Ultrasonic_init(void)
{
    ICU_ConfigType ICU_config;

    ICU_config.clock = F_CPU_8;
    ICU_config.edge  = RAISING;

    ICU_init(&ICU_config);
    ICU_setCallBack(Ultrasonic_edgeProcessing);

    GPIO_voidSetPinDirection(Ultrasonic_PORT, Ultrasonic_TRIGGER_PIN, PIN_OUT);
    GPIO_voidSetPinDirection(Ultrasonic_PORT, Ultrasonic_ECHO_PIN, PIN_IN);
}

void Ultrasonic_Trigger(void)
{
    GPIO_voidSetPinValue(Ultrasonic_PORT, Ultrasonic_TRIGGER_PIN, PIN_HIGH);
    _delay_us(10);
    GPIO_voidSetPinValue(Ultrasonic_PORT, Ultrasonic_TRIGGER_PIN, PIN_LOW);
}

u16 Ultrasonic_readDistance(void)
{
    u16 distance = 0;
    g_edgeCount = 0;

    Ultrasonic_Trigger();

    while(g_edgeCount < 2);
    distance = g_timeHigh * 0.01715;

    return distance;
}

void Ultrasonic_edgeProcessing(void)
{
    g_edgeCount++;

    if(g_edgeCount == 1)
    {
        ICU_clearTimerValue();
        ICU_setEdgeDetectionType(FALLING);
    }
    else if(g_edgeCount == 2)
    {
        g_timeHigh = ICU_getInputCaptureValue();
        ICU_setEdgeDetectionType(RAISING);
    }
}
