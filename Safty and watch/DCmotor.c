/*
 * DCmotor.c
 *
 *  Created on: Aug 31, 2019
 *      Author: Newname
 */


#include "DCmotor.h"






void Motor_right(void)
{
	Motor_break();
	DIO_vidSetPortBitValue(MOTOR_PORT, MOTOR_RIGHT_PIN, 1);
	DIO_vidSetPortBitValue(MOTOR_PORT, MOTO_LEFT_PIN, 0);
}

void Motor_left(void)
{
	Motor_break();
	DIO_vidSetPortBitValue(MOTOR_PORT, MOTOR_RIGHT_PIN, 0);
	DIO_vidSetPortBitValue(MOTOR_PORT, MOTO_LEFT_PIN, 1);
}

void Motor_break(void)
{
	DIO_vidSetPortBitValue(MOTOR_PORT, MOTOR_RIGHT_PIN, 0);
	DIO_vidSetPortBitValue(MOTOR_PORT, MOTO_LEFT_PIN, 0);
}
