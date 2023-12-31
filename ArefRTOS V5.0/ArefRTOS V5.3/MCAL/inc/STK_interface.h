/*
 * STK_interface.h
 *
 *  Created on: Sep 16, 2023
 *      Author: ahmed
 */

#ifndef INC_STK_INTERFACE_H_
#define INC_STK_INTERFACE_H_
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"core_cm3.h"
#include"PLATFORMTYPES.h"


/***********************************************************
* 														   *
*			    PRE-PROCESSOR CONSTANTS	                   *
*														   *
************************************************************/
#define DelayTime_1mS_NumberOfCounts 8000

/***********************************************************
* 														   *
*			    CONFIGURATION CONSTANTS	                   *
*														   *
************************************************************/



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
*			       FUNCTIONS PROTOTYPES                    *
*														   *
************************************************************/
void STK_SetPeriodicInterruptInterval(void);





#endif /* INC_STK_INTERFACE_H_ */
