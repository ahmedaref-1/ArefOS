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
/* @brief Those Macros are used to do SVC Calls  */
#define ArefRTOS_SVC_CALL_ACTIVATE_TASK 			ArefRTOS_SVC_REQUEST0
#define ArefRTOS_SVC_CALL_TERMINATE_TASK     		ArefRTOS_SVC_REQUEST1
#define ArefRTOS_SVC_CALL_DELAY_TASK				ArefRTOS_SVC_REQUEST2
#define ArefRTOS_SVC_CALL_PRIORITY_INVERSION		ArefRTOS_SVC_REQUEST3

/***********************************************************
* 														   *
*	              	 TYPE DEFINITIONS  	                   *
*														   *
************************************************************/
typedef void (*ptrFunc) (void) ;

/* @brief  this enum holds the available options for expected errors */
typedef enum
{
	NoError	,				// Function executed successfully
	StackOverflow	,		// StackOverflow error occurred
	ErrorInQueueInit	,	// Queue Initialization error occurred
	ExceedMaxNumberOfTasks,	// Number of tasks exceeded the number defined in @ref MAX_NUM_OF_TASKS in @ref ArefRTOS_Porting.h
	NULL_ARGs,				// User Passed Null Argument
	INVALID_OPERATION
}ArefRTOS_ErrorID;

/* @brief  this enum holds the available options for state of task */
typedef enum
{
	TS_Suspend,		//	All Created/Terminated/Unavailable Resource tasks will marked as TS_Suspend
	TS_Running,		//	Running Task Will Marked as TS_Running
	TS_Waiting,		//	All Activated tasks marked as TS_Waiting
	TS_Ready		//  In Case have task has higher priority will marked as TS_Ready
}ArefRTOS_TaskStates;

/* @brief  this struct holds some attributes that are not Entered by the user */
typedef struct
{
	ArefRTOS_TaskStates		TaskState ;		//	Current Task State
	uint32_t				_S_PSP_Task;	//	Start Address of task in stack
	uint32_t				_E_PSP_Task;	//	End Address of task in stack
	uint32_t*				pCurrentPSP;	//	Current stack pointer in function
	uint8_t					TimeDelayFlag;	//  Enable/Disable Function Delay
	uint32_t				TaskDelayTime;	//  Task periodicity
}ArefRTOS_PrivTaskRef;

/* @brief  this struct defined as task with some User Accessible parameters and some of it Non Accessible by user */
typedef struct
{
	/**** User Accessible ****/
	uint8_t  				TaskID		;				  // The ID of the Task
	uint32_t				TaskStackSize ;				  // Size Of Task
	uint8_t					TaskPriority;				  // Priority of task
	ptrFunc					pTaskFcn ;			      	  // Pointer to Task
    /**** Non User Accessible ****/
	ArefRTOS_PrivTaskRef	TaskPrivateStates;
}ArefRTOS_Task;

#if MUTEX_ENABLE == Enable
/*
 * @brief  this struct holds some attributes that aren't Entered by the user
 */
typedef struct
{
	uint32_t WaitingMessagesCounter ;
	uint32_t MaxNumberOfWaitingMessages;
	uint32_t NextPopedItemIndex	;
} ArefRTOS_PrivQueueRef ;
#endif

/***********************************************************
* 				PRIVATE TYPE-DEFINITIONS				   *
************************************************************/
#if MUTEX_ENABLE	== Enable
typedef struct
{
	ArefRTOS_Task* 				MutexHolder ;
	uint8_t						PriorityInversionFlag ;
	ArefRTOS_Task* 				InheritedTask;
	ArefRTOS_PrivQueueRef 		QueuePrivateData;

}ArefRTOS_Mutex;
#endif

#if QUEUE_MESSAGE_BOX_ENABLE   == 	Enable
/*
 * @brief  this struct defined as queue for data synchronization
 */
typedef struct
{
	ArefRTOS_PrivQueueRef 		QueuePrivateData;
}ArefRTOS_Queue;
#endif


/***********************************************************
* 														   *
*	             FUNCTIONS PROTOTYPES  	                   *
*														   *
************************************************************/
/***********************************************************
* 		GENERAL FUNCTIONS FOR SCHEDULER PROTOTYPES		   *
************************************************************/
/*
 * @brief This Function is used to initialize ArefRTOS.
 * @details This Function Will initialize some of HW like SysTick timer and some of SW codes.
 * @param  void
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_voidInit(void);

/*
 * @brief Create a new task and initialize its attributes.
 * This function is used to create a new task within the ArefRTOS system. It initializes the task's attributes and adds it to the task list.
 * @param pTask Pointer to the task with attributes of type ArefRTOS_Task.
 * @return ArefRTOS_ErrorID An error code indicating the success or failure of the task creation process.
 */
ArefRTOS_ErrorID ArefRTOS_voidCreateTask(ArefRTOS_Task* pTask);

/*
 * @brief Activate a specified task for execution.
 * This function activates a task within the ArefRTOS system, allowing it to begin execution.
 * It transitions the task to a "Waiting" state and triggers its execution.
 * @param pTask Pointer to the task to be activated.
 * @return ArefRTOS_ErrorID An error code indicating the success or failure of the task activation process.
 */
ArefRTOS_ErrorID ArefRTOS_voidActivateTask(ArefRTOS_Task* pTask);

/*
 * @brief Terminate or suspend a specified task.
 * This function is responsible for terminating or suspending a task within the ArefRTOS system.
 * It transitions the task to a "Suspend" state, effectively ending its execution.
 * @param pTask Pointer to the task to be terminated or suspended.
 * @return ArefRTOS_ErrorID An error code indicating the success or failure of the task termination process.
 */
ArefRTOS_ErrorID ArefRTOS_voidTerminateTask(ArefRTOS_Task* pTask);

/*
 * @brief Introduce a time-based delay in task execution.
 * This function introduces a time-based delay in the execution of a specified task within the ArefRTOS system.
 * It suspends the task temporarily, enables a delay flag, and sets the delay duration in ticks.
 * @param pTask Pointer to the task to which the delay is applied.
 * @param copy_u32NumberofTicks The number of timer ticks to delay the task's execution.
 * @return ArefRTOS_ErrorID An error code indicating the success or failure of the task delay process.
 */
ArefRTOS_ErrorID ArefRTOS_voidTaskDelay(ArefRTOS_Task* pTask, uint32_t copy_u32NumberofTicks);

/*
 * @brief Initialize and start the task scheduler.
 * This function initializes and activates the task scheduler within the ArefRTOS real-time operating system (RTOS).
 * It sets the system's operating mode to "Running," configures the initial task (typically the idle task), and begins task execution.
 * This function is essential for task management and execution.
 * @return ArefRTOS_ErrorID An error code indicating the success or failure of the task scheduler initialization and startup process.
 */
ArefRTOS_ErrorID ArefRTOS_voidStartScheduler(void);

/***********************************************************
* 		     MUTEX FUNCTIONS PROTOTYPES					   *
************************************************************/
#if MUTEX_ENABLE == Enable
/*
 * @brief This Function is used to create mutex  .
 * @param pMutex an object from @ref ArefRTOS_Mutex
 * @return MRTOS_ErrorID return one of @ref MRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_CreateMutex(ArefRTOS_Mutex *pMutex);
/*
 * @brief This Function is used to take a mutex .
 * @param pMutex an object from @ref ArefRTOS_Mutex
 * @param pCurrentRunningTask pointer to current running task.
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_AquireMutex(ArefRTOS_Mutex *pMutex,ArefRTOS_Task* pCurrentRunningTask);
/*
 * @brief This Function is used to give a mutex .
 * @param pMutex an object from @ref ArefRTOS_Mutex
 * @param pCurrentRunningTask pointer to current running task.
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_ReleaseMutex(ArefRTOS_Mutex *pMutex,ArefRTOS_Task* pCurrentRunningTask);
#endif

/***********************************************************
* 		   QUEUE MESSAGE BOX FUNCTIONS PROTOTYPES		   *
************************************************************/
#if QUEUE_MESSAGE_BOX_ENABLE == Enable
/*
 * @brief This Function is used to send element with size u32 to queue to be shared between tasks.
 * @param copy_u32ToData pointer to data to be add in queue
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_voidSendItemToQueue(uint32_t copy_u32ToData);
/*
 * @brief This Function is used to receive first element with size u32 .
 * @param pToData 	pointer to data to be add in queue
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_RecieveItemFromQueue(uint32_t *pToData);
/*
 * @brief This Function is used to reset the queue.
 * @param void
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
ArefRTOS_ErrorID ArefRTOS_ResetQueue(void );
#endif


#endif /* INC_AREFRTOS_SCHEDULER_H_ */
