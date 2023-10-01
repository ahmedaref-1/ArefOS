# ArefRTOS - A Self-Made Real-Time Operating System (Version 4.0) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 3.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS Terminate Task

The `ArefRTOS_voidTerminateTask` function is a vital component of the ArefRTOS real-time operating system (RTOS) responsible for terminating tasks within the system. It enables controlled task termination, allowing the RTOS to manage resources efficiently.

#### Function Explanation

1. **Error Handling**: This function begins by initializing a local error variable, `LOC_ArefRTOS_ErrorID`, to handle potential error conditions.

2. **Task Termination Request**: The function checks if the input pointer `pTask` is not NULL, indicating a valid task termination request.

3. **Error-Free Path**:
   - If `pTask` is not NULL, the function proceeds with task termination.
   - It sets `LOC_ArefRTOS_ErrorID` to `NoError`, indicating a successful execution path.
   - The task's state is updated to `TS_Suspend`, marking it as "Suspended." This change prevents further execution of the task.
   - A service call is initiated by invoking `ArefRTOS_voidCallService` with the argument `(uint8_t)ArefRTOS_SVC_CALL_TERMINATE_TASK`. This service call is essential for handling the task termination.

4. **Error Reporting**:
   - If the input pointer `pTask` is NULL, the function sets `LOC_ArefRTOS_ErrorID` to `NULL_ARGs`, signifying an error due to an invalid or NULL argument.

5. **Error Status Return**: Finally, the function returns the `LOC_ArefRTOS_ErrorID` value. This value communicates whether the task termination process was successful (`NoError`) or if an error occurred due to a NULL argument (`NULL_ARGs`).

In summary, the `ArefRTOS_voidTerminateTask` function provides a mechanism for controlled task termination within the ArefRTOS RTOS. It ensures that tasks are suspended appropriately and allows for error handling in cases of invalid task termination requests.


### 02. ArefRTOS Task Delay

The `ArefRTOS_voidTaskDelay` function is a critical component of the ArefRTOS real-time operating system (RTOS) that provides a mechanism for delaying the execution of tasks. It allows tasks to voluntarily suspend their execution for a specified number of system ticks.

#### Function Explanation

1. **Task State Transition**: This function changes the state of the specified task (`pTask`) to "Suspended." When a task is in the "Suspended" state, it temporarily halts its execution until the suspension is lifted.

2. **Enable Time Delay**: It enables a function delay for the task by setting the `TimeDelayFlag` in the task's private states to "Enable." This flag is used to track whether the task is currently in a delay state.

3. **Set Delay Duration**: The function sets the number of system ticks for which the task should delay its execution. This duration is specified by the `copy_u32NumberofTicks` parameter.

4. **Service Call for Delay**: A service call is made by invoking `ArefRTOS_voidCallService` with the argument `(uint8_t)ArefRTOS_SVC_CALL_DELAY_TASK`. This service call indicates the request to delay the task's execution.

5. **Error Handling**: The `LOC_ArefRTOS_ErrorID` remains set to `NoError` throughout the function execution, indicating that no errors have been encountered during the delay request.

In summary, the `ArefRTOS_voidTaskDelay` function allows tasks to voluntarily suspend their execution for a specified duration in terms of system ticks. It sets up the task for the delay, initiates the delay process through a service call, and facilitates controlled task timing within the RTOS.

### 03. ArefRTOS Check Delayed Tasks

The `ArefRTOS_voidCheckDelayedTasks` function is a crucial part of the ArefRTOS real-time operating system (RTOS) responsible for monitoring and managing delayed tasks. It iterates through the list of tasks, checking their delay status and performing actions accordingly.

#### Function Explanation

1. **Task Iteration**: The function begins by iterating through the total number of tasks within the RTOS. It uses `LOC_u8Counter` as a loop counter.

2. **Delay Task Check**: For each task, it checks two conditions:
   - **Task Suspended**: It verifies if the task is in a "Suspended" state (`TS_Suspend`).
   - **Time Delay Flag**: It checks if the time delay flag (`TimeDelayFlag`) is enabled (equal to 1). This flag indicates that the task is currently in a delay state.

3. **Delay Countdown**: If both conditions from step 2 are met, the function decrements the counter representing the task's remaining delay time (`TaskDelayTime`). This counter tracks how many system ticks are left for the task to remain in the delay state.

4. **Delay Expiration Check**: If the `TaskDelayTime` reaches 0, it means that the delay duration has expired. The function proceeds with the following actions:
   - **Disable Waiting**: The `TimeDelayFlag` is set to "Disable," indicating that the task is no longer in a delay state.
   - **State Transition**: The task's state is changed to "Waiting" (`TS_Waiting`), signifying that it is ready for execution.
   - **Service Call**: A service call is made with the argument `ArefRTOS_SVC_CALL_DELAY_TASK` to handle the task's transition from a delay state to a waiting state.

5. **Error Handling**: The function does not explicitly manage error states, and thus, there is no error code (`LOC_ArefRTOS_ErrorID`) associated with it.

In summary, the `ArefRTOS_voidCheckDelayedTasks` function is responsible for checking and managing delayed tasks within the ArefRTOS RTOS. It iterates through the task list, monitors delay conditions, and ensures tasks transition from delay states to waiting states as their delay durations expire. This function plays a crucial role in task timing and execution control.

### 04. SysTick Handler

The `SysTick_Handler` function is an essential part of the ArefRTOS real-time operating system (RTOS) responsible for handling system tick interrupts. It manages tasks, task scheduling, and context switching, ensuring the orderly execution of tasks within the RTOS.

#### Function Explanation

1. **Update Delayed Tasks & Scheduler Table**: This step involves two important subtasks:
   - **Update Delayed Tasks**: The function calls `ArefRTOS_voidCheckDelayedTasks` to update delayed tasks. It checks and manages tasks that are currently in a delay state, potentially transitioning them to a waiting state as their delay durations expire.
   - **Update Scheduler Table**: This step may update the scheduler table if needed. However, the specific details of this operation are not provided in the code comments.

2. **Task Selection**: The function calls `ArefRTOS_voidSecondStageDispatcher` to select the next task to execute. This step plays a crucial role in determining the task execution order within the RTOS.

3. **Context Switching**: Finally, the function initiates a context switch by calling `SCB_voidTrigPendSV`. Context switching is a fundamental RTOS operation that allows different tasks to run in the same processor by saving and restoring their context (registers, stack, etc.). This step ensures that the selected task can execute seamlessly.

In summary, the `SysTick_Handler` function is the interrupt handler responsible for managing system ticks in the ArefRTOS RTOS. It updates delayed tasks, selects the next task for execution, and triggers context switching as needed to maintain the RTOS's task scheduling and execution flow.
