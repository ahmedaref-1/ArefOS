# ArefRTOS - A Self-Made Real-Time Operating System (Version 5.3) 👨‍💻

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

---

### 04. ArefRTOS Send Item to Queue

The `ArefRTOS_voidSendItemToQueue` function is a critical part of the ArefRTOS real-time operating system (RTOS) used for adding items to a queue. Queues are essential for inter-task communication and synchronization.

#### Function Explanation

1. **Initialize Local Error Code**: The function begins by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Queue Size Check**: It checks if the waiting messages counter in the queue is less than or equal to the maximum queue size (`MESSAGE_QUEUE_MAX_SIZE`).

3. **Queue Preparation**: If there is space available in the queue:
   - It increments the waiting messages counter to prepare for adding a new item to the queue.
   - The queue is assumed to start from index 1, so the counter is used as the index.

4. **Data Addition**: It adds the provided data item (`copy_u32ToData`) to the queue at the current waiting messages counter index.
   - The queue array is assumed to be named `Global_MessagesQueueArray`.

5. **Error Handling**: If the waiting messages counter exceeds the maximum queue size, it sets the local error code to `INVALID_OPERATION`.

6. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError` if the item was successfully added to the queue or `INVALID_OPERATION` if the queue size limit was exceeded.

### 05. ArefRTOS Receive Item from Queue

The `ArefRTOS_RecieveItemFromQueue` function is responsible for retrieving items from a queue within the ArefRTOS real-time operating system (RTOS). Queues are crucial for inter-task communication and synchronization.

#### Function Explanation

1. **Initialize Local Error Code**: The function starts by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Queue Empty Check**: It checks if there are items in the queue to receive. The presence of items is determined by the `WaitingMessagesCounter`.

3. **Queue Item Retrieval**:
   - If there are items in the queue:
     - It increments the index for the next item to be popped from the queue.
     - If the index exceeds the maximum queue size, it wraps around to the beginning.
     - It retrieves the item from the queue and stores it in the location pointed to by `pToData`.
     - It decrements the counter to indicate one less item in the queue.

4. **Error Handling**:
   - If there are no items in the queue, it sets the local error code to `INVALID_OPERATION`.

5. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError` if an item was successfully received from the queue or `INVALID_OPERATION` if the queue was empty.

### 06. ArefRTOS Reset Queue

The `ArefRTOS_ResetQueue` function is used to reset a queue within the ArefRTOS real-time operating system (RTOS), preparing it for new data. Queues are essential for inter-task communication and synchronization.

#### Function Explanation

1. **Initialize Local Error Code**: The function begins by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Queue Reset**: It clears the waiting messages counter in the global message queue synchronization information, effectively emptying the queue.

3. **Index Reset**: It resets the next popped item index in the global message queue synchronization information to 0.

4. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError`, indicating that the queue has been successfully reset.

In summary, these three functions, `ArefRTOS_voidSendItemToQueue`, `ArefRTOS_RecieveItemFromQueue`, and `ArefRTOS_ResetQueue`, are fundamental for queue management and inter-task communication in the ArefRTOS system. They provide robust error handling to enhance system reliability.

---

### 07. ArefRTOS Acquire Binary Semaphore

The `ArefRTOS_AcquireBinarySemaphore` function is a fundamental component of the ArefRTOS real-time operating system (RTOS) responsible for acquiring binary semaphores. Binary semaphores are synchronization primitives that can have two states: acquired or not acquired.

#### Function Explanation

1. **Initialize Local Error Code**: The function begins by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Semaphore State Check**:
   - It checks if the `WaitingMessagesCounter` in the binary semaphore's private data is 0.

3. **Semaphore Acquisition**:
   - If the `WaitingMessagesCounter` is 0, it increments the `WaitingMessagesCounter` to 1, indicating that the semaphore has been acquired.

4. **Invalid Operation Handling**:
   - If the `WaitingMessagesCounter` is already 1, it sets the local error code to `INVALID_OPERATION` since the semaphore is already acquired.

5. **Error Handling**:
   - If neither of the above conditions is met (an unexpected situation), it sets the local error code to `INVALID_OPERATION` as a fallback.

6. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError` if the semaphore was successfully acquired, or `INVALID_OPERATION` if the semaphore was already acquired or an unexpected condition occurred.

### 08. ArefRTOS Release Binary Semaphore

The `ArefRTOS_ReleaseBinarySemaphore` function is a critical part of the ArefRTOS real-time operating system (RTOS) responsible for releasing binary semaphores, allowing other tasks to acquire them.

#### Function Explanation

1. **Initialize Local Error Code**: The function begins by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Semaphore State Check**:
   - It checks if the `WaitingMessagesCounter` in the binary semaphore's private data is 0.

3. **Invalid Operation Handling**:
   - If the `WaitingMessagesCounter` is 0, it sets the local error code to `INVALID_OPERATION` since the semaphore is not acquired.

4. **Semaphore Release**:
   - If the `WaitingMessagesCounter` is 1 (indicating the semaphore is acquired), it decrements the `WaitingMessagesCounter` to 0, releasing the semaphore.

5. **Error Handling**:
   - If neither of the above conditions is met (an unexpected situation), it sets the local error code to `INVALID_OPERATION` as a fallback.

6. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError` if the semaphore was successfully released, or `INVALID_OPERATION` if the semaphore was not acquired or an unexpected condition occurred.

### 09. ArefRTOS Get Binary Semaphore State

The `ArefRTOS_GetBinarySemaphoreState` function is responsible for retrieving the current state of a binary semaphore, providing information about whether it is acquired or not.

#### Function Explanation

1. **Initialize Local Error Code**: The function begins by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Semaphore State Check**:
   - It checks if the `WaitingMessagesCounter` in the binary semaphore's private data is 0 or 1.

3. **Semaphore State Retrieval**:
   - If the `WaitingMessagesCounter` is 0 or 1, it sets the value of `ptr_u8Flag` (a pointer provided as an argument) to the counter's value, indicating the current state of the binary semaphore.

4. **Error Handling**:
   - If the `WaitingMessagesCounter` does not meet the expected values (an unexpected situation), it sets the local error code to `INVALID_OPERATION` as a fallback.

5. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError` if the semaphore state was retrieved successfully, or `INVALID_OPERATION` if an unexpected condition occurred.

In summary, these three functions are essential for managing binary semaphores within the ArefRTOS RTOS. They provide mechanisms for acquiring, releasing, and retrieving the state of binary semaphores while ensuring robust error handling for various scenarios.
