################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../utils/fontlookup.cpp \
../utils/monochromedisplaypainter.cpp 

OBJS += \
./utils/fontlookup.o \
./utils/monochromedisplaypainter.o 

CPP_DEPS += \
./utils/fontlookup.d \
./utils/monochromedisplaypainter.d 


# Each subdirectory must supply rules for building sources it contributes
utils/%.o: ../utils/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTC_MESHSIM -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace" -I/usr/include/qt4 -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/interfaces" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


