/*
 * timer1.c
 *
 *  Created on: Sep 7, 2019
 *      Author: Newname
 */


#include "../macros.h"
#include "../MCAL/method.h"
#include "timer1.h"
#include "../types.h"



// Global interrupt
#define SREG_M		*((volatile u8*) 0x5F)


// TIMER1 SPECIAL REGs
#define TCCR1A	*((volatile u16*) 0x4F)
#define TCCR1B	*((volatile u16*) 0x4E)
#define TCNT1H	*((volatile u8*) 0x4D)
#define TCNT1L	*((volatile u8*) 0x4C)
#define OCR1AH	*((volatile u8*) 0x4B)
#define OCR1AL	*((volatile u8*) 0x4A)
#define OCR1A_WHOLE	*((volatile u16*) 0x4A)
#define OCR1BH	*((volatile u8*) 0x49)
#define OCR1BL	*((volatile u8*) 0x48)
#define OCR1B_WHOLE	*((volatile u16*) 0x48)
#define ICR1H	*((volatile u8*) 0x47)
#define ICR1L	*((volatile u8*) 0x46)
// END OF TIMER1 SPECIAL REGs

#define TIMSK	*((volatile u8*) 0x59)
#define TIFR	*((volatile u8*) 0x58)

// PINS on REGs
#define CS10	0
#define CS11	1
#define CS12	2
#define WGM12_TCCR1B	3
#define WGM13_TCCR1B	4
#define OCIE1B			3
#define OCIE1A			4

#define WGM10_TCCR1A	0
#define WGM11_TCCR1A	1

// Pointer to function
void (*Timer1_callback)(void) = 0;


f32 Tick_timer = 0;

// Value returned is Tick_time in microseconds
f32 Timer1_vidPreScaler(u16 scale)
{
	switch(scale)
	{
	case 1:
		set_bit(TCCR1B, CS10);
		clr_bit(TCCR1B, CS11);
		clr_bit(TCCR1B, CS12);
		break;

	case 8:
		clr_bit(TCCR1B, CS10);
		set_bit(TCCR1B, CS11);
		clr_bit(TCCR1B, CS12);
		break;

	case 64:
		set_bit(TCCR1B, CS10);
		set_bit(TCCR1B, CS11);
		clr_bit(TCCR1B, CS12);
		break;

	case 256:
		clr_bit(TCCR1B, CS10);
		clr_bit(TCCR1B, CS11);
		set_bit(TCCR1B, CS12);
		break;

	case 1024:
		set_bit(TCCR1B, CS10);
		clr_bit(TCCR1B, CS11);
		set_bit(TCCR1B, CS12);
		break;

	default:
		clr_bit(TCCR1B, CS10);
		clr_bit(TCCR1B, CS11);
		clr_bit(TCCR1B, CS12);
		return (Tick_timer = (scale / 8));
		break;
	}

	return (Tick_timer = (scale / 8));
}


void Timer1_vidInitialtize(u16 scale)
{
	// Set global interrupt
	set_bit(SREG_M, I_G_flag);

	// Set interrupt enable for timer1
	set_bit(TIMSK, OCIE1A);

	// CTC mode of operation
	set_bit(TCCR1B, WGM12_TCCR1B);
	clr_bit(TCCR1B, WGM13_TCCR1B);
	clr_bit(TCCR1B, WGM10_TCCR1A);
	clr_bit(TCCR1B, WGM11_TCCR1A);

	// set prescale
	Timer1_vidPreScaler(scale);
}

void Timer1_vidSetOCRValue(u16 counts)
{
	OCR1A_WHOLE = counts;
}

extern void Timer1_setcallback(void(*function)(void))
{
	Timer1_callback = function;
}

ISR(7)
{
	if(Timer1_callback != 0)
	{
		Timer1_callback();
	}
}
