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

#include <cmsis-plus/diag/trace.h>
#include <cstdint>
#include <cstddef>

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <cstddef>
#include <functional>
#include <memory>

namespace os
{
  namespace cmsis
  {
    namespace rtos
    {
      // ----------------------------------------------------------------------

      /// Status code values returned by CMSIS-RTOS functions.
      using status_t = uint32_t;

      namespace status
      {
        // Explicit namespace preferred over scoped enum,
        // otherwise too many casts are required.
        enum
          : status_t
            {
              //
          ///< function completed; no error or event occurred.
          ok = 0,

          ///< function completed; signal event occurred.
          event_signal = 0x08,

          ///< function completed; message event occurred.
          event_message = 0x10,

          ///< function completed; mail event occurred.
          event_mail = 0x20,

          ///< function completed; timeout occurred.
          event_timeout = 0x40,

          ///< parameter error: a mandatory parameter was missing or specified an incorrect object.
          error_parameter = 0x80,

          ///< resource not available: a specified resource was not available.
          error_resource = 0x81,

          ///< resource not available within given time: a specified resource was not available within the timeout period.
          error_timeout_resource = 0xC1,

          ///< not allowed in ISR context: the function cannot be called from interrupt service routines.
          error_isr = 0x82,

          ///< function called multiple times from ISR with same object.
          error_isr_recursive = 0x83,

          ///< system cannot determine priority or thread has illegal priority.
          error_priority = 0x84,

          ///< system is out of memory: it was impossible to allocate or reserve memory for the operation.
          error_no_memory = 0x85,

          ///< value of a parameter is out of range.
          error_value = 0x86,

          ///< unspecified RTOS error: run-time error but no other error message fits.
          error_os = 0xFF,

          ///< prevent from enum down-size compiler optimisation.
          /// (Actually redundant in C++ if the underlying type is 32 bits)
          reserved = 0x7FFFFFFF
        };
      } /* namespace status */

      // ----------------------------------------------------------------------

      using priority_t = int8_t;

      /// Priorities used for thread control.
      // Explicit namespace preferred over scoped enum,
      // otherwise too many casts are required.
      namespace priority
      {
        enum
          : priority_t
            {
              //
          idle = -30, ///< priority: idle (lowest)
          low = -20, ///< priority: low
          below_normal = -10, ///< priority: below normal
          normal = 0, ///< priority: normal (default)
          above_normal = +10, ///< priority: above normal
          high = +20, ///< priority: high
          realtime = +30 ///< priority: realtime (highest)
        // error = 0x84 ///< system cannot determine priority or thread has illegal priority
        };
      } /* namespace priority */

      // ----------------------------------------------------------------------

      /// Timer type value for the timer definition.
      enum class timer_type_t
        : uint32_t
          {
            //
        once = 0, //
        periodic = 1 //
      };

      using millis_t = uint32_t;
      using sys_ticks_t = uint32_t;

      constexpr sys_ticks_t WAIT_FOREVER = 0xFFFFFFFF;

      constexpr uint32_t MAX_SEMAPHORE_COUNT = 0xFFFFFFFF;

      class Mail_queue;
      class Message_queue;

      using signal_flags_t = int32_t;

      // TODO: Get rid of this ugly structure.
#if 1
      /// Event structure contains detailed information about an event.
      using event_t = struct event_s
        {
          status_t status; ///< status code: event or error information
          union
            {
              uint32_t v; ///< message as 32-bit value
              void* p;///< message or mail as void pointer
              signal_flags_t signals;///< signal flags
            }value; ///< event value
          union
            {
              Mail_queue* mail_id;    ///< mail id obtained by @ref osMailCreate
              Message_queue* message_id;///< message id obtained by @ref osMessageCreate
            }def;                               ///< event definition
        };
#endif

      // ----------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      typedef struct current_systick_s
      {
        uint64_t ticks; // Count of SysTick ticks since core reset
        uint32_t cycles; // Count of SysTick cycles since timer reload (24 bits)
        uint32_t divisor; // SysTick reload value (24 bits)
        uint32_t core_frequency_hz; // Core clock frequency Hz
      } current_systick_t;

#pragma GCC diagnostic pop

      //  ==== Kernel Control Functions ====
      namespace kernel
      {
        /// Initialise the RTOS Kernel for creating objects.
        /// @return status code that indicates the execution status of the function.
        status_t
        initialize (void);

        /// Start the RTOS Kernel.
        /// @return status code that indicates the execution status of the function.
        status_t
        start (void);

        /// Check if the RTOS kernel is already started.
        /// @return 0 RTOS is not started, 1 RTOS is started.
        bool
        is_running (void);

#if 0
        /// Get the RTOS kernel system timer counter.
        /// @return RTOS kernel system timer as 32-bit value
        uint32_t
        get_ticks (void);
#endif

        /// Get the current SysTick counter (ticks & cycles).
        /// @param [out] details pointer to storage where to store counters;
        /// may be null if details are not needed
        /// @return Number of ticks since reset.
        uint64_t
        get_current_systick (current_systick_t* details = nullptr);

        /// The RTOS kernel system timer frequency in Hz.
        /// \note Reflects the system timer setting and is typically defined in a configuration file.
        constexpr uint32_t SYS_TICK_FREQUENCY_HZ = 1000; // TODO: Param

        /// Convert a microseconds value to a RTOS kernel system timer value.
        /// Always round up.
        /// @param [in]  microsec     time value in microseconds.
        /// @return number of system ticks
        template<typename Rep_T>
          constexpr uint32_t
          compute_sys_ticks (Rep_T microsec)
          {
            // TODO: add some restrictions to match only numeric types
            return (uint32_t) ((((microsec) * ((Rep_T) SYS_TICK_FREQUENCY_HZ))
                + (Rep_T) 999999UL) / (Rep_T) 1000000UL);
          }

        const char*
        strerror (status_t);

      } /* namespace kernel */

      // ----------------------------------------------------------------------

      namespace scheduler
      {
        using status_t = bool;

        // Lock the scheduler (prevent it from switching threads).
        // @return The previous status of the scheduler
        status_t
        lock (void);

        // Restore the scheduler status
        status_t
        unlock (status_t status);

      } /* namespace scheduler */

      // TODO: define all levels of critical sections
      // (kernel, real-time(level), complete)
      namespace critical
      {
        using status_t = uint32_t;

        // Enter an IRQ critical section
        status_t
        enter (void);

        // Exit an IRQ critical section
        status_t
        exit (status_t status);
      }

      // ----------------------------------------------------------------------
      //  ==== Thread Management ====

      class Thread;

      namespace thread
      {
        /// Return the thread ID of the current running thread.
        /// @return thread ID for reference by other functions or NULL in case of error.
        Thread&
        get_current (void);

        /// Pass control to next thread that is in state \b READY.
        /// @return status code that indicates the execution status of the function.
        status_t
        yield (void);

        /// Wait for Signal, Message, Mail, or Timeout.
        /// @param [in] millisec          @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return event that contains signal, message, or mail information or error code.
        status_t
        wait (millis_t millisec, event_t* ret);

        /// Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
        /// @param [in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return event flag information or error code.
        status_t
        wait_signals (signal_flags_t signals, millis_t millisec,
                      signal_flags_t* ret);

#if 0
        /// Wait for Timeout (Time Delay).
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue "Time delay" value
        /// @return status code that indicates the execution status of the function.
        return_t
        delay (millis_t millisec);
#endif

        status_t
        sleep (sys_ticks_t ticks);

      }

      /// Entry point of a thread.
      typedef void
      (*Thread_func_vp) (void* args);

      // ======================================================================

      class Named_object
      {
      public:
        Named_object (const char* name);

        Named_object (const Named_object&) = delete;
        Named_object (Named_object&&) = delete;
        Named_object&
        operator= (const Named_object&) = delete;
        Named_object&
        operator= (Named_object&&) = delete;

        ~Named_object ();

        const char*
        get_name (void) const;

      protected:

        const char* const name_;
      };

      // ======================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      using thread_func_t = void* (*) (void* args);

      using thread_attr_t = struct thread_attr_s
        {
          const char* name;
          void* stack_addr;
          ::std::size_t stack_size_bytes;
          priority_t priority;
        };

      class Thread : public Named_object
      {
      public:

        /**
         * @brief Create a new tread.
         *
         * @details
         * Create a new thread, with attributes specified by attr.
         * If attr is NULL, the default attributes shall be used.
         * If the attributes specified by attr are modified later,
         * the thread's attributes shall not be affected.
         *
         * The thread is created executing function with args as its
         * sole argument. If the start_routine returns, the effect
         * shall be as if there was an implicit call to exit() using
         * the return value of function as the exit status. Note that
         * the thread in which main() was originally invoked differs
         * from this. When it returns from main(), the effect shall
         * be as if there was an implicit call to exit() using the
         * return value of main() as the exit status.
         *
         * Compatible with pthread_create().
         * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html
         */
        Thread (const thread_attr_t* attr, thread_func_t function, void* args);

        // Prevent any copy or move.
        Thread (const Thread&) = delete;
        Thread (Thread&&) = delete;
        Thread&
        operator= (const Thread&) = delete;
        Thread&
        operator= (Thread&&) = delete;

        ~Thread ();

        /**
         * @brief Compare thread IDs.
         *
         * @details
         * pthread_equal()
         * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_equal.html
         *
         * @return true if the given thread is the same as this thread.
         */
        bool
        operator== (const Thread& rhs) const;

        /**
         * @brief Cancel thread execution.
         *
         * @details
         * pthread_cancel()
         * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cancel.html
         *
         *
         * The cancel() function shall not return an error code of [EINTR].
         * If an implementation detects use of a thread ID after the end
         * of its lifetime, it is recommended that the function should
         * fail and report an [ESRCH] error.
         *
         * @return if successful, return Return::os_ok; otherwise an
         * error number is returned.
         */
        status_t
        cancel (void);

        /**
         * @brief Wait for thread termination.
         *
         * @details
         *
         * Suspend execution of the calling thread until the target thread
         * terminates, unless the target thread has already terminated.
         * On return from a successful join() call with a non-NULL
         * exit_ptr argument, the value passed to exit() by the
         * terminating thread shall be made available in the location
         * referenced by exit_ptr. When a join() returns successfully,
         * the target thread has been terminated. The results of
         * multiple simultaneous calls to join() specifying the
         * same target thread are undefined. If the thread calling
         * join() is cancelled, then the target thread shall not be
         * detached.
         *
         * pthread_join()
         * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
         *
         * @return if successful, return Return::os_ok; otherwise an
         * error number is returned.
         *
         * The join() function may fail if:
         * [EDEADLK] A deadlock was detected.
         *
         * The join() function shall not return an error code of [EINTR].
         */
        status_t
        join (void** exit_ptr);

        /**
         * @brief Detach a thread.
         *
         * @details
         * Indicate to the implementation that storage for the thread
         * thread can be reclaimed when that thread terminates. If
         * thread has not terminated, detach() shall not cause it
         * to terminate. The behaviour is undefined if the value
         * specified by the thread argument to detach() does not
         * refer to a joinable thread.
         *
         * pthread_detach()
         * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_detach.html
         *
         *
         * @return if successful, return Return::os_ok; otherwise an
         * error number is returned.
         *
         * The detach() function shall not return an error code of [EINTR].
         */
        status_t
        detach (void);

        /**
         * @brief Terminate thread.
         *
         * @details
         * Terminate the calling thread and make the value value_ptr
         * available to any successful join with the terminating thread.
         * Any cancellation cleanup handlers that have been pushed and
         * not yet popped shall be popped in the reverse order that
         * they were pushed and then executed. After all cancellation
         * cleanup handlers have been executed, if the thread has any
         * thread-specific data, appropriate destructor functions
         * shall be called in an unspecified order. Thread termination
         * does not release any application visible process resources,
         * including, but not limited to, mutexes and file descriptors,
         * nor does it perform any process-level cleanup actions,
         * including, but not limited to, calling any atexit() routines
         * that may exist.
         * An implicit call to exit() is made when a thread other
         * than the thread in which main() was first invoked returns
         * from the start routine that was used to create it.
         * The function's return value shall serve as the thread's
         * exit status.
         * The behaviour of exit() is undefined if called from a
         * cancellation cleanup handler or destructor function that
         * was invoked as a result of either an implicit or explicit
         * call to exit().
         * After a thread has terminated, the result of access to
         * local (auto) variables of the thread is undefined.
         * Thus, references to local variables of the exiting
         * thread should not be used for the exit() value_ptr
         * parameter value.
         *
         * pthread_exit()
         * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_exit.html
         *
         * @return -
         */
        void
        exit (void* value_ptr);

        /**
         * @brief Set dynamic scheduling priority.
         *
         * @details
         * Set the scheduling priority for the thread to the value given
         * by prio.
         *
         * pthread_setschedprio()
         * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_setschedprio.html
         *
         * @return if successful, return Return::os_ok; otherwise an
         * error number is returned.
         *
         * [EINVAL]
         * The value of prio is invalid for the scheduling policy of the
         * specified thread.
         * [EPERM]
         * The caller does not have appropriate privileges to set the
         * scheduling priority of the specified thread.
         *
         * If an implementation detects use of a thread ID after the end
         * of its lifetime, it is recommended that the function should
         * fail and report an [ESRCH] error.
         *
         * The pthread_setschedprio() function shall not return an error
         * code of [EINTR].
         */
        status_t
        set_sched_prio (priority_t prio);

        /**
         * @brief Get the current scheduling priority.
         *
         * No POSIX equivalent.
         */
        priority_t
        get_sched_prio (void);

#if 0
        // ???
        result_t
        set_cancel_state (int, int *);
        result_t
        set_cancel_type (int, int *);

        result_t
        get_sched_param (int *, struct sched_param *);
        result_t
        set_sched_param (int, const struct sched_param *);

        //void test_cancel(void);
#endif

        void*
        get_function_args (void);

#if defined(TESTING)
        void
        __run_function (void);
#endif

      protected:

        ::std::size_t stack_size_bytes_;

        void* stack_addr_;

        priority_t prio_;

        thread_func_t func_;
        void* func_args_;

        // Add other internal data

      };

#pragma GCC diagnostic pop

      // ======================================================================

      /// Entry point of a timer call back function.
      typedef void
      (*timer_func_t) (const void* args);

      class Timer : public Named_object
      {
      public:

        /// Create a timer.
        /// @param         name          name of the timer object.
        /// @param         function      name of the timer call back function.
        /// @param [in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
        /// @param [in]     argument      argument to the timer call back function.
        /// @return timer ID for reference by other functions or NULL in case of error.
        Timer (const char* name, timer_func_t function, timer_type_t type,
               void* args);

        Timer (const Timer&) = delete;
        Timer (Timer&&) = delete;
        Timer&
        operator= (const Timer&) = delete;
        Timer&
        operator= (Timer&&) = delete;

        /// Delete a timer that was created by @ref osTimerCreate.
        /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
        /// @return status code that indicates the execution status of the function.
        ~Timer ();

        /// Start or restart a timer.
        /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue "Time delay" value of the timer.
        /// @return status code that indicates the execution status of the function.
        status_t
        start (millis_t millisec);

        /// Stop the timer.
        /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
        /// @return status code that indicates the execution status of the function.
        status_t
        stop (void);

      protected:

        // Add internal data
      };

      // ======================================================================

      class Mutex : public Named_object
      {
      public:

        /// Create and Initialize a Mutex object.
        /// @param         name          name of the mutex object.
        /// @return mutex ID for reference by other functions or NULL in case of error.
        Mutex (const char* name);
        Mutex ();

        Mutex (const Mutex&) = delete;
        Mutex (Mutex&&) = delete;
        Mutex&
        operator= (const Mutex&) = delete;
        Mutex&
        operator= (Mutex&&) = delete;

        /// Delete a Mutex that was created by @ref osMutexCreate.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        ~Mutex ();

        /// Wait until a Mutex becomes available.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        status_t
        wait (void);

        status_t
        try_wait (sys_ticks_t ticks = 0);

        /// Release a Mutex that was obtained by @ref osMutexWait.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        status_t
        release (void);

      protected:

        // Add internal data
      };

      // ======================================================================

      class Recursive_mutex : public Named_object
      {
      public:

        /// Create and initialize a recursive mutex object.
        /// @param         name          name of the mutex object.
        /// @return mutex ID for reference by other functions or NULL in case of error.
        Recursive_mutex (const char* name);
        Recursive_mutex ();

        Recursive_mutex (const Recursive_mutex&) = delete;
        Recursive_mutex (Recursive_mutex&&) = delete;
        Recursive_mutex&
        operator= (const Recursive_mutex&) = delete;
        Recursive_mutex&
        operator= (Recursive_mutex&&) = delete;

        /// Delete a Mutex that was created by @ref osMutexCreate.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        ~Recursive_mutex ();

        /// Wait until a Mutex becomes available.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        status_t
        wait (void);

        // Normally should not return before ticks expire if ownership
        // is not obtained.
        status_t
        try_wait (sys_ticks_t ticks = 0);

        /// Release a Mutex that was obtained by @ref osMutexWait.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        status_t
        release (void);

      protected:

        // Add internal data
      };

      // ======================================================================

      class Condition_variable : public Named_object
      {
      public:

        Condition_variable (const char* name);
        Condition_variable ();

        Condition_variable (const Condition_variable&) = delete;
        Condition_variable (Condition_variable&&) = delete;
        Condition_variable&
        operator= (const Condition_variable&) = delete;
        Condition_variable&
        operator= (Condition_variable&&) = delete;

        /// Delete a Mutex that was created by @ref osMutexCreate.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        ~Condition_variable ();

        status_t
        notify_one () noexcept;

        status_t
        notify_all () noexcept;

      protected:

        // Add internal data

      };

      // ======================================================================

      class Semaphore : public Named_object
      {
      public:

        /// Create and Initialize a Semaphore object used for managing resources.
        /// @param         name          name of the semaphore object.
        /// @param [in]     count         number of available resources.
        /// @return semaphore ID for reference by other functions or NULL in case of error.
        Semaphore (const char* name, int32_t count, uint32_t max_count =
                       MAX_SEMAPHORE_COUNT);

        Semaphore (const Semaphore&) = delete;
        Semaphore (Semaphore&&) = delete;
        Semaphore&
        operator= (const Semaphore&) = delete;
        Semaphore&
        operator= (Semaphore&&) = delete;

        /// Delete a Semaphore that was created by @ref osSemaphoreCreate.
        /// @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
        /// @return status code that indicates the execution status of the function.
        ~Semaphore ();

        /// Wait until a Semaphore token becomes available.
        /// @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return number of available tokens, or -1 in case of incorrect parameters.
        int32_t
        wait (millis_t millisec);

        /// Release a Semaphore token.
        /// @param [in]     semaphore_id  semaphore object referenced with @ref osSemaphoreCreate.
        /// @return status code that indicates the execution status of the function.
        status_t
        release (void);

      protected:

        // Add internal data
      };

      // ======================================================================

      class Pool : public Named_object
      {
      public:

        /// Create and Initialize a memory pool.
        /// @param         name          name of the memory pool.
        /// @param         no            maximum number of blocks (objects) in the memory pool.
        /// @param         type          data type of a single block (object).
        /// @return memory pool ID for reference by other functions or NULL in case of error.
        Pool (const char* name, ::std::size_t items, ::std::size_t item_size,
              void* mem);

        Pool (const Pool&) = delete;
        Pool (Pool&&) = delete;
        Pool&
        operator= (const Pool&) = delete;
        Pool&
        operator= (Pool&&) = delete;

        /// Return an allocated memory block back to a specific memory pool.
        /// @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
        /// @param [in]     block         address of the allocated memory block that is returned to the memory pool.
        /// @return status code that indicates the execution status of the function.
        ~Pool ();

        /// Allocate a memory block from a memory pool.
        /// @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
        /// @return address of the allocated memory block or NULL in case of no memory available.
        void*
        alloc (void);

        /// Allocate a memory block from a memory pool and set memory block to zero.
        /// @param [in]     pool_id       memory pool ID obtain referenced with @ref osPoolCreate.
        /// @return address of the allocated memory block or NULL in case of no memory available.
        void*
        calloc (void);

        status_t
        free (void* block);

      protected:

        // Add internal data
      };

      // ======================================================================

      class Message_queue : public Named_object
      {
      public:

        /// Create and Initialize a Message Queue.
        /// @param         name          name of the queue.
        /// @param         queue_sz      maximum number of messages in the queue.
        /// @param         type          data type of a single message element (for debugger).
        /// @param [in]    thread_id     thread ID (obtained by @ref osThreadCreate or @ref osThreadGetId) or NULL.
        /// @return message queue ID for reference by other functions or NULL in case of error.
        Message_queue (const char* name, ::std::size_t items, void* mem,
                       Thread* thread);

        Message_queue (const Message_queue&) = delete;
        Message_queue (Message_queue&&) = delete;
        Message_queue&
        operator= (const Message_queue&) = delete;
        Message_queue&
        operator= (Message_queue&&) = delete;

        ~Message_queue ();

        /// Put a Message to a Queue.
        /// @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
        /// @param [in]     info          message information.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return status code that indicates the execution status of the function.
        status_t
        put (void* info, millis_t millisec);

        /// Get a Message or Wait for a Message from a Queue.
        /// @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return event information that includes status code.
        status_t
        get (millis_t millisec, void** ret);

      protected:

        // Add internal data
      };

      // ======================================================================

      class Mail_queue : public Named_object
      {
      public:

        /// Create and Initialize mail queue.
        /// @param         name          name of the queue
        /// @param         queue_sz      maximum number of messages in queue
        /// @param         type          data type of a single message element
        /// @param [in]    thread_id     thread ID (obtained by @ref osThreadCreate or @ref osThreadGetId) or NULL.
        /// @return mail queue ID for reference by other functions or NULL in case of error.
        Mail_queue (const char* name, ::std::size_t messages,
                    ::std::size_t message_size, void* mem, Thread* thread);

        Mail_queue (const Mail_queue&) = delete;
        Mail_queue (Mail_queue&&) = delete;
        Mail_queue&
        operator= (const Mail_queue&) = delete;
        Mail_queue&
        operator= (Mail_queue&&) = delete;

        ~Mail_queue ();

        /// Allocate a memory block from a mail.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return pointer to memory block that can be filled with mail or NULL in case of error.
        void*
        alloc (millis_t millisec);

        /// Allocate a memory block from a mail and set memory block to zero.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return pointer to memory block that can be filled with mail or NULL in case of error.
        void*
        calloc (millis_t millisec);

        /// Put a mail to a queue.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     mail          memory block previously allocated with @ref osMailAlloc or @ref osMailCAlloc.
        /// @return status code that indicates the execution status of the function.
        status_t
        put (void* mail);

        /// Get a mail from a queue.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return event that contains mail information or error code.
        status_t
        get (millis_t millisec, void** ret);

        /// Free a memory block from a mail.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     mail          pointer to the memory block that was obtained with @ref osMailGet.
        /// @return status code that indicates the execution status of the function.
        status_t
        free (void* mail);

      protected:

        // Add internal data
      };

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */

// ============================================================================
// Inline & template implementations.

namespace os
{
  namespace cmsis
  {
    namespace rtos
    {
      // ======================================================================

      inline
      Named_object::~Named_object ()
      {
        ;
      }

      inline const char*
      Named_object::get_name (void) const
      {
        return name_;
      }

      // ======================================================================

      inline bool
      Thread::operator == (const Thread& rhs) const
      {
        return this == &rhs;
      }

      inline void*
      Thread::get_function_args (void)
      {
        return func_args_;
      }

      // ======================================================================

      inline
      Mutex::Mutex () :
          Mutex
            { nullptr }
      {
        ;
      }

      // ======================================================================

      inline
      Condition_variable::Condition_variable () :
          Condition_variable
            { nullptr }
      {
        ;
      }

    // ======================================================================

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
