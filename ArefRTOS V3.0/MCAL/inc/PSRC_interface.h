/*
 * PSRC_interface.h
 *
 *  Created on: Sep 16, 2023
 *      Author: Ahmed Aref Omaira
 */

#ifndef INC_PSRC_INTERFACE_H_
#define INC_PSRC_INTERFACE_H_
/***********************************************************
* 														   *
*					  INCLUDES		                       *
*														   *
************************************************************/
#include"PLATFORMTYPES.h"



/***********************************************************
* 														   *
*			    PRE-PROCESSOR CONSTANTS	                   *
*														   *
************************************************************/
#define FORCE_INLINE __attribute__((always_inline)) inline
#define FORCE_NAKED __attribute__((naked))




/***********************************************************
* 														   *
*			    CONFIGURATION CONSTANTS	                   *
*														   *
************************************************************/



/***********************************************************
* 														   *
*			        TYPE-DEFINITIONS	                   *
*														   *
************************************************************/



/***********************************************************
* 														   *
*			        	VARIABLES	                       *
*														   *
************************************************************/



/***********************************************************
* 														   *
*			       FUNCTIONS PROTOTYPES                    *
*														   *
************************************************************/
/*
 * @brief   This function is used to switch access level to privileged.
 * @details in case of current mode is unprivileged access so we can change the access level only in handler mode(in IRQ).
 *          but if tried to switch access level while i am in the unprivileged level without an exception system call
 *          it will make a hard-fault exception.
 * @note    This Function is naked function just push LR only and Pop it in PC at the end of function.
 * @note    Can Change it in any ISR.
 * @note    to change it Need handler mode or Thread Mode with privileged access.
 * @param   void
 * @return  void
 */
FORCE_NAKED void PSRC_voidSetAccessLevel_PRIVILEGED(void);

/*
 * @brief   This function is used to switch access level to unprivileged.
 * @details in case of current mode is privileged access so we can change the access level.
 * @note    This Function is naked function just push LR only and Pop it in PC at the end of function.
 * @note    Can Change it in any ISR or in normal execution in kernel program.
 * @note    to change it Need handler mode or Thread Mode with privileged access.
 * @param   void
 * @return  void
 */
//FORCE_NAKED void PSRC_voidSetAccessLevel_UNPRIVILEGED(void);
#define PSRC_voidSetAccessLevel_UNPRIVILEGED()		__asm(" mrs r3, CONTROL  \n\t" \
											  " orr r3,r3,#0x1   \n\t" \
											  " msr CONTROL, r3 ");
/**
 * @brief  This function is used to switch to use process stack pointer as SP
 * @note   This Function is inline function so will take place in the caller line .
 * @note   Need Privileged Access Level.
 * @param  void
 * @return void
 */
FORCE_INLINE void PSRC_voidSetWorkingStack_PSP(void){
	__asm("MRS r0,CONTROL \n\t"
		  "MOV r1,#0x2    \n\t"
		  "ORR r0,r0,r1   \n\t"
		  "MSR CONTROL,r0");
}

/**
 * @brief  This function is used to switch to use main stack pointer as SP
 * @note   This Function is inline function so will take place in the caller line .
 * @note   Need Privileged Access Level.
 * @param  void
 * @return void
 */
FORCE_INLINE void PSRC_voidSetWorkingStack_MSP(void){
	__asm("MRS r0,CONTROL \n\t"
		  "MOV r1,#0x5    \n\t"
		  "AND r0,r0,r1   \n\t"
		  "MSR CONTROL,r0");
}

/**
 * @brief  This function is used to set the top of process stack pointer
 * @note   This Function is inline function so will take place in the caller line .
 * @note   Need Privileged  Access Level.
 * @param  copy_u32topOfPSP: the address of the top of process stack
 * @return void
 */
FORCE_INLINE void PSRC_voidSetPSP(uint32_t copy_u32topOfPSP){
	__asm("MOV r0,%[input0] \n\t"
		  "MSR PSP,r0"
		  :
		  :[input0]"r"(copy_u32topOfPSP));
}

/**
 * @brief  This function is used to get the current top of process stack pointer
 * @note   This Function is inline function so will take place in the caller line .
 * @note   Need Privileged Access Level.
 * @param  void
 * @return U32 --> Current Process Stack Pointer address
 */
FORCE_INLINE uint32_t PSRC_voidGetPSP(void){
	uint32_t Loc_u32TopOfStack =  0 ;
	__asm("MRS r0,PSP \n\t"
		  "MOV %[output0],r0"
		  :[output0]"=r"(Loc_u32TopOfStack));
	return (Loc_u32TopOfStack);
}

/**
 * @brief  This function is used to set the top of Main stack pointer
 * @note   This Function is inline function so will take place in the caller line .
 * @note   Need Privileged Access Level.
 * @param  copy_u32topOfPSP: the address of the top of the main stack
 * @return void
 */
FORCE_INLINE void PSRC_voidSetMSP(uint32_t copy_u32topOfMSP){
	__asm("MOV r0,%[input0] \n\t"
		  "MSR MSP,r0"
		  :
		  :[input0]"r"(copy_u32topOfMSP));
}

/**
 * @brief This function is used to get the current top of main stack pointer
 * @note  This Function is inline function so will take place in the caller line .
 * @note  Need Privileged Access Level.
 * @param void
 * @return U32 --> Current Main Stack Pointer address
 */
FORCE_INLINE uint32_t PSRC_voidGetMSP(void){
	uint32_t Loc_u32TopOfStack =  0 ;
	__asm("MRS r0,MSP \n\t"
		  "MOV %[output0],r0"
		  :[output0]"=r"(Loc_u32TopOfStack));
	return (Loc_u32TopOfStack);
}


#endif /* INC_PSRC_INTERFACE_H_ */
