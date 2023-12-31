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
ArefRTOS_Task Task1,Task2,Task3;
ArefRTOS_ErrorID Error;

/*******************************************************************************
                          Tasks Definitions
*******************************************************************************/
void task1(void){
	for(;;);
}
void task2(void){
	for(;;);
}
void task3(void){
	for(;;);
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
	Task2.TaskPriority = 2;
	Task2.TaskStackSize = 1024;
	Task2.pTaskFcn = task2;

	Task3.TaskID = 3;
	Task3.TaskPriority = 3;
	Task3.TaskStackSize = 1024;
	Task3.pTaskFcn = task3;

	//04. Create the Tasks
	ArefRTOS_voidCreateTask(&Task1);
	ArefRTOS_voidCreateTask(&Task2);
	ArefRTOS_voidCreateTask(&Task3);

    /* Loop forever */
	for(;;);
}
