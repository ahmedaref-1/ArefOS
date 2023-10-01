/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Ahmed Aref Omaira
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/*******************************************************************************
                             Includes
*******************************************************************************/
#include"ArefRTOS_SCHEDULER.h"

/*******************************************************************************
                         GLOBAL VARIABLES
*******************************************************************************/
ArefRTOS_Task Task1, Task2, Task3, Task4;
ArefRTOS_ErrorID Error;
uint8_t Task1LED, Task2LED, Task3LED, Task4LED;
/*******************************************************************************
                          Tasks Definitions
*******************************************************************************/
void task1(void){
	for(;;){
		Task1LED ^=1;
		ArefRTOS_voidTaskDelay(&Task1, 100);
	}
}
void task2(void){
	for(;;){
		Task2LED ^=1;
		ArefRTOS_voidTaskDelay(&Task2, 200);
	}
}

void task3(void){
	for(;;){
		Task3LED ^=1;
		ArefRTOS_voidTaskDelay(&Task3, 400);
	}
}

void task4(void){
	for(;;){
		Task4LED ^=1;
		ArefRTOS_voidTaskDelay(&Task4, 800);
	}
}

int main(void)
{
	//01. Initialize the OS Software & hardware
	if(ArefRTOS_voidInit()!= NoError)
		while(1);
	//03. Configure the Tasks
	Task1.TaskID = 1;
	Task1.TaskPriority = 1;
	Task1.TaskStackSize = 1024;
	Task1.pTaskFcn = task1;

	Task2.TaskID = 2;
	Task2.TaskPriority = 1;
	Task2.TaskStackSize = 1024;
	Task2.pTaskFcn = task2;

	Task3.TaskID = 3;
	Task3.TaskPriority = 1;
	Task3.TaskStackSize = 1024;
	Task3.pTaskFcn = task3;

	Task4.TaskID = 4;
	Task4.TaskPriority = 1;
	Task4.TaskStackSize = 1024;
	Task4.pTaskFcn = task4;

	//04. Create the Tasks
	ArefRTOS_voidCreateTask(&Task1);
	ArefRTOS_voidCreateTask(&Task2);
	ArefRTOS_voidCreateTask(&Task3);
	ArefRTOS_voidCreateTask(&Task4);

	//05. Activate the Tasks
	ArefRTOS_voidActivateTask(&Task1);
	ArefRTOS_voidActivateTask(&Task2);
	ArefRTOS_voidActivateTask(&Task3);
	ArefRTOS_voidActivateTask(&Task4);

	//06. Start the OS Scheduler
	ArefRTOS_voidStartScheduler();


    /* Loop forever */
	for(;;);
}
