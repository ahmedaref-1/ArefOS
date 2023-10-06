# ArefRTOS - A Self-Made Real-Time Operating System (Version 5.2) 👨‍💻

## Introduction 🌟

Welcome to ArefRTOS, a real-time operating system (RTOS) meticulously crafted from scratch. This README provides an overview of key features and functionalities within ArefRTOS Version 3.0.

## Features Included by the Scheduler 🚀

### 01. ArefRTOS Send Item to Queue

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

### 02. ArefRTOS Receive Item from Queue

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

### 03. ArefRTOS Reset Queue

The `ArefRTOS_ResetQueue` function is used to reset a queue within the ArefRTOS real-time operating system (RTOS), preparing it for new data. Queues are essential for inter-task communication and synchronization.

#### Function Explanation

1. **Initialize Local Error Code**: The function begins by initializing a local error code variable, `LOC_ArefRTOS_ErrorID`, to `NoError`.

2. **Queue Reset**: It clears the waiting messages counter in the global message queue synchronization information, effectively emptying the queue.

3. **Index Reset**: It resets the next popped item index in the global message queue synchronization information to 0.

4. **Error Status Return**: The function returns the error code (`LOC_ArefRTOS_ErrorID`), which will be `NoError`, indicating that the queue has been successfully reset.


In summary, these three functions, `ArefRTOS_voidSendItemToQueue`, `ArefRTOS_RecieveItemFromQueue`, and `ArefRTOS_ResetQueue`, are fundamental for queue management and inter-task communication in the ArefRTOS system. They provide robust error handling to enhance system reliability.
