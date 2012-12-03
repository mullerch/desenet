################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../target/stm32/vcom/usb_core.c \
../target/stm32/vcom/usb_desc.c \
../target/stm32/vcom/usb_endp.c \
../target/stm32/vcom/usb_hw.c \
../target/stm32/vcom/usb_init.c \
../target/stm32/vcom/usb_int.c \
../target/stm32/vcom/usb_istr.c \
../target/stm32/vcom/usb_mem.c \
../target/stm32/vcom/usb_prop.c \
../target/stm32/vcom/usb_pwr.c \
../target/stm32/vcom/usb_regs.c 

OBJS += \
./target/stm32/vcom/usb_core.o \
./target/stm32/vcom/usb_desc.o \
./target/stm32/vcom/usb_endp.o \
./target/stm32/vcom/usb_hw.o \
./target/stm32/vcom/usb_init.o \
./target/stm32/vcom/usb_int.o \
./target/stm32/vcom/usb_istr.o \
./target/stm32/vcom/usb_mem.o \
./target/stm32/vcom/usb_prop.o \
./target/stm32/vcom/usb_pwr.o \
./target/stm32/vcom/usb_regs.o 

C_DEPS += \
./target/stm32/vcom/usb_core.d \
./target/stm32/vcom/usb_desc.d \
./target/stm32/vcom/usb_endp.d \
./target/stm32/vcom/usb_hw.d \
./target/stm32/vcom/usb_init.d \
./target/stm32/vcom/usb_int.d \
./target/stm32/vcom/usb_istr.d \
./target/stm32/vcom/usb_mem.d \
./target/stm32/vcom/usb_prop.d \
./target/stm32/vcom/usb_pwr.d \
./target/stm32/vcom/usb_regs.d 


# Each subdirectory must supply rules for building sources it contributes
target/stm32/vcom/%.o: ../target/stm32/vcom/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux GCC C Compiler'
	arm-none-eabi-gcc -DINTERNAL_NEWLIB -DDEFINE_FREE -DDEFINE_MALLOC -DDEFINE_REALLOC -DDEFINE_CALLOC -DTC_STM32 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/xf" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/vcom" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


