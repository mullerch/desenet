################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../target/stm32/stdperiph/src/misc.c \
../target/stm32/stdperiph/src/stm32f10x_adc.c \
../target/stm32/stdperiph/src/stm32f10x_bkp.c \
../target/stm32/stdperiph/src/stm32f10x_can.c \
../target/stm32/stdperiph/src/stm32f10x_cec.c \
../target/stm32/stdperiph/src/stm32f10x_crc.c \
../target/stm32/stdperiph/src/stm32f10x_dac.c \
../target/stm32/stdperiph/src/stm32f10x_dbgmcu.c \
../target/stm32/stdperiph/src/stm32f10x_dma.c \
../target/stm32/stdperiph/src/stm32f10x_exti.c \
../target/stm32/stdperiph/src/stm32f10x_flash.c \
../target/stm32/stdperiph/src/stm32f10x_fsmc.c \
../target/stm32/stdperiph/src/stm32f10x_gpio.c \
../target/stm32/stdperiph/src/stm32f10x_i2c.c \
../target/stm32/stdperiph/src/stm32f10x_iwdg.c \
../target/stm32/stdperiph/src/stm32f10x_pwr.c \
../target/stm32/stdperiph/src/stm32f10x_rcc.c \
../target/stm32/stdperiph/src/stm32f10x_rtc.c \
../target/stm32/stdperiph/src/stm32f10x_sdio.c \
../target/stm32/stdperiph/src/stm32f10x_spi.c \
../target/stm32/stdperiph/src/stm32f10x_tim.c \
../target/stm32/stdperiph/src/stm32f10x_usart.c \
../target/stm32/stdperiph/src/stm32f10x_wwdg.c 

OBJS += \
./target/stm32/stdperiph/src/misc.o \
./target/stm32/stdperiph/src/stm32f10x_adc.o \
./target/stm32/stdperiph/src/stm32f10x_bkp.o \
./target/stm32/stdperiph/src/stm32f10x_can.o \
./target/stm32/stdperiph/src/stm32f10x_cec.o \
./target/stm32/stdperiph/src/stm32f10x_crc.o \
./target/stm32/stdperiph/src/stm32f10x_dac.o \
./target/stm32/stdperiph/src/stm32f10x_dbgmcu.o \
./target/stm32/stdperiph/src/stm32f10x_dma.o \
./target/stm32/stdperiph/src/stm32f10x_exti.o \
./target/stm32/stdperiph/src/stm32f10x_flash.o \
./target/stm32/stdperiph/src/stm32f10x_fsmc.o \
./target/stm32/stdperiph/src/stm32f10x_gpio.o \
./target/stm32/stdperiph/src/stm32f10x_i2c.o \
./target/stm32/stdperiph/src/stm32f10x_iwdg.o \
./target/stm32/stdperiph/src/stm32f10x_pwr.o \
./target/stm32/stdperiph/src/stm32f10x_rcc.o \
./target/stm32/stdperiph/src/stm32f10x_rtc.o \
./target/stm32/stdperiph/src/stm32f10x_sdio.o \
./target/stm32/stdperiph/src/stm32f10x_spi.o \
./target/stm32/stdperiph/src/stm32f10x_tim.o \
./target/stm32/stdperiph/src/stm32f10x_usart.o \
./target/stm32/stdperiph/src/stm32f10x_wwdg.o 

C_DEPS += \
./target/stm32/stdperiph/src/misc.d \
./target/stm32/stdperiph/src/stm32f10x_adc.d \
./target/stm32/stdperiph/src/stm32f10x_bkp.d \
./target/stm32/stdperiph/src/stm32f10x_can.d \
./target/stm32/stdperiph/src/stm32f10x_cec.d \
./target/stm32/stdperiph/src/stm32f10x_crc.d \
./target/stm32/stdperiph/src/stm32f10x_dac.d \
./target/stm32/stdperiph/src/stm32f10x_dbgmcu.d \
./target/stm32/stdperiph/src/stm32f10x_dma.d \
./target/stm32/stdperiph/src/stm32f10x_exti.d \
./target/stm32/stdperiph/src/stm32f10x_flash.d \
./target/stm32/stdperiph/src/stm32f10x_fsmc.d \
./target/stm32/stdperiph/src/stm32f10x_gpio.d \
./target/stm32/stdperiph/src/stm32f10x_i2c.d \
./target/stm32/stdperiph/src/stm32f10x_iwdg.d \
./target/stm32/stdperiph/src/stm32f10x_pwr.d \
./target/stm32/stdperiph/src/stm32f10x_rcc.d \
./target/stm32/stdperiph/src/stm32f10x_rtc.d \
./target/stm32/stdperiph/src/stm32f10x_sdio.d \
./target/stm32/stdperiph/src/stm32f10x_spi.d \
./target/stm32/stdperiph/src/stm32f10x_tim.d \
./target/stm32/stdperiph/src/stm32f10x_usart.d \
./target/stm32/stdperiph/src/stm32f10x_wwdg.d 


# Each subdirectory must supply rules for building sources it contributes
target/stm32/stdperiph/src/%.o: ../target/stm32/stdperiph/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Linux GCC C Compiler'
	arm-none-eabi-gcc -DINTERNAL_NEWLIB -DDEFINE_FREE -DDEFINE_MALLOC -DDEFINE_REALLOC -DDEFINE_CALLOC -DTC_STM32 -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -I"/home/desem/workspace/desenet/target/stm32/malloc" -I"/home/desem/workspace/desenet" -I"/home/desem/workspace/desenet/xf" -I"/home/desem/workspace/desenet/target/stm32" -I"/home/desem/workspace/desenet/target/stm32/cmsis" -I"/home/desem/workspace/desenet/target/stm32/critical" -I"/home/desem/workspace/desenet/target/stm32/vcom" -I"/home/desem/workspace/desenet/target/stm32/stdperiph/includes" -I"/home/desem/workspace/desenet/board/olimex-stm32f103-stk" -O0 -ffunction-sections -fdata-sections -Wall -Wa,-adhlns="$@.lst" -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


