################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../air3t/data/gamefield.cpp \
../air3t/data/model.cpp \
../air3t/data/player.cpp \
../air3t/data/statistics.cpp 

OBJS += \
./air3t/data/gamefield.o \
./air3t/data/model.o \
./air3t/data/player.o \
./air3t/data/statistics.o 

CPP_DEPS += \
./air3t/data/gamefield.d \
./air3t/data/model.d \
./air3t/data/player.d \
./air3t/data/statistics.d 


# Each subdirectory must supply rules for building sources it contributes
air3t/data/%.o: ../air3t/data/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux GCC C++ Compiler'
	arm-none-eabi-g++ -DTC_STM32 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/interfaces" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -fno-exceptions -fno-rtti -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


