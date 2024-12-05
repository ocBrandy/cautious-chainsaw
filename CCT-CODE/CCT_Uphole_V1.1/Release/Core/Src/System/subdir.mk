################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/System/stm32f1xx_hal_msp.c \
../Core/Src/System/stm32f1xx_it.c \
../Core/Src/System/syscalls.c \
../Core/Src/System/sysmem.c \
../Core/Src/System/system_stm32f1xx.c 

OBJS += \
./Core/Src/System/stm32f1xx_hal_msp.o \
./Core/Src/System/stm32f1xx_it.o \
./Core/Src/System/syscalls.o \
./Core/Src/System/sysmem.o \
./Core/Src/System/system_stm32f1xx.o 

C_DEPS += \
./Core/Src/System/stm32f1xx_hal_msp.d \
./Core/Src/System/stm32f1xx_it.d \
./Core/Src/System/syscalls.d \
./Core/Src/System/sysmem.d \
./Core/Src/System/system_stm32f1xx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/System/%.o Core/Src/System/%.su Core/Src/System/%.cyclo: ../Core/Src/System/%.c Core/Src/System/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-System

clean-Core-2f-Src-2f-System:
	-$(RM) ./Core/Src/System/stm32f1xx_hal_msp.cyclo ./Core/Src/System/stm32f1xx_hal_msp.d ./Core/Src/System/stm32f1xx_hal_msp.o ./Core/Src/System/stm32f1xx_hal_msp.su ./Core/Src/System/stm32f1xx_it.cyclo ./Core/Src/System/stm32f1xx_it.d ./Core/Src/System/stm32f1xx_it.o ./Core/Src/System/stm32f1xx_it.su ./Core/Src/System/syscalls.cyclo ./Core/Src/System/syscalls.d ./Core/Src/System/syscalls.o ./Core/Src/System/syscalls.su ./Core/Src/System/sysmem.cyclo ./Core/Src/System/sysmem.d ./Core/Src/System/sysmem.o ./Core/Src/System/sysmem.su ./Core/Src/System/system_stm32f1xx.cyclo ./Core/Src/System/system_stm32f1xx.d ./Core/Src/System/system_stm32f1xx.o ./Core/Src/System/system_stm32f1xx.su

.PHONY: clean-Core-2f-Src-2f-System

