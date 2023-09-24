# ArefRTOS - A Self-Made Real-Time Operating System (Version 1.0 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 1.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS Void Init

The `ArefRTOS_voidInit` function is responsible for initializing the ArefRTOS operating system. It performs the following crucial steps:

1. **Hardware Initialization**: This step involves configuring essential hardware settings such as clock speeds, memory management, and peripheral setups.

2. **Suspend State**: ArefRTOS begins in a suspended state, ensuring that tasks won't start executing until explicitly resumed.

3. **Main Stack Setup**: The OS reserves a specific memory area for the main stack, which is utilized to manage the call stack for the main task.

4. **Task Queue Initialization**: ArefRTOS initializes the task queue, which is vital for managing tasks that are ready to execute.

5. **Idle Task Initialization**: An idle task is a specialized task that runs when there are no other tasks ready to execute.

6. **Error Handling**: The `ArefRTOS_voidInit` function is designed to return an error code in case any of the initialization steps encounter issues.

### 02. ArefRTOS Void Create Task

The `ArefRTOS_voidCreateTask` function is responsible for creating new tasks within the ArefRTOS operating system. This process involves several critical steps:

1. **Task Stack Configuration**: It sets the task stack's starting point using the hardware stack locator present in the OS control structure.

2. **Task Stack Sizing**: The function calculates the end of the task stack by subtracting the task stack size and the minimum stack size required for the task control block from the starting point.

3. **Stack Overflow Check**: ArefRTOS checks for stack overflow to ensure the task stack remains within allocated bounds.

4. **Stack Alignment**: The hardware stack locator is aligned to 4 bytes to ensure proper alignment.

5. **Task Stack Initialization**: The task stack area is initialized, preparing it for task execution.

6. **Task Count Verification**: ArefRTOS checks the current number of tasks against the user-defined maximum number of tasks.

7. **Task State Management**: If the current number of tasks is less than the maximum limit, the function updates the scheduler table and sets the task state to suspended.

8. **Error Handling**: In case the current number of tasks reaches the maximum limit, the function returns an error code, indicating that the maximum number of tasks has been exceeded.

9. **Error Reporting**: The `ArefRTOS_voidCreateTask` function is designed to return an error code if any of the task creation steps encounter issues.

These functions and features form the foundation of ArefRTOS, empowering you to build real-time applications with precision and control.



