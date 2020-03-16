/*
 * KeyPad.c
 *
 *  Created on: Aug 28, 2019
 *      Author: Newname
 */


/*
 * KEY_PAD_2.C
 *
 *  Created on: Aug 27, 2019
 *      Author: Newname
 */

#include "../MCAL/DIO.h"
#include "KEY_PAD.h"
#include "../MCAL/LinkedList.h"
#include "util/delay.h"
#include "LCD.h"



// Flag1 for input1, flag2 for input2
u8 location = 0;

u8 result = 0, operation = 0;
u8 s_flag = 0, m_flag = 0, d_flag = 0, a_flag = 0;

/* For keypad calculator only
u8 character[4][4] = { '0', '1', '2', '3',
						'4', '5', '6', '7',
						'8', '9', '+', '-',
						'*', '/', '=', '#'};
*/

// For keypad locker only
u8 character[4][4] = { '0', '1', '2', '3',
						'4', '5', '6', '7',
						'8', '9', '!', '@',
						'$', '?', '=', '#'};




// Columns are input, Rows are output
void KeyPad_vidInitialize(void)
{
	// Set low nibble columns are input and pulled up
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL1, INPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL2, INPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL3, INPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, COL4, INPUT);

	// Set high nibble rows as output
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW1, OUTPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW2, OUTPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW3, OUTPUT);
	DIO_vidSetPinxDir(KEY_PAD_PORT, ROW4, OUTPUT);

	// Rows are all  zeros
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 0);
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 0);
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 0);
	DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 0);

	KeyPad_vidKey_Pressed();

}


u8 KeyPad_vidKey_Pressed(void)
{
	while(1)
	{
		// 1st stage Make sure key is released
		do
		{
			// Rows are all  zeros
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 0);
		}
		while((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f);

		// 2nd stage
		do
		{
			do
			{
				// delay for  noise
				_delay_ms(20);
			}
			while((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) == 0x0f);

			// Delay for De_bounce
			_delay_ms(20);
		}
		while((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) == 0x0f);


		while(1)
		{
			// 3rd stage
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 1);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 0;
				break;
			}

			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 1);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 1;
				break;
			}

			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 0);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 1);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 2;
				break;
			}

			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW1, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW2, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW3, 1);
			DIO_vidSetPortBitValue(KEY_PAD_PORT, ROW4, 0);

			if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) != 0x0f)
			{
				location = 3;
				break;
			}

		}


		// Check for column location
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) == 0x0E)
			result = character[location][COL1];
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) ==  0x0D)
			result = character[location][COL2];
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) ==  0x0B)
			result = character[location][COL3];
		if((DIO_GetPinValue(KEY_PAD_PORT) & 0x0f) ==  0x07)
			result = character[location][COL4];

		// Clear display by a button
		if (result == character[3][3])
		{
			// Clear structure and reinitialize it
			LinkedList_vidRestart();

			// Clear display from a start point to end point and always returns cursor to location 0,0
			LinkedList_vidClearOnDemand(1, 24);

			// Prompt user for input
			LCD_String("password:");
			LCD_vidGoTo(1, 2);


		}

		// INdication for "=" sign
		else if(result == character[3][2])
		{
			// Set ptr = root to start from the head of linked_list
			restart_vip();

			// Fetch input from structre
			LinkedList_s16GetFinalInputValue();

			// Check for user input
			caluculation_stage();

			// Return to the main function in lock.c
			LinkedList_vidRestart();

			break;
		}
		else
		{
			//crypt = 0;

			// Load current Location for AC from LCD
			u8 address = LCD_u8CurrentLocation_AC();

			// Show result from keyPad for the user
			LCD_vidData(result);

			//Wait more than 2 seconds to crypt the input on screen
			/*while(crypt < 20)
			{
				crypt++;
			}
			crypt = 0;*/

			// Return to the previous address and rewrite * instead
			LCD_vidInstruction(address + (1<<7), write_operation, IR);

			LCD_vidInstruction(0x2A, write_operation, DR);


			// Insert  into Linked_List
			LinkedList_vidInsertion(result);
		}
	}
	return result;
}





















