################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../target/linux-meshsim/meshsimboard.cpp \
../target/linux-meshsim/meshsimbuilder.cpp \
../target/linux-meshsim/meshsimfactory.cpp \
../target/linux-meshsim/meshsimtransceiver.cpp \
../target/linux-meshsim/trace.cpp 

OBJS += \
./target/linux-meshsim/meshsimboard.o \
./target/linux-meshsim/meshsimbuilder.o \
./target/linux-meshsim/meshsimfactory.o \
./target/linux-meshsim/meshsimtransceiver.o \
./target/linux-meshsim/trace.o 

CPP_DEPS += \
./target/linux-meshsim/meshsimboard.d \
./target/linux-meshsim/meshsimbuilder.d \
./target/linux-meshsim/meshsimfactory.d \
./target/linux-meshsim/meshsimtransceiver.d \
./target/linux-meshsim/trace.d 


# Each subdirectory must supply rules for building sources it contributes
target/linux-meshsim/%.o: ../target/linux-meshsim/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTC_MESHSIM -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace" -I/usr/include/qt4 -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/interfaces" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


