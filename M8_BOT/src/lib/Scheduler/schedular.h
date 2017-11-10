/*
 * schedular.h
 *
 * Created: 2/2/2017 12:50:47 AM
 *  Author: Taher Kawantwala
 */ 


#ifndef SCHEDULAR_H_
#define SCHEDULAR_H_

#include<avr/interrupt.h>
#include<inttypes.h>
#include "lib/common.h"

//Timer Counter 1 is used as the timer for Task processing module.
//It is used to generated 1ms tick for task processing functions.
#define TASK_TIMER_OVF_vec	TIMER1_OVF_vect		// Timer 1 overflow vector
#define TASK_TC_INTERVAL	1					// In ms.
#define MAX_TASKS			10					// Number of Tasks

// task states
#define RUNNABLE	0x00
#define RUNNING		0x01
#define STOPPED 	0x02
#define ERROR		0x03

// a task "type"
// pointer to a void function with no arguments
typedef void (*task_t)(void);

// basic task control block (TCB)
typedef struct __tcb_t
{
	uint8_t id; // task ID
	task_t task; // pointer to the task
	// delay before execution
	uint16_t delay, period;
	uint8_t status; // status of task
} tcb_t;

// scheduler functions
void initScheduler(void);
void addTask(uint8_t, task_t, uint16_t);
void deleteTask(uint8_t);
uint8_t getTaskStatus(uint8_t);
void dispatchTasks(void);


// the task list
tcb_t task_list[MAX_TASKS];




#endif /* SCHEDULAR_H_ */