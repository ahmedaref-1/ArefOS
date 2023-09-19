# ArefRTOS- A Self-Made Real-Time Operating System V1.0 👨‍💻

## Features Included By Scheduler 🚀

- **01.ArefRTOS Void Init**:  
  function is responsible for initializing the ArefRTOS operating system. It performs the following steps:  
  01. Initialize the hardware. This may involve setting up clock speeds, memory management, and peripherals.  
  02. Start the OS with the suspend state. This means that the OS will not start running tasks until it is explicitly resumed.  
  03. Specify the main stack area for the OS. The main stack is used by the OS to store the call stack for the main task.  
  04. Initialize the task queue. The task queue is used to store tasks that are ready to run.  
  05. Initialize the idle task. The idle task is a special task that runs when there are no other tasks ready to run.  
  06. The ArefRTOS_voidInit function returns an error code if any of the steps fail.

- **02.ArefRTOS_voidCreateTask**:  
  function is responsible for creating a new task in the ArefRTOS operating system. It performs the following steps:  
  01. Sets the start of the task stack using the hardware stack locator in the OS control structure.  
  02. Sets the end of the task stack by subtracting the task stack size and the minimum stack size (due to the task control block) from the start of the task stack.  
  03. Checks for stack overflow.  
  04. Aligns the hardware stack locator 4 bytes to ensure that it is aligned properly.  
  05. Initializes the task stack area.  
  06. Checks the current number of tasks against the maximum number of tasks defined by the user.  
  07.1. If the current number of tasks is less than the maximum number of tasks, then the function updates the scheduler table and sets the task state to suspended.  
  07.2. If the current number of tasks is equal to the maximum number of tasks, then the function returns an error code indicating that the maximum number of tasks 
        has been exceeded.  
  08. The ArefRTOS_voidCreateTask function returns an error code if any of the steps fail.  
