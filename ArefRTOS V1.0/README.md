# ArefRTOS- A Self-Made Real-Time Operating System V1.0 👨‍💻

## Features Included By Scheduler 🚀

- **01.ArefRTOS Void Init**
function is responsible for initializing the ArefRTOS operating system. It performs the following steps:

Initialize the hardware.
Start the OS with the suspend state.
Specify the main stack area for the OS.
Initialize the task queue.
Initialize the idle task.
Return an error code if any of the steps fail.

- **02.ArefRTOS_voidCreateTask**
function is responsible for creating a new task in the ArefRTOS operating system. It performs the following steps:

Set the start of the task stack using the hardware stack locator in the OS control structure.
Set the end of the task stack by subtracting the task stack size and the minimum stack size (due to the task control block) from the start of the task stack.
Check for stack overflow.
Align the hardware stack locator 4 bytes to ensure that it is aligned properly.
Initialize the task stack area.
Check the current number of tasks against the maximum number of tasks defined by the user.
Update the scheduler table and set the task state to suspended if the current number of tasks is less than the maximum number of tasks.
Return an error code indicating that the maximum number of tasks has been exceeded if the current number of tasks is equal to the maximum number of tasks.
Return an error code if any of the steps fail.






