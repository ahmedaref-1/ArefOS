# ArefRTOS - A Self-Made Real-Time Operating System (Version 2.0) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 2.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS SVC Handler

The `ArefRTOS SVC Handler` function is responsible for handling specific SVC calls related to task activation and termination in a real-time operating system. It involves a two-stage scheduling and dispatching process and potentially triggers a context switch using the PendSV exception.

#### Function Explanation

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

### 02. ArefRTOS First Stage Scheduler

This function, `ArefRTOS_voidFirstStageScheduler()`, is a crucial part of the task scheduling process in the ArefRTOS real-time operating system (RTOS). It ensures that tasks are placed in the ready queue in the correct order based on their priorities, enabling efficient task execution in the RTOS.

#### Function Explanation

1. **Create Local Variables**: Initializes temporary local variables for task management.

2. **Sort Tasks**: Calls `ArefRTOS_voidBubbleSort()` to sort tasks in the task table based on priority.

3. **Select Current and Next Task**:
   - 3.1. **Free Ready Queue**: Clears the ready queue by repeatedly dequeuing tasks until it's empty.
   - 3.2. **Update Ready Queue**: Updates the ready queue based on task priorities:

     - 3.2.1. **Check Priority Options**:
       - 3.2.1.1. **End of Task Table**: If at the end of the task table, enqueues the current task, sets its state to ready, and exits.
       - 3.2.1.2. **Lower Priority Task**: If the current task has a lower priority than the next task, enqueues the current task, sets its state to ready, and exits.
       - 3.2.1.3. **Same Priority Tasks (Round-Robin)**: If the current task has the same priority as the next task, enqueues the current task for round-robin scheduling and continues without breaking.
       - 3.2.1.4. **Higher Priority Task**: If the current task has a higher priority than the next task, this is not allowed, and the loop exits.

### 03. ArefRTOS Second Stage Dispatcher

The `ArefRTOS_voidSecondStageDispatcher` function plays a crucial role in the task scheduling process within the ArefRTOS real-time operating system (RTOS). It manages the selection of the current and next tasks to be executed, ensuring smooth task execution and handling scenarios when there are no ready tasks.

#### Function Explanation

1. **Create Local Variables**: The function initializes temporary local variables to facilitate task management.
   
3. **Select Current and Next Task**:
   - 3.1. **Empty Queue and Non-Suspended Current Task**:
     - If the ready task queue is empty and the current task is not suspended:
       - The current task is set to a running state.
       - The current task is added back to the end of the ready task queue.
       - The next task is set to be the same as the current task.

   - 3.2. **Ready Queue is Not Empty**:
     - If the ready task queue is not empty:
       - The first task in the FIFO, `LOC_currentTask`, is dequeued and assigned.
       - The next task is set to `LOC_currentTask`.
       - `LOC_currentTask` is marked as running.
       - If the current task and the next task have the same priority, and the current task is not suspended, round-robin scheduling is applied:
         - The current task is added back to the end of the ready task queue.
         - The current task is marked as ready.

This function is pivotal in the proper execution of tasks within the ArefRTOS RTOS, ensuring tasks are scheduled and executed based on their priority and handling scenarios when no ready tasks are available.
