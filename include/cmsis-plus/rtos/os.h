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
 * The code is inspired by ARM CMSIS cmsis_os.h file, v1.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_RTOS_OS_H_
#define CMSIS_PLUS_RTOS_OS_H_

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <cstddef>

namespace os
{
  namespace cmsis
  {
    namespace rtos
    {
      // ----------------------------------------------------------------------

      /// Status code values returned by CMSIS-RTOS functions.
      using return_t = uint32_t;

      ///< function completed; no error or event occurred.
      constexpr return_t RETURN_OK = 0;

      ///< function completed; events occurred.
      enum class Event
        : return_t
          {
            //
        signal = 0x08, ///< function completed; signal event occurred.
        message = 0x10, ///< function completed; message event occurred.
        mail = 0x20, ///< function completed; mail event occurred.
        timeout = 0x40, ///< function completed; timeout occurred.
      };

      ///< function completed; errors occurred.
      enum class Error
        : return_t
          {
            //
        parameter = 0x80, ///< parameter error: a mandatory parameter was missing or specified an incorrect object.
        resource = 0x81, ///< resource not available: a specified resource was not available.
        timeout_resource = 0xC1, ///< resource not available within given time: a specified resource was not available within the timeout period.
        isr = 0x82, ///< not allowed in ISR context: the function cannot be called from interrupt service routines.
        isr_recursive = 0x83, ///< function called multiple times from ISR with same object.
        priority = 0x84, ///< system cannot determine priority or thread has illegal priority.
        no_memory = 0x85, ///< system is out of memory: it was impossible to allocate or reserve memory for the operation.
        value = 0x86, ///< value of a parameter is out of range.
        os = 0xFF, ///< unspecified RTOS error: run-time error but no other error message fits.
        reserved = 0x7FFFFFFF ///< prevent from enum down-size compiler optimization.
      };

      // ----------------------------------------------------------------------

      using priority_t = uint32_t;

      /// Priorities used for thread control.
      enum class Priority
        : priority_t
          {
            //
        idle = -3, ///< priority: idle (lowest)
        low = -2, ///< priority: low
        below_normal = -1, ///< priority: below normal
        normal = 0, ///< priority: normal (default)
        above_normal = +1, ///< priority: above normal
        high = +2, ///< priority: high
        realtime = +4, ///< priority: realtime (highest)
        error = 0x84 ///< system cannot determine priority or thread has illegal priority
      };

      // ----------------------------------------------------------------------

      using timer_type_t = uint32_t;

      /// Timer type value for the timer definition.
      enum class Timer_type
        : timer_type_t
          {
            //
        once = 0, //
        periodic = 1 //
      };

      constexpr uint32_t WAIT_FOREVER = 0xFFFFFFFF;

      constexpr uint32_t MAX_SEMAPHORE_COUNT = 0xFFFFFFFF;

      class MailQueue;
      class MessageQueue;

      using signals_t = int32_t;

      /// Event structure contains detailed information about an event.
      using event_t = struct
        {
          return_t status; ///< status code: event or error information
          union
            {
              uint32_t v; ///< message as 32-bit value
              void* p;///< message or mail as void pointer
              signals_t signals;///< signal flags
            }value; ///< event value
          union
            {
              MailQueue* mail_id;     ///< mail id obtained by \ref osMailCreate
              MessageQueue* message_id;///< message id obtained by \ref osMessageCreate
            }def;                               ///< event definition
        };

      // ----------------------------------------------------------------------

      //  ==== Kernel Control Functions ====
      namespace kernel
      {
        /// Initialize the RTOS Kernel for creating objects.
        /// \return status code that indicates the execution status of the function.
        return_t
        initialise (void);

        /// Start the RTOS Kernel.
        /// \return status code that indicates the execution status of the function.
        return_t
        start (void);

        /// Check if the RTOS kernel is already started.
        /// \return 0 RTOS is not started, 1 RTOS is started.
        bool
        is_running (void);

        /// Get the RTOS kernel system timer counter.
        /// \return RTOS kernel system timer as 32-bit value
        uint32_t
        get_ticks (void);

        /// The RTOS kernel system timer frequency in Hz.
        /// \note Reflects the system timer setting and is typically defined in a configuration file.
        constexpr uint32_t SYS_TICK_FREQUENCY_HZ = 1000; // TODO: Param

        /// Convert a microseconds value to a RTOS kernel system timer value.
        /// \param         microsec     time value in microseconds.
        /// \return time value normalized to the \ref osKernelSysTickFrequency
        constexpr uint32_t
        compute_sys_ticks (uint32_t microsec)
        {
          return (((uint64_t) microsec) * SYS_TICK_FREQUENCY_HZ) / 1000000;
        }

      } /* namespace kernel */

      // ----------------------------------------------------------------------
      //  ==== Thread Management ====

      class Thread;

      namespace thread
      {
        /// Return the thread ID of the current running thread.
        /// \return thread ID for reference by other functions or NULL in case of error.
        Thread&
        get_current (void);

        /// Pass control to next thread that is in state \b READY.
        /// \return status code that indicates the execution status of the function.
        void
        yield (void);

        /// Wait for Signal, Message, Mail, or Timeout.
        /// \param[in] millisec          \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// \return event that contains signal, message, or mail information or error code.
        return_t
        wait (uint32_t millisec, event_t* ret);

        /// Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
        /// \param[in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// \return event flag information or error code.
        return_t
        wait_signals (signals_t signals, uint32_t millisec, event_t* ret);

        /// Wait for Timeout (Time Delay).
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue "Time delay" value
        /// \return status code that indicates the execution status of the function.
        return_t
        delay (uint32_t millisec);
      }

      /// Entry point of a thread.
      typedef void
      (*thread_func_t) (const void* args);

      // ======================================================================

      class Thread
      {
      public:

        /// Create a thread and add it to Active Threads and set it to state READY.
        /// \param         name         name of the thread function.
        /// \param         priority     initial priority of the thread function.
        /// \param         instances    number of possible thread instances.
        /// \param         stacksz      stack size (in bytes) requirements for the thread function.
        /// \param[in]     argument      pointer that is passed to the thread function as start argument.
        /// \return thread ID for reference by other functions or NULL in case of error.
        Thread (const char* name, thread_func_t function, priority_t prio,
                void* stack, std::size_t stack_size_bytes,
                uint32_t max_instances, const void* args);

        ~Thread ();

        /// Terminate execution of a thread and remove it from Active Threads.
        /// \param[in]     thread_id   thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
        /// \return status code that indicates the execution status of the function.
        return_t
        terminate (void);

        /// Get current priority of an active thread.
        /// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
        /// \return current priority value of the thread function.
        priority_t
        get_priority (void);

        /// Change priority of an active thread.
        /// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
        /// \param[in]     priority      new priority value for the thread function.
        /// \return status code that indicates the execution status of the function.
        priority_t
        set_priority (priority_t prio);

        /// Set the specified Signal Flags of an active thread.
        /// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
        /// \param[in]     signals       specifies the signal flags of the thread that should be set.
        /// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
        signals_t
        set_signals (signals_t signals);

        /// Clear the specified Signal Flags of an active thread.
        /// \param[in]     thread_id     thread ID obtained by \ref osThreadCreate or \ref osThreadGetId.
        /// \param[in]     signals       specifies the signal flags of the thread that shall be cleared.
        /// \return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters or call from ISR.
        signals_t
        clear_signals (signals_t signals);
      };

      /// Entry point of a timer call back function.
      typedef void
      (*timer_func_t) (const void* args);

      // ======================================================================

      class Timer
      {
      public:

        /// Create a timer.
        /// \param         name          name of the timer object.
        /// \param         function      name of the timer call back function.
        /// \param[in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
        /// \param[in]     argument      argument to the timer call back function.
        /// \return timer ID for reference by other functions or NULL in case of error.
        Timer (const char* name, timer_func_t function, timer_type_t type,
               void* args);

        /// Delete a timer that was created by \ref osTimerCreate.
        /// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
        /// \return status code that indicates the execution status of the function.
        ~Timer ();

        /// Start or restart a timer.
        /// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue "Time delay" value of the timer.
        /// \return status code that indicates the execution status of the function.
        return_t
        start (uint32_t millisec);

        /// Stop the timer.
        /// \param[in]     timer_id      timer ID obtained by \ref osTimerCreate.
        /// \return status code that indicates the execution status of the function.
        return_t
        stop (void);
      };

      // ======================================================================

      class Mutex
      {
      public:

        /// Create and Initialize a Mutex object.
        /// \param         name          name of the mutex object.
        /// \return mutex ID for reference by other functions or NULL in case of error.
        Mutex (const char* name);

        /// Delete a Mutex that was created by \ref osMutexCreate.
        /// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
        /// \return status code that indicates the execution status of the function.
        ~Mutex ();

        /// Wait until a Mutex becomes available.
        /// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// \return status code that indicates the execution status of the function.
        return_t
        wait (uint32_t millisec);

        /// Release a Mutex that was obtained by \ref osMutexWait.
        /// \param[in]     mutex_id      mutex ID obtained by \ref osMutexCreate.
        /// \return status code that indicates the execution status of the function.
        return_t
        release (void);
      };

      // ======================================================================

      class Semaphore
      {
      public:

        /// Create and Initialize a Semaphore object used for managing resources.
        /// \param         name          name of the semaphore object.
        /// \param[in]     count         number of available resources.
        /// \return semaphore ID for reference by other functions or NULL in case of error.
        Semaphore (const char* name, int32_t count, uint32_t max_count =
                       MAX_SEMAPHORE_COUNT);

        /// Delete a Semaphore that was created by \ref osSemaphoreCreate.
        /// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
        /// \return status code that indicates the execution status of the function.
        ~Semaphore ();

        /// Wait until a Semaphore token becomes available.
        /// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// \return number of available tokens, or -1 in case of incorrect parameters.
        int32_t
        wait (uint32_t millisec);

        /// Release a Semaphore token.
        /// \param[in]     semaphore_id  semaphore object referenced with \ref osSemaphoreCreate.
        /// \return status code that indicates the execution status of the function.
        return_t
        release (void);
      };

      // ======================================================================

      class Pool
      {
      public:

        /// Create and Initialize a memory pool.
        /// \param         name          name of the memory pool.
        /// \param         no            maximum number of blocks (objects) in the memory pool.
        /// \param         type          data type of a single block (object).
        /// \return memory pool ID for reference by other functions or NULL in case of error.
        Pool (const char* name, std::size_t items, uint32_t item_size,
              void* mem);

        /// Return an allocated memory block back to a specific memory pool.
        /// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
        /// \param[in]     block         address of the allocated memory block that is returned to the memory pool.
        /// \return status code that indicates the execution status of the function.
        ~Pool ();

        /// Allocate a memory block from a memory pool.
        /// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
        /// \return address of the allocated memory block or NULL in case of no memory available.
        void*
        alloc (void);

        /// Allocate a memory block from a memory pool and set memory block to zero.
        /// \param[in]     pool_id       memory pool ID obtain referenced with \ref osPoolCreate.
        /// \return address of the allocated memory block or NULL in case of no memory available.
        void*
        calloc (void);

        return_t
        free (void* block);
      };

      // ======================================================================

      class MessageQueue
      {
      public:

        /// Create and Initialize a Message Queue.
        /// \param         name          name of the queue.
        /// \param         queue_sz      maximum number of messages in the queue.
        /// \param         type          data type of a single message element (for debugger).
        /// \param[in]     thread_id     thread ID (obtained by \ref osThreadCreate or \ref osThreadGetId) or NULL.
        /// \return message queue ID for reference by other functions or NULL in case of error.
        MessageQueue (const char* name, std::size_t items, void* mem,
                      Thread* thread);

        /// Put a Message to a Queue.
        /// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
        /// \param[in]     info          message information.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// \return status code that indicates the execution status of the function.
        return_t
        put (void* info, uint32_t millisec);

        /// Get a Message or Wait for a Message from a Queue.
        /// \param[in]     queue_id      message queue ID obtained with \ref osMessageCreate.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// \return event information that includes status code.
        return_t
        get (uint32_t millisec, event_t* ret);
      };

      // ======================================================================

      class MailQueue
      {
      public:

        /// Create and Initialize mail queue.
        /// \param         name          name of the queue
        /// \param         queue_sz      maximum number of messages in queue
        /// \param         type          data type of a single message element
        /// \param[in]     thread_id     thread ID (obtained by \ref osThreadCreate or \ref osThreadGetId) or NULL.
        /// \return mail queue ID for reference by other functions or NULL in case of error.
        MailQueue (const char* name, std::size_t messages,
                   std::size_t message_size, void* mem, Thread* thread);

        /// Allocate a memory block from a mail.
        /// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// \return pointer to memory block that can be filled with mail or NULL in case of error.
        void*
        alloc (uint32_t millisec);

        /// Allocate a memory block from a mail and set memory block to zero.
        /// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// \return pointer to memory block that can be filled with mail or NULL in case of error.
        void*
        calloc (uint32_t millisec);

        /// Put a mail to a queue.
        /// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
        /// \param[in]     mail          memory block previously allocated with \ref osMailAlloc or \ref osMailCAlloc.
        /// \return status code that indicates the execution status of the function.
        return_t
        put (void* mail);

        /// Get a mail from a queue.
        /// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
        /// \param[in]     millisec      \ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// \return event that contains mail information or error code.
        return_t
        get (uint32_t millisec, event_t* ret);

        /// Free a memory block from a mail.
        /// \param[in]     queue_id      mail queue ID obtained with \ref osMailCreate.
        /// \param[in]     mail          pointer to the memory block that was obtained with \ref osMailGet.
        /// \return status code that indicates the execution status of the function.
        return_t
        free (void* mail);
      };

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
