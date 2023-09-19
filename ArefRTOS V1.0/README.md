# ArefRTOS- A Self-Made Real-Time Operating System V1.0  👨‍💻

## Features Included By Scheduler 🚀

- **ArefRTOS Void Init**: 
function is responsible for initializing the ArefRTOS operating system. It performs the following steps:
01.Initialize the hardware. This may involve setting up clock speeds, memory management, and peripherals.
02.Start the OS with the suspend state. This means that the OS will not start running tasks until it is explicitly resumed.
03.Specify the main stack area for the OS. The main stack is used by the OS to store the call stack for the main task.
04.Initialize the task queue. The task queue is used to store tasks that are ready to run.
05.Initialize the idle task. The idle task is a special task that runs when there are no other tasks ready to run.
06.The ArefRTOS_voidInit() function returns an error code if any of the steps fail.








