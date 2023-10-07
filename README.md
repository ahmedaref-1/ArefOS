# ArefOS - A Self-Made Real-Time Operating System 👨‍💻

Welcome to the ArefOS project! This repository is your gateway to mastering Real-Time Operating Systems (RTOS) through a self-made RTOS.

## Project Overview

ArefOS is a project designed to help you excel in your RTOS course. It provides you with a hands-on experience in building and understanding a Real-Time Operating System. Whether you're a student looking to ace your RTOS course or an enthusiast eager to dive into the world of embedded systems, this project is here to support your learning journey.However, ArefRTOS is implemented for educational and learning purposes so it may exist some faults I didn't handle so you can fork the project and try to add your modification.

## ArefRTOS Features 🚀
#  Version 5.0

1. ***Basic RTOS Operations***: ArefRTOS supports the fundamental operations required by any real-time operating system, ensuring efficient task management and resource allocation.

2. **Priority-Based Scheduler**: Our scheduler follows a priority-based approach, ensuring that the task with the highest priority is executed first. This ensures that critical tasks are prioritized for execution.

3. **Round-Robin Scheduling**: In scenarios where two or more tasks have the same highest priority, ArefRTOS employs a Round-Robin scheduling algorithm to fairly distribute CPU time among these tasks.

4. **IDLE Task Optimization**: The IDLE task in ArefRTOS has been optimized to enter a low-power sleep mode and await an event when no active tasks are pending, conserving energy and enhancing system responsiveness.

5. **Queue APIs**: We've implemented a set of Queue APIs that enable seamless data sharing between tasks. These queues facilitate inter-task communication and synchronization.

6. **Binary Semaphores**: ArefRTOS supports Binary Semaphores as a signaling mechanism. These semaphores help coordinate access to resources and ensure mutual exclusion when needed.

7. **Counting Semaphores**: For scenarios where multiple tasks need to access a shared resource, Counting Semaphores are available as a signaling mechanism. These semaphores allow you to control access based on the availability of resources.

8. **Mutex (Mutual Exclusion)**: ArefRTOS includes support for Mutexes, which serve as a locking mechanism to protect critical sections of code. This ensures that only one task can access a shared resource at a time.

With these features, ArefRTOS version 5.x is well-equipped to handle a wide range of real-time applications, ensuring efficient task management, resource utilization, and responsiveness in your embedded systems.


## Getting Started 🛠️

1. **Clone the Repository**: Start by cloning this repository to your local machine.

2. **Explore the Code**: Dive into the ArefOS source code to see how it's implemented. Use the provided examples to understand different aspects of RTOS.

3. **Documentation**: Check out the documentation folder for explanations of key concepts and usage instructions.

4. **Experiment**: Modify and extend ArefOS to experiment and reinforce your understanding of RTOS principles.

## Contribution 🤝

We welcome contributions from the community to make ArefOS even more valuable for RTOS learners. If you have ideas, bug fixes, or improvements, please feel free to open an issue or submit a pull request.

Happy learning, and may your journey to mastering Real-Time Operating Systems with ArefOS be both educational and rewarding!👩‍🚀
