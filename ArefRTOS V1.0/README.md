# ArefRTOS - A Self-Made Real-Time Operating System (Version 1.0) 👨‍💻

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





