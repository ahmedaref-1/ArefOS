# ArefRTOS - A Self-Made Real-Time Operating System (Version 3.0) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 3.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS Activate Task

The `ArefRTOS_voidActivateTask` function is a crucial component of the ArefRTOS real-time operating system (RTOS). Its primary purpose is to activate tasks within the system, facilitating their execution.

#### Function Explanation

1. **Task Activation**: This function initiates the execution of a specified task by transitioning it from an idle or waiting state to an active state. This activation allows the task to perform its designated operations.

2. **Error Handling**: The function includes error-handling mechanisms to ensure the reliability of the system. It checks for a NULL argument (`pTask`) to prevent unintended activations. If a NULL argument is encountered, it sets an error code (`NULL_ARGs`) to indicate the error.

3. **Service Call**: Upon successful activation request, the function triggers a service call (`ArefRTOS_voidCallService`) with an argument specific to task activation (`(uint8_t)ArefRTOS_SVC_CALL_ACTIVATE_TASK`). This service call is essential for initiating the task's execution.

4. **Error Reporting**: If a NULL argument (`pTask`) is detected, the function sets an error code (`NULL_ARGs`). This error reporting mechanism helps users identify and address issues related to invalid inputs.

5. **Error Status Return**: The function returns an error status code to the caller. This code indicates whether the task activation process was successful (`NoError`) or if an error occurred due to a NULL argument (`NULL_ARGs`).

In summary, the `ArefRTOS_voidActivateTask` function serves as a vital component of the ArefRTOS RTOS, responsible for initiating task execution while incorporating error-handling mechanisms. It enhances system reliability by providing a structured approach to task activation.

### 02. ArefRTOS Start Scheduler

The `ArefRTOS_voidStartScheduler` function is a pivotal function within the ArefRTOS real-time operating system (RTOS). It is responsible for initializing and starting the task scheduler, which is essential for managing and executing tasks within the system.

#### Function Explanation

1. **Operating Mode Setup**: This function sets the operating mode of the RTOS to "Running." This state indicates that the RTOS is operational and ready to manage tasks.

2. **Idle Task Initialization**: It assigns the system's initial task as the idle task. The idle task is typically the lowest-priority task in the system, responsible for running when no higher-priority tasks are ready.

3. **Processor Stack Pointer (PSP) Setup**: The system's PSP is set to point to the current process's (idle process) PSP. This configuration prepares the system to execute tasks with the correct stack context.

4. **Idle Task Activation**: The function activates the idle task by calling `ArefRTOS_voidActivateTask` with the pointer to the idle task. This activation allows the idle task to begin execution.

5. **System Ticker Start**: It starts the system ticker, which is crucial for tracking time and managing time-based operations within the RTOS.

6. **Stack Pointer Switch**: The function switches the system's stack pointer (SP) to point to the PSP. This configuration ensures that task-related operations use the PSP for stack management.

7. **Access Level Adjustment**: The access level is switched to "Unprivileged Mode." This is a security measure to restrict certain privileged operations, enhancing system security.

8. **Task Entry Point**: Finally, the function calls the entry point of the task designated as the current task. This step initiates the execution of the first task in the system.

### 03. SysTick Handler

The `SysTick_Handler` function is an essential part of the ArefRTOS real-time operating system (RTOS). It is called periodically by the SysTick timer and is responsible for managing task execution and context switching within the system.

#### Function Explanation

1. **Task Selection**: This function initiates the task selection process by calling `ArefRTOS_voidSecondStageDispatcher()`. The dispatcher is responsible for determining which task should run next based on scheduling policies and task priorities.

2. **Context Switching**: After selecting the appropriate task, the `SysTick_Handler` triggers context switching by invoking `SCB_voidTrigPendSV()`. Context switching ensures a seamless transition between tasks and allows the selected task to begin execution.

In summary, the `SysTick_Handler` function plays a critical role in task scheduling and context switching within the ArefRTOS RTOS. It selects the next task to execute and triggers the necessary context switch to ensure smooth task execution.


In summary, the `ArefRTOS_voidStartScheduler` function is responsible for initializing and starting the task scheduler in the ArefRTOS RTOS. It sets up the system, activates the initial task (usually the idle task), and begins task execution.
