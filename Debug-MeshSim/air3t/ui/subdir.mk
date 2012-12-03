################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../air3t/ui/gameview.cpp \
../air3t/ui/menuview.cpp \
../air3t/ui/playerview.cpp \
../air3t/ui/textview.cpp \
../air3t/ui/view.cpp 

OBJS += \
./air3t/ui/gameview.o \
./air3t/ui/menuview.o \
./air3t/ui/playerview.o \
./air3t/ui/textview.o \
./air3t/ui/view.o 

CPP_DEPS += \
./air3t/ui/gameview.d \
./air3t/ui/menuview.d \
./air3t/ui/playerview.d \
./air3t/ui/textview.d \
./air3t/ui/view.d 


# Each subdirectory must supply rules for building sources it contributes
air3t/ui/%.o: ../air3t/ui/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTC_MESHSIM -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace" -I/usr/include/qt4 -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/interfaces" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


