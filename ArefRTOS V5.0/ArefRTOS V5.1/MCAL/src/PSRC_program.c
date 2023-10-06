/*
 * PSRC_program.c
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include "PSRC_interface.h"
#include "PSRC_config.h"
#include "PSRC_private.h"


/***********************************************************
* 														   *
*			       FUNCTIONS DEFINITION                    *
*														   *
************************************************************/
FORCE_NAKED void PSRC_voidSetAccessLevel_PRIVILEGED(void){
	__asm("MRS r3,CONTROL \n\t"
		  "LSR r3,r3,#0x1 \n\t"
		  "LSL r3,r3,#0x1 \n\t"
		  "MSR CONTROL,r3");
}


/*FORCE_NAKED void PSRC_voidSetAccessLevel_UNPRIVILEGED(void){
	__asm("MRS r3,CONTROL \n\t"
		  "ORR r3,r3,#0x1 \n\t"
		  "MSR CONTROL,r3");
}*/



