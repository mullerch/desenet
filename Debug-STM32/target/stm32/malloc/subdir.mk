################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../target/stm32/malloc/malloc.c \
../target/stm32/malloc/mallocr.c \
../target/stm32/malloc/mlock.c \
../target/stm32/malloc/realloc.c 

OBJS += \
./target/stm32/malloc/malloc.o \
./target/stm32/malloc/mallocr.o \
./target/stm32/malloc/mlock.o \
./target/stm32/malloc/realloc.o 

C_DEPS += \
./target/stm32/malloc/malloc.d \
./target/stm32/malloc/mallocr.d \
./target/stm32/malloc/mlock.d \
./target/stm32/malloc/realloc.d 


# Each subdirectory must supply rules for building sources it contributes
target/stm32/malloc/%.o: ../target/stm32/malloc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux GCC C Compiler'
	arm-none-eabi-gcc -DINTERNAL_NEWLIB -DDEFINE_FREE -DDEFINE_MALLOC -DDEFINE_REALLOC -DDEFINE_CALLOC -DTC_STM32 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/xf" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/vcom" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


