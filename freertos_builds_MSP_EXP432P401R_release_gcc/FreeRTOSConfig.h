/*
    FreeRTOS V9.0.0 - Copyright (C) 2017 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>>> AND MODIFIED BY <<<< the FreeRTOS exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
    ***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
    the FAQ page "My application does not run, what could be wrong?".  Have you
    defined configASSERT()?

    http://www.FreeRTOS.org/support - In return for receiving this top quality
    embedded software for free we request you assist our global community by
    participating in the support forum.

    http://www.FreeRTOS.org/training - Investing in training allows your team to
    be as productive as possible as early as possible.  Now you can receive
    FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
    Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 *
 * See http://www.freertos.org/a00110.html.
 *----------------------------------------------------------*/

/* 
 * This #include is needed to workaround an __INLINE warning for TI
 * compilers. Some FreeRTOS ports files include some CMSIS headers that
 * yield duplicate definition warnings.
 */
#ifdef __TI_COMPILER_VERSION__
#include <stdlib.h>
#endif

/*
 * This #ifdef guards against the driverlib.h file being included from IAR
 * assembly files.
 */
#ifndef __IAR_SYSTEMS_ASM__
#include <ti/devices/msp432p4xx/driverlib/cs.h>
#endif

/* 
 * Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
 * standard names - can't be used with CCS due to limitations in the assemblers
 * pre-processing.
 */
#ifndef __TI_COMPILER_VERSION__
    #define xPortPendSVHandler  PendSV_Handler
    #define vPortSVCHandler     SVC_Handler
    #define xPortSysTickHandler SysTick_Handler
#endif

#ifndef NULL
#define NULL (void *)0
#endif

#define configCPU_CLOCK_HZ              CS_getMCLK()

#define configUSE_PREEMPTION            1
#define configUSE_TIME_SLICING          0
#define configUSE_IDLE_HOOK             0
#define configUSE_TICK_HOOK             0
#define configTICK_RATE_HZ              ( ( TickType_t ) 1000 )
#define configMINIMAL_STACK_SIZE        ( ( unsigned short ) 256 )
#define configTOTAL_HEAP_SIZE           ( ( size_t ) ( 0x8000 ) )
#define configMAX_TASK_NAME_LEN         ( 12 )
#define configUSE_TRACE_FACILITY        1
#define configUSE_16_BIT_TICKS          0
#define configIDLE_SHOULD_YIELD         0
#define configUSE_CO_ROUTINES           0
#define configUSE_MUTEXES               1
#define configUSE_RECURSIVE_MUTEXES     1
#define configUSE_QUEUE_SETS            0
#define configUSE_COUNTING_SEMAPHORES   1

#define configMAX_PRIORITIES            ( 10UL )
#define configQUEUE_REGISTRY_SIZE       0

/* Constants provided for debugging. */
#define configUSE_MALLOC_FAILED_HOOK    1
#define configCHECK_FOR_STACK_OVERFLOW  2
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }

#define configUSE_TICKLESS_IDLE         1

#define configEXPECTED_IDLE_TIME_BEFORE_SLEEP   5

/* Timer related defines. */
#define configUSE_TIMERS                1
#define configTIMER_TASK_PRIORITY       5
#define configTIMER_QUEUE_LENGTH        20
#define configTIMER_TASK_STACK_DEPTH    ( configMINIMAL_STACK_SIZE * 2 )

#define configENABLE_BACKWARD_COMPATIBILITY 0

/* Needed for POSIX/pthread. */
#define configUSE_APPLICATION_TASK_TAG  1

#if defined(__TI_COMPILER_VERSION__)
#include <ti/posix/freertos/PTLS.h>
#define traceTASK_DELETE( pxTCB ) PTLS_taskDeleteHook( pxTCB )
#elif defined(__IAR_SYSTEMS_ICC__)
#ifndef __IAR_SYSTEMS_ASM__
#include <ti/posix/freertos/Mtx.h>
#define traceTASK_DELETE( pxTCB ) Mtx_taskDeleteHook( pxTCB )
#endif
#endif

/*
 *  Enable thread local storage
 *
 *  Assign TLS array index ownership here to avoid collisions.
 *  TLS storage is needed to implement thread-safe errno with
 *  TI and IAR compilers. With GNU compiler, we enable newlib.
 */
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)

#define configNUM_THREAD_LOCAL_STORAGE_POINTERS 1

#if defined(__TI_COMPILER_VERSION__)
#define PTLS_TLS_INDEX 0  /* ti.posix.freertos.PTLS */
#elif defined(__IAR_SYSTEMS_ICC__)
#define MTX_TLS_INDEX 0  /* ti.posix.freertos.Mtx */
#endif

#elif defined(__GNUC__)
/* note: system locks required by newlib are not implemented */
#define configUSE_NEWLIB_REENTRANT 1
#endif

/*
 *  Priority 7 (shifted 5 since only the top 3 bits are implemented).
 *  Priority 7 is the lowest priority.
 */
#define configKERNEL_INTERRUPT_PRIORITY ( 7 << 5 )

/*
 *  Priority 1 (shifted 5 since only the top 3 bits are implemented).
 *  Priority 1 is the second highest priority.
 *  Priority 0 is the highest priority.
 */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY ( 1 << 5 )

/*
 * Use the Cortex-M3 optimised task selection rather than the generic C code
 * version.
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION 1

/*
 * Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function.
 */
#define INCLUDE_vTaskPrioritySet                1
#define INCLUDE_uxTaskPriorityGet               1
#define INCLUDE_vTaskDelete                     1
#define INCLUDE_vTaskSuspend                    1
#define INCLUDE_vTaskDelayUntil                 1
#define INCLUDE_vTaskDelay                      1
#define INCLUDE_uxTaskGetStackHighWaterMark     0
#define INCLUDE_xTaskGetSchedulerState          1
#define INCLUDE_xTaskGetIdleTaskHandle          0
#define INCLUDE_eTaskGetState                   1
#define INCLUDE_xSemaphoreGetMutexHolder        0

#endif /* FREERTOS_CONFIG_H */
