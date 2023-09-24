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

