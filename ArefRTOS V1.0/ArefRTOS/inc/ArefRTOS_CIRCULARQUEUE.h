/*
 * ArefRTOS_CIRCULARQUEUE.h
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

#ifndef INC_AREFRTOS_CIRCULARQUEUE_H_
#define INC_AREFRTOS_CIRCULARQUEUE_H_
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"ArefRTOS_PORTING.h"
#include"ArefRTOS_SCHEDULER.h"


/******************************************
 *                                        *
 *        CONFIGURATION MACROS            *
 * 										  *
 ******************************************/
/*
	@stackSize
		Description: refer to size of queue
		Limitation : Must be an Integer Number.
*/
#define queueSize	MAX_NUM_OF_TASKS
/*
	@workingDataType
		Description: refer to Queue element data type
		Limitation : Must be a standard type (int,floa,double and char) or a predefined data type.
*/
#define workingDataType ArefRTOS_Task*


/******************************************
 *                                        *
 *           TYPEDEFENITIONS              *
 * 										  *
 ******************************************/
typedef workingDataType queueEntry;

typedef struct queue
{
	int _rear ;
	int _front;
	int _counter;
	queueEntry _queueArr[queueSize];
}queue_t;

typedef enum {
	QUEUE_NO_ERROR,
	QUEUE_FULL,
	QUEUE_EMPTY,
	QUEUE_NULL_OPERATION
}QUEUE_STATE;


/******************************************
 *                                        *
 *          FUNCTION PROTOTYPES           *
 * 										  *
 ******************************************/
QUEUE_STATE queue_create(queue_t *ptq);
QUEUE_STATE queue_add(queueEntry dataEnqueue ,queue_t *ptq);
QUEUE_STATE queue_get(queueEntry *dataEnqueue ,queue_t *ptq);
uint8_t queue_size(queue_t *ptq);


#endif /* INC_AREFRTOS_CIRCULARQUEUE_H_ */
