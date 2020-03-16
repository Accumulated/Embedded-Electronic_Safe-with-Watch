/*
 * LinkedList.c
 *
 *  Created on: Aug 29, 2019
 *      Author: Newname
 */



#include "../types.h"
#include "DIO.h"
#include "LinkedList.h"
#include "stdlib.h"
#include <util/delay.h>
#include "../HAL/LCD.h"
#include "../HAL/KEY_PAD.h"
#include "../HAL/DCmotor.h"



u8 shots = 0x33;


// linked list for numbers
typedef struct node
{
	u8 value;
	struct node *next;

	// numeric = 1 only for valid numbers
}
node;

// Define head and a pointer to move with
node *root, *ptr;
u8 NoOfNodes = 0;

// Password of 4 chars and (times) indicator for fetching from Linked list
u8 password[4] = {0}, my_password[4] = {'@', '!', '?', '7'};
s8 times = -1;


/********************************* READY *********************************/
// Initialize linked list
void LinkedList_vidInitialize(void)
{
	// Initializing list
	root = malloc(sizeof(node));
	root -> value = 0;
	root -> next = NULL;
	ptr = root;
}


/********************************* READY *********************************/
// Insertion for values from user
void LinkedList_vidInsertion(u8 value)			// if still, you can change to u8
{
	// Inserting sequence initialized
	ptr = root;
	node *tmp = malloc(sizeof(node));
	NoOfNodes++;

	// Insert patient details
	tmp -> next = ptr -> next;

	tmp -> value = (char)value;

	// Update list
	ptr -> next = tmp;

	// Indicate success
}


/********************************* READY *********************************/
u8 LinkedList_u8DeleteEntire(void)
{
	// Buffer to save the previous condition of ptr
	node *last_before_stack = NULL;

	// Checking on the next field
	if(ptr -> next != NULL)
	{
		last_before_stack = ptr;
		ptr = ptr -> next;
		LinkedList_u8DeleteEntire();
		ptr = last_before_stack;
	}

	// free current node and return
	free(ptr);
	NoOfNodes = 0;
	return 0;
}


/********************************* READY *********************************/
// Get one input from user only and it can be more than one digit
u8 LinkedList_s16GetFinalInputValue(void)
{
	// Buffer to save the previous condition of ptr
	node *last_before_stack = NULL;

	// Buffer to save times value
	s8 tmp = times;

	// Checking on the next field
	if(ptr -> next != NULL && ptr != root)
	{
		times++;
		last_before_stack = ptr;
		ptr = ptr -> next;
		LinkedList_s16GetFinalInputValue();
		ptr = last_before_stack;
		password[tmp+1] = ptr->value;
	}
	else if(ptr -> next != NULL && ptr == root)
	{
		times = -1;
		last_before_stack = ptr;
		ptr = ptr -> next;
		LinkedList_s16GetFinalInputValue();
		ptr = last_before_stack;
	}
	else
	{
		password[tmp+1] = ptr->value;
	}

	return (0);
}


/********************************* READY *********************************/
u8 LinkdeList_u8NumberOfNodes(void)
{
	return NoOfNodes;
}


/********************************* READY *********************************/
void LinkedList_vidRestart (void)
{
	restart_vip();
	LinkedList_u8DeleteEntire();
	LinkedList_vidInitialize();
}


/********************************* READY *********************************/
void caluculation_stage(void)
{
	u8 tmp1 = 0;

	// Reverse the string to cooperate with "insertion" method used in Linked_list
	tmp1 = password[3];
	password[3] = password[0];
	password[0] = tmp1;

	tmp1 = password[1];
	password[1] = password[2];
	password[2] = tmp1;

	// Invalid either the length is invalid or the password its self is invalid
	if(LinkdeList_u8NumberOfNodes() != 4 || validation_for_password())
	{

		// Clear display wherever you want
		LinkedList_vidClearOnDemand(1, 24);

		// Notify user for invalid password
		LCD_vidGoTo(1, 1);
		LCD_String("invalid Password");

		if(shots-1 != 0x30)
		{
			shots-- ;

			// Notify user for number of tries left
			LCD_vidGoTo(1, 2);
			LCD_String("#");
			LCD_vidData(shots);
			LCD_String("tries");
		}
		else
		{

			shots = 0x33;
			while(1)
			{

				// Stop buzzer
				DIO_vidSetPortBitValue(PORTA, 7, 0);

				// Blink leds --> ON
				 PORTA_REG &= 0xf8;

				// Blink user for being a thief
				LinkedList_vidClearOnDemand(1, 24);

				_delay_ms(1200);
				LCD_String("fuckin thief");

				// Start buzzer
				DIO_vidSetPortBitValue(PORTA, 7, 1);

				// Blink leds --> OFF
				PORTA_REG |=  0x07;

				LCD_vidGoTo(1, 2);
				LCD_String("_|_");

				_delay_ms(1200);
			}
		}

	}


	// valid input
	else
	{
		u8 i = 0;
		// Clear display wherever you want
		LinkedList_vidClearOnDemand(1, 24);
		shots = 0x33;
		Motor_right();
		PORTA_REG |= 0x18;

		//LCD_vidGoTo(1, 1);
	//	LCD_String("WELCOME");
		hide_me = 1;
		while(1)
		{
			i++;
			LinkedList_vidClearOnDemand(1, 32);
			LCD_String("WELCOME");
			LCD_vidGoTo(1, 2);
			LCD_String("ABDEL_MOTALEB");
			_delay_ms(1500);
			LCD_vidShift();
			if(i == 5)
			{
				DIO_vidSetPortValue(PORTA, 0x00);
				LinkedList_vidClearOnDemand(1, 32);
				// Set clock positions

				// 9 and 10 positions are  for hours;
				LCD_vidInstruction(0x48 + (1<<7), write_operation, IR);
				LCD_vidInstruction(0x30, write_operation, DR);
				LCD_vidInstruction(hours + 0x30, write_operation, DR);

				LCD_String(":");
				LCD_vidInstruction(0x4B + (1<<7), write_operation, IR);
				if(minutes < 10)
				{
					// 12 and 13 positions are  for minutes
					LCD_vidInstruction(0x30, write_operation, DR);
					LCD_vidInstruction(minutes + 0x30, write_operation, DR);
				}
				else
				{
						LCD_vidInstruction((minutes/10 + 0x30), write_operation, DR);
						LCD_vidInstruction((minutes%10 + 0x30), write_operation, DR);
				}

				LCD_String(":");
				LCD_vidInstruction(0x4E + (1<<7), write_operation, IR);
				if(seconds < 10)
				{
					// Type zero at the beginning then seconds
					LCD_vidInstruction(0x30, write_operation, DR);
					LCD_vidInstruction(seconds + 0x30, write_operation, DR);
				}
				else
				{
					LCD_vidInstruction((seconds/10 + 0x30), write_operation, DR);
					LCD_vidInstruction((seconds%10 + 0x30), write_operation, DR);
				}

				hide_me = 0;
				break;
			}
		}

	}
}


/********************************* READY *********************************/
u8 validation_for_password(void)
{
	for(u8 j = 0; j < 4; j++)
	{
		if(password[j] != my_password[j])
		{
			// Invalid password
			return 1;
		}
	}

	// Indicate success
	return 0;
}


/********************************* READY *********************************/
void restart_vip(void)
{
	ptr = root;
}


/********************************* READY *********************************/
// Clear display via start and end locations specified by the user
void LinkedList_vidClearOnDemand(u8 start, u8 end)
{

	// Start where the user has provided
	LCD_vidGoTo(start, 1);

	// Move with addresses to clear the display pixel by pixel
	for(start; start <= end; start ++)
	{
		// Indication for second line in LCD
		if(start > 16)
		{
			start = 1;
			LCD_vidGoTo(start, 2);
			end -= 16;
		}
		LCD_vidInstruction(0x20, write_operation, DR);
	}

	LCD_vidGoTo(1, 1);

}
