/*
 * ArefRTOS_PORTING.c
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"ArefRTOS_PORTING.h"


/***********************************************************
* 														   *
*			    PRE-PROCESSOR CONSTANTS	                   *
*														   *
************************************************************/
#define _attribute_(ATT)  __attribute__ ((ATT))
#define __WEAK__	weak
#define __NACKED__	naked


/***********************************************************
* 														   *
*			        TYPE-DEFINITIONS	                   *
*														   *
************************************************************/


/***********************************************************
* 														   *
*			        	VARIABLES	                       *
*														   *
************************************************************/


/***********************************************************
* 														   *
*			       FUNCTIONS DEFINITIONS                   *
*														   *
************************************************************/
void ArefRTOS_voidCallService(uint8_t copy_u8ServiceID)
{
	switch(copy_u8ServiceID)
	{
		case ArefRTOS_SVC_REQUEST0 :
			__asm("SVC #0x00");
			break;
		case ArefRTOS_SVC_REQUEST1 :
			__asm("SVC #0x01");
			break;
		case ArefRTOS_SVC_REQUEST2 :
			__asm("SVC #0x02");
			break;
	}
}


void ArefRTOS_voidHardwareInit(void){
	/* To initialize the STK driver */

	/* To Set Priorities	*/
    //01.Set the PendSV interrupt priority to the lowest level*/
	*(volatile uint32_t *)0xE000ED22 = 0xFF;
}


/***********************************************************
* 														   *
*			       IRQ FAULTS DEFINITIONS                  *
*														   *
************************************************************/
_attribute_(weak) void HardFault_Handler (void)
  {
  	while(1);
  }

_attribute_(weak) void MemManage_Handler(void)
  {
  	while(1);
  }

_attribute_(weak) void BusFault_Handler(void)
  {
  	while(1);
  }

_attribute_(__WEAK__) void UsageFault_Handler(void)
  {
  	while(1);
  }


/***********************************************************
* 														   *
*			      IRQ HANDLERS DEFINITIONS                 *
*														   *
************************************************************/
FORCE_NAKED void SVC_Handler(void)
{
	__asm("TST   LR, #4   \n\t"
		  "ITE   EQ 	  \n\t"
		  "MRSEQ R0,MSP   \n\t"
		  "MRSNE R0,PSP   \n\t"
		  "B 	 ArefRTOS_SVC_CALL");
}



