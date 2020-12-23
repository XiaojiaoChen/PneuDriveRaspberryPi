################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.c \
../Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.c 

C_DEPS += \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.d \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.d 

OBJS += \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.o \
./Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.o 


# Each subdirectory must supply rules for building sources it contributes
Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.o: ../Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DARM_MATH_CM7 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Inc -I../Drivers/CMSIS/Include -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/User" -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Drivers/CMSIS/DSP/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Src/framework/frameInc" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/CMSIS/DSP/Source/CommonTables/arm_common_tables.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.o: ../Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DARM_MATH_CM7 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Inc -I../Drivers/CMSIS/Include -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/User" -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Drivers/CMSIS/DSP/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Src/framework/frameInc" -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Drivers/CMSIS/DSP/Source/CommonTables/arm_const_structs.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

