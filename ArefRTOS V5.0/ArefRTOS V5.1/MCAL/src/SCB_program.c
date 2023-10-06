/*
 * SCB_program.c
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include "SCB_interface.h"
#include "SCB_config.h"
#include "SCB_private.h"


/***********************************************************
* 														   *
*			       FUNCTIONS DEFINITION                    *
*														   *
************************************************************/
void SCB_voidTrigPendSV(void)
{
	SET_BIT(SCB_ICSR,SCB_ICSR_PendSV_BitMask);
}
