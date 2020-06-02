################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Inc/duration.cpp \
../Inc/time.cpp 

OBJS += \
./Inc/duration.o \
./Inc/time.o 

CPP_DEPS += \
./Inc/duration.d \
./Inc/time.d 


# Each subdirectory must supply rules for building sources it contributes
Inc/duration.o: ../Inc/duration.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DARM_MATH_CM7 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/User" -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Drivers/CMSIS/DSP/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Src/framework/frameInc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Src/framework/frameInc/rosserialInc" -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Inc/duration.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
Inc/time.o: ../Inc/time.cpp
	arm-none-eabi-g++ "$<" -mcpu=cortex-m7 -std=gnu++14 -g3 -DUSE_HAL_DRIVER -DSTM32F767xx -DARM_MATH_CM7 -DDEBUG '-D__FPU_PRESENT=1U' -c -I../Middlewares/Third_Party/FreeRTOS/Source/include -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/User" -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/STM32F7xx_HAL_Driver/Inc -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Drivers/CMSIS/DSP/Include" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Src/framework/frameInc" -I"C:/Users/40207/OneDrive/OneDriveDocumentation/STM32CubeIDEWorkSpaceOnedrive/PneuDriveRaspberryPi/Src/framework/frameInc/rosserialInc" -I../Inc -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-threadsafe-statics -fno-use-cxa-atexit -Wall -fstack-usage -MMD -MP -MF"Inc/time.d" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

