# ArefRTOS - A Self-Made Real-Time Operating System (Version 4.0) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 3.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS Terminate Task

The `ArefRTOS_voidTerminateTask` function is a crucial component of the ArefRTOS real-time operating system (RTOS). It is responsible for terminating or suspending tasks within the system, effectively ending their execution.

#### Function Explanation

1. **Error Handling**: The function starts by initializing a local error variable, `LOC_ArefRTOS_ErrorID`, to manage error conditions.

2. **Check for NULL Argument**: It checks whether the input pointer `pTask` is not NULL.

3. **Task Termination**:
   - If `pTask` is not NULL:
     - `LOC_ArefRTOS_ErrorID` is set to `NoError`, indicating that no errors have occurred.
     - The task's state, accessed via `pTask->TaskPrivateStates.TaskState`, is updated to `TS_Suspend`. This transition places the task into a "Suspend" state, effectively suspending its execution.
     - A service call is made by invoking `ArefRTOS_voidCallService` with the argument `(uint8_t)ArefRTOS_SVC_CALL_TERMINATE_TASK`. This service call initiates the task termination process.

4. **Error Reporting**: If the input pointer `pTask` is NULL, the function sets `LOC_ArefRTOS_ErrorID` to `NULL_ARGs`, indicating an error due to an invalid argument.

5. **Error Status Return**: The function returns the value of `LOC_ArefRTOS_ErrorID`. This value serves as an error code, indicating either `NoError` if the task termination was successful or `NULL_ARGs` if the function encountered a NULL argument.

In summary, the `ArefRTOS_voidTerminateTask` function is a crucial part of the ArefRTOS RTOS, responsible for terminating or suspending tasks. It includes error handling, updates the task's state, and triggers the task termination process through a service call. The function's return value communicates the success or failure of the operation.
