/*
 * lock.c
 *
 *  Created on: Sep 10, 2019
 *      Author: Newname
 */

#define F_CPU 8000000ul
#include "MCAL/DIO.h"
#include "MCAL/method.h"
#include "HAL/timer1.h"
#include "MCAL/LinkedList.h"
#include "HAL/LCD.h"
#include "HAL/KEY_PAD.h"
#include <util/delay.h>

u8 location_before_timer = 0;

u8 counter = 0;
u8 seconds = 0, minutes = 0, hours = 0, crypt = 0;
u8 hide_me = 0;

void APP(void);



int main(void)
{
	// Print function simulation to output data
	LCD_vidInitialize();

	// Output port to control other hardware on kit
	DIO_vidSetPortxDir(PORTA, OUTPUT);

	// Set clock positions
	LCD_vidGoTo(9, 2);
	LCD_String("00");
	LCD_vidGoTo(11, 2);
	LCD_String(":");
	LCD_String("00");
	LCD_vidGoTo(14, 2);
	LCD_String(":");
	LCD_String("00");

	// Start timer and count till reaches 50 msec
	Timer1_vidInitialtize(8);

	// OCR is high to leave for ISR enough time for execution
	Timer1_vidSetOCRValue(50000);


	Timer1_setcallback(APP);


	// Prompt user for input
	LCD_vidGoTo(1, 1);
	LCD_String("password:");
	LCD_vidGoTo(1, 2);

	// Memory block simulation to fetch data
	LinkedList_vidInitialize();



	while(1)
	{
		// Key_Pad its self is an infinite loop only exits with '=' sign
		KeyPad_vidInitialize();
		_delay_ms(2000);

		LinkedList_vidClearOnDemand(1, 24);

		LCD_vidGoTo(1, 1);
		LCD_String("password:");
		LCD_vidGoTo(1, 2);

		// This only happens when you press '=' sign
		LinkedList_vidRestart();
	}
}


void APP(void)
{
	_delay_us(10);
	counter++;

	// counter is low to let the function execute correctly without interrupting
	if(counter == 20)
	{

		// Variable used externally to connect timer with encrypting user input on screen
		crypt += counter;

		if(hide_me)
		{
			// Hide the clock
		}
		else
		{
			// READ_FROM IR and assign to GLOBAL VAR. DATA
			location_before_timer = LCD_u8CurrentLocation_AC();
		}


		// Operation on fields are done first
		seconds++;
		if(seconds >= 60)
		{
			seconds = 0;
			minutes++;

			if(hide_me)
			{
				// Hide the clock
			}
			else
			{
				// 15 and 16 positions are for seconds
				LCD_vidInstruction(0x4E + (1<<7), write_operation, IR);
				// Type two zeros in seconds lane
				LCD_vidInstruction(0x30, write_operation, DR);
				LCD_vidInstruction(0x30, write_operation, DR);

			}

			if(minutes >= 60)
			{
				minutes = 0;
				hours++;

				// Indication for minutes

				if(hide_me)
				{
					// Hide the clock
				}
				else
				{
					// 12 and 13 positions are  for minutes lane
					LCD_vidInstruction(0x4B + (1<<7), write_operation, IR);
					// Type two zeros
					LCD_vidInstruction(0x30, write_operation, DR);
					LCD_vidInstruction(0x30, write_operation, DR);

					// Indication for hours

					// 9 and 10 positions are  for hours;
					LCD_vidInstruction(0x49 + (1<<7), write_operation, IR);
					LCD_vidInstruction(hours + 0x30, write_operation, DR);
				}
			}
			else if(minutes < 10)
			{
				// Indication for minutes
				if(hide_me)
				{
					// Hide the clock
				}
				else
				{
					// 12 and 13 positions are  for minutes
					LCD_vidInstruction(0x4B + (1<<7), write_operation, IR);
					// Type zero at the beginning then minutes
					LCD_vidInstruction(0x30, write_operation, DR);
					LCD_vidInstruction(minutes + 0x30, write_operation, DR);
				}
			}
			else
			{
				if(hide_me)
				{
					// Hide the clock
				}
				else
				{
					// 12 and 13 positions are  for minutes
					LCD_vidInstruction(0x4B + (1<<7), write_operation, IR);
					LCD_vidInstruction((minutes/10 + 0x30), write_operation, DR);
					LCD_vidInstruction((minutes%10 + 0x30), write_operation, DR);
				}
			}
		}

		else if(seconds < 10)
		{
			// 15 and 16 positions are for seconds
			if(hide_me)
			{
				// Hide the clock
			}
			else
			{
				//LCD_vidGoTo(15, 1);
				LCD_vidInstruction(0x4E + (1<<7), write_operation, IR);
				// Type zero at the beginning then seconds
				LCD_vidInstruction(0x30, write_operation, DR);
				LCD_vidInstruction(seconds + 0x30, write_operation, DR);
			}
		}

		else
		{
			if(hide_me)
			{
				// Hide the clock
			}
			else
			{
				LCD_vidInstruction(0x4E + (1<<7), write_operation, IR);
				LCD_vidInstruction((seconds/10 + 0x30), write_operation, DR);
				LCD_vidInstruction((seconds%10 + 0x30), write_operation, DR);
			}
		}

		// Reset counter
		counter = 0;
		LCD_vidInstruction(location_before_timer + (1<<7), write_operation, IR);

	}
}


