################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include xf/subdir.mk
-include utils/subdir.mk
-include target/stm32/subdir.mk
-include target/stm32/vcom/subdir.mk
-include target/stm32/syscalls/subdir.mk
-include target/stm32/stdperiph/src/subdir.mk
-include target/stm32/startup/subdir.mk
-include target/stm32/malloc/subdir.mk
-include target/stm32/hal/subdir.mk
-include target/stm32/critical/subdir.mk
-include target/stm32/cmsis/subdir.mk
-include phy/subdir.mk
-include fixmath/subdir.mk
-include board/olimex-stm32f103-stk/board/usb-vcom/subdir.mk
-include board/olimex-stm32f103-stk/board/transceiver/subdir.mk
-include board/olimex-stm32f103-stk/board/subdir.mk
-include board/olimex-stm32f103-stk/board/shared-spi/subdir.mk
-include board/olimex-stm32f103-stk/board/display/subdir.mk
-include air3t/ui/subdir.mk
-include air3t/data/subdir.mk
-include .metadata/.plugins/org.eclipse.cdt.make.core/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
desenet.hex \

SECONDARY_LIST += \
desenet.lst \

SECONDARY_SIZE += \
desenet.siz \


# All Target
all: desenet.elf secondary-outputs

# Tool invocations
desenet.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: ARM Linux GCC C++ Linker'
	arm-none-eabi-g++ -T"/home/desem/Workspace/desenet/target/stm32/stm32-flash.ld" -nostartfiles -Xlinker --gc-sections -Wl,-Map,desenet.map -mcpu=cortex-m3 -mthumb -g3 -gdwarf-2 -o"desenet.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

desenet.hex: desenet.elf
	@echo 'Invoking: ARM Linux GNU Create Flash Image'
	arm-none-eabi-objcopy -O ihex desenet.elf "desenet.hex"
	@echo 'Finished building: $@'
	@echo ' '

desenet.lst: desenet.elf
	@echo 'Invoking: ARM Linux GNU Create Listing'
	arm-none-eabi-objdump -h -S desenet.elf >"desenet.lst"
	@echo 'Finished building: $@'
	@echo ' '

desenet.siz: desenet.elf
	@echo 'Invoking: ARM Linux GNU Print Size'
	arm-none-eabi-size  --format=berkeley desenet.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(SECONDARY_FLASH)$(CXX_DEPS)$(S_UPPER_DEPS)$(SECONDARY_LIST)$(C++_DEPS)$(SECONDARY_SIZE)$(ASM_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(C_UPPER_DEPS) desenet.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
