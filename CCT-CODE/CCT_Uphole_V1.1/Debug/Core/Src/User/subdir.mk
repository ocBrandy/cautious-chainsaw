################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/User/DMA.c \
../Core/Src/User/Fpga.c \
../Core/Src/User/Gpio.c \
../Core/Src/User/Init.c \
../Core/Src/User/Passthrough.c \
../Core/Src/User/Tim.c \
../Core/Src/User/Usart.c \
../Core/Src/User/main.c 

OBJS += \
./Core/Src/User/DMA.o \
./Core/Src/User/Fpga.o \
./Core/Src/User/Gpio.o \
./Core/Src/User/Init.o \
./Core/Src/User/Passthrough.o \
./Core/Src/User/Tim.o \
./Core/Src/User/Usart.o \
./Core/Src/User/main.o 

C_DEPS += \
./Core/Src/User/DMA.d \
./Core/Src/User/Fpga.d \
./Core/Src/User/Gpio.d \
./Core/Src/User/Init.d \
./Core/Src/User/Passthrough.d \
./Core/Src/User/Tim.d \
./Core/Src/User/Usart.d \
./Core/Src/User/main.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/User/%.o Core/Src/User/%.su Core/Src/User/%.cyclo: ../Core/Src/User/%.c Core/Src/User/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xE -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../Core/Inc/User -I../Core/Inc/System -I../Drivers/SEGGER_RTT/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-User

clean-Core-2f-Src-2f-User:
	-$(RM) ./Core/Src/User/DMA.cyclo ./Core/Src/User/DMA.d ./Core/Src/User/DMA.o ./Core/Src/User/DMA.su ./Core/Src/User/Fpga.cyclo ./Core/Src/User/Fpga.d ./Core/Src/User/Fpga.o ./Core/Src/User/Fpga.su ./Core/Src/User/Gpio.cyclo ./Core/Src/User/Gpio.d ./Core/Src/User/Gpio.o ./Core/Src/User/Gpio.su ./Core/Src/User/Init.cyclo ./Core/Src/User/Init.d ./Core/Src/User/Init.o ./Core/Src/User/Init.su ./Core/Src/User/Passthrough.cyclo ./Core/Src/User/Passthrough.d ./Core/Src/User/Passthrough.o ./Core/Src/User/Passthrough.su ./Core/Src/User/Tim.cyclo ./Core/Src/User/Tim.d ./Core/Src/User/Tim.o ./Core/Src/User/Tim.su ./Core/Src/User/Usart.cyclo ./Core/Src/User/Usart.d ./Core/Src/User/Usart.o ./Core/Src/User/Usart.su ./Core/Src/User/main.cyclo ./Core/Src/User/main.d ./Core/Src/User/main.o ./Core/Src/User/main.su

.PHONY: clean-Core-2f-Src-2f-User

