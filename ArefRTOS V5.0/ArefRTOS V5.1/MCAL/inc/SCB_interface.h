/*
 * SCB_interface.h
 *
 *  Created on: Sep 16, 2023
 *      Author: ahmed
 */

#ifndef INC_SCB_INTERFACE_H_
#define INC_SCB_INTERFACE_H_
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include "PLATFORMTYPES.h"
#include "COMMONMACROS.h"

/***********************************************************
* 														   *
*	        	PRE-PROCESSOR CONSTANTS		               *
*														   *
************************************************************/
#define FORCE_INLINE __attribute__((always_inline)) inline


/***********************************************************
* 														   *
*	        	  FUNCTION PROTOTYPES 		               *
*														   *
************************************************************/
void SCB_voidTrigPendSV(void);

#endif /* INC_SCB_INTERFACE_H_ */
