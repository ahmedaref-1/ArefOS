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
	__asm("MRS r0,CONTROL \n\t"
		  "LSR r0,r0,#0x1 \n\t"
		  "LSL r0,r0,#0x1 \n\t"
		  "MSR CONTROL,r0");
}


FORCE_NAKED void PSRC_voidSetAccessLevel_UNPRIVILEGED(void){
	__asm("MRS r0,CONTROL \n\t"
		  "ORR r0,r0,#0x1 \n\t"
		  "MSR CONTROL,r0");
}



