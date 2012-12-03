################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../target/stm32/builder.cpp \
../target/stm32/factory.cpp \
../target/stm32/isrs.cpp \
../target/stm32/vectors.cpp 

OBJS += \
./target/stm32/builder.o \
./target/stm32/factory.o \
./target/stm32/isrs.o \
./target/stm32/vectors.o 

CPP_DEPS += \
./target/stm32/builder.d \
./target/stm32/factory.d \
./target/stm32/isrs.d \
./target/stm32/vectors.d 


# Each subdirectory must supply rules for building sources it contributes
target/stm32/%.o: ../target/stm32/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux GCC C++ Compiler'
	arm-none-eabi-g++ -DTC_STM32 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/interfaces" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


