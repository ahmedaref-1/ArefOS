################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ArefRTOS/src/ArefRTOS_CIRCULARQUEUE.c \
../ArefRTOS/src/ArefRTOS_PORTING.c \
../ArefRTOS/src/ArefRTOS_SCHEDULER.c 

OBJS += \
./ArefRTOS/src/ArefRTOS_CIRCULARQUEUE.o \
./ArefRTOS/src/ArefRTOS_PORTING.o \
./ArefRTOS/src/ArefRTOS_SCHEDULER.o 

C_DEPS += \
./ArefRTOS/src/ArefRTOS_CIRCULARQUEUE.d \
./ArefRTOS/src/ArefRTOS_PORTING.d \
./ArefRTOS/src/ArefRTOS_SCHEDULER.d 


# Each subdirectory must supply rules for building sources it contributes
ArefRTOS/src/%.o ArefRTOS/src/%.su ArefRTOS/src/%.cyclo: ../ArefRTOS/src/%.c ArefRTOS/src/subdir.mk
	arm-none-eabi-gcc  -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I../Inc -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/Utilities/inc" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/ArefRTOS/inc" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/CMSIS_Version5" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/MCAL/inc" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/CMSIS_Version5" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ArefRTOS-2f-src

clean-ArefRTOS-2f-src:
	-$(RM) ./ArefRTOS/src/ArefRTOS_CIRCULARQUEUE.cyclo ./ArefRTOS/src/ArefRTOS_CIRCULARQUEUE.d ./ArefRTOS/src/ArefRTOS_CIRCULARQUEUE.o ./ArefRTOS/src/ArefRTOS_CIRCULARQUEUE.su ./ArefRTOS/src/ArefRTOS_PORTING.cyclo ./ArefRTOS/src/ArefRTOS_PORTING.d ./ArefRTOS/src/ArefRTOS_PORTING.o ./ArefRTOS/src/ArefRTOS_PORTING.su ./ArefRTOS/src/ArefRTOS_SCHEDULER.cyclo ./ArefRTOS/src/ArefRTOS_SCHEDULER.d ./ArefRTOS/src/ArefRTOS_SCHEDULER.o ./ArefRTOS/src/ArefRTOS_SCHEDULER.su

.PHONY: clean-ArefRTOS-2f-src

