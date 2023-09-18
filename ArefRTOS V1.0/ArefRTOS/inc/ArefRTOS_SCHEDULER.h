/*
 * ArefRTOS_SCHEDULER.h
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

#ifndef INC_AREFRTOS_SCHEDULER_H_
#define INC_AREFRTOS_SCHEDULER_H_
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"ArefRTOS_PORTING.h"


/***********************************************************
* 														   *
*	              PRE-PROCESSOR CONSTANTS                  *
*														   *
************************************************************/


/***********************************************************
* 														   *
*	              	 TYPE DEFINITIONS  	                   *
*														   *
************************************************************/
typedef void (*ptrFunc) (void) ;

/* @brief  this enum holds the available options for expects errors */
typedef enum
{
	NoError	,					//	The function executed successfully
	StackOverflow	,			//	Return when happen overflow in function
	ErrorInQueueInit	,		// 	Error in Queue Initialization
	ExceedMaxNumberOfTasks,		//	Happens when number of tasks larger than defined in @ref AX_NUM_OF_TASKS in @ref MRTOS_Porting.h
	NULL_ARGs,					// User Passed Null Argument
	INVALID_OPERATION
}ArefRTOS_ErrorID;

/* @brief  this enum holds the available options for state of task */
typedef enum
{
	TS_Suspend,		//	All Created/Terminated/Unavailable Resource Tasks Will Marked as TS_Suspend
	TS_Running,		//	Running Task Will Marked as TS_Running
	TS_Waiting,		//	All Activated tasks marked as TS_Waiting
	TS_Ready		//  In Case have task has higher priority will marked as TS_Ready
}ArefRTOS_TaskStates;

/* @brief  this struct holds some attributes that Not Entered by the user */
typedef struct
{
	ArefRTOS_TaskStates		taskState ;		//	Current Task State
	uint32_t				_S_PSP_Task ;	//	Start Address of task in stack
	uint32_t				_E_PSP_Task ;	//	End Address of task in stack
	uint32_t*				pCurrentPSP ;	//	Current stack pointer in function
	uint8_t					TimeDelayFlag;	// Enable/Disable Function Delay
	uint32_t				_taskDelayTime;			  // Task periodicity.
}ArefRTOS_PrivTaskRef;

/* @brief  this struct defined as task with some User Accessible parameters and some of it Non Accessible by user */
typedef struct
{
	/**** User Accessible ****/
	uint8_t  				taskID		;				  // The ID of the Task
	uint32_t				taskStackSize ;				  // Size Of Task
	uint8_t					taskPriority;				  // Priority of task
	ptrFunc					pTaskFcn ;			      	  // Pointer to Task
    /**** Non User Accessible ****/
	ArefRTOS_PrivTaskRef	taskPrivateStates;
}ArefRTOS_Task;


/***********************************************************
* 														   *
*	             FUNCTIONS PROTOTYPES  	                   *
*														   *
************************************************************/
/**
 * @brief This Function is used to initialize ArefRTOS.
 * @details This Function Will initialize some of HW like SysTick timer and some of SW codes.
 * @param  void
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_voidInit(void);

/**
 * @brief This Function is used to Create task area.
 *
 * @param pTask 	pointer to task that has attributes @ref ArefRTOS_Task
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_voidCreateTask(ArefRTOS_Task* pTask);

#endif /* INC_AREFRTOS_SCHEDULER_H_ */
