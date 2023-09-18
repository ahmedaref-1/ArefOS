/*
 * ArefRTOS_SCHEDULER.c
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"ArefRTOS_SCHEDULER.h"
#include"ArefRTOS_PORTING.h"
#include"ArefRTOS_CIRCULARQUEUE.h"


/***********************************************************
* 														   *
*			    PRE-PROCESSOR CONSTANTS	                   *
*														   *
************************************************************/
#define SAFETY_SPACE_BETWEEN_STACKS		4
#define START_OF_STACK_IN_HW			_estack
#define START_OF_HEAP_IN_HW	   			_eheap
#define MIN_STACK_SIZE				    (16*4+68)
#define INIT_VAL_FOR_REG				0x0

/***********************************************************
* 														   *
*			      PRE-PROCESSOR MACROS    	               *
*														   *
************************************************************/
#define CHECK_STACK_OVF(ADDRESS)		if( ADDRESS < (uint32_t)&START_OF_HEAP_IN_HW)	\
												LOC_ArefRTOS_ErrorID = StackOverflow

// Decrease The SP of Task in Current PSP
#define DECREASE_PC_BY(TSK_REF, NUM)	TSK_REF->taskPrivateStates.pCurrentPSP -=NUM
#define INCREASE_PC_BY(TSK_REF, NUM)	TSK_REF->taskPrivateStates.pCurrentPSP +=NUM


//	DUMMY_XPSR should T =1 to avoid BUS fault
#define RST_xPSR	(uint32_t)0x01000000
//	(EXC_RETURN) Return to thread with PSP stack as stack area
#define RST_LR		(uint32_t)0xFFFFFFFD


/***********************************************************
* 														   *
*			        TYPE-DEFINITIONS	                   *
*														   *
************************************************************/
/**
 * @brief Operating Systm States
 *
 */
typedef enum
{
	OS_Suspend	,
	OS_Running
}OS_States;


/* @brief This struct holds the needed attributes to manage the OS */
 struct
{
	uint32_t			_S_MSP_Task;  						//	Start of Main Stack Area
	uint32_t			_E_MSP_Task ;						//	End of Main Stack Area that defined by using @ref MainStackSize
	uint32_t			HW_Stack_Locator;					//	Current free address in Hardware Stack
	OS_States			OS_State ;							//	Current OS state.
	ArefRTOS_Task*		OS_Tasks[MAX_NUM_OF_TASKS] ;		//	Pointer to user created tasks
	ArefRTOS_Task*		OS_currentTask ;
	ArefRTOS_Task*		OS_nextTask ;
	uint8_t				CurrentNumberofTasks ;				//	Number of tasks created by user
}OS_Control;


/***********************************************************
* 														   *
*			        	VARIABLES	                       *
*														   *
************************************************************/
/**
 * @brief This variable holds the highest ready tasks
 *
 */
queue_t	Global_QueueOfReadyTasks;

/* @brief This Varible hold the whole tasks in system */
ArefRTOS_Task*	Global_pTasks[MAX_NUM_OF_TASKS]	;


/*  @brief Is object from @ref MRTOS_Task that named as IDLE task will take place while there is no running tasks.*/
static ArefRTOS_Task	Global_IdleTask ;


/***********************************************************
* 														   *
*			    PRIVATE FUNCTIONS PROTOTYPES               *
*														   *
************************************************************/


/***********************************************************
* 														   *
*			    PRIVATE FUNCTIONS DEFINITIONS              *
*														   *
************************************************************/
/* @brief This Function is used to create the Main stack area with used defined size @ref MainStackSize
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
static ArefRTOS_ErrorID ArefRTOS_staticMainStack()
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NoError ;
	// Initialize start of main stack as top of HW stack
	OS_Control._S_MSP_Task = (uint32_t)&START_OF_STACK_IN_HW ;
	// Initialize the end of main stack
	OS_Control._E_MSP_Task = (uint32_t)(OS_Control._S_MSP_Task - MainStackSize) ;
	//	Check if exceeded the available stack size,The processor uses a full descending stack
	CHECK_STACK_OVF(OS_Control._E_MSP_Task) ;
	// Reserve byte as safety and hold the last address in HW stack
	OS_Control.HW_Stack_Locator = ( OS_Control._E_MSP_Task - SAFETY_SPACE_BETWEEN_STACKS) ;

	return LOC_ArefRTOS_ErrorID ;
}

/* @brief Definition of Idle Task */
static void ArefRTOS_staticIdleTask(void)
{
	while(1)
	{
		__asm("NOP") ;
	}
}

/**
 * @brief This is function is used to set attributes of IDLE task
 * @details	This function is inline
 *
 * @return FORCE_INLINE return one of @ref ArefRTOS_ErrorID
 */
FORCE_INLINE static void ArefRTOS_staticIdleTaskInit (void)
{
	Global_IdleTask.taskID = 0 ;
	Global_IdleTask.taskPriority	=	255	;		//	Highest Number is Lowest Priority
	Global_IdleTask.taskStackSize = 100;
	Global_IdleTask.pTaskFcn = ArefRTOS_staticIdleTask;
}

/**
 * @brief This Function used to design the task of stack
 * @details Define The Task Frame
 *
 * 	Task Frame When Create Task For First Time Will Add the Initial values after Reset
	 * ==================================================================================
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r3
	 * r2
	 * r1
	 * r0
	 *====
	 *r4,r5, r6 , r7 ,r8 ,r9, r10,r11 (Saved/Restore)Manual

 * @param pTask Pointer to task
 * @return void
 */
FORCE_INLINE static void ArefRTOS_voidDesignTaskStack (ArefRTOS_Task* pTask)
{
	uint8_t LOC_u8Counter = 0 ;
	/*Task Frame When Create Task For First Time Will Add the Initial values after Reset
	 * ==================================================================================
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r3
	 * r2
	 * r1
	 * r0
	 *====
	 *r4, r5, r6 , r7 ,r8 ,r9, r10,r11 (Saved/Restore)Manual
	 */
	//	Set The Current SP pointed to The Last Item after create task space
	pTask->taskPrivateStates.pCurrentPSP = (uint32_t *)pTask->taskPrivateStates._S_PSP_Task	;
	//	Decrease the value of Current PSP

	DECREASE_PC_BY(pTask,1) ;
	//	Set Some of RESET values for CPU Registers
	*(pTask->taskPrivateStates.pCurrentPSP) =	RST_xPSR	;

	DECREASE_PC_BY(pTask,1) ;
	*(pTask->taskPrivateStates.pCurrentPSP) = (uint32_t)pTask->pTaskFcn ;	//	PC Has the address of the Task to start execution

	DECREASE_PC_BY(pTask,1) ;
	*(pTask->taskPrivateStates.pCurrentPSP) = RST_LR	;

	for(LOC_u8Counter = 0 ; LOC_u8Counter < 13 ; LOC_u8Counter++ )
	{

		DECREASE_PC_BY(pTask,1) ;
		*(pTask->taskPrivateStates.pCurrentPSP) = INIT_VAL_FOR_REG ;
	}

}



/***********************************************************
* 														   *
*			      USER FUNCTIONS DEFINITIONS               *
*														   *
************************************************************/
ArefRTOS_ErrorID ArefRTOS_voidCreateTask(ArefRTOS_Task* pTask)
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NoError ;

	// Set Start of Task Stack by HW Stack Locator in OS_Control
	pTask->taskPrivateStates._S_PSP_Task	=	OS_Control.HW_Stack_Locator	;

	//	Set End of Stack E=	S - (Stack Size of Task + MIN_STACK_SIZE due to TCB)
	pTask->taskPrivateStates._E_PSP_Task	=	pTask->taskPrivateStates._S_PSP_Task - (pTask->taskStackSize + MIN_STACK_SIZE);

	//	Check Overflow
	CHECK_STACK_OVF(pTask->taskPrivateStates._E_PSP_Task);

	//Align 4 Byte as safety
	OS_Control.HW_Stack_Locator = pTask->taskPrivateStates._E_PSP_Task - SAFETY_SPACE_BETWEEN_STACKS	;

	// Initialize Stack Area
	ArefRTOS_voidDesignTaskStack(pTask);

	//	Check Current Number of Tasks comparing between number of defined tasks
	if(OS_Control.CurrentNumberofTasks <= MAX_NUM_OF_TASKS)
	{
		//	Update Scheduler Table
		OS_Control.OS_Tasks[OS_Control.CurrentNumberofTasks]= pTask ;
		OS_Control.CurrentNumberofTasks++ ;

		// Update Task State
		pTask->taskPrivateStates.taskState = TS_Suspend ;
	}
	else
	{
		// Exceed Max Number of Tasks Defined by user
		LOC_ArefRTOS_ErrorID =	ExceedMaxNumberOfTasks ;
	}

	return LOC_ArefRTOS_ErrorID;

}

ArefRTOS_ErrorID ArefRTOS_voidInit(void)
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NoError ;

	// HW initialize
	ArefRTOS_voidHardwareInit();

	/* SW initialize
		//	OS Start With Suspend State
		//	Specify Main Stack Area for OS
		//	Initiate Queue
		//	Configure IDLE TASK
	 */
	//	OS Start With Suspend State
	OS_Control.OS_State	=	OS_Suspend ;

	//	Specify Main Stack Area for OS
	LOC_ArefRTOS_ErrorID = ArefRTOS_staticMainStack();

	//	Initialize Task Queue
	LOC_ArefRTOS_ErrorID = queue_create(&Global_QueueOfReadyTasks)	;
	if(LOC_ArefRTOS_ErrorID != (ArefRTOS_ErrorID)QUEUE_NO_ERROR)
		LOC_ArefRTOS_ErrorID = ErrorInQueueInit	;

	// Initialize Idle Task
	ArefRTOS_staticIdleTaskInit();
	LOC_ArefRTOS_ErrorID = ArefRTOS_voidCreateTask(&Global_IdleTask);

	return LOC_ArefRTOS_ErrorID ;
}


