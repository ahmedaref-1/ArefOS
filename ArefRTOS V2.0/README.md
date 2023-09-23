# ArefRTOS - A Self-Made Real-Time Operating System (Version 2.0) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 2.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS SVC Handler

The `ArefRTOS SVC Handler` function is responsible for handling specific SVC calls related to task activation and termination in a real-time operating system. It involves a two-stage scheduling and dispatching process and potentially triggers a context switch using the PendSV exception.


1. **Extracting SVC Parameter**: The function begins by extracting a single byte (`SVCParameter`) from the stack frame. This byte is obtained by casting a pointer to `svc_args[6]` (which points to the R1 register) as a `uint8_t*` and then subtracting 2 from it.

2. **Switch Statement**: It enters a `switch` statement based on the value of `SVCParameter`.

3. **Case: ArefRTOS_SVC_CALL_ACTIVATE_TASK**:
    - 3.1. Calls `ArefRTOS_voidFirstStageScheduler()` to decide which task to execute next based on priority.
    - 3.2. Checks if the OS is in the running state. If it is, it proceeds with additional steps.
    - 3.3. Calls `ArefRTOS_voidSecondStageDispatcher()` to switch the CPU from one process to another, saving and restoring the state of each process.
    - 3.4. Triggers a PendSV (Pendable Supervisor Call) exception, which is often used in context switching routines in RTOS implementations.

4. **Case: ArefRTOS_SVC_CALL_TERMINATE_TASK**:
    - 4.1. Performs the same steps as in the `ArefRTOS_SVC_CALL_ACTIVATE_TASK` case.

5. **Default Case**: Handles unknown SVC calls by taking no action.







