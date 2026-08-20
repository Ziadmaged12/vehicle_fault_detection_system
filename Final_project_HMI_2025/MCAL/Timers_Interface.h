/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer_interface.h
 *
 * Description: Header file for the AVR Timers driver
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/

#ifndef TIMER_INTERFACE_H_
#define TIMER_INTERFACE_H_

#include "../MACROS/STD_types.h"
#include "../MACROS/macros.h"
#include <avr/io.h>

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum
{
	TIMER0_ID,
	TIMER1_ID,
	TIMER2_ID
} Timer_ID_Type;

typedef enum
{
	NO_CLOCK,
	F_CPU_CLOCK,
	F_CPU_8,
	F_CPU_64,
	F_CPU_256,
	F_CPU_1024
} Timer_ClockType;

typedef enum
{
	NORMAL_MODE,
	COMPARE_MODE
} Timer_ModeType;

/* Configuration structure */
typedef struct
{
	u16 timer_InitialValue;
	u16 timer_Compare_MatchValue;  /* Used only in compare mode */
	Timer_ID_Type timer_ID;
	Timer_ClockType timer_clock;
	Timer_ModeType timer_mode;
} Timer_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * Initialize the Timer driver according to the configuration structure.
 */
void Timer_init(const Timer_ConfigType *Config_Ptr);

/*
 * Description :
 * Disable the specified Timer completely.
 */
void Timer_deInit(Timer_ID_Type timer_type);

/*
 * Description :
 * Set the callback function for the given Timer ID.
 */
void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID);

#endif /* TIMER_INTERFACE_H_ */
