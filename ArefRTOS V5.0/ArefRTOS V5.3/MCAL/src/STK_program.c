/*
 * _program.c
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"STK_config.h"
#include"STK_private.h"
#include"STK_interface.h"


/***********************************************************
* 														   *
*			    FUNCTIONS DEFINITIONS	                   *
*														   *
************************************************************/
void STK_SetPeriodicInterruptInterval(void){
	SysTick_Config(DelayTime_1mS_NumberOfCounts);
}

