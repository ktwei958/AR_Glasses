################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../Middlewares/LVGL/GUI/lvgl/Pages/_Template/TemplateView.cpp 

OBJS += \
./Middlewares/LVGL/GUI/lvgl/Pages/_Template/TemplateView.o 

CPP_DEPS += \
./Middlewares/LVGL/GUI/lvgl/Pages/_Template/TemplateView.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/LVGL/GUI/lvgl/Pages/_Template/%.o Middlewares/LVGL/GUI/lvgl/Pages/_Template/%.su Middlewares/LVGL/GUI/lvgl/Pages/_Template/%.cyclo: ../Middlewares/LVGL/GUI/lvgl/Pages/_Template/%.cpp Middlewares/LVGL/GUI/lvgl/Pages/_Template/subdir.mk
	arm-none-eabi-g++ "$<" -mcpu=cortex-m4 -std=gnu++14 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F412Cx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Drivers/BSP/inc -I../Middlewares/LVGL/GUI -I../Middlewares/LVGL/GUI/lvgl -I../Middlewares/LVGL/GUI/src -I../Middlewares/LVGL/GUI/lvgl/examples/porting -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -I../Drivers/BSP/GH3220/Kernel_code/driver/inc -I../Drivers/BSP/GH3220/Kernel_code/kernel/inc -I../Drivers/BSP/GH3220/Kernel_code/module/gh_agc -I"D:/lvgl/STM32F412_TEST_LVGL_SD251119/Drivers/BSP/GH3220/Kernel_code/module/gh_common" -I../Drivers/BSP/GH3220/Kernel_code/module/gh_ecg -I../Drivers/BSP/GH3220/Kernel_code/module/gh_other -I../Drivers/BSP/GH3220/Kernel_code/module/gh_protocol -I../Drivers/BSP/GH3220/Kernel_code/module/gh_soft_adt -I../Drivers/BSP/GH3220/Algo_code/goodix_algo_application/inc -I../Drivers/BSP/GH3220/Algo_code/goodix_algo_call/inc -I../Drivers/BSP/GH3220/Algo_code/goodix_algo_call/inc/hr_exc -I../Drivers/BSP/GH3220/Algo_code/goodix_algo_call/inc/spo2_pre_exc -I../Core/USER/APP/inc -I"D:/lvgl/STM32F412_TEST_LVGL_SD251119/Middlewares/LVGL/GUI/lvgl/pngle" -I"D:/lvgl/STM32F412_TEST_LVGL_SD251119/Middlewares/LVGL/GUI/lvgl/Pages" -I"D:/lvgl/STM32F412_TEST_LVGL_SD251119/Middlewares/LVGL/GUI/lvgl/Pages/_Template" -O0 -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti -fno-use-cxa-atexit -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-LVGL-2f-GUI-2f-lvgl-2f-Pages-2f-_Template

clean-Middlewares-2f-LVGL-2f-GUI-2f-lvgl-2f-Pages-2f-_Template:
	-$(RM) ./Middlewares/LVGL/GUI/lvgl/Pages/_Template/TemplateView.cyclo ./Middlewares/LVGL/GUI/lvgl/Pages/_Template/TemplateView.d ./Middlewares/LVGL/GUI/lvgl/Pages/_Template/TemplateView.o ./Middlewares/LVGL/GUI/lvgl/Pages/_Template/TemplateView.su

.PHONY: clean-Middlewares-2f-LVGL-2f-GUI-2f-lvgl-2f-Pages-2f-_Template

