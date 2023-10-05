/*
 * SCB_private.h
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

#ifndef INC_SCB_PRIVATE_H_
#define INC_SCB_PRIVATE_H_
/****************************************************
*													*
* 						MACROS						*
* 													*
*****************************************************/
#define SCB_BASE_ADDRESS	0xE000ED00
#define SCB_ICSR			*((volatile uint32_t *) (SCB_BASE_ADDRESS + 0x04))
#define SCB_ICSR_PendSV_BitMask 	28



#endif /* INC_SCB_PRIVATE_H_ */
