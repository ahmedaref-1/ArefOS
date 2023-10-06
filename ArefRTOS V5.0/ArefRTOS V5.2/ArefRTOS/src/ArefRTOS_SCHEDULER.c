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

// Decrease the current PSP of the task by a number of steps
#define DECREASE_SP_POSITION(TSK_REF, NUM)	TSK_REF->TaskPrivateStates.pCurrentPSP -=NUM
#define INCREASE_SP_POSITION(TSK_REF, NUM)	TSK_REF->TaskPrivateStates.pCurrentPSP +=NUM


//	Dummy reset value for the _XPSR register where T must be = 1 to avoid BUS fault
#define RST_xPSR	(uint32_t)0x01000000
//	(EXC_RETURN) Return to thread mode using PSP stack as stack area
#define RST_LR		(uint32_t)0xFFFFFFFD


/***********************************************************
* 														   *
*			        TYPE-DEFINITIONS	                   *
*														   *
************************************************************/
/* @brief Operating Systm States */
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
	ArefRTOS_Task*		OS_SchedulerTable[MAX_NUM_OF_TASKS] ;		//	Pointer to user created tasks
	ArefRTOS_Task*		OS_CurrentTask ;
	ArefRTOS_Task*		OS_NextTask ;
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

/*  @brief Is object from @ref ArefRTOS_Task that named as IDLE task will take place while there is no running tasks.*/
static ArefRTOS_Task	Global_IdleTask ;
uint8_t IDLETaskLED,STKHandlerLED;

#if QUEUE_MESSAGE_BOX_ENABLE == Enable
	// Global Queue Array Definition
	static uint32_t Global_MessagesQueueArray[(MESSAGE_QUEUE_MAX_SIZE+1)] ;
	/*
	 * @brief Is object from @ref ArefRTOS_Queue used to create number of queue buffers used in application
	 */
	static ArefRTOS_Queue Global_MessagesQueueSynchnroizationInformation ;
#endif


/***********************************************************
* 														   *
*			    PRIVATE FUNCTIONS PROTOTYPES               *
*														   *
************************************************************/
static ArefRTOS_ErrorID ArefRTOS_staticMainStack();
static void ArefRTOS_voidIdleTask(void);
FORCE_INLINE static void ArefRTOS_voidIdleTaskInit (void);
FORCE_INLINE static void ArefRTOS_voidDesignTaskStack (ArefRTOS_Task* pTask);

/***********************************************************
* 														   *
*			    PRIVATE FUNCTIONS DEFINITIONS              *
*														   *
************************************************************/
/* @brief This Function is used to create the Main stack area with user defined size @ref MainStackSize
 * @return ArefRTOS_ErrorID return one of @ref ArefRTOS_ErrorID
 */
static ArefRTOS_ErrorID ArefRTOS_staticMainStack()
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NoError ;
	// 01. Initialize start of main stack as top of HW stack
	OS_Control._S_MSP_Task = (uint32_t)&START_OF_STACK_IN_HW ;
	// 02. Initialize the end of main stack
	OS_Control._E_MSP_Task = (uint32_t)(OS_Control._S_MSP_Task - MAINSTACKSIZE) ;
	// 03. Check if exceeded the available stack size,The processor uses a full descending stack
	CHECK_STACK_OVF(OS_Control._E_MSP_Task) ;
	// 04. Reserve four bytes as safety and hold the last address in HW stack
	OS_Control.HW_Stack_Locator = ( OS_Control._E_MSP_Task - SAFETY_SPACE_BETWEEN_STACKS) ;

	return LOC_ArefRTOS_ErrorID ;
}

/* @brief Definition of Idle Task */
static void ArefRTOS_voidIdleTask(void)
{
	for(;;){
		IDLETaskLED^=1;
		// "WFE" instruction waits for various events, including both external and exceptional interrupts.
		__asm("WFE");
	}

}

/**
 * @brief This is function is used to set attributes of IDLE task
 * @details	This function is inline
 * @return FORCE_INLINE return one of @ref ArefRTOS_ErrorID
 */
FORCE_INLINE static void ArefRTOS_voidIdleTaskInit (void)
{
	Global_IdleTask.TaskID 			= 0 ;
	Global_IdleTask.TaskPriority	=	255	; // Take care that highest number is lowest priority
	Global_IdleTask.TaskStackSize 	= 100;
	Global_IdleTask.pTaskFcn 		= ArefRTOS_voidIdleTask;
}

/**
 * @brief This Function used to design the stack of the task
 * @details Define The Task Frame
 * 	Task Frame When Create Task For First Time Will Add the Initial values after Reset
	 * ==================================================================================
	 *                    *Automatically saved and restored*
	 * XPSR
	 * PC (Next Task Instruction which should be Run)
	 * LR (return register which is saved in CPU while TASk1 running before TaskSwitching)
	 * r12
	 * r4
	 * r3
	 * r2
	 * r1
	 * r0
	 *====================================================================================
	 *                      *Manually saved and restored*
	 *r5
	 *r6
	 *r7
	 *r8
	 *r9
	 *r10
	 *r11
	 ====================================================================================
 * @param pTask Pointer to task
 * @return void
 */
FORCE_INLINE static void ArefRTOS_voidDesignTaskStack (ArefRTOS_Task* pTask)
{
	uint8_t LOC_u8Counter = 0 ;

	// 01. Set The Current PSP to point to the start address of the process stack
	pTask->TaskPrivateStates.pCurrentPSP 	= (uint32_t *)pTask->TaskPrivateStates._S_PSP_Task	;

	// 02. Set the xPSR Register with the reset value
	*(pTask->TaskPrivateStates.pCurrentPSP) =	RST_xPSR	;

	// 03. Set the PC Register with the address of the Task to start execution
	DECREASE_SP_POSITION(pTask,1) ;
	*(pTask->TaskPrivateStates.pCurrentPSP) = (uint32_t)pTask->pTaskFcn ;

	// 04. Set the LR Register with the EXC_RETURN Code to know that it needs to return to a Thread mode with SP pointing to Process Stack
	DECREASE_SP_POSITION(pTask,1) ;
	*(pTask->TaskPrivateStates.pCurrentPSP) = RST_LR	;

	// 05. Set the registers from r0 to r12 with reset value for registers
	for(LOC_u8Counter = 0 ; LOC_u8Counter < 13 ; LOC_u8Counter++ )
	{
		DECREASE_SP_POSITION(pTask,1) ;
		*(pTask->TaskPrivateStates.pCurrentPSP) = INIT_VAL_FOR_REG ;
	}

}

static void ArefRTOS_voidBubbleSort(){
	// 01.Initialize the counter and create a temp variable of type ArefRTOS Task
	uint8_t LOC_u8Counter = OS_Control.CurrentNumberofTasks ;
	ArefRTOS_Task *LOC_tempTask = NULL ;

	// 02.Apply the bubble sort algorithm based on priority (lowest value is the highest priority)
	for(uint8_t i = 0 ; i < LOC_u8Counter - 1 ; i++){
		for(uint8_t j = 0 ; j < LOC_u8Counter - i - 1; j++  ){
			if(OS_Control.OS_SchedulerTable[j]->TaskPriority > OS_Control.OS_SchedulerTable[j+1]->TaskPriority){
				LOC_tempTask = OS_Control.OS_SchedulerTable[j];
				OS_Control.OS_SchedulerTable[j] = OS_Control.OS_SchedulerTable[j+1];
				OS_Control.OS_SchedulerTable[j+1] = LOC_tempTask;
			}
		}
	}
}

static void ArefRTOS_voidFirstStageScheduler(){

	// 01.Create some temporary local variables
	ArefRTOS_Task *LOC_currentTask = NULL , *LOC_nextTask = NULL,*LOC_tempTask = NULL ;
	uint8_t LOC_u8Counter = 0;


	// 02.Sort Tasks in the tasks table
	ArefRTOS_voidBubbleSort();

	// 03.Select Current and Next Task
		//03.1.Free Ready Queue
		while(queue_get(&LOC_tempTask, &Global_QueueOfReadyTasks) != QUEUE_EMPTY);

		// 03.2.Update Ready Queue
		for(LOC_u8Counter = 0 ; LOC_u8Counter < OS_Control.CurrentNumberofTasks; LOC_u8Counter++)
		{
			LOC_currentTask = OS_Control.OS_SchedulerTable[LOC_u8Counter];
			LOC_nextTask = OS_Control.OS_SchedulerTable[LOC_u8Counter+1];

			/*
			  -The OS tasks table is already sorted from the previous step
			  -Waiting Tasks will be added to the ready queue only if their state is not suspended
			   and their priorities are the highest priority among those tasks in the OS tasks table
			*/
			if(LOC_currentTask->TaskPrivateStates.TaskState != TS_Suspend)
			{
			   // 03.2.1.Check Priority Options

				/*
				 	 03.2.1.1.Case we reached the end of the OS tasks table or
				 	 Then Enqueue the current task, change its state to ready and terminate
				 */
				if(LOC_nextTask->TaskPriority == TS_Suspend){
					queue_add(LOC_currentTask, &Global_QueueOfReadyTasks);
					LOC_currentTask->TaskPrivateStates.TaskState = TS_Ready;
					break ;
				}
				/*
				 	 03.2.1.2.Case current task priority number is less than next task priority number (current task has higher priority)
				 	 Then Enqueue the current task, change its state to ready and terminate
				 */
				else if(LOC_currentTask->TaskPriority < LOC_nextTask->TaskPriority){
					queue_add(LOC_currentTask, &Global_QueueOfReadyTasks);
					LOC_currentTask->TaskPrivateStates.TaskState = TS_Ready;
					break ;
				}
				/*
				 	 03.2.1.3.Case current task priority number equals next task priority number (current task has higher priority)
				 	 Then Round-Robin takes place, Enqueue current task to the ready queue
				 	 and by default the next task will be the current task for the next iteration so it will be handled automatically
				 	 So just don't break and keep it looping to enqueue tasks that should run in Round-Robin with time slices
				 */
				else if(LOC_currentTask->TaskPriority == LOC_nextTask->TaskPriority)
				{
					queue_add(LOC_currentTask, &Global_QueueOfReadyTasks);
					LOC_currentTask->TaskPrivateStates.TaskState= TS_Ready ;
				}
				/*
					03.2.1.4.Case current task priority number is more than next task priority number (current task has less priority)
					Then this is not allowed to happen as we already sorted the OS tasks table
				*/
				else
				{
					break ;
				}
			}
		}
}

static void ArefRTOS_voidSecondStageDispatcher(){

	// 01.Create some temporary local variables
	uint8_t LOC_u8QueueSize = queue_size(&Global_QueueOfReadyTasks);
	ArefRTOS_Task *LOC_currentTask = NULL ;

	// 03.Select Current and Next Task to be running
		/*
	 	 03.1.Case that the ready queue is empty and there is no ready tasks and the current task is suspended
	 	  	  Then activate the idle task
		 */
	if((0 == LOC_u8QueueSize)&&(OS_Control.OS_CurrentTask->TaskPrivateStates.TaskState == TS_Suspend)){
		OS_Control.OS_NextTask = &Global_IdleTask;
	}
		/*
		 03.2.Case that the ready queue is empty and there is no ready tasks and the current task is not suspended
		 	  Then let the last task that was running, continue running
		 */
	else if((0 == LOC_u8QueueSize)&&(OS_Control.OS_CurrentTask->TaskPrivateStates.TaskState != TS_Suspend))
	{
		OS_Control.OS_CurrentTask->TaskPrivateStates.TaskState = TS_Running;
		queue_add(OS_Control.OS_CurrentTask,&Global_QueueOfReadyTasks);
		OS_Control.OS_NextTask = OS_Control.OS_CurrentTask;
	}
		/*
	 	 03.3.Case that the ready queue is not empty
	 	  	  Then enqueue the first task in the FIFO, assign it to be the next task
	 	  	  check if the current task is of same priority then apply the round robin algorithm
		 */
	else
	{
		queue_get(&LOC_currentTask, &Global_QueueOfReadyTasks);
		OS_Control.OS_NextTask = LOC_currentTask ;
		OS_Control.OS_NextTask->TaskPrivateStates.TaskState = TS_Running;
		if((OS_Control.OS_CurrentTask->TaskPriority == OS_Control.OS_NextTask->TaskPriority)
				&&(OS_Control.OS_CurrentTask->TaskPrivateStates.TaskState != TS_Suspend)){
			queue_add(OS_Control.OS_CurrentTask,&Global_QueueOfReadyTasks);
			OS_Control.OS_CurrentTask->TaskPrivateStates.TaskState = TS_Ready;
		}
	}
}


/***********************************************************
* 														   *
*			      USER FUNCTIONS DEFINITIONS               *
*														   *
************************************************************/
/***********************************************************
* 		GENERAL FUNCTIONS FOR SCHEDULER DEFINITIONS		   *
************************************************************/
ArefRTOS_ErrorID ArefRTOS_voidCreateTask(ArefRTOS_Task* pTask)
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NoError ;

	// 01.Set Start of Task Stack by HW Stack Locator in OS_Control
	pTask->TaskPrivateStates._S_PSP_Task	=	OS_Control.HW_Stack_Locator	;

	// 02.Set End of Stack End = Start - (Stack Size of Task + MIN_STACK_SIZE due to TCB)
	pTask->TaskPrivateStates._E_PSP_Task	=	pTask->TaskPrivateStates._S_PSP_Task - (pTask->TaskStackSize + MIN_STACK_SIZE);

	// 03.Check for Stack Overflow
	CHECK_STACK_OVF(pTask->TaskPrivateStates._E_PSP_Task);

	// 04.Align 4 Bytes as safety
	OS_Control.HW_Stack_Locator = pTask->TaskPrivateStates._E_PSP_Task - SAFETY_SPACE_BETWEEN_STACKS	;

	// 05.Initialize Stack Area
	ArefRTOS_voidDesignTaskStack(pTask);

	// 06.Check Current Number of Tasks comparing between number of defined tasks
	if(OS_Control.CurrentNumberofTasks <= MAX_NUM_OF_TASKS)
	{
		// 6.1.Update Scheduler Table
		OS_Control.OS_SchedulerTable[OS_Control.CurrentNumberofTasks]= pTask ;
		OS_Control.CurrentNumberofTasks++ ;

		// 6.2.Update Task State
		pTask->TaskPrivateStates.TaskState = TS_Suspend ;
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

	// 01.HW initialize
	ArefRTOS_voidHardwareInit();

	// 02.OS Start With Suspend State
	OS_Control.OS_State	=	OS_Suspend ;

	// 03.Specify Main Stack Area for OS
	LOC_ArefRTOS_ErrorID = ArefRTOS_staticMainStack();

	// 04.Initialize Task Queue
	LOC_ArefRTOS_ErrorID = queue_create(&Global_QueueOfReadyTasks)	;
	if(LOC_ArefRTOS_ErrorID != (ArefRTOS_ErrorID)QUEUE_NO_ERROR)
		LOC_ArefRTOS_ErrorID = ErrorInQueueInit	;

	// 05.Initialize Idle Task
	ArefRTOS_voidIdleTaskInit();
	LOC_ArefRTOS_ErrorID = ArefRTOS_voidCreateTask(&Global_IdleTask);

	return LOC_ArefRTOS_ErrorID ;
}

ArefRTOS_ErrorID ArefRTOS_voidActivateTask(ArefRTOS_Task* pTask)
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NULL_ARGs ;
	if(pTask != NULL)
	{
		LOC_ArefRTOS_ErrorID =	NoError  ;
		// 01.Add It in Waiting State
		pTask->TaskPrivateStates.TaskState = TS_Waiting;
		// 02.Call Service Called SVC_CALL_ACTIVATE_TASK
		ArefRTOS_voidCallService((uint8_t)ArefRTOS_SVC_CALL_ACTIVATE_TASK);
	}
	else
	{
		LOC_ArefRTOS_ErrorID =	NULL_ARGs ;
	}

	return LOC_ArefRTOS_ErrorID ;
}

ArefRTOS_ErrorID ArefRTOS_voidTerminateTask(ArefRTOS_Task* pTask)
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NULL_ARGs ;
	if(pTask != NULL)
	{
		LOC_ArefRTOS_ErrorID =	NoError  ;
		// 01.Add It in Suspend State
		pTask->TaskPrivateStates.TaskState = TS_Suspend;
		// 02.Call Service Called SVC_CALL_TERMINATE_TASK
		ArefRTOS_voidCallService((uint8_t)ArefRTOS_SVC_CALL_TERMINATE_TASK);
	}
	else
	{
		LOC_ArefRTOS_ErrorID =	NULL_ARGs ;
	}

	return LOC_ArefRTOS_ErrorID ;
}

ArefRTOS_ErrorID ArefRTOS_voidTaskDelay(ArefRTOS_Task* pTask, uint32_t copy_u32NumberofTicks)
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NoError ;
	// 01.Change Task state to be Suspend
	pTask->TaskPrivateStates.TaskState = TS_Suspend;
	// 02.Enable Function Delay
	pTask->TaskPrivateStates.TimeDelayFlag = Enable ;
	// 03.Add number of ticks
	pTask->TaskPrivateStates.TaskDelayTime = copy_u32NumberofTicks;
	// 04.Call Service Called SVC_CALL_DELAY_TASK
	ArefRTOS_voidCallService((uint8_t)ArefRTOS_SVC_CALL_DELAY_TASK);

	return LOC_ArefRTOS_ErrorID ;
}

ArefRTOS_ErrorID ArefRTOS_voidStartScheduler(void)
{
	ArefRTOS_ErrorID	LOC_ArefRTOS_ErrorID =	NoError ;

	// 01.Set OS Operating Mode
	OS_Control.OS_State = OS_Running ;
	// 02.Idle task as an Initial Task
	OS_Control.OS_CurrentTask = &Global_IdleTask ;
	// 03.Set the system PSP to the current process(idle process) psp
	PSRC_voidSetPSP((uint32_t)Global_IdleTask.TaskPrivateStates.pCurrentPSP);
	// 04.Activate the idle Task
	LOC_ArefRTOS_ErrorID =ArefRTOS_voidActivateTask(&Global_IdleTask);
	// 05.Start the System Ticker
	ArefRTOS_voidStartTicker();
	// 06.Switch the system SP to point to the PSP
	PSRC_voidSetWorkingStack_PSP();
	// 07.Switch the Access Level to Unprivileged Mode
	PSRC_voidSetAccessLevel_UNPRIVILEGED();
	// 08.Call Entry Point of Task
	OS_Control.OS_CurrentTask->pTaskFcn();

	return LOC_ArefRTOS_ErrorID ;

}

/***********************************************************
* 		     MUTEX FUNCTIONS DEFINITIONS				   *
************************************************************/
#if MUTEX_ENABLE == Enable
ArefRTOS_ErrorID ArefRTOS_CreateMutex(ArefRTOS_Mutex *pMutex)
{
    // 01. Initialize the local error code variable to NoError.
    ArefRTOS_ErrorID LOC_ArefRTOS_ErrorID = NoError;

    if (pMutex != NULL)
    {
        // 02. If the pMutex pointer is not NULL, it means the function was called with a valid mutex pointer.

        // 03. Set the WaitingMessagesCounter in the mutex's private data structure to indicate that the mutex is available.
        pMutex->QueuePrivateData.WaitingMessagesCounter = Set;

        // 04. Reset the priorityInversionFlag to indicate that no priority inversion has occurred.
        pMutex->PriorityInversionFlag = Reset;

        // 05. Set the inheritedTask pointer to NULL, indicating that no task has inherited this mutex.
        pMutex->InheritedTask = NULL;
    }
    else
    {
        // 06. If the pMutex pointer is NULL, it means the function was called with an invalid argument.

        // 07. Set the local error code to NULL_ARGs to indicate an invalid argument error.
        LOC_ArefRTOS_ErrorID = NULL_ARGs;
    }

    // 08. Return the error code, which will be NoError if the mutex was successfully created and initialized,
    // or NULL_ARGs if the input argument was NULL.
    return LOC_ArefRTOS_ErrorID;
}

ArefRTOS_ErrorID ArefRTOS_AquireMutex(ArefRTOS_Mutex *pMutex, ArefRTOS_Task* pCurrentRunningTask)
{
    // 01. Initialize the local error code variable to NoError.
    ArefRTOS_ErrorID LOC_ArefRTOS_ErrorID = NoError;

    // 02. Check if both the mutex pointer and the current running task pointer are not NULL.
    if (pMutex != NULL && pCurrentRunningTask != NULL)
    {
        // 03. Check if the mutex is available (WaitingMessagesCounter == Set).
        if (pMutex->QueuePrivateData.WaitingMessagesCounter == Set)
        {
            // 04. Mutex is available, so the current running task can acquire it.

            // 05. Set the MutexHolder to the current running task, indicating ownership.
            pMutex->MutexHolder = pCurrentRunningTask;

            // 06. Mark the mutex as unavailable by setting WaitingMessagesCounter to Reset.
            pMutex->QueuePrivateData.WaitingMessagesCounter = Reset;
        }
        else
        {
            // 07. Mutex is not available; a task is already holding it.

            // 08. Check if the current task's priority is higher than or equal to the mutex holder's priority.
            if (pCurrentRunningTask->TaskPriority <= pMutex->MutexHolder->TaskPriority)
            {
                // 09. Current task has higher or equal priority than the mutex holder, indicating potential priority inversion.

                // 10. Check if priority inversion has not occurred before for this mutex.
                if (pMutex->PriorityInversionFlag == Reset)
                {
                    // 11. Priority inversion hasn't occurred before.

                    // 12. Assign the inherited task to the current running task.
                	//Inherited Task here means the task that the mutex holder task would inherit its priority
                    pMutex->PriorityInversionFlag = Set;
                    pMutex->InheritedTask = pCurrentRunningTask;

                    // 13. Swap priorities between the current task (inherited task)and the mutex holder.
                    // such that the mutex holder finishes execution an releases the mutex
                    uint8_t tempPriority = pMutex->MutexHolder->TaskPriority;
                    pMutex->MutexHolder->TaskPriority = pCurrentRunningTask->TaskPriority;
                    pCurrentRunningTask->TaskPriority = tempPriority;
                }
                else
                {
                    // 14. Priority inversion has occurred before.

                    // 15. Restore priorities before changing them.
                    uint8_t tempPriority = pMutex->InheritedTask->TaskPriority;
                    pMutex->InheritedTask->TaskPriority = pMutex->MutexHolder->TaskPriority;
                    pMutex->MutexHolder->TaskPriority = tempPriority;

                    // 16. Update the new inherited task to the current running task(of higher priority).
                    pMutex->InheritedTask = pCurrentRunningTask;

                    // 17. Swap priorities between the current task (new inherited task) and the mutex holder.
                    tempPriority = pMutex->MutexHolder->TaskPriority;
                    pMutex->MutexHolder->TaskPriority = pCurrentRunningTask->TaskPriority;
                    pCurrentRunningTask->TaskPriority = tempPriority;
                }

                // 18. Call a service function to handle priority inversion.
                ArefRTOS_voidCallService(ArefRTOS_SVC_CALL_PRIORITY_INVERSION);
            }
            else
            {
                // 19. The current task's priority is lower than the mutex holder's priority.
                // No priority inversion is needed in this case.
                // It's assumed that this case shouldn't occur, as lower-priority tasks shouldn't reach this point.
            }
        }
    }
    else
    {
        // 20. If either the mutex pointer or the current running task pointer is NULL, it's an invalid argument.

        // 21. Set the local error code to NULL_ARGs to indicate an invalid argument error.
        LOC_ArefRTOS_ErrorID = NULL_ARGs;
    }

    // 22. Return the error code, which will be NoError if the mutex acquisition was successful,
    // or NULL_ARGs if there was an invalid argument.
    return LOC_ArefRTOS_ErrorID;
}

ArefRTOS_ErrorID ArefRTOS_ReleaseMutex(ArefRTOS_Mutex *pMutex, ArefRTOS_Task* pCurrentRunningTask)
{
    // 01. Initialize the local error code variable to NoError.
    ArefRTOS_ErrorID LOC_ArefRTOS_ErrorID = NoError;

    // 02. Check if both the mutex pointer and the current running task pointer are not NULL.
    if (pMutex != NULL && pCurrentRunningTask != NULL)
    {
        // 03. Check if the mutex is currently unavailable (locked).
        if (pMutex->QueuePrivateData.WaitingMessagesCounter == Reset)
        {
            // 04. Check if the current running task is the holder of the mutex.
            if (pMutex->MutexHolder == pCurrentRunningTask)
            {
                // 05. The current running task is the correct holder of the mutex.

                // 06. Check the priority inversion flag.
                if (pMutex->PriorityInversionFlag == Reset)
                {
                    // 07. No priority inversion has occurred, so the mutex can be released.

                    // 08. Set the WaitingMessagesCounter to 1 to indicate the mutex is available.
                    pMutex->QueuePrivateData.WaitingMessagesCounter = Set;
                }
                else
                {
                    // 09. Priority inversion has occurred, and the mutex is still unavailable.

                    // 10. Set WaitingMessagesCounter to 0 to indicate the mutex is unavailable.
                    pMutex->QueuePrivateData.WaitingMessagesCounter = Reset;

                    // 11. Clear the priority inversion flag.
                    pMutex->PriorityInversionFlag = Reset;

                    // 12. Swap priorities to restore them.
                    uint8_t tempPriority = pMutex->InheritedTask->TaskPriority;
                    pMutex->InheritedTask->TaskPriority = pMutex->MutexHolder->TaskPriority;
                    pMutex->MutexHolder->TaskPriority = tempPriority;

                    // 13. Update the mutex holder to the inherited task.
                    // where the inherited task is the higher priority task that was waiting for the lower priority task (mutex holder) to release the mutex
                    // and helped her to execute wihtout preemption by giving it its priority
                    pMutex->MutexHolder = pMutex->InheritedTask;

                    // 14. Call a service (SVC) to handle priority inversion.
                    ArefRTOS_voidCallService(ArefRTOS_SVC_CALL_PRIORITY_INVERSION);
                }
            }
            else
            {
                // 15. The current running task does not have permission to release the mutex.
                LOC_ArefRTOS_ErrorID = INVALID_OPERATION;
            }
        }
        else
        {
            // 16. The mutex is already available (unlocked).
            LOC_ArefRTOS_ErrorID = INVALID_OPERATION;
        }
    }
    else
    {
        // 17. One or both of the input arguments (mutex or current running task) are NULL.
        LOC_ArefRTOS_ErrorID = NULL_ARGs;
    }

    // 18. Return the error code, which reflects the outcome of the mutex release operation.
    return LOC_ArefRTOS_ErrorID;
}

#endif

/***********************************************************
* 		   QUEUE MESSAGE BOX FUNCTIONS PROTOTYPES		   *
************************************************************/
#if QUEUE_MESSAGE_BOX_ENABLE == Enable
ArefRTOS_ErrorID ArefRTOS_voidSendItemToQueue(uint32_t copy_u32ToData)
{
    // 01. Initialize the local error code variable to NoError.
    ArefRTOS_ErrorID LOC_ArefRTOS_ErrorID = NoError;

    // 02. Check if the waiting messages counter in the queue is less than or equal to the maximum queue size.
    if (Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.WaitingMessagesCounter <= MESSAGE_QUEUE_MAX_SIZE)
    {
        // 03. Increment the waiting messages counter to prepare for adding a new item to the queue.
        // Note: The queue is assumed to start from index 1.
        Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.WaitingMessagesCounter++;

        // 04. Add the provided data item to the queue at the current waiting messages counter index.
        // Note: The queue array is assumed to be named Global_voidQueueArray.
        // Insert the data at the next available index in the queue.
        Global_MessagesQueueArray[Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.WaitingMessagesCounter] = copy_u32ToData;
    }
    else
    {
        // 05. If the message waiting counter exceeds the maximum queue size, set the local error code to INVALID_OPERATION.
        LOC_ArefRTOS_ErrorID = INVALID_OPERATION;
    }

    // 06. Return the error code, which will be NoError if the item was successfully added to the queue,
    // or INVALID_OPERATION if the queue size limit was exceeded.
    return LOC_ArefRTOS_ErrorID;
}

ArefRTOS_ErrorID ArefRTOS_RecieveItemFromQueue(uint32_t *pToData)
{
    // 01. Initialize the local error code variable to NoError.
    ArefRTOS_ErrorID LOC_ArefRTOS_ErrorID = NoError;

    // 02. Check if there are items in the queue to receive.
    if (Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.WaitingMessagesCounter != 0)
    {
        // 03. Increment the index for the next item to be popped from the queue.
        Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.NextPopedItemIndex++;

        // 04. If the index exceeds the maximum queue size, wrap around to the beginning.
        if (Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.NextPopedItemIndex > (MESSAGE_QUEUE_MAX_SIZE - 1))
        {
            Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.NextPopedItemIndex = 1;
        }

        // 05. Pop the item from the queue and store it in the location pointed to by pToData.
        *pToData = Global_MessagesQueueArray[Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.NextPopedItemIndex];

        // 06. Decrement the counter to indicate one less item in the queue.
        Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.WaitingMessagesCounter--;
    }
    else
    {
        // 07. If there are no items in the queue, set the local error code to INVALID_OPERATION.
        LOC_ArefRTOS_ErrorID = INVALID_OPERATION;
    }

    // 08. Return the error code, which will be NoError if an item was successfully received from the queue,
    // or INVALID_OPERATION if the queue was empty.
    return LOC_ArefRTOS_ErrorID;
}

ArefRTOS_ErrorID ArefRTOS_ResetQueue(void)
{
    // 01. Initialize the local error code variable to NoError.
    ArefRTOS_ErrorID LOC_ArefRTOS_ErrorID = NoError;

    // 02. Clear the waiting messages counter in the global message queue synchronization information.
    Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.WaitingMessagesCounter = 0;

    // 03. Reset the next popped item index in the global message queue synchronization information.
    Global_MessagesQueueSynchnroizationInformation.QueuePrivateData.NextPopedItemIndex = 0;

    // 04. Return the error code, which will be NoError indicating that the queue has been successfully reset.
    return LOC_ArefRTOS_ErrorID;
}

#endif
/******************************************************************************
* 																			  *
* 					HANDLER MODE FUNCTIONS DEFINITION						  *
* 																			  *
*******************************************************************************/
void ArefRTOS_SVC_CALL( uint32_t *svc_args )
{
 /*Stack contains:
  * xPSR
  * PC
  * LR
  * R12
  * R3
  * R2
  * R1
  * R0
  * First argument (R0) is svc_args[0]
  */
  uint8_t SVCParameter = *((uint8_t *)(((uint8_t*)svc_args[6])-2)) ;

  switch(SVCParameter)
  {
    case ArefRTOS_SVC_CALL_ACTIVATE_TASK:
    case ArefRTOS_SVC_CALL_TERMINATE_TASK:
    case ArefRTOS_SVC_CALL_DELAY_TASK:
    case ArefRTOS_SVC_CALL_PRIORITY_INVERSION:
    		// 01. Calling the Scheduler to decide which process to execute next, based on priority
    		ArefRTOS_voidFirstStageScheduler();
    		// 02. Check if OS Working
    		if(OS_Control.OS_State == OS_Running)
    		{
    			if(OS_Control.OS_CurrentTask->TaskID != Global_IdleTask.TaskID )
    			{
    				// 02.1.Calling Dispatcher to switch the CPU from one process to another, saving and restoring the state of each process.
    				ArefRTOS_voidSecondStageDispatcher();
    				// 02.2.Context Switching
    				SCB_voidTrigPendSV();
    			}

    		}
    	break;

    default:    /* unknown SVC */
      break;
  }
}

FORCE_INLINE static void ArefRTOS_voidCheckDelayedTasks (void)
{
	uint8_t LOC_u8Counter = 0 ;
	// 01.Iterate through the total number of tasks
	for(LOC_u8Counter = 0 ; LOC_u8Counter < OS_Control.CurrentNumberofTasks; LOC_u8Counter++)
	{
		// 02.Check if the task is suspended and waiting time flag is enabled
		if((OS_Control.OS_SchedulerTable[LOC_u8Counter]->TaskPrivateStates.TaskState == TS_Suspend) && (OS_Control.OS_SchedulerTable[LOC_u8Counter]->TaskPrivateStates.TimeDelayFlag == 1))
		{
			// 03.Decrement the counter of the waiting task
			OS_Control.OS_SchedulerTable[LOC_u8Counter]->TaskPrivateStates.TaskDelayTime-- ;
			if(OS_Control.OS_SchedulerTable[LOC_u8Counter]->TaskPrivateStates.TaskDelayTime == 0)
			{
				// 04.Disable Waiting
				OS_Control.OS_SchedulerTable[LOC_u8Counter]->TaskPrivateStates.TimeDelayFlag = Disable ;
				// 05.Change the task state to waiting state
				OS_Control.OS_SchedulerTable[LOC_u8Counter]->TaskPrivateStates.TaskState = TS_Waiting ;
				// 06.Call service ID called SVC_CALL_DELAY_TASK
				ArefRTOS_voidCallService(ArefRTOS_SVC_CALL_DELAY_TASK);
			}
		}
	}
}
/******************************************************************************
* 																			  *
* 					IRQ HANDLER FUNCTIONS DEFINITION						  *
* 																			  *
*******************************************************************************/
FORCE_NAKED void PendSV_Handler(void){

		/*
		 	01.Save Current Context of current task
			xPSR,PC,LR,R12,R3,R2,R1,R0 automatically Pushed
		 */
		OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP = (uint32_t*)PSRC_voidGetPSP();

		/*
		 	02.Save Rest of registers of current task
			R11,R10,R9,R8,R7,R6,R5,R4 manually Pushed
		 */
		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R4	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));

		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R5	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));

		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R6	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));

		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R7	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));

		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R8	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));

		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R9	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));

		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R10	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));

		DECREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// Decrease Current SP by one
		__asm volatile ("MOV %0,R11	" : "=r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));


		/*
		 	03.Switch the control to the next task process stack
		 */
		if(OS_Control.OS_NextTask != NULL)
		{
			OS_Control.OS_CurrentTask = OS_Control.OS_NextTask;
			OS_Control.OS_NextTask = NULL ;
		}

		/*
		 	04.Restore the Rest of registers from the new task process stack
			R11,R10,R9,R8,R7,R6,R5,R4 manually Poped
		 */
		__asm volatile ("MOV R11,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		__asm volatile ("MOV R10,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		__asm volatile ("MOV R9,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		__asm volatile ("MOV R8,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		__asm volatile ("MOV R7,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		__asm volatile ("MOV R6,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		__asm volatile ("MOV R5,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		__asm volatile ("MOV R4,%0" : :"r"(*(OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP)));
		INCREASE_SP_POSITION(OS_Control.OS_CurrentTask,1);	// increase Current SP by one

		/*
		 	05.Update CPU PSP with the new task PSP value
		 */
		PSRC_voidSetPSP((uint32_t)OS_Control.OS_CurrentTask->TaskPrivateStates.pCurrentPSP);

		/*
		 	06.Switch the control to the next task and exit
		 */
		__asm("BX LR");

}

void SysTick_Handler(void){
	STKHandlerLED ^=1;
	//01.Update the delayed tasks & Update the scheduler table if needed
	ArefRTOS_voidCheckDelayedTasks();
	//02.Select Task to execute
	ArefRTOS_voidSecondStageDispatcher();
	//03.Context Switching
	SCB_voidTrigPendSV();
}
