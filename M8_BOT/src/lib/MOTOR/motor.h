/*
 * motor.h
 *
 * Created: 10/14/2017 11:16:10 PM
 *  Author: Taher Kawantwala
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_

#define MOTOR_PORT	PORTD
#define MOTOR_DDR	DDRD
#define M1_0		PD4
#define M1_1		PD5
#define M2_0		PD6
#define M2_1		PD7

//Function Decleration
void Motor_init(void);
void Motor_forward(void);
void Motor_reverse(void);
void Motor_stop(void);
void Motor_left(void);
void Motor_right(void);





#endif /* MOTOR_H_ */