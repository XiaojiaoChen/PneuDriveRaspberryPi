################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Src/framework/frameSrc/subdir.mk
-include Src/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
-include Middlewares/Third_Party/FreeRTOS/Source/subdir.mk
-include Inc/subdir.mk
-include Drivers/STM32F7xx_HAL_Driver/Src/subdir.mk
-include Drivers/CMSIS/DSP/Source/TransformFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/SupportFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/StatisticsFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/MatrixFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/FilteringFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/FastMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/ControllerFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/ComplexMathFunctions/subdir.mk
-include Drivers/CMSIS/DSP/Source/CommonTables/subdir.mk
-include Drivers/CMSIS/DSP/Source/BasicMathFunctions/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

-include ../makefile.defs

BUILD_ARTIFACT_NAME := PneuDriveRaspberryPi
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX := 
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME).$(BUILD_ARTIFACT_EXTENSION)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
PneuDriveRaspberryPi.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
PneuDriveRaspberryPi.list \

OBJCOPY_BIN += \
PneuDriveRaspberryPi.bin \


# All Target
all: main-build

# Main-build Target
main-build: PneuDriveRaspberryPi.elf secondary-outputs

# Tool invocations
PneuDriveRaspberryPi.elf: $(OBJS) $(USER_OBJS) C:\Users\40207\OneDrive\OneDriveDocumentation\STM32CubeIDEWorkSpaceOnedrive\PneuDriveRaspberryPi\STM32F767ZITX_FLASH.ld
	arm-none-eabi-g++ -o "PneuDriveRaspberryPi.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"C:\Users\40207\OneDrive\OneDriveDocumentation\STM32CubeIDEWorkSpaceOnedrive\PneuDriveRaspberryPi\STM32F767ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="PneuDriveRaspberryPi.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -lstdc++ -lsupc++ -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

PneuDriveRaspberryPi.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "PneuDriveRaspberryPi.list"
	@echo 'Finished building: $@'
	@echo ' '

PneuDriveRaspberryPi.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "PneuDriveRaspberryPi.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents fail-specified-linker-script-missing warn-no-linker-script-specified
.SECONDARY:

-include ../makefile.targets
