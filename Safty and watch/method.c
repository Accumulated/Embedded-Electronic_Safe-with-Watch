/*
 * method.c
 *
 *  Created on: Sep 3, 2019
 *      Author: Newname
 */

/*REGISRERS used for Interrupt*/
#define SREG_M		*((volatile u8*) 0x5F)
#define	GICR_M		*((volatile u8*) 0x5B)
#define	MCUSR_M		*((volatile u8*) 0x55)
#define	MCUCSR_M	*((volatile u8*) 0x54)



#include "method.h"
#include "DIO.h"
#include "../types.h"
#include "../macros.h"


/* Control */
#define ENABLE	1
#define DISABLE	0


// Set Global enable in Register SREG
void INTERRUPT_vidGlobalEnable(u8 MODE)
{
	switch(MODE)
	{
	case DISABLE:
		clr_bit(SREG_M, 7);
		break;
	case ENABLE:
		set_bit(SREG_M, 7);
		break;
	default:
		break;
	}
}


// Enable EXTERNAL HARDWARE INTERRUPTS, INT0, INT1, INT2 FLAGS
void INTERRUPT_vidSetInterrupt(u8 PIN_NAME)
{
	switch(PIN_NAME)
	{
	case INT0_BIT:
		set_bit(GICR_M, INT0_BIT);
		break;
	case INT1_BIT:
		set_bit(GICR_M, INT1_BIT);
		break;
	case INT2_BIT:
		set_bit(GICR_M, INT2_BIT);
		break;
	default:
		break;
	}
}


// Disable EXTERNAL HARDWARE INTERRUPTS, INT0, INT1, INT2 FLAGS
void INTERRUPT_vidclrInterrupt(u8 PIN_NAME)
{
	switch(PIN_NAME)
	{
	case INT0_BIT:
		clr_bit(GICR_M, INT0_BIT);
		break;
	case INT1_BIT:
		clr_bit(GICR_M, INT1_BIT);
		break;
	case INT2_BIT:
		clr_bit(GICR_M, INT2_BIT);
		break;
	default:
		break;
	}
}

// Mode for triggering
void INTERRUPT_vidSetMCUPin(u8 pin_name)
{
	switch(pin_name)
	{
	case ISC00_BIT:
		set_bit(MCUSR_M, ISC00_BIT);
		break;
	case ISC01_BIT:
		set_bit(MCUSR_M, ISC01_BIT);
		break;
	case ISC10_BIT:
		set_bit(MCUSR_M, ISC10_BIT);
		break;
	case ISC11_BIT:
		set_bit(MCUSR_M, ISC11_BIT);
		break;
	case ISC2_BIT:
		set_bit(MCUCSR_M, ISC2_BIT);
		break;
	default:
		break;
	}
}

// Mode for triggering
void INTERRUPT_vidClrMCUPin(u8 pin_name)
{
	switch(pin_name)
	{
	case ISC00_BIT:
		clr_bit(MCUSR_M, ISC00_BIT);
		break;
	case ISC01_BIT:
		clr_bit(MCUSR_M, ISC01_BIT);
		break;
	case ISC10_BIT:
		clr_bit(MCUSR_M, ISC10_BIT);
		break;
	case ISC11_BIT:
		clr_bit(MCUSR_M, ISC11_BIT);
		break;
	case ISC2_BIT:
		clr_bit(MCUCSR_M, ISC2_BIT);
		break;
	default:
		break;
	}
}
