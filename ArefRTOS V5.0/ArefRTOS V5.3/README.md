# ArefRTOS - A Self-Made Real-Time Operating System (Version 5.3) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 3.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS Acquire Binary Semaphore

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

---

### 02. ArefRTOS Release Binary Semaphore

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

---

### 03. ArefRTOS Get Binary Semaphore State

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
