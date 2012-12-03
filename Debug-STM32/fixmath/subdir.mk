################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../fixmath/fix16.c \
../fixmath/fix16_trig.c \
../fixmath/fract32.c \
../fixmath/uint32.c 

OBJS += \
./fixmath/fix16.o \
./fixmath/fix16_trig.o \
./fixmath/fract32.o \
./fixmath/uint32.o 

C_DEPS += \
./fixmath/fix16.d \
./fixmath/fix16_trig.d \
./fixmath/fract32.d \
./fixmath/uint32.d 


# Each subdirectory must supply rules for building sources it contributes
fixmath/%.o: ../fixmath/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux GCC C Compiler'
	arm-none-eabi-gcc -DINTERNAL_NEWLIB -DDEFINE_FREE -DDEFINE_MALLOC -DDEFINE_REALLOC -DDEFINE_CALLOC -DTC_STM32 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/xf" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/vcom" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


