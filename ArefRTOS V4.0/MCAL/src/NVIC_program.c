/*
 * NVIC_program.c
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"NVIC_config.h"
#include"NVIC_private.h"
#include"NVIC_interface.h"


/***********************************************************
* 														   *
*			    FUNCTIONS DEFINITIONS	                   *
*														   *
************************************************************/
void NVIC_SetPendSVInterruptPriority(void){
	__NVIC_SetPriority(PendSV_IRQn, PendSV_InterruptPriorityNumber);
}
