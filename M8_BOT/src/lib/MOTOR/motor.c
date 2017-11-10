/*
 * motor.c
 *
 * Created: 10/14/2017 11:16:28 PM
 *  Author: Taher Kawantwala
 */ 
#include <avr/io.h>
#include "lib/MOTOR/motor.h"

//Function Defination
void Motor_init(void)
{
	MOTOR_DDR |= (1<<M1_0)|(1<<M1_1)|(1<<M2_0)|(1<<M2_1);
	Motor_stop();
}

void Motor_stop(void)
{
	MOTOR_PORT &= ~((1<<M1_0)|(1<<M1_1)|(1<<M2_0)|(1<<M2_1));
}

void Motor_forward(void)
{
	MOTOR_PORT |= (1<<M1_0)|(1<<M2_0);
	MOTOR_PORT &= ~((1<<M1_1)|(1<<M2_1));
}

void Motor_reverse(void)
{
	MOTOR_PORT |= (1<<M1_1)|(1<<M2_1);
	MOTOR_PORT &= ~((1<<M1_0)|(1<<M2_0));
}

void Motor_right(void)
{
	MOTOR_PORT |= (1<<M1_0);
	MOTOR_PORT &= ~((1<<M2_0)|(1<<M2_1)|(1<<M1_1));
}

void Motor_left(void)
{
	MOTOR_PORT |= (1<<M2_0);
	MOTOR_PORT &= ~((1<<M1_0)|(1<<M2_1)|(1<<M1_1));
}