/* ----------------------------------------------------------------------
 * $Date:        5. February 2013
 * $Revision:    V1.02
 *
 * Project:      CMSIS-RTOS API
 * Title:        cmsis_os.h template header file
 *
 * Version 0.02
 *    Initial Proposal Phase
 * Version 0.03
 *    osKernelStart added, optional feature: main started as thread
 *    osSemaphores have standard behavior
 *    osTimerCreate does not start the timer, added osTimerStart
 *    osThreadPass is renamed to osThreadYield
 * Version 1.01
 *    Support for C++ interface
 *     - const attribute removed from the osXxxxDef_t typedef's
 *     - const attribute added to the osXxxxDef macros
 *    Added: osTimerDelete, osMutexDelete, osSemaphoreDelete
 *    Added: osKernelInitialize
 * Version 1.02
 *    Control functions for short timeouts in microsecond resolution:
 *    Added: osKernelSysTick, osKernelSysTickFrequency, osKernelSysTickMicroSec
 *    Removed: osSignalGet 
 *----------------------------------------------------------------------------
 *
 * Copyright (c) 2013 ARM LIMITED
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *  - Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  - Neither the name of ARM  nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS AND CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *---------------------------------------------------------------------------*/

/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code exposes a fully compliant ARM CMSIS API in the
 * context of the C++ version.
 */

#ifndef _CMSIS_OS_H
#define _CMSIS_OS_H

/// @note MUST REMAIN UNCHANGED: @b osCMSIS identifies the CMSIS-RTOS API version.
#define osCMSIS           0x00010002 ///< API version (main [31:16] .sub [15:0])

/// @note CAN BE CHANGED: @b osCMSIS_KERNEL identifies the underlying RTOS kernel and version number.
#define osCMSIS_KERNEL    0x00010000	 ///< RTOS identification and version (main [31:16] .sub [15:0])

/// @note MUST REMAIN UNCHANGED: @b osKernelSystemId shall be consistent in every CMSIS-RTOS.
#define osKernelSystemId "CMSIS++ V1.00" ///< RTOS identification string

/// @note MUST REMAIN UNCHANGED: @b osFeature_xxx shall be consistent in every CMSIS-RTOS.
#define osFeature_MainThread   1 ///< main thread      1=main can be thread, 0=not available
#define osFeature_Pool         1 ///< Memory Pools:    1=available, 0=not available
#define osFeature_MailQ        1 ///< Mail Queues:     1=available, 0=not available
#define osFeature_MessageQ     1 ///< Message Queues:  1=available, 0=not available
#define osFeature_Signals      8 ///< maximum number of Signal Flags available per thread
#define osFeature_Semaphore    30 ///< maximum count for @ref osSemaphoreCreate function
#define osFeature_Wait         1 ///< osWait function: 1=available, 0=not available
#define osFeature_SysTick      1 ///< osKernelSysTick functions: 1=available, 0=not available

#include <stdint.h>
#include <stddef.h>

#include <cmsis-plus/rtos/os-impl.h>

#ifdef  __cplusplus
extern "C"
{
#endif

// ==== Enumeration, structures, defines ====

/// Priority used for thread control.
/// @note MUST REMAIN UNCHANGED: @b osPriority shall be consistent in every CMSIS-RTOS.
  typedef enum
  {
    osPriorityIdle = -3, ///< priority: idle (lowest)
    osPriorityLow = -2, ///< priority: low
    osPriorityBelowNormal = -1, ///< priority: below normal
    osPriorityNormal = 0, ///< priority: normal (default)
    osPriorityAboveNormal = +1, ///< priority: above normal
    osPriorityHigh = +2, ///< priority: high
    osPriorityRealtime = +3, ///< priority: realtime (highest)
    osPriorityError = 0x84 ///< system cannot determine priority or thread has illegal priority
  } osPriority;

  /// Timeout value.
  /// @note MUST REMAIN UNCHANGED: @b osWaitForever shall be consistent in every CMSIS-RTOS.
#define osWaitForever     0xFFFFFFFF ///< wait forever timeout value

  /// Status code values returned by CMSIS-RTOS functions.
  /// @note MUST REMAIN UNCHANGED: @b osStatus shall be consistent in every CMSIS-RTOS.
  typedef enum
  {
    osOK = 0, ///< function completed; no error or event occurred.
    osEventSignal = 0x08, ///< function completed; signal event occurred.
    osEventMessage = 0x10, ///< function completed; message event occurred.
    osEventMail = 0x20, ///< function completed; mail event occurred.
    osEventTimeout = 0x40, ///< function completed; timeout occurred.
    osErrorParameter = 0x80, ///< parameter error: a mandatory parameter was missing or specified an incorrect object.
    osErrorResource = 0x81, ///< resource not available: a specified resource was not available.
    osErrorTimeoutResource = 0xC1, ///< resource not available within given time: a specified resource was not available within the timeout period.
    osErrorISR = 0x82, ///< not allowed in ISR context: the function cannot be called from interrupt service routines.
    osErrorISRRecursive = 0x83, ///< function called multiple times from ISR with same object.
    osErrorPriority = 0x84, ///< system cannot determine priority or thread has illegal priority.
    osErrorNoMemory = 0x85, ///< system is out of memory: it was impossible to allocate or reserve memory for the operation.
    osErrorValue = 0x86, ///< value of a parameter is out of range.
    osErrorOS = 0xFF, ///< unspecified RTOS error: run-time error but no other error message fits.
    os_status_reserved = 0x7FFFFFFF ///< prevent from enum down-size compiler optimization.
  } osStatus;

  /// Timer type value for the timer definition.
  /// @note MUST REMAIN UNCHANGED: @b os_timer_type shall be consistent in every CMSIS-RTOS.
  typedef enum
  {
    osTimerOnce = 0, ///< one-shot timer
    osTimerPeriodic = 1 ///< repeating timer
  } os_timer_type;

  /// Entry point of a thread.
  /// @note MUST REMAIN UNCHANGED: @b os_pthread shall be consistent in every CMSIS-RTOS.
  typedef void
  (*os_pthread) (void const *argument);

  /// Entry point of a timer call back function.
  /// @note MUST REMAIN UNCHANGED: @b os_ptimer shall be consistent in every CMSIS-RTOS.
  typedef void
  (*os_ptimer) (void const *argument);

// >>> the following data type definitions may shall adapted towards a specific RTOS

#define OS_PRIOTHREAD_SIZE_PTR (OS_INTEGER_MAX_NUMBER_OF_THREADS+1)

#if __SIZEOF_POINTER__ == 4
#define OS_THREAD_SIZE_PTRS  (7)
#define OS_TIMER_SIZE_PTRS  (4)
#define OS_MUTEX_SIZE_PTRS  (4)
#define OS_SEMAPHORE_SIZE_PTRS  (3+OS_PRIOTHREAD_SIZE_PTR)
#define OS_POOL_SIZE_PTRS  (4+OS_PRIOTHREAD_SIZE_PTR)
#define OS_MESSAGEQ_SIZE_PTRS  (5+2*OS_PRIOTHREAD_SIZE_PTR)
#define OS_MAILQ_SIZE_PTRS  (1)
#elif __SIZEOF_POINTER__ == 8
#define OS_THREAD_SIZE_PTRS  (6)
#define OS_TIMER_SIZE_PTRS  (4)
#define OS_MUTEX_SIZE_PTRS  (3)
#define OS_SEMAPHORE_SIZE_PTRS  (2+OS_PRIOTHREAD_SIZE_PTR)
#define OS_POOL_SIZE_PTRS  (3+OS_PRIOTHREAD_SIZE_PTR)
#define OS_MESSAGEQ_SIZE_PTRS  (4+2*OS_PRIOTHREAD_SIZE_PTR)
#define OS_MAILQ_SIZE_PTRS  (1)
#else
#error "Unsupported platform."
#endif

  typedef struct os_thread_data
  {
    void* content[OS_THREAD_SIZE_PTRS];
  } osThread;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_thread_attr
  {
    const char* name;
    void* stack_addr;
    size_t stack_size_bytes;
    uint8_t priority;
  } osThreadAttr;

#pragma GCC diagnostic pop

  typedef struct os_timer_data
  {
    void* content[OS_TIMER_SIZE_PTRS];
  } osTimer;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_timer_attr
  {
    const char* name;
    uint8_t type;
  } osTimerAttr;

#pragma GCC diagnostic pop

  typedef struct os_mutex_data
  {
    void* content[OS_MUTEX_SIZE_PTRS];
  } osMutex;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_mutex_attr
  {
    const char* name;
    uint8_t priority_ceiling;
    uint8_t protocol;
    uint8_t robustness;
    uint8_t type;
  } osMutexAttr;

#pragma GCC diagnostic pop

  typedef struct os_semaphore_data
  {
    void* content[OS_SEMAPHORE_SIZE_PTRS];
  } osSemaphore;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  typedef struct os_semaphore_attr
  {
    const char* name;
    int16_t initial_count;
    int16_t max_count;
  } osSemaphoreAttr;

#pragma GCC diagnostic pop

  typedef struct os_pool_data
  {
    void* content[OS_POOL_SIZE_PTRS];
  } osPool;

  typedef struct os_pool_attr
  {
    const char* name;
    void* pool_addr;
    // size_t pool_size_bytes;
  } osPoolAttr;

  typedef struct os_messageQ_data
  {
    void* content[OS_MESSAGEQ_SIZE_PTRS];
  } osMessageQ;

  typedef struct os_messageQ_attr
  {
    const char* name;
    void* queue_addr;
    size_t queue_size_bytes;
  } osMessageQAttr;

  typedef struct os_mailQ_data
  {
    osPool pool;
    osMessageQ queue;
  } osMailQ;

  /// Thread ID identifies the thread (pointer to a thread control block).
  /// @note CAN BE CHANGED: @b os_thread_cb is implementation specific in every CMSIS-RTOS.
  typedef osThread* osThreadId;

  /// Timer ID identifies the timer (pointer to a timer control block).
  /// @note CAN BE CHANGED: @b os_timer_cb is implementation specific in every CMSIS-RTOS.
  typedef osTimer* osTimerId;

  /// Mutex ID identifies the mutex (pointer to a mutex control block).
  /// @note CAN BE CHANGED: @b os_mutex_cb is implementation specific in every CMSIS-RTOS.
  typedef osMutex* osMutexId;

  /// Semaphore ID identifies the semaphore (pointer to a semaphore control block).
  /// @note CAN BE CHANGED: @b os_semaphore_cb is implementation specific in every CMSIS-RTOS.
  typedef osSemaphore* osSemaphoreId;

  /// Pool ID identifies the memory pool (pointer to a memory pool control block).
  /// @note CAN BE CHANGED: @b os_pool_cb is implementation specific in every CMSIS-RTOS.
  typedef osPool* osPoolId;

  /// Message ID identifies the message queue (pointer to a message queue control block).
  /// @note CAN BE CHANGED: @b os_messageQ_cb is implementation specific in every CMSIS-RTOS.
  typedef osMessageQ* osMessageQId;

  /// Mail ID identifies the mail queue (pointer to a mail queue control block).
  /// @note CAN BE CHANGED: @b os_mailQ_cb is implementation specific in every CMSIS-RTOS.
  typedef osMailQ* osMailQId;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /// Thread Definition structure contains startup information of a thread.
  /// @note CAN BE CHANGED: @b os_thread_def is implementation specific in every CMSIS-RTOS.
  typedef struct os_thread_def
  {
    const char* name;
    os_pthread pthread; ///< start address of thread function
    osPriority tpriority; ///< initial thread priority
    // uint32_t instances; ///< maximum number of instances of that thread function
    uint32_t stacksize; ///< stack size requirements in bytes; 0 is default stack size
    osThread* data;
  } osThreadDef_t;

#pragma GCC diagnostic pop

  /// Timer Definition structure contains timer parameters.
  /// @note CAN BE CHANGED: @b os_timer_def is implementation specific in every CMSIS-RTOS.
  typedef struct os_timer_def
  {
    const char* name;
    os_ptimer ptimer; ///< start address of a timer function
    osTimer* data;
  } osTimerDef_t;

  /// Mutex Definition structure contains setup information for a mutex.
  /// @note CAN BE CHANGED: @b os_mutex_def is implementation specific in every CMSIS-RTOS.
  typedef struct os_mutex_def
  {
    const char* name;
    osMutex* data;
  } osMutexDef_t;

  /// Semaphore Definition structure contains setup information for a semaphore.
  /// @note CAN BE CHANGED: @b os_semaphore_def is implementation specific in every CMSIS-RTOS.
  typedef struct os_semaphore_def
  {
    const char* name;
    osSemaphore* data;
  } osSemaphoreDef_t;

  /// Definition structure for memory block allocation.
  /// @note CAN BE CHANGED: @b os_pool_def is implementation specific in every CMSIS-RTOS.
  typedef struct os_pool_def
  {
    const char* name;
    uint32_t items; ///< number of items (elements) in the pool
    uint32_t item_sz; ///< size of an item
    void* pool; ///< pointer to memory for pool
    uint32_t pool_sz;
    osPool* data;
  } osPoolDef_t;

  /// Definition structure for message queue.
  /// @note CAN BE CHANGED: @b os_messageQ_def is implementation specific in every CMSIS-RTOS.
  typedef struct os_messageQ_def
  {
    const char* name;
    uint32_t items; ///< number of elements in the queue
    uint32_t item_sz; ///< size of an item
    void* queue; ///< pointer to memory array for messages
    uint32_t queue_sz;
    osMessageQ* data;
  } osMessageQDef_t;

  /// Definition structure for mail queue.
  /// @note CAN BE CHANGED: @b os_mailQ_def is implementation specific in every CMSIS-RTOS.
  typedef struct os_mailQ_def
  {
    const char* name;
    uint32_t items; ///< number of elements in the queue
    uint32_t pool_item_sz; ///< size of a pool item
    uint32_t queue_item_sz; ///< size of a queue item
    void* pool; ///< pointer to memory array for pool
    uint32_t pool_sz;
    void* queue; ///< pointer to memory array for queue
    uint32_t queue_sz;
    osMailQ* data;
  } osMailQDef_t;

  /// Event structure contains detailed information about an event.
  /// @note MUST REMAIN UNCHANGED: @b os_event shall be consistent in every CMSIS-RTOS.
  ///       However the struct may be extended at the end.
  typedef struct
  {
    osStatus status; ///< status code: event or error information
    union
    {
      uint32_t v; ///< message as 32-bit value
      void* p; ///< message or mail as void pointer
      int32_t signals; ///< signal flags
    } value; ///< event value
    union
    {
      osMailQId mail_id; ///< mail id obtained by @ref osMailCreate
      osMessageQId message_id; ///< message id obtained by @ref osMessageCreate
    } def; ///< event definition
  } osEvent;

//  ==== Kernel Control Functions ====

  /// Initialize the RTOS Kernel for creating objects.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osKernelInitialize shall be consistent in every CMSIS-RTOS.
  osStatus
  osKernelInitialize (void);

  /// Start the RTOS Kernel.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osKernelStart shall be consistent in every CMSIS-RTOS.
  osStatus
  osKernelStart (void);

  /// Check if the RTOS kernel is already started.
  /// @note MUST REMAIN UNCHANGED: @b osKernelRunning shall be consistent in every CMSIS-RTOS.
  /// @return 0 RTOS is not started, 1 RTOS is started.
  int32_t
  osKernelRunning (void);

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))     // System Timer available

  /// Get the RTOS kernel system timer counter
  /// @note MUST REMAIN UNCHANGED: @b osKernelSysTick shall be consistent in every CMSIS-RTOS.
  /// @return RTOS kernel system timer as 32-bit value
  uint32_t
  osKernelSysTick (void);

  /// The RTOS kernel system timer frequency in Hz
  /// @note Reflects the system timer setting and is typically defined in a configuration file.
#define osKernelSysTickFrequency 100000000

  /// Convert a microseconds value to a RTOS kernel system timer value.
  /// @param         microsec     time value in microseconds.
  /// @return time value normalized to the @ref osKernelSysTickFrequency
#define osKernelSysTickMicroSec(microsec) (((uint64_t)microsec * (osKernelSysTickFrequency)) / 1000000)

#endif    // System Timer available

//  ==== Thread Management ====

  /// Create a Thread Definition with function, priority, and stack requirements.
  /// @param         name         name of the thread function.
  /// @param         priority     initial priority of the thread function.
  /// @param         instances    number of possible thread instances.
  /// @param         stacksz      stack size (in bytes) requirements for the thread function.
  /// @note CAN BE CHANGED: The parameters to @b osThreadDef shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osThreadDef(name, priority, instances, stacksz)  \
extern const osThreadDef_t os_thread_def_##name
#else                            // define the object
#define osThreadDef(name, priority, instances, stacksz)  \
struct os_thread_data os_thread_data_##name; \
const osThreadDef_t os_thread_def_##name = \
{ "##name", (os_pthread)(name), (priority), /* (instances), */ (stacksz), &os_thread_data_##name }
#endif

  /// Access a Thread definition.
  /// @param         name          name of the thread definition object.
  /// @note CAN BE CHANGED: The parameter to @b osThread shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#define osThread(name)  \
&os_thread_def_##name

  /// Create a thread and add it to Active Threads and set it to state READY.
  /// @param [in]     thread_def    thread definition referenced with @ref osThread.
  /// @param [in]     argument      pointer that is passed to the thread function as start argument.
  /// @return thread ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osThreadCreate shall be consistent in every CMSIS-RTOS.
  osThreadId
  osThreadCreate (const osThreadDef_t* thread_def, void* argument);

  /// Return the thread ID of the current running thread.
  /// @return thread ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osThreadGetId shall be consistent in every CMSIS-RTOS.
  osThreadId
  osThreadGetId (void);

  /// Terminate execution of a thread and remove it from Active Threads.
  /// @param [in]     thread_id   thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osThreadTerminate shall be consistent in every CMSIS-RTOS.
  osStatus
  osThreadTerminate (osThreadId thread_id);

  /// Pass control to next thread that is in state @b READY.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osThreadYield shall be consistent in every CMSIS-RTOS.
  osStatus
  osThreadYield (void);

  /// Change priority of an active thread.
  /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
  /// @param [in]     priority      new priority value for the thread function.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osThreadSetPriority shall be consistent in every CMSIS-RTOS.
  osStatus
  osThreadSetPriority (osThreadId thread_id, osPriority priority);

  /// Get current priority of an active thread.
  /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
  /// @return current priority value of the thread function.
  /// @note MUST REMAIN UNCHANGED: @b osThreadGetPriority shall be consistent in every CMSIS-RTOS.
  osPriority
  osThreadGetPriority (osThreadId thread_id);

//  ==== Generic Wait Functions ====

  /// Wait for Timeout (Time Delay).
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue "time delay" value
  /// @return status code that indicates the execution status of the function.
  osStatus
  osDelay (uint32_t millisec);

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))     // Generic Wait available

  /// Wait for Signal, Message, Mail, or Timeout.
  /// @param [in] millisec          @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
  /// @return event that contains signal, message, or mail information or error code.
  /// @note MUST REMAIN UNCHANGED: @b osWait shall be consistent in every CMSIS-RTOS.
  osEvent
  osWait (uint32_t millisec);

#endif  // Generic Wait available

//  ==== Timer Management Functions ====

  /// Define a Timer object.
  /// @param         name          name of the timer object.
  /// @param         function      name of the timer call back function.
  /// @note CAN BE CHANGED: The parameter to @b osTimerDef shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osTimerDef(name, function)  \
extern const osTimerDef_t os_timer_def_##name
#else                            // define the object
#define osTimerDef(name, function)  \
struct os_timer_data os_timer_data_##name; \
const osTimerDef_t os_timer_def_##name = \
{ "##name", (os_ptimer)(function), &os_timer_data_##name }
#endif

  /// Access a Timer definition.
  /// @param         name          name of the timer object.
  /// @note CAN BE CHANGED: The parameter to @b osTimer shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#define osTimer(name) \
&os_timer_def_##name

  /// Create a timer.
  /// @param [in]     timer_def     timer object referenced with @ref osTimer.
  /// @param [in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
  /// @param [in]     argument      argument to the timer call back function.
  /// @return timer ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osTimerCreate shall be consistent in every CMSIS-RTOS.
  osTimerId
  osTimerCreate (const osTimerDef_t* timer_def, os_timer_type type,
                 void* argument);

  /// Start or restart a timer.
  /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue "time delay" value of the timer.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osTimerStart shall be consistent in every CMSIS-RTOS.
  osStatus
  osTimerStart (osTimerId timer_id, uint32_t millisec);

  /// Stop the timer.
  /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osTimerStop shall be consistent in every CMSIS-RTOS.
  osStatus
  osTimerStop (osTimerId timer_id);

  /// Delete a timer that was created by @ref osTimerCreate.
  /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osTimerDelete shall be consistent in every CMSIS-RTOS.
  osStatus
  osTimerDelete (osTimerId timer_id);

//  ==== Signal Management ====

  /// Set the specified Signal Flags of an active thread.
  /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
  /// @param [in]     signals       specifies the signal flags of the thread that should be set.
  /// @return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
  /// @note MUST REMAIN UNCHANGED: @b osSignalSet shall be consistent in every CMSIS-RTOS.
  int32_t
  osSignalSet (osThreadId thread_id, int32_t signals);

  /// Clear the specified Signal Flags of an active thread.
  /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
  /// @param [in]     signals       specifies the signal flags of the thread that shall be cleared.
  /// @return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters or call from ISR.
  /// @note MUST REMAIN UNCHANGED: @b osSignalClear shall be consistent in every CMSIS-RTOS.
  int32_t
  osSignalClear (osThreadId thread_id, int32_t signals);

  /// Wait for one or more Signal Flags to become signaled for the current @b RUNNING thread.
  /// @param [in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
  /// @return event flag information or error code.
  /// @note MUST REMAIN UNCHANGED: @b osSignalWait shall be consistent in every CMSIS-RTOS.
  osEvent
  osSignalWait (int32_t signals, uint32_t millisec);

//  ==== Mutex Management ====

  /// Define a Mutex.
  /// @param         name          name of the mutex object.
  /// @note CAN BE CHANGED: The parameter to @b osMutexDef shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osMutexDef(name)  \
extern const osMutexDef_t os_mutex_def_##name
#else                            // define the object
#define osMutexDef(name)  \
struct os_mutex_data os_mutex_data_##name; \
const osMutexDef_t os_mutex_def_##name = \
  { "##name", &os_mutex_data_##name }
#endif

  /// Access a Mutex definition.
  /// @param         name          name of the mutex object.
  /// @note CAN BE CHANGED: The parameter to @b osMutex shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#define osMutex(name)  \
&os_mutex_def_##name

  /// Create and Initialize a Mutex object.
  /// @param [in]     mutex_def     mutex definition referenced with @ref osMutex.
  /// @return mutex ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osMutexCreate shall be consistent in every CMSIS-RTOS.
  osMutexId
  osMutexCreate (const osMutexDef_t* mutex_def);

  /// Wait until a Mutex becomes available.
  /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osMutexWait shall be consistent in every CMSIS-RTOS.
  osStatus
  osMutexWait (osMutexId mutex_id, uint32_t millisec);

  /// Release a Mutex that was obtained by @ref osMutexWait.
  /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osMutexRelease shall be consistent in every CMSIS-RTOS.
  osStatus
  osMutexRelease (osMutexId mutex_id);

  /// Delete a Mutex that was created by @ref osMutexCreate.
  /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osMutexDelete shall be consistent in every CMSIS-RTOS.
  osStatus
  osMutexDelete (osMutexId mutex_id);

//  ==== Semaphore Management Functions ====

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))     // Semaphore available

  /// Define a Semaphore object.
  /// @param         name          name of the semaphore object.
  /// @note CAN BE CHANGED: The parameter to @b osSemaphoreDef shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osSemaphoreDef(name)  \
extern const osSemaphoreDef_t os_semaphore_def_##name
#else                            // define the object
#define osSemaphoreDef(name)  \
struct os_semaphore_data os_semaphore_data_##name; \
const osSemaphoreDef_t os_semaphore_def_##name = \
  { "##name", &os_semaphore_data_##name }
#endif

  /// Access a Semaphore definition.
  /// @param         name          name of the semaphore object.
  /// @note CAN BE CHANGED: The parameter to @b osSemaphore shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#define osSemaphore(name)  \
&os_semaphore_def_##name

  /// Create and Initialize a Semaphore object used for managing resources.
  /// @param [in]     semaphore_def semaphore definition referenced with @ref osSemaphore.
  /// @param [in]     count         number of available resources.
  /// @return semaphore ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osSemaphoreCreate shall be consistent in every CMSIS-RTOS.
  osSemaphoreId
  osSemaphoreCreate (const osSemaphoreDef_t* semaphore_def, int32_t count);

  /// Wait until a Semaphore token becomes available.
  /// @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
  /// @return number of available tokens, or -1 in case of incorrect parameters.
  /// @note MUST REMAIN UNCHANGED: @b osSemaphoreWait shall be consistent in every CMSIS-RTOS.
  int32_t
  osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec);

  /// Release a Semaphore token.
  /// @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osSemaphoreRelease shall be consistent in every CMSIS-RTOS.
  osStatus
  osSemaphoreRelease (osSemaphoreId semaphore_id);

  /// Delete a Semaphore that was created by @ref osSemaphoreCreate.
  /// @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osSemaphoreDelete shall be consistent in every CMSIS-RTOS.
  osStatus
  osSemaphoreDelete (osSemaphoreId semaphore_id);

#endif     // Semaphore available

//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))  // Memory Pool Management available

  /// @brief Define a Memory Pool.
  /// @param         name          name of the memory pool.
  /// @param         no            maximum number of blocks (objects) in the memory pool.
  /// @param         type          data type of a single block (object).
  /// @note CAN BE CHANGED: The parameter to @b osPoolDef shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osPoolDef(name, no, type)   \
extern const osPoolDef_t os_pool_def_##name
#else                            // define the object
#define osPoolDef(name, items, type)   \
struct { struct os_pool_data data; type pool[items]; } os_pool_##name; \
const osPoolDef_t os_pool_def_##name = \
{ "##name", (items), sizeof(type), os_pool_##name.pool, sizeof(os_pool_##name.pool), &os_pool_##name.data }
#endif

  /// @brief Access a Memory Pool definition.
  /// @param         name          name of the memory pool
  /// @note CAN BE CHANGED: The parameter to @b osPool shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#define osPool(name) \
&os_pool_def_##name

  /// Create and Initialize a memory pool.
  /// @param [in]     pool_def      memory pool definition referenced with @ref osPool.
  /// @return memory pool ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osPoolCreate shall be consistent in every CMSIS-RTOS.
  osPoolId
  osPoolCreate (const osPoolDef_t* pool_def);

  /// Allocate a memory block from a memory pool.
  /// @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
  /// @return address of the allocated memory block or NULL in case of no memory available.
  /// @note MUST REMAIN UNCHANGED: @b osPoolAlloc shall be consistent in every CMSIS-RTOS.
  void*
  osPoolAlloc (osPoolId pool_id);

  /// Allocate a memory block from a memory pool and set memory block to zero.
  /// @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
  /// @return address of the allocated memory block or NULL in case of no memory available.
  /// @note MUST REMAIN UNCHANGED: @b osPoolCAlloc shall be consistent in every CMSIS-RTOS.
  void*
  osPoolCAlloc (osPoolId pool_id);

  /// Return an allocated memory block back to a specific memory pool.
  /// @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
  /// @param [in]     block         address of the allocated memory block that is returned to the memory pool.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osPoolFree shall be consistent in every CMSIS-RTOS.
  osStatus
  osPoolFree (osPoolId pool_id, void* block);

#endif   // Memory Pool Management available

//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))     // Message Queues available

  /// @brief Create a Message Queue Definition.
  /// @param         name          name of the queue.
  /// @param         queue_sz      maximum number of messages in the queue.
  /// @param         type          data type of a single message element (for debugger).
  /// @note CAN BE CHANGED: The parameter to @b osMessageQDef shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osMessageQDef(name, queue_sz, type)   \
extern const osMessageQDef_t os_messageQ_def_##name
#else                            // define the object
#define osMessageQDef(name, items, type)   \
struct { \
    struct os_messageQ_data data; \
    void* queue[items]; \
} os_messageQ_##name; \
const osMessageQDef_t os_messageQ_def_##name = { \
    "##name", \
    (items), \
    sizeof (void*), \
    os_messageQ_##name.queue, \
    sizeof(os_messageQ_##name.queue), \
    &os_messageQ_##name.data \
}
#endif

  /// @brief Access a Message Queue Definition.
  /// @param         name          name of the queue
  /// @note CAN BE CHANGED: The parameter to @b osMessageQ shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#define osMessageQ(name) \
&os_messageQ_def_##name

  /// Create and Initialize a Message Queue.
  /// @param [in]     queue_def     queue definition referenced with @ref osMessageQ.
  /// @param [in]     thread_id     thread ID (obtained by @ref osThreadCreate or @ref osThreadGetId) or NULL.
  /// @return message queue ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osMessageCreate shall be consistent in every CMSIS-RTOS.
  osMessageQId
  osMessageCreate (const osMessageQDef_t* queue_def, osThreadId thread_id);

  /// Put a Message to a Queue.
  /// @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
  /// @param [in]     info          message information.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osMessagePut shall be consistent in every CMSIS-RTOS.
  osStatus
  osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec);

  /// Get a Message or Wait for a Message from a Queue.
  /// @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
  /// @return event information that includes status code.
  /// @note MUST REMAIN UNCHANGED: @b osMessageGet shall be consistent in every CMSIS-RTOS.
  osEvent
  osMessageGet (osMessageQId queue_id, uint32_t millisec);

#endif     // Message Queues available

//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))     // Mail Queues available

  /// @brief Create a Mail Queue Definition.
  /// @param         name          name of the queue
  /// @param         queue_sz      maximum number of messages in queue
  /// @param         type          data type of a single message element
  /// @note CAN BE CHANGED: The parameter to @b osMailQDef shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#if defined (osObjectsExternal)  // object is external
#define osMailQDef(name, queue_sz, type) \
extern const osMailQDef_t os_mailQ_def_##name
#else                            // define the object
#define osMailQDef(name, items, type) \
struct { \
    os_mailQ_data data; \
    type pool[items]; \
    void* queue[items]; \
} osmailQ_##name; \
const osMailQDef_t os_mailQ_def_##name = { \
    "##name", \
    (items), \
    sizeof (type), \
    sizeof (void*), \
    os_mailQ_##name.pool, \
    sizeof(os_mailQ_##name.pool), \
    os_mailQ_##name.queue, \
    sizeof(os_mailQ_##name.queue), \
    &osmailQ_##name.data \
}
#endif

  /// @brief Access a Mail Queue Definition.
  /// @param         name          name of the queue
  /// @note CAN BE CHANGED: The parameter to @b osMailQ shall be consistent but the
  ///       macro body is implementation specific in every CMSIS-RTOS.
#define osMailQ(name)  \
&os_mailQ_def_##name

  /// Create and Initialize mail queue.
  /// @param [in]     queue_def     reference to the mail queue definition obtain with @ref osMailQ
  /// @param [in]     thread_id     thread ID (obtained by @ref osThreadCreate or @ref osThreadGetId) or NULL.
  /// @return mail queue ID for reference by other functions or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osMailCreate shall be consistent in every CMSIS-RTOS.
  osMailQId
  osMailCreate (const osMailQDef_t* queue_def, osThreadId thread_id);

  /// Allocate a memory block from a mail.
  /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
  /// @return pointer to memory block that can be filled with mail or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osMailAlloc shall be consistent in every CMSIS-RTOS.
  void*
  osMailAlloc (osMailQId queue_id, uint32_t millisec);

  /// Allocate a memory block from a mail and set memory block to zero.
  /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
  /// @return pointer to memory block that can be filled with mail or NULL in case of error.
  /// @note MUST REMAIN UNCHANGED: @b osMailCAlloc shall be consistent in every CMSIS-RTOS.
  void*
  osMailCAlloc (osMailQId queue_id, uint32_t millisec);

  /// Put a mail to a queue.
  /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
  /// @param [in]     mail          memory block previously allocated with @ref osMailAlloc or @ref osMailCAlloc.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osMailPut shall be consistent in every CMSIS-RTOS.
  osStatus
  osMailPut (osMailQId queue_id, void* mail);

  /// Get a mail from a queue.
  /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
  /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
  /// @return event that contains mail information or error code.
  /// @note MUST REMAIN UNCHANGED: @b osMailGet shall be consistent in every CMSIS-RTOS.
  osEvent
  osMailGet (osMailQId queue_id, uint32_t millisec);

  /// Free a memory block from a mail.
  /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
  /// @param [in]     mail          pointer to the memory block that was obtained with @ref osMailGet.
  /// @return status code that indicates the execution status of the function.
  /// @note MUST REMAIN UNCHANGED: @b osMailFree shall be consistent in every CMSIS-RTOS.
  osStatus
  osMailFree (osMailQId queue_id, void* mail);

#endif  // Mail Queues available

// --------------------------------------------------------------------------
// Calls from Interrupt Service Routines
//
// The following CMSIS-RTOS functions can be called from threads and Interrupt Service Routines (ISR):
//
// - osKernelRunning
// - osSignalSet
// - osSemaphoreRelease
// - osPoolAlloc, osPoolCAlloc, osPoolFree
// - osMessagePut, osMessageGet
// - osMailAlloc, osMailCAlloc, osMailGet, osMailPut, osMailFree

#ifdef  __cplusplus
}
#endif

#endif  // _CMSIS_OS_H
