################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DFREERTOS -DNOAPPLICATION -D__MSP432P401R__ -Dgcc -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/damnlib" -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/damnlib/inc" -g -gdwarf-3 -gstrict-dwarf -Wall -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


