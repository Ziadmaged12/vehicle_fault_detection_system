/******************************************************************************
 *
 * Module: Timer
 *
 * File Name: Timer_Program.c
 *
 * Description: Source file for the AVR Timers driver with dynamic configuration
 *
 * Author: Ziad Maged
 *
 *******************************************************************************/

#include "Timers_Interface.h"
#include <avr/interrupt.h>

/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Pointers to callback functions */
static volatile void (*g_Timer0_CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer1_CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_Timer2_CallBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                          Functions Definitions                              *
 *******************************************************************************/

void Timer_init(const Timer_ConfigType *Config_Ptr)
{
	switch (Config_Ptr->timer_ID)
	{
	case TIMER0_ID:
		/* Set initial value */
		TCNT0 = (u8)Config_Ptr->timer_InitialValue;

		if (Config_Ptr->timer_mode == NORMAL_MODE)
		{
			/* Normal mode */
			CLEAR_BIT(TCCR0, WGM00);
			CLEAR_BIT(TCCR0, WGM01);

			/* Enable overflow interrupt */
			SET_BIT(TIMSK, TOIE0);
		}
		else if (Config_Ptr->timer_mode == COMPARE_MODE)
		{
			/* CTC mode */
			CLEAR_BIT(TCCR0, WGM00);
			SET_BIT(TCCR0, WGM01);

			/* Set Compare value */
			OCR0 = (u8)Config_Ptr->timer_Compare_MatchValue;

			/* Enable compare interrupt */
			SET_BIT(TIMSK, OCIE0);
		}

		/* Configure the clock */
		TCCR0 = (TCCR0 & 0xF8) | (Config_Ptr->timer_clock & 0x07);
		break;

	case TIMER1_ID:
		/* Set initial value */
		TCNT1 = Config_Ptr->timer_InitialValue;

		if (Config_Ptr->timer_mode == NORMAL_MODE)
		{
			/* Normal mode */
			CLEAR_BIT(TCCR1A, WGM10);
			CLEAR_BIT(TCCR1A, WGM11);
			CLEAR_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1B, WGM13);

			/* Enable overflow interrupt */
			SET_BIT(TIMSK, TOIE1);
		}
		else if (Config_Ptr->timer_mode == COMPARE_MODE)
		{
			/* CTC mode (OCR1A as TOP) */
			CLEAR_BIT(TCCR1A, WGM10);
			CLEAR_BIT(TCCR1A, WGM11);
			SET_BIT(TCCR1B, WGM12);
			CLEAR_BIT(TCCR1B, WGM13);

			/* Set Compare value */
			OCR1A = Config_Ptr->timer_Compare_MatchValue;

			/* Enable compare A interrupt */
			SET_BIT(TIMSK, OCIE1A);
		}

		/* Configure the clock */
		TCCR1B = (TCCR1B & 0xF8) | (Config_Ptr->timer_clock & 0x07);
		break;

	case TIMER2_ID:
		/* Set initial value */
		TCNT2 = (u8)Config_Ptr->timer_InitialValue;

		if (Config_Ptr->timer_mode == NORMAL_MODE)
		{
			CLEAR_BIT(TCCR2, WGM20);
			CLEAR_BIT(TCCR2, WGM21);
			SET_BIT(TIMSK, TOIE2);
		}
		else if (Config_Ptr->timer_mode == COMPARE_MODE)
		{
			CLEAR_BIT(TCCR2, WGM20);
			SET_BIT(TCCR2, WGM21);
			OCR2 = (u8)Config_Ptr->timer_Compare_MatchValue;
			SET_BIT(TIMSK, OCIE2);
		}

		/* Configure clock */
		TCCR2 = (TCCR2 & 0xF8) | (Config_Ptr->timer_clock & 0x07);
		break;
	}
}

void Timer_deInit(Timer_ID_Type timer_type)
{
	switch (timer_type)
	{
	case TIMER0_ID:
		TCCR0 = 0;
		TCNT0 = 0;
		CLEAR_BIT(TIMSK, TOIE0);
		CLEAR_BIT(TIMSK, OCIE0);
		break;

	case TIMER1_ID:
		TCCR1A = 0;
		TCCR1B = 0;
		TCNT1 = 0;
		CLEAR_BIT(TIMSK, TOIE1);
		CLEAR_BIT(TIMSK, OCIE1A);
		break;

	case TIMER2_ID:
		TCCR2 = 0;
		TCNT2 = 0;
		CLEAR_BIT(TIMSK, TOIE2);
		CLEAR_BIT(TIMSK, OCIE2);
		break;
	}
}

void Timer_setCallBack(void(*a_ptr)(void), Timer_ID_Type a_timer_ID)
{
	switch (a_timer_ID)
	{
	case TIMER0_ID: g_Timer0_CallBackPtr = a_ptr; break;
	case TIMER1_ID: g_Timer1_CallBackPtr = a_ptr; break;
	case TIMER2_ID: g_Timer2_CallBackPtr = a_ptr; break;
	}
}

/*******************************************************************************
 *                          ISR Definitions                                    *
 *******************************************************************************/

/* Timer0 Overflow */
ISR(TIMER0_OVF_vect)
{
	if (g_Timer0_CallBackPtr != NULL_PTR)
		(*g_Timer0_CallBackPtr)();
}

/* Timer0 Compare Match */
ISR(TIMER0_COMP_vect)
{
	if (g_Timer0_CallBackPtr != NULL_PTR)
		(*g_Timer0_CallBackPtr)();
}

/* Timer1 Overflow */
ISR(TIMER1_OVF_vect)
{
	if (g_Timer1_CallBackPtr != NULL_PTR)
		(*g_Timer1_CallBackPtr)();
}

/* Timer1 Compare Match A */
ISR(TIMER1_COMPA_vect)
{
	if (g_Timer1_CallBackPtr != NULL_PTR)
		(*g_Timer1_CallBackPtr)();
}

/* Timer2 Overflow */
ISR(TIMER2_OVF_vect)
{
	if (g_Timer2_CallBackPtr != NULL_PTR)
		(*g_Timer2_CallBackPtr)();
}

/* Timer2 Compare Match */
ISR(TIMER2_COMP_vect)
{
	if (g_Timer2_CallBackPtr != NULL_PTR)
		(*g_Timer2_CallBackPtr)();
}
