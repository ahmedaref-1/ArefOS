# ArefRTOS - A Self-Made Real-Time Operating System (Version 5.1) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 3.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS Create Mutex

The `ArefRTOS_CreateMutex` function is an essential part of the ArefRTOS real-time operating system (RTOS), responsible for creating and initializing mutex objects. Mutexes are crucial for managing shared resources and preventing race conditions in multi-threaded applications.

#### Function Explanation

1. **Initialize Local Error Code**: The function starts by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Argument Validation**: It checks whether the `pMutex` pointer is not NULL, indicating that the function was called with a valid mutex pointer.

3. **Mutex Initialization**:
   - If `pMutex` is not NULL:
     - It sets the `WaitingMessagesCounter` in the mutex's private data structure to indicate that the mutex is available for use.
     - It resets the `PriorityInversionFlag` to indicate that no priority inversion has occurred.
     - The `InheritedTask` pointer is set to NULL, indicating that no task has inherited this mutex.

4. **Error Handling**:
   - If the `pMutex` pointer is NULL, it means the function was called with an invalid argument.
   - In this case, the local error code (`LOC_ArefRTOS_ErrorID`) is set to `NULL_ARGs` to indicate an invalid argument error.

5. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError` if the mutex was successfully created and initialized or `NULL_ARGs` if the input argument was NULL.

In summary, the `ArefRTOS_CreateMutex` function is responsible for creating and initializing mutex objects, which are essential for resource management and thread synchronization. It ensures proper error handling and reports invalid arguments, enhancing the robustness of the ArefRTOS system.

### 02. ArefRTOS Acquire Mutex

The `ArefRTOS_AquireMutex` function is a critical component of the ArefRTOS real-time operating system (RTOS), responsible for managing the acquisition of mutexes by tasks. Mutexes are essential for ensuring exclusive access to shared resources and preventing race conditions in multi-threaded applications.

#### Function Explanation

1. **Initialize Local Error Code**: The function begins by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Argument Validation**: It checks if both the `pMutex` pointer (representing the mutex) and the `pCurrentRunningTask` pointer (representing the current running task) are not NULL. This ensures that the function is called with valid arguments.

3. **Mutex Availability Check**:
   - If the `pMutex` is available (indicated by `WaitingMessagesCounter` equal to `Set`):
     - The current running task can acquire the mutex.
   
4. **Mutex Acquisition**:
   - The function sets the `MutexHolder` field of the mutex to the current running task, indicating ownership.
   - It marks the mutex as unavailable by setting `WaitingMessagesCounter` to `Reset`.

5. **Mutex Unavailability**:
   - If the mutex is not available (already held by another task):
   
6. **Priority Comparison**:
   - It compares the priority of the current running task with the priority of the mutex holder.
   
7. **Priority Inversion Handling**:
   - If the current running task's priority is higher than or equal to the mutex holder's priority:
     - It checks whether priority inversion has occurred before for this mutex.
     - If not, it assigns the inherited task (the task that its priority will be inherited ) to the current running task.
     - It swaps priorities between the current task (inherited task) and the mutex holder to handle priority inversion.
     - It calls a service function (`ArefRTOS_voidCallService`) to handle priority inversion.
   - If priority inversion has occurred before, it restores priorities before changing them, updates the new inherited task, and swaps priorities again.

8. **Error Handling**:
   - If either the `pMutex` pointer or the `pCurrentRunningTask` pointer is NULL (invalid argument), the local error code (`LOC_ArefRTOS_ErrorID`) is set to `NULL_ARGs` to indicate an invalid argument error.

9. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError` if the mutex acquisition was successful or `NULL_ARGs` if there was an invalid argument.

In summary, the `ArefRTOS_AquireMutex` function is responsible for managing the acquisition of mutexes, ensuring proper synchronization between tasks. It handles priority inversion scenarios and provides robust error handling to enhance the reliability of the ArefRTOS system.

### 03. ArefRTOS Release Mutex

The `ArefRTOS_ReleaseMutex` function is a vital component of the ArefRTOS real-time operating system (RTOS), responsible for releasing mutexes. Mutexes are used to ensure exclusive access to shared resources in multi-threaded applications.

#### Function Explanation

1. **Initialize Local Error Code**: The function starts by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Argument Validation**: It checks whether both the `pMutex` pointer (representing the mutex) and the `pCurrentRunningTask` pointer (representing the current running task) are not NULL. This ensures that the function is called with valid arguments.

3. **Mutex Availability Check**:
   - It checks if the mutex is currently unavailable (locked). This is determined by the `WaitingMessagesCounter` field of the mutex's private data structure.

4. **Mutex Ownership Check**:
   - If the mutex is locked, it checks if the current running task is the holder of the mutex (`pMutex->MutexHolder == pCurrentRunningTask`).

5. **Priority Inversion Handling**:
   - If the mutex is locked and the current running task is the mutex holder, it checks the priority inversion flag.
   - If no priority inversion has occurred (`pMutex->PriorityInversionFlag == Reset`), it sets the `WaitingMessagesCounter` to `Set` to indicate that the mutex is available for use.

6. **Priority Inversion Detected**:
   - If priority inversion has occurred, it handles the situation as follows:
     - Sets `WaitingMessagesCounter` to `Reset` to indicate that the mutex is unavailable.
     - Clears the priority inversion flag.
     - Swaps priorities to restore them.
     - Updates the mutex holder to the inherited task (higher priority task waiting for the mutex to release).
     - Calls a service function (`ArefRTOS_voidCallService`) to handle priority inversion.

7. **Invalid Operations**:
   - If the mutex is already available (unlocked), or if the current running task does not have permission to release the mutex, it sets the local error code (`LOC_ArefRTOS_ErrorID`) to `INVALID_OPERATION`.

8. **Error Handling**:
   - If either the `pMutex` pointer or the `pCurrentRunningTask` pointer is NULL (invalid argument), it sets the local error code to `NULL_ARGs` to indicate an invalid argument error.

9. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), reflecting the outcome of the mutex release operation. It can be `NoError` for a successful release, `INVALID_OPERATION` for an invalid operation, or `NULL_ARGs` for invalid arguments.

In summary, the `ArefRTOS_ReleaseMutex` function is responsible for releasing mutexes, ensuring proper synchronization between tasks, and handling priority inversion scenarios. It provides robust error handling to enhance the reliability of the ArefRTOS system.
