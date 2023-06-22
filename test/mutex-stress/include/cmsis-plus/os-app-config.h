/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_RTOS_OS_APP_CONFIG_H_
#define CMSIS_PLUS_RTOS_OS_APP_CONFIG_H_

// ----------------------------------------------------------------------------

#define OS_INTEGER_SYSTICK_FREQUENCY_HZ                     (1000)

// With 4 bits NVIC, there are 16 levels, 0 = highest, 15 = lowest

#if 1
// Disable all interrupts from 15 to 4, keep 3-2-1 enabled
#define OS_INTEGER_RTOS_CRITICAL_SECTION_INTERRUPT_PRIORITY (4)
#endif

#define OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES               (2*os::rtos::port::stack::default_size_bytes)

// ----------------------------------------------------------------------------

#if defined(USE_FREERTOS)

// Request the inclusion of a custom implementations.
#define OS_USE_RTOS_PORT_SCHEDULER                      (1)

#if 1
#define OS_USE_RTOS_PORT_TIMER                          (1)
#define OS_USE_RTOS_PORT_CLOCK_SYSTICK_WAIT_FOR         (1)
#define OS_USE_RTOS_PORT_MUTEX                          (1)
#define OS_USE_RTOS_PORT_SEMAPHORE                      (1)
#define OS_USE_RTOS_PORT_MESSAGE_QUEUE                  (1)
#define OS_USE_RTOS_PORT_EVENT_FLAGS                    (1)
#endif

#endif /* defined(USE_FREERTOS) */

// ----------------------------------------------------------------------------


#if 0
#define OS_TRACE_RTOS_CLOCKS
#define OS_TRACE_RTOS_CONDVAR
#define OS_TRACE_RTOS_EVFLAGS
#define OS_TRACE_RTOS_LISTS
#define OS_TRACE_RTOS_MEMPOOL
#define OS_TRACE_RTOS_MQUEUE
#define OS_TRACE_RTOS_MUTEX
#define OS_TRACE_RTOS_RTC_TICK
#define OS_TRACE_RTOS_SCHEDULER
#define OS_TRACE_RTOS_SEMAPHORE
#define OS_TRACE_RTOS_SYSCLOCK_TICK
#define OS_TRACE_RTOS_THREAD
#define OS_TRACE_RTOS_THREAD_CONTEXT
#define OS_TRACE_RTOS_THREAD_FLAGS
#define OS_TRACE_RTOS_TIMER

#define OS_TRACE_LIBC_MALLOC
#define OS_TRACE_LIBC_ATEXIT
#endif

#if defined(DEBUG)
//#define OS_TRACE_RTOS_LISTS
#define OS_TRACE_RTOS_CLOCKS
#define OS_TRACE_LIBC_MALLOC
#define OS_TRACE_LIBC_ATEXIT
#define OS_TRACE_RTOS_MUTEX
#define OS_TRACE_RTOS_THREAD
//#define OS_TRACE_RTOS_THREAD_CONTEXT
#endif
#define OS_TRACE_RTOS_RTC_TICK
//#define OS_TRACE_RTOS_SYSCLOCK_TICK

// ----------------------------------------------------------------------------

#if !defined(__ARM_EABI__)

#define OS_USE_TRACE_POSIX_STDOUT

#endif


// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_APP_CONFIG_H_ */
