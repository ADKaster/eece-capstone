################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
posix/clock.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/clock.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/memory.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/memory.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/mqueue.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/mqueue.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/pthread.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/pthread.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/pthread_barrier.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/pthread_barrier.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/pthread_cond.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/pthread_cond.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/pthread_mutex.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/pthread_mutex.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/pthread_rwlock.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/pthread_rwlock.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/sched.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/sched.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/semaphore.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/semaphore.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/sleep.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/sleep.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

posix/timer.o: C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/freertos/timer.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/bin/arm-none-eabi-gcc.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DDeviceFamily_MSP432P401x -D__MSP432P401R__ -I"C:/Users/Andrew/Git/eece_capstone_experimental/eece-capstone/freertos_builds_MSP_EXP432P401R_release_gcc" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/include" -I"C:/FreeRTOSv10.0.0/FreeRTOS/Source/portable/GCC/ARM_CM4F" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/ti/posix/gcc" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source" -I"C:/ti/simplelink_msp432p4_sdk_1_60_00_12/source/third_party/CMSIS/Include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccsv7/tools/compiler/gcc-arm-none-eabi-6-2017-q1-update/arm-none-eabi/include" -ffunction-sections -fdata-sections -g -gdwarf-3 -gstrict-dwarf -Wall --specs=nano.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o"$@" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


