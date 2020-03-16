/*
 * t.c
 *
 *  Created on: Sep 7, 2019
 *      Author: Newname
 */



#define F_CPU 8000000ul
#include "MCAL/DIO.h"
#include "HAL/timer1.h"
#include "HAL/LCD.h"
#include "MCAL/method.h"
#include <stdlib.h>

u16 counter = 0, flag1 = 1;
u8 seconds = 0, minutes = 0, hours = 0;

void APP(void);
void LinkedList_vidCompanion(u8 show);



int main(void)
{
	// Start LCD
	LCD_vidInitialize();

	// Set clock positions
	LCD_vidGoTo(9, 1);
	LCD_String("00");
	LCD_vidGoTo(11, 1);
	LCD_String(":");
	LCD_String("00");
	LCD_vidGoTo(14, 1);
	LCD_String(":");
	LCD_String("00");

	DIO_vidSetPortxDir(PORTC, OUTPUT);

	// Start timer
	Timer1_vidInitialtize(8);
	Timer1_vidSetOCRValue(1000);


	Timer1_setcallback(APP);


	while(1);
}


void APP(void)
{
	counter++;

	// Indication for seconds
	if(counter == 1000)
	{
		// Operation on fields are done first
		seconds++;
		if(seconds >= 60)
		{
			seconds = 0;
			minutes++;

			// 15 and 16 positions are for seconds
			LCD_vidInstruction(14+(1<<7), write_operation, IR);
			// Type two zeros in seconds lane
			LCD_vidInstruction(0x30, write_operation, DR);
			LCD_vidInstruction(0x30, write_operation, DR);


			if(minutes >= 60)
			{
				minutes = 0;
				hours++;

				// Indication for minutes

				// 12 and 13 positions are  for minutes lane
				LCD_vidInstruction(11+(1<<7), write_operation, IR);

				// Type two zeros
				LCD_vidInstruction(0x30, write_operation, DR);
				LCD_vidInstruction(0x30, write_operation, DR);

				// Indication for hours

				// 9 and 10 positions are  for hours;
				//LCD_vidGoTo(9, 1);
				LCD_vidInstruction(9+(1<<7), write_operation, IR);
				LinkedList_vidCompanion(hours);

			}
			else if(minutes < 10)
			{
				// Indication for minutes

				// 12 and 13 positions are  for minutes
				LCD_vidInstruction(11+(1<<7), write_operation, IR);

				// Type zero at the beginning then minutes
				LCD_vidInstruction(0x30, write_operation, DR);
				LinkedList_vidCompanion(minutes);
			}
			else
			{
				// 12 and 13 positions are  for minutes
				LCD_vidInstruction(11+(1<<7), write_operation, IR);
				LinkedList_vidCompanion(minutes);
			}
		}

		else if(seconds < 10)
		{
			// 15 and 16 positions are for seconds

			//LCD_vidGoTo(15, 1);
			LCD_vidInstruction(14+(1<<7), write_operation, IR);

			// Type zero at the beginning then seconds
			LCD_vidInstruction(0x30, write_operation, DR);

			LinkedList_vidCompanion(seconds);
		}

		else
		{
			LCD_vidInstruction(14+(1<<7), write_operation, IR);
			LinkedList_vidCompanion(seconds);
		}

		// Reset counter
		counter = 0;

	}
}






//  for user screen output
void LinkedList_vidCompanion(u8 show)
{

	u8 ptr [3];
	s8 i = 0, kk = 0;

	// get values in order
	do
	{
		ptr[i] = (show%10) + 0x30;
		i++;
		show /= 10;
	}while(show);

	ptr[i] = '\0';



	// reverse the string sent
	for(kk = i-1; kk >= 0; kk-- )
	{
		LCD_vidInstruction(ptr[kk], write_operation, DR);
	}
}

