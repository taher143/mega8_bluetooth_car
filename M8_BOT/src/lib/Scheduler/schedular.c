/*
* schedular.c
*
* Created: 2/2/2017 12:50:29 AM
*  Author: Taher Kawantwala
*/
#include "schedular.h"

static volatile unsigned char TaskTick;		//Task tick variable, updated in ISR.
// keeps track of number of timer interrupts
//uint16_t count = 0;

// scheduler function definitions
// initializes the task list
void initScheduler(void)
{
	for(uint8_t i=0; i<MAX_TASKS; i++)
	{
		task_list[i].id = 0;
		task_list[i].task = (task_t)0x00;
		task_list[i].delay = 0;
		task_list[i].period = 0;
		task_list[i].status = STOPPED;
	}
	
	//Configure the timer for 1ms interrupts to run tasks every 1ms.
	TCCR1A = (1 << WGM11)	| (1 << WGM10);				//Need timer in Fast PWM mode.
	TCCR1B = (1 << WGM13)	| (1 << WGM12) |			//Need timer in Fast PWM mode.
	(0 << CS02)	|										//8000000/64/1000 => OCR1A.
	(1 << CS01)	|										//So choose prescaler of 64.
	(1 << CS00);
	OCR1A   = F_CPU/64/(1000/TASK_TC_INTERVAL);			//This in combination with above will give 1ms overflows.
	TIFR  |= (1 << TOV1);								//Clear any previous interrupt.
	TIMSK |= (1 << TOIE1);								//Enable timer overflow interrupt.
	sei();
}

// adds a new task to the task list
// scans through the list and
// places the new task data where
// it finds free space
void addTask(uint8_t id, task_t task,uint16_t period)
{
	uint8_t idx = 0, done = 0x00;
	while( idx < MAX_TASKS )
	{
		if( task_list[idx].status == STOPPED )
		{
			task_list[idx].id = id;
			task_list[idx].task = task;
			task_list[idx].delay = period;
			task_list[idx].period = period;
			task_list[idx].status = RUNNABLE;
			done = 0x01;
		}
		if( done ) break;
		idx++;
	}
}

// remove task from task list
// note STOPPED is equivalent
// to removing a task
void deleteTask(uint8_t id)
{
	for(uint8_t i=0;i<MAX_TASKS;i++)
	{
		if( task_list[i].id == id )
		{
			task_list[i].status = STOPPED;
			break;
		}
	}

}


// gets the task status
// returns ERROR if id is invalid
uint8_t getTaskStatus(uint8_t id)
{
	for(uint8_t i=0;i<MAX_TASKS;i++)
	{
		if( task_list[i].id == id )
		return task_list[i].status;
	} return ERROR;
}


// dispatches tasks when they are ready to run
void dispatchTasks(void)
{
	if (TaskTick)
	{
		TaskTick=0;
		for(uint8_t i=0;i<MAX_TASKS;i++)
		{
			// check for a valid task ready to run
			if(task_list[i].status == RUNNABLE )
			{
				if (!task_list[i].delay)
				{
					// task is now running
					task_list[i].status = RUNNING;
					// call the task
					(*task_list[i].task)();
					// reset the delay
					task_list[i].delay = task_list[i].period;
					// task is runnable again
					task_list[i].status = RUNNABLE;
				}
				else
				{
					task_list[i].delay--;
				}
			}
		}
	}
}


// generates a "tick"
// each tick 1ms apart
ISR(TASK_TIMER_OVF_vec)
{
	TaskTick = 1;	//Task timer tick variable.
}
