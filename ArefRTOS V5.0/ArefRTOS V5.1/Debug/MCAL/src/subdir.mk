################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../MCAL/src/NVIC_program.c \
../MCAL/src/PSRC_program.c \
../MCAL/src/RCC_program.c \
../MCAL/src/SCB_program.c \
../MCAL/src/STK_program.c 

OBJS += \
./MCAL/src/NVIC_program.o \
./MCAL/src/PSRC_program.o \
./MCAL/src/RCC_program.o \
./MCAL/src/SCB_program.o \
./MCAL/src/STK_program.o 

C_DEPS += \
./MCAL/src/NVIC_program.d \
./MCAL/src/PSRC_program.d \
./MCAL/src/RCC_program.d \
./MCAL/src/SCB_program.d \
./MCAL/src/STK_program.d 


# Each subdirectory must supply rules for building sources it contributes
MCAL/src/%.o MCAL/src/%.su MCAL/src/%.cyclo: ../MCAL/src/%.c MCAL/src/subdir.mk
	arm-none-eabi-gcc  -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -c -I../Inc -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/Utilities/inc" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/ArefRTOS/inc" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/CMSIS_Version5" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/MCAL/inc" -I"D:/Embedded Systems Labs Solution/STM32Cube IDE Projects/ArefRTOS/CMSIS_Version5" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-MCAL-2f-src

clean-MCAL-2f-src:
	-$(RM) ./MCAL/src/NVIC_program.cyclo ./MCAL/src/NVIC_program.d ./MCAL/src/NVIC_program.o ./MCAL/src/NVIC_program.su ./MCAL/src/PSRC_program.cyclo ./MCAL/src/PSRC_program.d ./MCAL/src/PSRC_program.o ./MCAL/src/PSRC_program.su ./MCAL/src/RCC_program.cyclo ./MCAL/src/RCC_program.d ./MCAL/src/RCC_program.o ./MCAL/src/RCC_program.su ./MCAL/src/SCB_program.cyclo ./MCAL/src/SCB_program.d ./MCAL/src/SCB_program.o ./MCAL/src/SCB_program.su ./MCAL/src/STK_program.cyclo ./MCAL/src/STK_program.d ./MCAL/src/STK_program.o ./MCAL/src/STK_program.su

.PHONY: clean-MCAL-2f-src

