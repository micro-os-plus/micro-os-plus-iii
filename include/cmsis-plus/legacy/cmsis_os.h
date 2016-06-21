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
 *    osSemaphores have standard behaviour
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
 *
 * The code exposes a fully compliant ARM CMSIS API in the
 * context of the CMSIS++.
 */

/*
 * Calls from Interrupt Service Routines
 *
 * The following CMSIS-RTOS functions can be called both from threads and
 * Interrupt Service Routines (ISR):
 *
 * - osKernelRunning
 * - osSignalSet
 * - osSemaphoreRelease
 * - osPoolAlloc, osPoolCAlloc, osPoolFree
 * - osMessagePut, osMessageGet
 * - osMailAlloc, osMailCAlloc, osMailGet, osMailPut, osMailFree
 */

#ifndef CMSIS_OS_H_
#define CMSIS_OS_H_

/// @note MUST REMAIN UNCHANGED: @b osCMSIS identifies the CMSIS-RTOS API version.
#define osCMSIS           0x00010002 ///< API version (main [31:16] .sub [15:0])

/// @note CAN BE CHANGED: @b osCMSIS_KERNEL identifies the underlying RTOS kernel and version number.
#define osCMSIS_KERNEL    0x00010000	 ///< RTOS identification and version (main [31:16] .sub [15:0])

/// @note MUST REMAIN UNCHANGED: @b osKernelSystemId shall be consistent in every CMSIS-RTOS.
#define osKernelSystemId "CMSIS++ V1.00" ///< RTOS identification string

/// @note MUST REMAIN UNCHANGED: @b osFeature_xxx shall be consistent in every CMSIS-RTOS.
#define osFeature_MainThread   0 ///< main thread      1=main can be thread, 0=not available
#define osFeature_Pool         1 ///< Memory Pools:    1=available, 0=not available
#define osFeature_MailQ        1 ///< Mail Queues:     1=available, 0=not available
#define osFeature_MessageQ     1 ///< Message Queues:  1=available, 0=not available
#define osFeature_Signals      8 ///< maximum number of Signal Flags available per thread
#define osFeature_Semaphore    30 ///< maximum count for @ref osSemaphoreCreate function
#define osFeature_Wait         1 ///< osWait function: 1=available, 0=not available
#define osFeature_SysTick      1 ///< osKernelSysTick functions: 1=available, 0=not available

// Include the CMSIS++ C API structures declarations.
#include <cmsis-plus/rtos/os-c-decls.h>

#include <stdint.h>
#include <stddef.h>

#ifdef  __cplusplus
extern "C"
{
#endif

// ==== Enumeration, structures, defines ====

/// Priority used for thread control.
/// @note The names MUST REMAIN UNCHANGED: @b osPriority shall be consistent in every CMSIS-RTOS.
  typedef enum
  {
    osPriorityIdle = os_thread_priority_idle, ///< priority: idle (lowest)
    osPriorityLow = os_thread_priority_low, ///< priority: low
    osPriorityBelowNormal = os_thread_priority_below_normal, ///< priority: below normal
    osPriorityNormal = os_thread_priority_normal, ///< priority: normal (default)
    osPriorityAboveNormal = os_thread_priority_above_normal, ///< priority: above normal
    osPriorityHigh = os_thread_priority_high, ///< priority: high
    osPriorityRealtime = os_thread_priority_realtime, ///< priority: realtime (highest)
    osPriorityError = os_thread_priority_error ///< system cannot determine priority or thread has illegal priority
  } osPriority;

  /**
   * @brief Timeout value.
   * @note The names MUST REMAIN UNCHANGED: @b osWaitForever shall be consistent in every CMSIS-RTOS.
   */
#define osWaitForever     0xFFFFFFFF ///< wait forever timeout value

  /**
   * @brief Status code values returned by CMSIS-RTOS functions.
   * @note The names MUST REMAIN UNCHANGED: @b osStatus shall be consistent in every CMSIS-RTOS.
   */
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

  /**
   * @brief Timer type value for the timer definition.
   * @note The names MUST REMAIN UNCHANGED: @b os_timer_type shall be consistent in every CMSIS-RTOS.
   */
  typedef enum
  {
    osTimerOnce = os_timer_once, ///< one-shot timer
    osTimerPeriodic = os_timer_periodic ///< repeating timer
  } os_timer_type;

  /**
   * @brief Entry point of a thread.
   * @note MUST REMAIN UNCHANGED: @b os_pthread shall be consistent in every CMSIS-RTOS.
   */
  typedef void
  (*os_pthread) (void const *argument);

  /**
   * @brief Entry point of a timer call back function.
   * @note MUST REMAIN UNCHANGED: @b os_ptimer shall be consistent in every CMSIS-RTOS.
   */
  typedef void
  (*os_ptimer) (void const *argument);

  // Redefine some CMSIS++ struct's to the legacy code.
  typedef os_thread_t osThread;
  typedef os_thread_attr_t osThreadAttr;

  typedef os_timer_t osTimer;
  typedef os_timer_attr_t osTimerAttr;

  typedef os_mutex_t osMutex;
  typedef os_mutex_attr_t osMutexAttr;

  typedef os_semaphore_t osSemaphore;
  typedef os_semaphore_attr_t osSemaphoreAttr;

  typedef os_mempool_t osPool;
  typedef os_mempool_attr_t osPoolAttr;

  typedef os_mqueue_t osMessageQ;
  typedef os_mqueue_attr_t osMessageQAttr;

  typedef struct os_mail_queue_s
  {
    os_mempool_t pool;
    os_mqueue_t queue;
  } os_mail_queue_t;

  typedef os_mail_queue_t osMailQ;

  /**
   * @brief Thread ID identifies the thread (pointer to a thread control block).
   * @note CAN BE CHANGED: @b os_thread_cb is implementation specific in every CMSIS-RTOS.
   */
  typedef osThread* osThreadId;

  /**
   * @brief Timer ID identifies the timer (pointer to a timer control block).
   * @note CAN BE CHANGED: @b os_timer_cb is implementation specific in every CMSIS-RTOS.
   */
  typedef osTimer* osTimerId;

  /**
   * @brief Mutex ID identifies the mutex (pointer to a mutex control block).
   * @note CAN BE CHANGED: @b os_mutex_cb is implementation specific in every CMSIS-RTOS.
   */
  typedef osMutex* osMutexId;

  /**
   * @brief Semaphore ID identifies the semaphore (pointer to a semaphore control block).
   * @note CAN BE CHANGED: @b os_semaphore_cb is implementation specific in every CMSIS-RTOS.
   */
  typedef osSemaphore* osSemaphoreId;

  /**
   * @brief Pool ID identifies the memory pool (pointer to a memory pool control block).
   * @note CAN BE CHANGED: @b os_pool_cb is implementation specific in every CMSIS-RTOS.
   */
  typedef osPool* osPoolId;

  /**
   * @brief Message ID identifies the message queue (pointer to a message queue control block).
   * @note CAN BE CHANGED: @b os_messageQ_cb is implementation specific in every CMSIS-RTOS.
   */
  typedef osMessageQ* osMessageQId;

  /**
   * @brief Mail ID identifies the mail queue (pointer to a mail queue control block).
   * @note CAN BE CHANGED: @b os_mailQ_cb is implementation specific in every CMSIS-RTOS.
   */
  typedef osMailQ* osMailQId;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Thread definition structure contains startup information of a thread.
   * @note CAN BE CHANGED: @b os_thread_def is implementation specific in every CMSIS-RTOS.
   */
  typedef struct os_thread_def
  {
    const char* name;
    os_pthread pthread; ///< start address of thread function
    osPriority tpriority; ///< initial thread priority
    uint32_t instances; ///< maximum number of instances of that thread function
    uint32_t stacksize; ///< stack size requirements in bytes; 0 is default stack size
    osThread* data;
    uint64_t* stack; // align the stack at 8 bytes
  } osThreadDef_t;

#pragma GCC diagnostic pop

  /**
   * @brief Timer definition structure contains timer parameters.
   * @note CAN BE CHANGED: @b os_timer_def is implementation specific in every CMSIS-RTOS.
   */
  typedef struct os_timer_def
  {
    const char* name;
    os_ptimer ptimer; ///< start address of a timer function
    osTimer* data;
  } osTimerDef_t;

  /**
   * @brief Mutex definition structure contains setup information for a mutex.
   * @note CAN BE CHANGED: @b os_mutex_def is implementation specific in every CMSIS-RTOS.
   */
  typedef struct os_mutex_def
  {
    const char* name;
    osMutex* data;
  } osMutexDef_t;

  /**
   * @brief Semaphore definition structure contains setup information for a semaphore.
   * @note CAN BE CHANGED: @b os_semaphore_def is implementation specific in every CMSIS-RTOS.
   */
  typedef struct os_semaphore_def
  {
    const char* name;
    osSemaphore* data;
  } osSemaphoreDef_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

  /**
   * @brief Definition structure for memory block allocation.
   * @note CAN BE CHANGED: @b os_pool_def is implementation specific in every CMSIS-RTOS.
   */
  typedef struct os_pool_def
  {
    const char* name;
    uint32_t items; ///< number of items (elements) in the pool
    uint32_t item_sz; ///< size of an item
    void* pool; ///< pointer to memory for pool
    uint32_t pool_sz;
    osPool* data;
  } osPoolDef_t;

  /**
   * @brief Definition structure for message queue.
   * @note CAN BE CHANGED: @b os_messageQ_def is implementation specific in every CMSIS-RTOS.
   */
  typedef struct os_messageQ_def
  {
    const char* name;
    uint32_t items; ///< number of elements in the queue
    uint32_t item_sz; ///< size of an item
    void* queue; ///< pointer to memory array for messages
    uint32_t queue_sz;
    osMessageQ* data;
  } osMessageQDef_t;

  /**
   * @brief Definition structure for mail queue.
   * @note CAN BE CHANGED: @b os_mailQ_def is implementation specific in every CMSIS-RTOS.
   */
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

  /**
   * @brief Event structure contains detailed information about an event.
   * @note MUST REMAIN UNCHANGED: @b os_event shall be consistent in every CMSIS-RTOS.
   *       However the struct may be extended at the end.
   */
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

#pragma GCC diagnostic pop

//  ==== Kernel Control Functions ====

  /**
   * @brief Initialize the RTOS.
   * @retval osOK The RTOS kernel was initialised.
   * @retval osErrorISR The function cannot be invoked from Interrupt Service Routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osKernelInitialize shall be consistent in every CMSIS-RTOS.
   */
  osStatus
  osKernelInitialize (void);

  /**
   * Start the RTOS scheduler.
   * @retval osOK The RTOS scheduler has been successfully started.
   * @retval osErrorISR The function cannot be invoked from Interrupt Service Routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osKernelStart shall be consistent in every CMSIS-RTOS.
   */
  osStatus
  osKernelStart (void);

  /**
   * @brief Check if the RTOS scheduler is started.
   * @retval 0 The RTOS is not started.
   * @retval 1 The RTOS is started.
   */
  int32_t
  osKernelRunning (void);

#if (defined (osFeature_SysTick)  &&  (osFeature_SysTick != 0))     // System Timer available

  /**
   * @brief Get the system timer counter.
   * @return The RTOS kernel system timer as a 32-bits value.
   *
   * @note MUST REMAIN UNCHANGED: @b osKernelSysTick shall be consistent in every CMSIS-RTOS.
   */
  uint32_t
  osKernelSysTick (void);

  /**
   * @brief The RTOS kernel system timer frequency.
   *
   * @details
   * Specifies the frequency of the Kernel SysTick timer in Hz.
   * The value is typically use to scale a time value and is for example
   * used in osKernelSysTickMicroSec.
   *
   * @note Reflects the system timer setting and is typically defined
   * in a configuration file.
   */
#define osKernelSysTickFrequency (OS_INTEGER_SYSTICK_FREQUENCY_HZ)

  /**
   * @brief Convert a microseconds value to ticks.
   *
   * @details
   * Allows to scale a microsecond value to the frequency of the
   * Kernel SysTick timer. This macro is typically used to check
   * for short timeouts in polling loops.
   *
   * @param         microsec     time value in microseconds.
   * @return time value normalized to the @ref osKernelSysTickFrequency.
   */
#define osKernelSysTickMicroSec(microsec) (((uint64_t)microsec * (osKernelSysTickFrequency)) / 1000000)

#endif    // System Timer available

//  ==== Thread Management ====

  /**
   * @brief Create a Thread Definition.
   * @param         name         name of the thread function.
   * @param         priority     initial priority of the thread function.
   * @param         instances    number of possible thread instances.
   * @param         stacksz      stack size (in bytes) requirements for the thread function.
   *
   * @details
   * Define the attributes of a thread functions that can be created
   * by the function osThreadCreate using osThread.
   *
   * @note CAN BE CHANGED: The parameters to @b osThreadDef shall be
   * consistent but the macro body is implementation specific in
   * every CMSIS-RTOS.
   */
#if defined (osObjectsExternal)  // object is external
#define osThreadDef(name, priority, instances, stacksz)  \
extern const osThreadDef_t os_thread_def_##name
#else                            // define the object
#define osThreadAllocatedDef(name, priority, instances, stacksz)  \
struct { \
    osThread data[instances]; \
} os_thread_##name; \
const osThreadDef_t os_thread_def_##name = \
{ \
    #name, \
    (os_pthread)(name), \
    (priority), \
    (instances), \
    (stacksz), \
    &os_thread_##name.data[0], \
    0 \
}
#define osThreadStaticDef(name, priority, instances, stacksz)  \
struct { \
    uint64_t stack[(instances)*((stacksz+sizeof(uint64_t)-1)/sizeof(uint64_t))]; \
    osThread data[instances]; \
} os_thread_##name; \
const osThreadDef_t os_thread_def_##name = \
{ \
    #name, \
    (os_pthread)(name), \
    (priority), \
    (instances), \
    (stacksz), \
    &os_thread_##name.data[0], \
    &os_thread_##name.stack[0] \
}
#if defined(osObjectsStatic)
#define osThreadDef(name, priority, instances, stacksz)  osThreadStaticDef(name, priority, instances, stacksz)
#else
#define osThreadDef(name, priority, instances, stacksz)  osThreadAllocatedDef(name, priority, instances, stacksz)
#endif
#endif

  /**
   * @brief Access a Thread definition.
   * @param         name          name of the thread definition object.
   *
   * @details
   * Access to the thread definition for the function osThreadCreate.
   *
   * @note CAN BE CHANGED: The parameter to @b osThread shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#define osThread(name)  \
&os_thread_def_##name

  /**
   * @brief Create a thread.
   * @param [in]     thread_def    Thread definition referenced with @ref osThread.
   * @param [in]     args      Pointer that is passed to the thread function as start argument.
   * @return thread ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osThreadCreate shall be consistent
   * in every CMSIS-RTOS.
   */
  osThreadId
  osThreadCreate (const osThreadDef_t* thread_def, void* args);

  /**
   * @brief Get the current thread.
   * @return thread ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osThreadGetId shall be consistent in
   * every CMSIS-RTOS.
   */
  osThreadId
  osThreadGetId (void);

  /**
   * @brief Terminate a thread.
   * @param [in]     thread_id   thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
   * @retval osOK The specified thread has been successfully terminated.
   * @retval osErrorParameter thread_id is incorrect.
   * @retval osErrorResource thread_id refers to a thread that is not an active thread.
   * @retval osErrorISR osThreadTerminate cannot be called from interrupt service routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osThreadTerminate shall be consistent in
   * every CMSIS-RTOS.
   */
  osStatus
  osThreadTerminate (osThreadId thread_id);

  /**
   * @brief Yield control.
   * @retval osOK The function has been correctly executed.
   * @retval osErrorISR The function cannot be called from interrupt service routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osThreadYield shall be consistent in
   * every CMSIS-RTOS.
   */
  osStatus
  osThreadYield (void);

  /**
   * @brief Change thread priority.
   * @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
   * @param [in]     priority      new priority value for the thread function.
   * @retval osOK The priority of the specified thread has been successfully changed.
   * @retval osErrorParameter thread_id is incorrect.
   * @retval osErrorValue incorrect priority value.
   * @retval osErrorResource thread_id refers to a thread that is not an active thread.
   * @retval osErrorISR osThreadSetPriority cannot be called from interrupt service routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osThreadSetPriority shall be consistent
   * in every CMSIS-RTOS.
   */
  osStatus
  osThreadSetPriority (osThreadId thread_id, osPriority priority);

  /**
   * @brief Get thread priority.
   * @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
   * @return The current priority value of the thread, or osPriorityError if error.
   *
   * @note MUST REMAIN UNCHANGED: @b osThreadGetPriority shall be consistent
   * in every CMSIS-RTOS.
   */
  osPriority
  osThreadGetPriority (osThreadId thread_id);

//  ==== Generic Wait Functions ====

  /**
   * @brief Time Delay.
   * @param [in] millisec Timeout value "time delay" value.
   * @retval osEventTimeout The time delay is executed.
   * @retval osErrorISR osDelay() cannot be called from interrupt service routines.
   */
  osStatus
  osDelay (uint32_t millisec);

#if (defined (osFeature_Wait)  &&  (osFeature_Wait != 0))     // Generic Wait available

  /**
   * @brief Wait for Signal, Message, Mail, or Timeout.
   * @param [in] millisec Timeout value or 0 in case of no time-out.
   * @retval osEventSignal A signal event occurred and is returned.
   * @retval osEventMessage A message event occurred and is returned.
   * @retval osEventMail A mail event occurred and is returned.
   * @retval osEventTimeout The time delay is executed.
   * @retval osErrorISR osWait() cannot be called from interrupt service routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osWait shall be consistent in every CMSIS-RTOS.
   */
  osEvent
  osWait (uint32_t millisec);

#endif  // Generic Wait available

//  ==== Timer Management Functions ====

  /**
   * @brief Define timer.
   * @param         name          name of the timer object.
   * @param         function      name of the timer call back function.
   *
   * @details
   * Define the attributes of a timer object.
   *
   * @note CAN BE CHANGED: The parameter to @b osTimerDef shall be consistent
   *      but the macro body is implementation specific in every CMSIS-RTOS.
   */
#if defined (osObjectsExternal)  // object is external
#define osTimerDef(name, function)  \
extern const osTimerDef_t os_timer_def_##name
#else                            // define the object
#define osTimerDef(name, function)  \
struct { \
    osTimer data; \
} os_timer_##name; \
const osTimerDef_t os_timer_def_##name = \
{ \
    #name, \
    (os_ptimer)(function), \
    &os_timer_##name.data \
}
#endif

  /**
   * @brief Access the timer.
   * @param         name          name of the timer object.
   *
   * @details
   * Provide access to the timer definition for the function osTimerCreate.
   *
   * @note CAN BE CHANGED: The parameter to @b osTimer shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#define osTimer(name) \
&os_timer_def_##name

  /**
   * @brief Create a timer.
   * @param [in] timer_def     Timer object referenced with @ref osTimer.
   * @param [in] type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
   * @param [in] args      Argument to the timer call back function.
   * @return timer ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osTimerCreate shall be consistent in every CMSIS-RTOS.
   */
  osTimerId
  osTimerCreate (const osTimerDef_t* timer_def, os_timer_type type, void* args);

  /**
   * @brief Start the timer.
   * @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
   * @param [in]     millisec      Timeout value "time delay" value of the timer.
   * @retval osOK The specified timer has been started or restarted.
   * @retval osErrorISR osTimerStart cannot be called from interrupt service routines.
   * @retval osErrorParameter timer_id is incorrect.
   *
   * @note MUST REMAIN UNCHANGED: @b osTimerStart shall be consistent in every CMSIS-RTOS.
   */
  osStatus
  osTimerStart (osTimerId timer_id, uint32_t millisec);

  /**
   * @brief Stop the timer.
   * @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
   * @retval osOK: the specified timer has been stopped.
   * @retval osErrorISR osTimerStop cannot be called from interrupt service routines.
   * @retval osErrorParameter timer_id is incorrect.
   * @retval osErrorResource the timer is not started.
   *
   * @note MUST REMAIN UNCHANGED: @b osTimerStop shall be consistent in every CMSIS-RTOS.
   */
  osStatus
  osTimerStop (osTimerId timer_id);

  /**
   * @brief Delete the timer.
   * @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
   * @retval osOK The specified timer has been deleted.
   * @retval osErrorISR osTimerDelete cannot be called from interrupt service routines.
   * @retval osErrorParameter timer_id is incorrect.
   *
   * @note MUST REMAIN UNCHANGED: @b osTimerDelete shall be consistent in every CMSIS-RTOS.
   */
  osStatus
  osTimerDelete (osTimerId timer_id);

//  ==== Signal Management ====

  /**
   * @brief Set signal flags.
   * @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
   * @param [in]     signals       specifies the signal flags of the thread that should be set.
   * @return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
   *
   * @note MUST REMAIN UNCHANGED: @b osSignalSet shall be consistent in every CMSIS-RTOS.
   */
  int32_t
  osSignalSet (osThreadId thread_id, int32_t signals);

  /**
   * @brief Clear the specified Signal Flags of an active thread.
   * @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
   * @param [in]     signals       specifies the signal flags of the thread that shall be cleared.
   * @return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters or call from ISR.
   *
   * @note MUST REMAIN UNCHANGED: @b osSignalClear shall be consistent in every CMSIS-RTOS.
   */
  int32_t
  osSignalClear (osThreadId thread_id, int32_t signals);

  /**
   * @brief Wait for one or more Signal Flags to become signaled for the current @b RUNNING thread.
   * @param [in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @retval osOK No signal received when the timeout value millisec was 0.
   * @retval osEventTimeout Signal not occurred within timeout.
   * @retval osEventSignal Signal occurred, value.signals contains the signal flags; these signal flags are cleared.
   * @retval osErrorValue The value signals is outside of the permitted range.
   * @retval osErrorISR osSignalWait cannot be called from interrupt service routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osSignalWait shall be consistent in every CMSIS-RTOS.
   */
  osEvent
  osSignalWait (int32_t signals, uint32_t millisec);

//  ==== Mutex Management ====

  /**
   * @brief Define a mutex.
   * @param         name          name of the mutex object.
   *
   * @details
   * Define a mutex object that is referenced by osMutex.
   *
   * @note CAN BE CHANGED: The parameter to @b osMutexDef shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#if defined (osObjectsExternal)  // object is external
#define osMutexDef(name)  \
extern const osMutexDef_t os_mutex_def_##name
#else                            // define the object
#define osMutexDef(name)  \
osMutex os_mutex_data_##name; \
const osMutexDef_t os_mutex_def_##name = \
{ \
    #name, \
    &os_mutex_data_##name \
}
#endif

  /**
   * @brief Access a mutex.
   * @param         name          name of the mutex object.
   *
   * @details
   * Provides osMutexCreate access to the mutex object definition.
   *
   * @note CAN BE CHANGED: The parameter to @b osMutex shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#define osMutex(name)  \
&os_mutex_def_##name

  /**
   * @brief Create a mutex.
   * @param [in]     mutex_def     mutex definition referenced with @ref osMutex.
   * @return mutex ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osMutexCreate shall be consistent
   * in every CMSIS-RTOS.
   */
  osMutexId
  osMutexCreate (const osMutexDef_t* mutex_def);

  /**
   * @brief Wait for mutex.
   * @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @retval osOK The mutex has been obtain.
   * @retval osErrorTimeoutResource: the mutex could not be obtained in the given time.
   * @retval osErrorResource The mutex could not be obtained when no timeout was specified.
   * @retval osErrorParameter The parameter mutex_id is incorrect.
   * @retval osErrorISR osMutexWait cannot be called from interrupt service routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osMutexWait shall be consistent in
   * every CMSIS-RTOS.
   */
  osStatus
  osMutexWait (osMutexId mutex_id, uint32_t millisec);

  /**
   * @brief Release the mutex.
   * @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
   * @retval osOK The mutex has been correctly released.
   * @retval osErrorResource The mutex was not obtained before.
   * @retval osErrorParameter The parameter mutex_id is incorrect.
   * @retval osErrorISR osMutexRelease cannot be called from interrupt service routines.
   *
   * @note MUST REMAIN UNCHANGED: @b osMutexRelease shall be consistent
   * in every CMSIS-RTOS.
   */
  osStatus
  osMutexRelease (osMutexId mutex_id);

  /**
   * @brief Delete the mutex.
   * @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
   * @retval osOK The mutex object has been deleted.
   * @retval osErrorISR osMutexDelete cannot be called from interrupt service routines.
   * @retval osErrorResource All tokens have already been released.
   * @retval osErrorParameter The parameter mutex_id is incorrect.
   *
   * @note MUST REMAIN UNCHANGED: @b osMutexDelete shall be consistent
   * in every CMSIS-RTOS.
   */
  osStatus
  osMutexDelete (osMutexId mutex_id);

//  ==== Semaphore Management Functions ====

#if (defined (osFeature_Semaphore)  &&  (osFeature_Semaphore != 0))     // Semaphore available

  /**
   * @brief Define a Semaphore object.
   * @param         name          name of the semaphore object.
   *
   * @note CAN BE CHANGED: The parameter to @b osSemaphoreDef shall be consistent but the
   *      macro body is implementation specific in every CMSIS-RTOS.
   */
#if defined (osObjectsExternal)  // object is external
#define osSemaphoreDef(name)  \
extern const osSemaphoreDef_t os_semaphore_def_##name
#else                            // define the object
#define osSemaphoreDef(name)  \
osSemaphore os_semaphore_data_##name; \
const osSemaphoreDef_t os_semaphore_def_##name = \
{ \
    #name, \
    &os_semaphore_data_##name \
}
#endif

  /**
   * @brief Access a Semaphore definition.
   * @param         name          name of the semaphore object.
   *
   * @note CAN BE CHANGED: The parameter to @b osSemaphore shall be
   * consistent but the macro body is implementation specific in
   * every CMSIS-RTOS.
   */
#define osSemaphore(name)  \
&os_semaphore_def_##name

  /**
   * @brief Create a semaphore.
   * @param [in]     semaphore_def semaphore definition referenced with @ref osSemaphore.
   * @param [in]     count         number of available resources.
   * @return semaphore ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osSemaphoreCreate shall be consistent
   * in every CMSIS-RTOS.
   */
  osSemaphoreId
  osSemaphoreCreate (const osSemaphoreDef_t* semaphore_def, int32_t count);

  /**
   * @brief Wait until a Semaphore token becomes available.
   * @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @return number of available tokens, or -1 in case of incorrect parameters.
   *
   * @note MUST REMAIN UNCHANGED: @b osSemaphoreWait shall be consistent
   * in every CMSIS-RTOS.
   */
  int32_t
  osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec);

  /**
   * @brief Release the semaphore.
   * @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
   * @retval osOK The semaphore has been released.
   * @retval osErrorResource All tokens have already been released.
   * @retval osErrorParameter The parameter semaphore_id is incorrect.
   *
   * @note MUST REMAIN UNCHANGED: @b osSemaphoreRelease shall be consistent
   * in every CMSIS-RTOS.
   */
  osStatus
  osSemaphoreRelease (osSemaphoreId semaphore_id);

  /**
   * @brief Delete the semaphore.
   * @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
   * @retval osOK The semaphore object has been deleted.
   * @retval osErrorISR osSemaphoreDelete cannot be called from interrupt service routines.
   * @retval osErrorResource The semaphore object could not be deleted.
   * @retval osErrorParameter The parameter semaphore_id is incorrect.
   *
   * @note MUST REMAIN UNCHANGED: @b osSemaphoreDelete shall be consistent
   * in every CMSIS-RTOS.
   */
  osStatus
  osSemaphoreDelete (osSemaphoreId semaphore_id);

#endif     // Semaphore available

//  ==== Memory Pool Management Functions ====

#if (defined (osFeature_Pool)  &&  (osFeature_Pool != 0))  // Memory Pool Management available

  /**
   * @brief Define a Memory Pool.
   * @param         name          name of the memory pool.
   * @param         items            maximum number of blocks (objects) in the memory pool.
   * @param         type          data type of a single block (object).
   *
   * @note CAN BE CHANGED: The parameter to @b osPoolDef shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#if defined (osObjectsExternal)  // object is external
#define osPoolDef(name, no, type)   \
extern const osPoolDef_t os_pool_def_##name
#else                            // define the object
#define osPoolAllocatedDef(name, items, type)   \
struct { \
    osPool data; \
} os_pool_##name; \
const osPoolDef_t os_pool_def_##name = \
{ \
    #name, \
    (items), \
    sizeof(type), \
    0, \
    0, \
    &os_pool_##name.data \
}
#define osPoolStaticDef(name, items, type)   \
struct { \
    osPool data; \
    struct { \
      type pool[items]; \
    } storage; \
} os_pool_##name; \
const osPoolDef_t os_pool_def_##name = \
{ \
    #name, \
    (items), \
    sizeof(type), \
    &os_pool_##name.storage, \
    sizeof(os_pool_##name.storage), \
    &os_pool_##name.data \
}
#if defined(osObjectsStatic)
#define osPoolDef(name, items, type) osPoolStaticDef(name, items, type)
#else
#define osPoolDef(name, items, type) osPoolAllocatedDef(name, items, type)
#endif
#endif

  /**
   * @brief Access a Memory Pool definition.
   * @param         name          name of the memory pool.
   *
   * @note CAN BE CHANGED: The parameter to @b osPool shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#define osPool(name) \
&os_pool_def_##name

  /**
   * @brief Create a memory pool.
   * @param [in]     pool_def      memory pool definition referenced with @ref osPool.
   * @return memory pool ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osPoolCreate shall be consistent in
   * every CMSIS-RTOS.
   */
  osPoolId
  osPoolCreate (const osPoolDef_t* pool_def);

  /**
   * @brief Allocate a memory block.
   * @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
   * @return address of the allocated memory block or NULL in case of no memory available.
   *
   * @note MUST REMAIN UNCHANGED: @b osPoolAlloc shall be consistent
   * in every CMSIS-RTOS.
   */
  void*
  osPoolAlloc (osPoolId pool_id);

  /**
   * @brief Allocate and clear a memory block.
   * @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
   * @return address of the allocated memory block or NULL in case of no memory available.
   *
   * @note MUST REMAIN UNCHANGED: @b osPoolCAlloc shall be consistent in every CMSIS-RTOS.
   */
  void*
  osPoolCAlloc (osPoolId pool_id);

  /**
   * @brief Free a memory block.
   * @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
   * @param [in]     block         address of the allocated memory block that is returned to the memory pool.
   * @retval osOK The memory block is released.
   * @retval osErrorValue The block does not belong to the memory pool.
   * @retval osErrorParameter A parameter is invalid or outside of a permitted range.
   *
   * @note MUST REMAIN UNCHANGED: @b osPoolFree shall be consistent in every CMSIS-RTOS.
   */
  osStatus
  osPoolFree (osPoolId pool_id, void* block);

#endif   // Memory Pool Management available

//  ==== Message Queue Management Functions ====

#if (defined (osFeature_MessageQ)  &&  (osFeature_MessageQ != 0))     // Message Queues available

  /**
   * @brief Create a Message Queue Definition.
   * @param         name          name of the queue.
   * @param         items      maximum number of messages in the queue.
   * @param         type          data type of a single message element (for debugger).
   *
   * @note CAN BE CHANGED: The parameter to @b osMessageQDef shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#if defined (osObjectsExternal)  // object is external
#define osMessageQDef(name, queue_sz, type)   \
extern const osMessageQDef_t os_messageQ_def_##name
#else                            // define the object
#define osMessageQAllocatedDef(name, items, type)   \
struct { \
    osMessageQ data; \
} os_messageQ_##name; \
const osMessageQDef_t os_messageQ_def_##name = { \
    #name, \
    (items), \
    sizeof (void*), \
    0, \
    0, \
    &os_messageQ_##name.data \
}
#define osMessageQStaticDef(name, items, type)   \
struct { \
    osMessageQ data; \
    struct { \
      void* queue[items]; \
      os_mqueue_index_t links[2 * items]; \
      os_mqueue_prio_t prios[items]; \
    } storage; \
} os_messageQ_##name; \
const osMessageQDef_t os_messageQ_def_##name = { \
    #name, \
    (items), \
    sizeof (void*), \
    &os_messageQ_##name.storage, \
    sizeof(os_messageQ_##name.storage), \
    &os_messageQ_##name.data \
}

#if defined(osObjectsStatic)
#define osMessageQDef(name, items, type) osMessageQStaticDef(name, items, type)
#else
#define osMessageQDef(name, items, type) osMessageQAllocatedDef(name, items, type)
#endif
#endif

  /**
   * @brief Access a Message Queue.
   * @param         name          name of the queue.
   *
   * @details
   * Access to the message queue definition for the function osMessageCreate.
   *
   * @note CAN BE CHANGED: The parameter to @b osMessageQ shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#define osMessageQ(name) \
&os_messageQ_def_##name

  /**
   * @brief Create a message queue.
   * @param [in]     queue_def     queue definition referenced with @ref osMessageQ.
   * @param [in]     thread_id     thread ID (obtained by @ref osThreadCreate or @ref osThreadGetId) or NULL.
   * @return message queue ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osMessageCreate shall be consistent
   * in every CMSIS-RTOS.
   */
  osMessageQId
  osMessageCreate (const osMessageQDef_t* queue_def, osThreadId thread_id);

  /**
   * @brief Put a message to a queue.
   * @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
   * @param [in]     info          message information.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @retval osOK The message is put into the queue.
   * @retval osErrorResource: no memory in the queue was available.
   * @retval osErrorTimeoutResource: no memory in the queue was available during the given time limit.
   * @retval osErrorParameter: a parameter is invalid or outside of a permitted range. status code that indicates the execution status of the function.
   *
   * @note MUST REMAIN UNCHANGED: @b osMessagePut shall be consistent
   * in every CMSIS-RTOS.
   */
  osStatus
  osMessagePut (osMessageQId queue_id, uint32_t info, uint32_t millisec);

  /**
   * @brief Get a message or Wait for a Message from a Queue.
   * @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @retval osOK No message is available in the queue and no timeout was specified.
   * @retval osEventTimeout No message has arrived during the given timeout period.
   * @retval osEventMessage Message received, value.p contains the pointer to message.
   * @retval osErrorParameter A parameter is invalid or outside of a permitted range.
   *
   * @note MUST REMAIN UNCHANGED: @b osMessageGet shall be consistent
   * in every CMSIS-RTOS.
   */
  osEvent
  osMessageGet (osMessageQId queue_id, uint32_t millisec);

#endif     // Message Queues available

//  ==== Mail Queue Management Functions ====

#if (defined (osFeature_MailQ)  &&  (osFeature_MailQ != 0))     // Mail Queues available

  /**
   * @brief Create a mail queue definition.
   * @param         name          name of the queue.
   * @param         items      maximum number of messages in queue.
   * @param         type          data type of a single message element.
   * @note CAN BE CHANGED: The parameter to @b osMailQDef shall be consistent but the
   *      macro body is implementation specific in every CMSIS-RTOS.
   */
#if defined (osObjectsExternal)  // object is external
#define osMailQDef(name, queue_sz, type) \
extern const osMailQDef_t os_mailQ_def_##name
#else                            // define the object
#define osMailQAllocatedDef(name, items, type) \
struct { \
    osMailQ data; \
    struct { \
      type pool[items]; \
    } pool_storage; \
    struct { \
      void* queue[items]; \
      os_mqueue_index_t links[2 * items]; \
      os_mqueue_prio_t prios[items]; \
    } queue_storage; \
} os_mailQ_##name; \
const osMailQDef_t os_mailQ_def_##name = { \
    #name, \
    (items), \
    sizeof (type), \
    sizeof (void*), \
    0, \
    0, \
    0, \
    0, \
    &os_mailQ_##name.data \
}
#define osMailQStaticDef(name, items, type) \
struct { \
    osMailQ data; \
    struct { \
      type pool[items]; \
    } pool_storage; \
    struct { \
      void* queue[items]; \
      os_mqueue_index_t links[2 * items]; \
      os_mqueue_prio_t prios[items]; \
    } queue_storage; \
} os_mailQ_##name; \
const osMailQDef_t os_mailQ_def_##name = { \
    #name, \
    (items), \
    sizeof (type), \
    sizeof (void*), \
    &os_mailQ_##name.pool_storage, \
    sizeof(os_mailQ_##name.pool_storage), \
    &os_mailQ_##name.queue_storage, \
    sizeof(os_mailQ_##name.queue_storage), \
    &os_mailQ_##name.data \
}
#if defined(osObjectsStatic)
#define osMailQDef(name, items, type) osMailQStaticDef(name, items, type)
#else
#define osMailQDef(name, items, type) osMailQAllocatedDef(name, items, type)
#endif
#endif

  /**
   * @brief Access a mail queue definition.
   * @param         name          name of the queue.
   *
   * @note CAN BE CHANGED: The parameter to @b osMailQ shall be consistent but the
   *       macro body is implementation specific in every CMSIS-RTOS.
   */
#define osMailQ(name)  \
&os_mailQ_def_##name

  /**
   * @brief Create a mail queue.
   * @param [in]     mail_def     reference to the mail queue definition obtain with @ref osMailQ.
   * @param [in]     thread_id     thread ID (obtained by @ref osThreadCreate or @ref osThreadGetId) or NULL.
   * @return mail queue ID for reference by other functions or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osMailCreate shall be consistent
   * in every CMSIS-RTOS.
   */
  osMailQId
  osMailCreate (const osMailQDef_t* mail_def, osThreadId thread_id);

  /**
   * @brief Allocate a memory block from a mail.
   * @param [in]     mail_id      mail queue ID obtained with @ref osMailCreate.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @return pointer to memory block that can be filled with mail or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osMailAlloc shall be consistent
   * in every CMSIS-RTOS.
   */
  void*
  osMailAlloc (osMailQId mail_id, uint32_t millisec);

  /**
   * @brief Allocate and clear a memory block from a mail.
   * @param [in]     mail_id      mail queue ID obtained with @ref osMailCreate.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @return pointer to memory block that can be filled with mail or NULL in case of error.
   *
   * @note MUST REMAIN UNCHANGED: @b osMailCAlloc shall be consistent
   * in every CMSIS-RTOS.
   */
  void*
  osMailCAlloc (osMailQId mail_id, uint32_t millisec);

  /**
   * @brief Put a mail to a queue.
   * @param [in]     mail_id      mail queue ID obtained with @ref osMailCreate.
   * @param [in]     mail          memory block previously allocated with @ref osMailAlloc or @ref osMailCAlloc.
   * @retval osOK The message is put into the queue.
   * @retval osErrorValue Mail was previously not allocated as memory slot.
   * @retval osErrorParameter A parameter is invalid or outside of a permitted range.
   *
   * @note MUST REMAIN UNCHANGED: @b osMailPut shall be consistent in
   * every CMSIS-RTOS.
   */
  osStatus
  osMailPut (osMailQId mail_id, void* mail);

  /**
   * @brief Get a mail from a queue.
   * @param [in]     mail_id      mail queue ID obtained with @ref osMailCreate.
   * @param [in]     millisec      Timeout value or 0 in case of no time-out.
   * @retval osOK No mail is available in the queue and no timeout was specified.
   * @retval osEventTimeout No mail has arrived during the given timeout period.
   * @retval osEventMail Mail received, value.p contains the pointer to mail content.
   * @retval osErrorParameter A parameter is invalid or outside of a permitted range.
   *
   * @note MUST REMAIN UNCHANGED: @b osMailGet shall be consistent
   * in every CMSIS-RTOS.
   */
  osEvent
  osMailGet (osMailQId mail_id, uint32_t millisec);

  /**
   * @brief Free a memory block from a mail.
   * @param [in]     mail_id      mail queue ID obtained with @ref osMailCreate.
   * @param [in]     mail          pointer to the memory block that was obtained with @ref osMailGet.
   * @retval osOK The mail block is released.
   * @retval osErrorValue Mail block does not belong to the mail queue pool.
   * @retval osErrorParameter The value to the parameter queue_id is incorrect.
   *
   * @note MUST REMAIN UNCHANGED: @b osMailFree shall be consistent in
   * every CMSIS-RTOS.
   */
  osStatus
  osMailFree (osMailQId mail_id, void* mail);

#endif  // Mail Queues available

#ifdef  __cplusplus
}
#endif

#endif  /* CMSIS_OS_H_ */
