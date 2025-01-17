################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SEGGER_RTT/Src/SEGGER_RTT.c \
../Drivers/SEGGER_RTT/Src/SEGGER_RTT_printf.c 

OBJS += \
./Drivers/SEGGER_RTT/Src/SEGGER_RTT.o \
./Drivers/SEGGER_RTT/Src/SEGGER_RTT_printf.o 

C_DEPS += \
./Drivers/SEGGER_RTT/Src/SEGGER_RTT.d \
./Drivers/SEGGER_RTT/Src/SEGGER_RTT_printf.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SEGGER_RTT/Src/%.o Drivers/SEGGER_RTT/Src/%.su Drivers/SEGGER_RTT/Src/%.cyclo: ../Drivers/SEGGER_RTT/Src/%.c Drivers/SEGGER_RTT/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/User -I../Core/Inc/System -I../Drivers/SEGGER_RTT/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Drivers-2f-SEGGER_RTT-2f-Src

clean-Drivers-2f-SEGGER_RTT-2f-Src:
	-$(RM) ./Drivers/SEGGER_RTT/Src/SEGGER_RTT.cyclo ./Drivers/SEGGER_RTT/Src/SEGGER_RTT.d ./Drivers/SEGGER_RTT/Src/SEGGER_RTT.o ./Drivers/SEGGER_RTT/Src/SEGGER_RTT.su ./Drivers/SEGGER_RTT/Src/SEGGER_RTT_printf.cyclo ./Drivers/SEGGER_RTT/Src/SEGGER_RTT_printf.d ./Drivers/SEGGER_RTT/Src/SEGGER_RTT_printf.o ./Drivers/SEGGER_RTT/Src/SEGGER_RTT_printf.su

.PHONY: clean-Drivers-2f-SEGGER_RTT-2f-Src

