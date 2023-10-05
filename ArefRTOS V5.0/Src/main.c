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
ArefRTOS_Mutex Mutex1;
/*******************************************************************************
                          Tasks Definitions
*******************************************************************************/
void task1(void){
	uint8_t LOC_Count = 0;
	for(;;){
		Task1LED ^=1;
		LOC_Count++;
		if(LOC_Count == 125){
			ArefRTOS_AquireMutex(&Mutex1, &Task1);
			ArefRTOS_voidActivateTask(&Task2);
		}
		if(LOC_Count == 250){
			LOC_Count =0;
			ArefRTOS_ReleaseMutex(&Mutex1, &Task1);
		}
	}
}
void task2(void){
	uint8_t LOC_Count = 0;
	for(;;){
		Task2LED ^=1;
		LOC_Count++;
		if(LOC_Count == 125)
			ArefRTOS_voidActivateTask(&Task3);
		if(LOC_Count == 250){
			LOC_Count =0;
			ArefRTOS_voidTerminateTask(&Task2);
		}
	}
}

void task3(void){
	uint8_t LOC_Count = 0;
	for(;;){
		Task3LED ^=1;
		LOC_Count++;
		if(LOC_Count == 125)
			ArefRTOS_voidActivateTask(&Task4);
		if(LOC_Count == 250){
			LOC_Count =0;
			ArefRTOS_voidTerminateTask(&Task3);
		}
	}
}

void task4(void){
	uint8_t LOC_Count = 0;
	for(;;){
		Task4LED ^=1;
		LOC_Count++;
		if(LOC_Count==5)
			ArefRTOS_AquireMutex(&Mutex1, &Task4);
		if(LOC_Count == 255){
			LOC_Count = 0;
			ArefRTOS_ReleaseMutex(&Mutex1, &Task4);
			ArefRTOS_voidTerminateTask(&Task4);
		}
	}
}

int main(void)
{
	//01. Initialize the OS Software & hardware
	if(ArefRTOS_voidInit()!= NoError)
		while(1);
	//03. Configure the Tasks
	Task1.TaskID = 1;
	Task1.TaskPriority = 4;
	Task1.TaskStackSize = 1024;
	Task1.pTaskFcn = task1;

	Task2.TaskID = 2;
	Task2.TaskPriority = 3;
	Task2.TaskStackSize = 1024;
	Task2.pTaskFcn = task2;

	Task3.TaskID = 3;
	Task3.TaskPriority = 2;
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

	//05. Create Mutex
	ArefRTOS_CreateMutex(&Mutex1);

	//06. Activate the Tasks
	ArefRTOS_voidActivateTask(&Task1);


	//07. Start the OS Scheduler
	ArefRTOS_voidStartScheduler();


    /* Loop forever */
	for(;;);
}
