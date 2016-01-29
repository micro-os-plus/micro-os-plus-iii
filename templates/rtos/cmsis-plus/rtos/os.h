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
#if defined(OS_INCLUDE_CMSIS_THREAD_VARIADICS)
#include <functional>
#endif
#include <memory>

namespace os
{
  namespace cmsis
  {
    namespace rtos
    {
      // ----------------------------------------------------------------------

      /// Status code values returned by CMSIS-RTOS functions.
      using return_t = uint32_t;

      enum Return
        : return_t
          {
            //
        ///< function completed; no error or event occurred.
        os_ok = 0,

        ///< function completed; signal event occurred.
        os_event_signal = 0x08,

        ///< function completed; message event occurred.
        os_event_message = 0x10,

        ///< function completed; mail event occurred.
        os_event_mail = 0x20,

        ///< function completed; timeout occurred.
        os_event_timeout = 0x40,

        ///< parameter error: a mandatory parameter was missing or specified an incorrect object.
        os_error_parameter = 0x80,

        ///< resource not available: a specified resource was not available.
        os_error_resource = 0x81,

        ///< resource not available within given time: a specified resource was not available within the timeout period.
        os_error_timeout_resource = 0xC1,

        ///< not allowed in ISR context: the function cannot be called from interrupt service routines.
        os_error_isr = 0x82,

        ///< function called multiple times from ISR with same object.
        os_error_isr_recursive = 0x83,

        ///< system cannot determine priority or thread has illegal priority.
        os_error_priority = 0x84,

        ///< system is out of memory: it was impossible to allocate or reserve memory for the operation.
        os_error_no_memory = 0x85,

        ///< value of a parameter is out of range.
        os_error_value = 0x86,

        ///< unspecified RTOS error: run-time error but no other error message fits.
        os_error_os = 0xFF,

        ///< prevent from enum down-size compiler optimization.
        /// (Actually redundant in C++ if the underlying type is 32 bits)
        os_return_reserved = 0x7FFFFFFF
      };

      // ----------------------------------------------------------------------

      using priority_t = int32_t;

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
        realtime = +3, ///< priority: realtime (highest)
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

      using millis_t = uint32_t;
      using sys_ticks_t = uint32_t;

      constexpr sys_ticks_t WAIT_FOREVER = 0xFFFFFFFF;

      constexpr uint32_t MAX_SEMAPHORE_COUNT = 0xFFFFFFFF;

      class Mail_queue;
      class Message_queue;

      using signals_t = int32_t;

      // TODO: Get rid of this ugly structure.
#if 1
      /// Event structure contains detailed information about an event.
      using event_t = struct event_t
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
      } Current_systick;

#pragma GCC diagnostic pop

      //  ==== Kernel Control Functions ====
      namespace kernel
      {
        /// Initialise the RTOS Kernel for creating objects.
        /// @return status code that indicates the execution status of the function.
        return_t
        initialize (void);

        /// Start the RTOS Kernel.
        /// @return status code that indicates the execution status of the function.
        return_t
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
        get_current_systick (Current_systick* details = nullptr);

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
            return (uint32_t) ((((microsec) * ((Rep_T)SYS_TICK_FREQUENCY_HZ)) + (Rep_T)999999UL)
                / (Rep_T)1000000UL);
          }

        const char*
        strerror (return_t);

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
        return_t
        yield (void);

        /// Wait for Signal, Message, Mail, or Timeout.
        /// @param [in] millisec          @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return event that contains signal, message, or mail information or error code.
        return_t
        wait (millis_t millisec, event_t* ret);

        /// Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
        /// @param [in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return event flag information or error code.
        return_t
        wait_signals (signals_t signals, millis_t millisec, signals_t* ret);

#if 0
        /// Wait for Timeout (Time Delay).
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue "Time delay" value
        /// @return status code that indicates the execution status of the function.
        return_t
        delay (millis_t millisec);
#endif

        return_t
        sleep (sys_ticks_t ticks);

      }

      /// Entry point of a thread.
      typedef void
      (*Thread_func_cvp) (const void* args);

      // Other possible entry points.
      typedef void
      (*Thread_func_vp) (void* args);

      typedef void
      (*Thread_func_v) (void);

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

      class Thread : public Named_object
      {
      public:

        /// Create a thread and add it to Active Threads and set it to state READY.
        /// @param         name         name of the thread function.
        /// @param         priority     initial priority of the thread function.
        /// @param         stacksz      stack size (in bytes) requirements for the thread function.
        /// @param [in]     argument      pointer that is passed to the thread function as start argument.
        /// @return thread ID for reference by other functions or NULL in case of error.
        Thread (const char* name, void* stack, ::std::size_t stack_size_bytes,
                Priority prio, Thread_func_cvp function, const void* args);

        Thread (const char* name, void* stack, ::std::size_t stack_size_bytes,
                Priority prio, Thread_func_vp function, void* args);

        Thread (const char* name, void* stack, ::std::size_t stack_size_bytes,
                Priority prio, Thread_func_v function);

        template<typename Callable_T, typename ... Args_T>
          explicit
          Thread (const char* name, ::std::size_t stack_size_bytes,
                  Priority prio, Callable_T&& function, Args_T&&... args);

        // Prevent any copy or move.
        Thread (const Thread&) = delete;
        Thread (Thread&&) = delete;
        Thread&
        operator= (const Thread&) = delete;
        Thread&
        operator= (Thread&&) = delete;

        /// Terminate execution of a thread and remove it from Active Threads.
        /// @param [in]     thread_id   thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
        /// @return status code that indicates the execution status of the function.
        ~Thread ();

        /// Get current priority of an active thread.
        /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
        /// @return current priority value of the thread function.
        Priority
        get_priority (void);

        /// Change priority of an active thread.
        /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
        /// @param [in]     priority      new priority value for the thread function.
        /// @return status code that indicates the execution status of the function.
        Priority
        set_priority (Priority prio);

        /// Set the specified Signal Flags of an active thread.
        /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
        /// @param [in]     signals       specifies the signal flags of the thread that should be set.
        /// @return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters.
        signals_t
        set_signals (signals_t signals);

        /// Clear the specified Signal Flags of an active thread.
        /// @param [in]     thread_id     thread ID obtained by @ref osThreadCreate or @ref osThreadGetId.
        /// @param [in]     signals       specifies the signal flags of the thread that shall be cleared.
        /// @return previous signal flags of the specified thread or 0x80000000 in case of incorrect parameters or call from ISR.
        signals_t
        clear_signals (signals_t signals);

        void
        join (void);

        void
        detach (void);

#if defined(TESTING)
        void
        __run_function (void);
#endif

      protected:

        // Type of unique pointer used to store argument,
        // possibly with deleter when using bind().
        using Args_ptr = ::std::unique_ptr<void*, void (*) (void**)>;

        template<typename Binding_T>
          static void
          run_function_object (const void* binding);

        template<typename Binding_T>
          static void
          delete_function_object (const Args_ptr::element_type* func_obj);

      protected:

        Priority prio_;

        Thread_func_cvp func_ptr_;

        // Empty argument pointer with empty deleter.
        Args_ptr args_ptr_
          { nullptr, nullptr };

        // Add other internal data
      };

#pragma GCC diagnostic pop

      /// Entry point of a timer call back function.
      typedef void
      (*timer_func_t) (const void* args);

      // ======================================================================

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
        return_t
        start (millis_t millisec);

        /// Stop the timer.
        /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
        /// @return status code that indicates the execution status of the function.
        return_t
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
        return_t
        wait (void);

        return_t
        try_wait (sys_ticks_t ticks = 0);

        /// Release a Mutex that was obtained by @ref osMutexWait.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        return_t
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
        return_t
        wait (void);

        // Normally should not return before ticks expire if ownership
        // is not obtained.
        return_t
        try_wait (sys_ticks_t ticks = 0);

        /// Release a Mutex that was obtained by @ref osMutexWait.
        /// @param [in]     mutex_id      mutex ID obtained by @ref osMutexCreate.
        /// @return status code that indicates the execution status of the function.
        return_t
        release (void);

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
        return_t
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

        return_t
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
        return_t
        put (void* info, millis_t millisec);

        /// Get a Message or Wait for a Message from a Queue.
        /// @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return event information that includes status code.
        return_t
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
        return_t
        put (void* mail);

        /// Get a mail from a queue.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return event that contains mail information or error code.
        return_t
        get (millis_t millisec, void** ret);

        /// Free a memory block from a mail.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     mail          pointer to the memory block that was obtained with @ref osMailGet.
        /// @return status code that indicates the execution status of the function.
        return_t
        free (void* mail);

      protected:

        // Add internal data
      };

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

      inline
      Thread::Thread (const char* name, void* stack,
                      ::std::size_t stack_size_bytes, Priority prio,
                      Thread_func_vp function, void* args) :
          Thread
            { name, stack, stack_size_bytes, prio, (Thread_func_cvp) function,
                (const void*) args }
      {
        ;
      }

      inline
      Thread::Thread (const char* name, void* stack,
                      ::std::size_t stack_size_bytes, Priority prio,
                      Thread_func_v function) :
          Thread
            { name, stack, stack_size_bytes, prio, (Thread_func_cvp) function,
                (const void*) nullptr }
      {
        ;
      }

      template<typename F_T>
        void
        Thread::run_function_object (const void* binding)
        {
          using Function_object = F_T;

          Function_object* b = (Function_object*) binding;
          (*b) ();
        }

      template<typename F_T>
        void
        Thread::delete_function_object (const Args_ptr::element_type* func_obj)
        {
          using Function_object = F_T;

          Function_object* b = (Function_object*) func_obj;

          // os::trace::printf ("%s\n", __PRETTY_FUNCTION__);

          // The delete now has the knowledge required to
          // correctly delete the object (i.e. the object size).
          delete b;
        }

      template<typename Callable_T, typename ... Args_T>
        Thread::Thread (const char* name, ::std::size_t stack_size_bytes,
                        Priority prio, Callable_T&& function, Args_T&&... args) :
            Thread
              { name, (void*) nullptr, stack_size_bytes, prio,
                  (Thread_func_cvp) nullptr, (const void*) nullptr }
        {
          using Function_object = decltype(::std::bind (::std::forward<Callable_T> (function),
                  ::std::forward<Args_T>(args)...));

          // Dynamic allocation! The size depends on the number of arguments.
          // This creates a small problem, since both running the function
          // and deleting the object requires the type. It is passes as
          // template functions.
          Function_object* funct_obj = new Function_object (
              ::std::bind (::std::forward<Callable_T> (function),
                           ::std::forward<Args_T>(args)...));

          // The function to start the thread is a custom proxy that
          // knows how to get the variadic arguments.
          func_ptr_ = &run_function_object<Function_object>;

          // Create a unique_ptr with the raw pointer to the function
          // and a custom deleter.
          Args_ptr ap
            { (Args_ptr::element_type*) funct_obj,
                (Args_ptr::deleter_type) &delete_function_object<Function_object> };

          // Move pointer to thread storage.
          args_ptr_ = ::std::move (ap);
        }

      inline
      Mutex::Mutex () :
          Mutex (nullptr)
      {
        ;
      }

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
