/*
 * ArefRTOS_PORTING.h
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

#ifndef INC_AREFRTOS_PORTING_H_
#define INC_AREFRTOS_PORTING_H_
/***********************************************************
* 														   *
*	           USER PRE-PROCESSOR OPTIONS                  *
*														   *
************************************************************/
/* @brief Choose one of
 *                      CORTEX_M3
 *                      CORTEX_M4
 */
#define __CPU__     			CORTEX_M3

/* @brief Write down maximum number of tasks */
#define MAX_NUM_OF_TASKS		10

/* @brief This Macro is used to define the tick time in microsecond */
#define TICK_TIME 				4000

/* @brief This Macro is used to define the SIZE of main stack area
 * @details This area is used by Kernel, Interrupts, and Exceptions */
#define MAINSTACKSIZE   	 	0x800

/* @brief This Macro is used to define the handler name of pendSV */
#define PEND_SV_HANDLER_NAME  	PendSV_Handler

/* @brief This Macro is used to define symbol of end of stack */
#define END_OF_STACK_SYMBOL		_estack

/* @brief This Macro is used to define symbol of end of heap */
#define END_OF_HEAP_SYMBOL		_eheap


/***********************************************************
* 														   *
*	              PRE-PROCESSOR CONSTANTS                  *
*														   *
************************************************************/
/* @brief This Macro is used to make a function an inline function */
# define FORCE_INLINE __attribute__((always_inline)) inline
/* @brief This Macro is used to make a function a naked function */
# define FORCE_NAKED __attribute__((naked))


/***********************************************************
* 														   *
*	           EXTERN AND GLOBAL VARIABLES                 *
*														   *
************************************************************/
/* @brief This Symbol is defined for the top of stack defined by linkerscript (.ld) File */
extern int END_OF_STACK_SYMBOL ;
/* @brief This Symbol is defined for the top of heap defined by linkerscript (.ld) File */
extern int END_OF_HEAP_SYMBOL  ;


/***********************************************************
* 														   *
*	                     INCLUDES     			           *
*														   *
************************************************************/
#if (__CPU__ == CORTEX_M3) ||( __CPU__ == CORTEX_M4)
#include "PSRC_interface.h"			// Include PSR and Control Registers and Functions
#include "STK_interface.h"			// Include SysTick Timer Registers and Functions
#include "SCB_interface.h"			// Include the System Control Block Registers and Functions
#include "NVIC_interface.h"			// Include the Nested Vector Interrupt Controller Registers and Functions
#include "RCC_interface.h"			// Include the Reset and Control Registers and Functions

#else
    #error "Undefined CPU."
#endif


/***********************************************************
* 														   *
*	              CONFIGURATION CONSTANTS   		       *
*														   *
************************************************************/
/* @brief Those Macros are used to do SVC Calls  */
#define ArefRTOS_SVC_REQUEST0 				0
#define ArefRTOS_SVC_REQUEST1      			1
#define ArefRTOS_SVC_REQUEST2 				2


/***********************************************************
* 														   *
*	              FUNCTIONS PROTOTYPES     			       *
*														   *
************************************************************/
/**
 * @brief 	This Function is used to make SVC calls.
 * @details Just Edit SVC assembly code.
 * @param  	serviceID
 * @return 	void
 */
void ArefRTOS_voidCallService(uint8_t copy_u8ServiceID);

/**
 * @brief This Function is used to initialize the HW.
 * @details This Function Will initialize some of HW like systick timer.
 * @param  void
 * @return void
 */
void ArefRTOS_voidHardwareInit(void);

#endif /* INC_AREFRTOS_PORTING_H_ */
