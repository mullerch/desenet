################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../xf/initialevent.cpp \
../xf/xf.cpp \
../xf/xfevent.cpp \
../xf/xfnulltransition.cpp \
../xf/xfreactive.cpp \
../xf/xfstaticevent.cpp \
../xf/xfthread.cpp \
../xf/xftimeout.cpp \
../xf/xftimeoutmanager.cpp 

OBJS += \
./xf/initialevent.o \
./xf/xf.o \
./xf/xfevent.o \
./xf/xfnulltransition.o \
./xf/xfreactive.o \
./xf/xfstaticevent.o \
./xf/xfthread.o \
./xf/xftimeout.o \
./xf/xftimeoutmanager.o 

CPP_DEPS += \
./xf/initialevent.d \
./xf/xf.d \
./xf/xfevent.d \
./xf/xfnulltransition.d \
./xf/xfreactive.d \
./xf/xfstaticevent.d \
./xf/xfthread.d \
./xf/xftimeout.d \
./xf/xftimeoutmanager.d 


# Each subdirectory must supply rules for building sources it contributes
xf/%.o: ../xf/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -DTC_MESHSIM -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace" -I/usr/include/qt4 -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/interfaces" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


