/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
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

#if defined(__ARM_EABI__)

#define OS_INTEGER_RTOS_MAIN_STACK_SIZE_BYTES               (3000)


#if defined(OS_USE_TRACE_SEGGER_RTT)
#define OS_EXCLUDE_RTOS_IDLE_SLEEP (1)
#endif

#endif /* defined(__ARM_EABI__) */

#define OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES  (1)
#define OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES        (1)

// ----------------------------------------------------------------------------

#if defined(USE_FREERTOS)

// Request the inclusion of a custom implementations.
#define OS_USE_RTOS_PORT_SCHEDULER                      (1)

#if 1
#define OS_USE_RTOS_PORT_TIMER                          (1)
#define OS_USE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR        (1)
#define OS_USE_RTOS_PORT_MUTEX                          (1)
#define OS_USE_RTOS_PORT_SEMAPHORE                      (1)
#define OS_USE_RTOS_PORT_MESSAGE_QUEUE                  (1)
#define OS_USE_RTOS_PORT_EVENT_FLAGS                    (1)
#endif

#endif /* defined(USE_FREERTOS) */

// ----------------------------------------------------------------------------

#if defined(DEBUG)

#define OS_TRACE_RTOS_CLOCKS
#define OS_TRACE_RTOS_CONDVAR
#define OS_TRACE_RTOS_EVFLAGS
#define OS_TRACE_RTOS_MEMPOOL
#define OS_TRACE_RTOS_MQUEUE
#define OS_TRACE_RTOS_MUTEX
#define OS_TRACE_RTOS_RTC_TICK
#define OS_TRACE_RTOS_SCHEDULER
#define OS_TRACE_RTOS_SEMAPHORE
#define OS_TRACE_RTOS_THREAD
#define OS_TRACE_RTOS_THREAD_FLAGS
#define OS_TRACE_RTOS_TIMER

#define OS_TRACE_LIBC_MALLOC
#define OS_TRACE_LIBC_ATEXIT

#if !defined(__ARM_EABI__) || defined(OS_USE_TRACE_SEGGER_RTT)
#define OS_TRACE_RTOS_SYSCLOCK_TICK
#define OS_TRACE_RTOS_LISTS
#define OS_TRACE_RTOS_LISTS_CLOCKS
#define OS_TRACE_RTOS_THREAD_CONTEXT
#endif

#else
#define OS_TRACE_RTOS_THREAD
#define OS_TRACE_RTOS_MQUEUE
#define OS_TRACE_RTOS_MEMPOOL
#define OS_TRACE_LIBC_MALLOC
#endif

#define OS_TRACE_RTOS_RTC_TICK
//#define OS_TRACE_RTOS_SYSCLOCK_TICK

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_APP_CONFIG_H_ */
