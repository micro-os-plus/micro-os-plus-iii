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

// Include CMSIS++ OS implementation definitions.
#include <cmsis-plus/rtos/os-impl.h>

#include <cstdint>
#include <cstddef>

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

      /**
       * Type of status code values returned by CMSIS-RTOS functions.
       */
      using result_t = uint32_t;

      namespace result
      {
        // Explicit namespace preferred over scoped enum,
        // otherwise too many casts are required.
        enum
          : result_t
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
      } /* namespace result */

      // ----------------------------------------------------------------------

      using millis_t = uint32_t;
      using systicks_t = uint32_t;
      using duration_t = uint32_t;

      constexpr systicks_t WAIT_FOREVER = 0xFFFFFFFF;

      class Thread;
      class Mail_queue;
      class Message_queue;

      using signal_flags_t = int32_t;

      // TODO: Get rid of this ugly structure.
#if 1
      /// Event structure contains detailed information about an event.
      using event_t = struct event_s
        {
          result_t status; ///< result code: event or error information
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

      //  ==== Kernel Control Functions ====

      namespace kernel
      {
        /**
         * @brief Initialise RTOS kernel.
         * @return result code that indicates the execution status of the function.
         */
        result_t
        initialize (void);

        /**
         * @brief Get an error string.
         * @param [in] res an integer result code.
         * @return a null terminated string.
         */
        const char*
        strerror (result_t res);

        bool
        is_in_irq (void);

      } /* namespace kernel */

      // ----------------------------------------------------------------------

      class Systick_clock
      {
      public:

        static constexpr uint32_t frequency_hz = 1000; // TODO: Param
        using rep = uint64_t;
        using sleep_rep = duration_t;

        /**
         * @brief Tell the relative time now.
         * @return the number of SysTick ticks since startup.
         */
        static rep
        now (void);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        using current_t = struct current_s
          {
            uint64_t ticks; // Count of SysTick ticks since core reset
            uint32_t cycles;// Count of SysTick cycles since timer reload (24 bits)
            uint32_t divisor;// SysTick reload value (24 bits)
            uint32_t core_frequency_hz;// Core clock frequency Hz
          };

#pragma GCC diagnostic pop

        /**
         * @brief Tell the current time.
         * @return number of ticks since startup.
         */
        static rep
        now (current_t* details);

        /**
         * @brief Convert microseconds to ticks.
         * @param [in] micros the number of microseconds.
         * @return number of ticks.
         */
        template<typename Rep_T>
          static constexpr uint32_t
          ticks_cast (Rep_T microsec);

        /**
         * @brief Sleep a number of ticks.
         * @param [in] ticks the number of ticks to sleep.
         * @return result code that indicates the execution status of the function.
         */
        static result_t
        sleep_for (sleep_rep ticks);
      };

      class Realtime_clock
      {
      public:

        static constexpr uint32_t frequency_hz = 1;
        using rep = uint64_t;
        using sleep_rep = duration_t;

        /**
         * @brief Tell the absolute time now.
         * @return number of seconds since 1 January 1970 00:00:00.
         */
        static uint64_t
        now (void);

        /**
         * @brief Sleep a number of seconds.
         * @param [in] secs the number of seconds to sleep.
         * @return result code that indicates the execution status of the function.
         */
        static result_t
        sleep_for (sleep_rep secs);

        // TODO: read hardware regs
        // TODO: write hw regs

        // TODO: write alarm (array)
        // TODO: read alarm

        // TODO: capabilities : nr. of alarms
      };

      // ----------------------------------------------------------------------

      namespace scheduler
      {
        using status_t = bool;

        /**
         * @brief Start the RTOS kernel.
         * @return result code that indicates the execution status of the function.
         */
        result_t
        start (void);

        /**
         * @brief Check if RTOS is running.
         * @return true if RTOS is running, false if RTOS was not started.
         */
        bool
        is_running (void);

        /**
         * @brief Lock the scheduler.
         * @return the previous status of the scheduler.
         */
        status_t
        lock (void);

        /**
         * @brief Unlock the scheduler.
         * @param [in] status the new status of the scheduler.
         * @return the previous status of the scheduler.
         */
        status_t
        unlock (status_t status);

        class Critical_section
        {
        public:

          Critical_section ();

          ~Critical_section ();

        protected:

          const status_t status_;
        };

        // TODO: move them to implementation ThreadsRegistry
        void
        __register_thread (Thread* thread);

        void
        __unregister_thread (Thread* thread);

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

      class Critical_section_irq
      {
      public:

        Critical_section_irq ();

        ~Critical_section_irq ();

      protected:

        const critical::status_t status_;
      };

      // ----------------------------------------------------------------------

      //  ==== Thread Management ====

      class Thread;

      namespace this_thread
      {
        /**
         * @brief Get current thread.
         * @return a reference to the current running thread.
         */
        Thread&
        get (void);

        /**
         * @brief Yield control to next thread.
         * @return result code that indicates the execution status of the function.
         */
        result_t
        yield (void);

        /// Legacy
        /// Wait for Signal, Message, Mail, or Timeout.
        /// @param [in] millisec          @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return event that contains signal, message, or mail information or error code.
        result_t
        wait (millis_t millisec, event_t* ret);

        /// Legacy
        /// Wait for one or more Signal Flags to become signaled for the current \b RUNNING thread.
        /// @param [in]     signals       wait until all specified signal flags set or 0 for any single signal flag.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return event flag information or error code.
        result_t
        wait_signals (signal_flags_t signals, millis_t millisec,
                      signal_flags_t* ret);

      } /* namespace this_thread */

      // ======================================================================

      /**
       * @brief Base class for named objects.
       */
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

        ~Named_object () = default;

        const char*
        get_name (void) const;

      protected:

        const char* const name_;
      };

      // ======================================================================

      namespace thread
      {
        /**
         * Type of priorities used for thread control.
         */
        using priority_t = uint8_t;

        // Explicit namespace preferred over scoped enum,
        // otherwise too many casts are required.
        namespace priority
        {
          enum
            : priority_t
              {
                //
            idle = 0, ///< priority: idle (lowest)
            low = 0x40, ///< priority: low
            below_normal = 0x60, ///< priority: below normal
            normal = 0x80, ///< priority: normal (default)
            above_normal = 0xA0, ///< priority: above normal
            high = 0xC0, ///< priority: high
            realtime = 0xE0, ///< priority: realtime (highest)
            // error = 0x84 ///< system cannot determine priority or thread has illegal priority
            max = 0xFF
          };
        } /* namespace priority */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        /**
         * @brief Thread attributes.
         */
        class Attributes : public Named_object
        {
        public:

          Attributes (const char* name);

          Attributes (const Attributes&) = default;
          Attributes (Attributes&&) = default;
          Attributes&
          operator= (const Attributes&) = default;
          Attributes&
          operator= (Attributes&&) = default;

          /**
           * @brief Delete a mutex attributes.
           */
          ~Attributes () = default;

          result_t
          get_stack_address (void** stack_address) const;

          result_t
          set_stack_address (void* stack_address);

          result_t
          get_stack_size_bytes (::std::size_t* stack_size_bytes) const;

          result_t
          set_stack_size_bytes (::std::size_t stack_size_bytes);

          result_t
          get_priority (priority_t* priority) const;

          result_t
          set_priority (priority_t priority);

        protected:

          void* stack_addr_;

          ::std::size_t stack_size_bytes_;

          priority_t priority_;

        };

#pragma GCC diagnostic pop

        extern const Attributes initializer;

        using func_args_t = void*;
        using func_t = void* (*) (func_args_t args);
      }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      class Thread : public Named_object
      {
      public:

        /**
         * @brief Create a new thread with default settings.
         */
        Thread (thread::func_t function, void* args);

        /**
         * @brief Create a new thread with custom settings.
         */
        Thread (const thread::Attributes& attr, thread::func_t function,
                void* args);

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
         * @return true if the given thread is the same as this thread.
         */
        bool
        operator== (const Thread& rhs) const;

        /**
         * @brief Cancel thread execution.
         * @return if successful, return status::ok; otherwise an
         * error number is returned.
         */
        result_t
        cancel (void);

        /**
         * @brief Wait for thread termination.
         * @return if successful, return status::ok; otherwise an
         * error number is returned.
         */
        result_t
        join (void** exit_ptr);

        /**
         * @brief Detach a thread.
         * @return if successful, return status::ok; otherwise an
         * error number is returned.
         */
        result_t
        detach (void);

        /**
         * @brief Terminate thread.
         * @return -
         */
        void
        exit (void* value_ptr);

        /**
         * @brief Set dynamic scheduling priority.
         * @return if successful, return status::ok; otherwise an
         * error number is returned.
         *
         * - [EINVAL] The value of prio is invalid for the scheduling policy of the
         * specified thread.
         * - [EPERM] The caller does not have appropriate privileges to set the
         * scheduling priority of the specified thread.
         */
        result_t
        set_sched_prio (thread::priority_t prio);

        /**
         * @brief Get the current scheduling priority.
         * @return priority.
         */
        thread::priority_t
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

        // TODO: group them in a Stack object
        void* stack_addr_;
        ::std::size_t stack_size_bytes_;

        thread::func_t func_;

        thread::func_args_t func_args_;

        thread::priority_t prio_;

        // Add other internal data

      };

#pragma GCC diagnostic pop

      // ======================================================================

      namespace timer
      {
        /// Entry point of a timer call back function.
        typedef void
        (*func_t) (void* args);

        /// Timer type value for the timer definition.
        using type_t = enum class type
        : uint32_t
          {
            //
            once = 0,//
            periodic = 1//
          };
      }

      class Timer : public Named_object
      {
      public:

        /// Create a timer.
        /// @param         name          name of the timer object.
        /// @param         function      name of the timer call back function.
        /// @param [in]     type          osTimerOnce for one-shot or osTimerPeriodic for periodic behavior.
        /// @param [in]     argument      argument to the timer call back function.
        /// @return timer ID for reference by other functions or NULL in case of error.
        Timer (const char* name, timer::func_t function, timer::type_t type,
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
        result_t
        start (millis_t millisec);

        /// Stop the timer.
        /// @param [in]     timer_id      timer ID obtained by @ref osTimerCreate.
        /// @return status code that indicates the execution status of the function.
        result_t
        stop (void);

      protected:

        // Add internal data
      };

      // ======================================================================

      namespace mutex
      {
        using protocol_t = enum class protocol
        : uint8_t
          {
            //
            none = 0,//
            inherit = 1,//
            protect = 2
          };

        using robustness_t = enum class robustness
        : uint8_t
          {
            //
            stalled = 0,//
            robust = 1,//
          };

        using type_t = enum class type
        : uint8_t
          {
            //
            normal = 0,//
            errorcheck = 1,//
            recursive = 2,
          };

        using count_t = uint32_t;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        /**
         * @brief Mutex attributes.
         */
        class Attributes : public Named_object
        {
        public:

          Attributes (const char* name);

          Attributes (const Attributes&) = default;
          Attributes (Attributes&&) = default;
          Attributes&
          operator= (const Attributes&) = default;
          Attributes&
          operator= (Attributes&&) = default;

          /**
           * @brief Delete a mutex attributes.
           */
          ~Attributes () = default;

          result_t
          get_prio_ceiling (thread::priority_t* prio_ceiling) const;

          result_t
          set_prio_ceiling (thread::priority_t prio_ceiling);

          result_t
          get_protocol (mutex::protocol_t* protocol) const;

          result_t
          set_protocol (mutex::protocol_t protocol);

          result_t
          get_robustness (mutex::robustness_t* robustness) const;

          result_t
          set_robustness (mutex::robustness_t robustness);

          result_t
          get_type (mutex::type_t* type) const;

          result_t
          set_type (mutex::type_t type);

        protected:

          thread::priority_t priority_ceiling_;
          mutex::protocol_t protocol_;
          mutex::robustness_t robustness_;
          mutex::type_t type_;

        };

#pragma GCC diagnostic pop

        extern const Attributes normal_initializer;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        class Recursive_attributes : public Attributes
        {
        public:

          Recursive_attributes (const char* name);

          Recursive_attributes (const Recursive_attributes&) = default;
          Recursive_attributes (Recursive_attributes&&) = default;
          Recursive_attributes&
          operator= (const Recursive_attributes&) = default;
          Recursive_attributes&
          operator= (Recursive_attributes&&) = default;

          /**
           * @brief Delete a recursive mutex attributes.
           */
          ~Recursive_attributes () = default;

        };

#pragma GCC diagnostic pop

        extern const Recursive_attributes recursive_initializer;

      } /* namespace mutex */

      // ======================================================================

      class Mutex : public Named_object
      {
      public:

        /**
         * @brief Create and initialise a mutex with default attributes.
         */
        Mutex ();
        /**
         * @brief Create and initialise a mutex with custom attributes.
         */
        Mutex (const mutex::Attributes& attr);

        Mutex (const Mutex&) = delete;
        Mutex (Mutex&&) = delete;
        Mutex&
        operator= (const Mutex&) = delete;
        Mutex&
        operator= (Mutex&&) = delete;

        /**
         * @brief Delete a mutex.
         */
        ~Mutex ();

        bool
        operator== (const Mutex& rhs) const;

        /**
         * @brief Lock the mutex.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        lock (void);

        /**
         * @brief Try to lock the mutex.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        try_lock (void);

        /**
         * @brief Timed attempt to lock the mutex.
         * @param [in] ticks Number of ticks to wait.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        timed_lock (systicks_t ticks);

        /**
         * @brief Unlock the mutex.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        unlock (void);

        /**
         * @brief Get the priority ceiling of a mutex.
         * @param [out] prio_ceiling pointer to location where to store the priority.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        get_prio_ceiling (thread::priority_t* prio_ceiling) const;

        /**
         * @brief Set the priority ceiling of a mutex.
         * @param [in] prio_ceiling new priority.
         * @param [out] old_prio_ceiling pointer to location where to
         * store the previous priority; may be nullptr.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        set_prio_ceiling (thread::priority_t prio_ceiling,
                          thread::priority_t* old_prio_ceiling = nullptr);

        /**
         * @brief Mark state protected by robust mutex as consistent.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        consistent (void);

      protected:

        // Can be updated in different contexts (interrupts or threads)
        Thread* volatile owner_;

        // Can be updated in different contexts (interrupts or threads)
        volatile mutex::count_t count_;

        volatile thread::priority_t prio_ceiling_;

        // Constants set during construction.
        const mutex::type_t type_; // normal = 0, errorcheck = 1, recursive = 2
        const mutex::protocol_t protocol_; // none = 0, inherit = 1, protect = 2
        const mutex::robustness_t robustness_; // stalled = 0, robust = 1

      };

      // ======================================================================

      namespace cond
      {

        /**
         * @brief Condition variable attributes.
         */
        class Attributes : public Named_object
        {
        public:

          Attributes (const char* name);

          Attributes (const Attributes&) = default;
          Attributes (Attributes&&) = default;
          Attributes&
          operator= (const Attributes&) = default;
          Attributes&
          operator= (Attributes&&) = default;

          /**
           * @brief Delete a condition variable attributes.
           */
          ~Attributes () = default;
        };

        extern const Attributes initializer;
      } /* namespace cond */

      // ======================================================================

      class Condition_variable : public Named_object
      {
      public:

        /**
         * @brief Create and initialise a mutex.
         */
        Condition_variable ();
        Condition_variable (const cond::Attributes& attr);

        Condition_variable (const Condition_variable&) = delete;
        Condition_variable (Condition_variable&&) = delete;
        Condition_variable&
        operator= (const Condition_variable&) = delete;
        Condition_variable&
        operator= (Condition_variable&&) = delete;

        /**
         * @brief Delete a condition variable.
         */
        ~Condition_variable ();

        bool
        operator== (const Condition_variable& rhs) const;

        /**
         * @brief Signal a condition.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        signal (void);

        /**
         * @brief Broadcast a condition.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        broadcast (void);

        /**
         * @brief Wait on a condition.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        wait (Mutex* mutex);

        /**
         * @brief Wait on a condition with timeout.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        timed_wait (Mutex* mutex, systicks_t ticks);

      protected:

        // Add internal data

      };

      // ======================================================================

      namespace semaphore
      {
        using count_t = int32_t;
        constexpr count_t max_count_value = 0x7FFFFFFF;

        /**
         * @brief Semaphore attributes.
         */
        class Attributes : public Named_object
        {
        public:

          Attributes (const char* name);

          Attributes (const Attributes&) = default;
          Attributes (Attributes&&) = default;
          Attributes&
          operator= (const Attributes&) = default;
          Attributes&
          operator= (Attributes&&) = default;

          /**
           * @brief Delete semaphore attributes.
           */
          ~Attributes () = default;

          result_t
          get_intial_count (count_t* initial_count) const;

          result_t
          set_intial_count (count_t initial_count);

          result_t
          get_max_count (count_t* max_count) const;

          result_t
          set_max_count (count_t max_count);

        protected:

          int32_t initial_count_;

          int32_t max_count_;
        };

        extern const Attributes counting_initializer;

        /**
         * @brief Semaphore attributes.
         */
        class Binary_attributes : public Attributes
        {
        public:

          Binary_attributes (const char* name);

          Binary_attributes (const Binary_attributes&) = default;
          Binary_attributes (Binary_attributes&&) = default;
          Binary_attributes&
          operator= (const Binary_attributes&) = default;
          Binary_attributes&
          operator= (Binary_attributes&&) = default;

          /**
           * @brief Delete semaphore attributes.
           */
          ~Binary_attributes () = default;
        };

        extern const Binary_attributes binary_initializer;
      } /* namespace semaphore */

      /**
       * @class Semaphore
       * @brief POSIX semaphore.
       * @details
       * Supports both counting and binary semaphores.
       *
       * Compatible with POSIX semaphore.
       * http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html
       */
      class Semaphore : public Named_object
      {
      public:

        /**
         * @brief Create a default semaphore.
         */
        Semaphore ();

        /**
         * @brief Create custom semaphore.
         */
        Semaphore (const semaphore::Attributes& attr);

        Semaphore (const Semaphore&) = delete;
        Semaphore (Semaphore&&) = delete;
        Semaphore&
        operator= (const Semaphore&) = delete;
        Semaphore&
        operator= (Semaphore&&) = delete;

        /**
         * @brief Destroy the semaphore.
         */
        ~Semaphore ();

        bool
        operator== (const Semaphore& rhs) const;

        /**
         * @brief Post to (unlock) the semaphore.
         * @return status code that indicates the execution status of the function.
         */
        result_t
        post (void);

        /**
         * @brief Wait to lock the semaphore.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        wait ();

        /**
         * @brief Try to lock  the semaphore.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        try_wait ();

        /**
         * @brief Timed wait to lock the semaphore.
         * @param [in] ticks Number of ticks to wait.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        timed_wait (systicks_t ticks);

        /**
         * @brief Get the semaphore value.
         * @param [out] value Pointer to integer where to store the value.
         * @return If successful, return status::ok; otherwise return an
         * error number.
         */
        result_t
        get_value (semaphore::count_t* value);

      protected:

        // Can be updated in different contexts (interrupts or threads)
        volatile semaphore::count_t count_;

        // Constants set during construction.
        const semaphore::count_t max_count_;

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

        result_t
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
        result_t
        put (void* info, millis_t millisec);

        /// Get a Message or Wait for a Message from a Queue.
        /// @param [in]     queue_id      message queue ID obtained with @ref osMessageCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out.
        /// @return event information that includes status code.
        result_t
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
        result_t
        put (void* mail);

        /// Get a mail from a queue.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     millisec      @ref CMSIS_RTOS_TimeOutValue or 0 in case of no time-out
        /// @return event that contains mail information or error code.
        result_t
        get (millis_t millisec, void** ret);

        /// Free a memory block from a mail.
        /// @param [in]     queue_id      mail queue ID obtained with @ref osMailCreate.
        /// @param [in]     mail          pointer to the memory block that was obtained with @ref osMailGet.
        /// @return status code that indicates the execution status of the function.
        result_t
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

      namespace kernel
      {
        ;
      } /* namespace kernel */

      namespace scheduler
      {
        inline
        Critical_section::Critical_section () :
            status_ (lock ())
        {
          ;
        }

        inline
        Critical_section::~Critical_section ()
        {
          unlock (status_);
        }

      }
      /* namespace scheduler */

      /**
       * @details
       * Round up the microseconds value and convert to number of
       * ticks, using the SysTick frequency in Hz.
       */
      template<typename Rep_T>
        constexpr uint32_t
        Systick_clock::ticks_cast (Rep_T microsec)
        {
          // TODO: add some restrictions to match only numeric types
          return (uint32_t) ((((microsec) * ((Rep_T) frequency_hz))
              + (Rep_T) 999999ul) / (Rep_T) 1000000ul);
        }

      // ======================================================================

      inline const char*
      Named_object::get_name (void) const
      {
        return name_;
      }

      // ======================================================================

      inline
      Critical_section_irq::Critical_section_irq () :
          status_ (critical::enter ())
      {
        ;
      }

      inline
      Critical_section_irq::~Critical_section_irq ()
      {
        critical::exit (status_);
      }

      // ======================================================================

      namespace thread
      {
        inline
        Attributes::Attributes (const char* name) :
            Named_object (name)
        {
          stack_addr_ = nullptr;
          stack_size_bytes_ = 0;
          priority_ = thread::priority::normal;
        }

        inline result_t
        Attributes::get_stack_address (void** stack_address) const
        {
          if (stack_address != nullptr)
            {
              *stack_address = stack_addr_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_stack_address (void* stack_address)
        {
          stack_addr_ = stack_address;
          return result::ok;
        }

        inline result_t
        Attributes::get_stack_size_bytes (::std::size_t* stack_size_bytes) const
        {
          if (stack_size_bytes != nullptr)
            {
              *stack_size_bytes = stack_size_bytes_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_stack_size_bytes (::std::size_t stack_size_bytes)
        {
          stack_size_bytes_ = stack_size_bytes;
          return result::ok;
        }

        inline result_t
        Attributes::get_priority (priority_t* priority) const
        {
          if (priority != nullptr)
            {
              *priority = priority_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_priority (priority_t priority)
        {
          priority_ = priority;
          return result::ok;
        }

      }
      /**
       * @details
       * pthread_equal()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_equal.html
       */
      inline bool
      Thread::operator== (const Thread& rhs) const
      {
        return this == &rhs;
      }

      inline void*
      Thread::get_function_args (void)
      {
        return func_args_;
      }

      // ======================================================================

      namespace mutex
      {
        inline
        Attributes::Attributes (const char* name) :
            Named_object (name)
        {
          priority_ceiling_ = thread::priority::max;
          protocol_ = protocol::none;
          robustness_ = robustness::stalled;
          type_ = type::normal;
        }

        inline result_t
        Attributes::get_prio_ceiling (thread::priority_t* prio_ceiling) const
        {
          if (prio_ceiling != nullptr)
            {
              *prio_ceiling = priority_ceiling_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_prio_ceiling (thread::priority_t prio_ceiling)
        {
          priority_ceiling_ = prio_ceiling;
          return result::ok;
        }

        inline result_t
        Attributes::get_protocol (protocol_t* protocol) const
        {
          if (protocol != nullptr)
            {
              *protocol = protocol_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_protocol (protocol_t protocol)
        {
          protocol_ = protocol;
          return result::ok;
        }

        inline result_t
        Attributes::get_robustness (robustness_t* robustness) const
        {
          if (robustness != nullptr)
            {
              *robustness = robustness_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_robustness (robustness_t robustness)
        {
          robustness_ = robustness;
          return result::ok;
        }

        inline result_t
        Attributes::get_type (type_t* type) const
        {
          if (type != nullptr)
            {
              *type = type_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_type (type_t type)
        {
          type_ = type;
          return result::ok;
        }
      }

      // ======================================================================

      namespace mutex
      {
        inline
        Recursive_attributes::Recursive_attributes (const char* name) :
            Attributes (name)
        {
          type_ = type::recursive;
        }
      }

      // ======================================================================

      inline bool
      Mutex::operator== (const Mutex& rhs) const
      {
        return this == &rhs;
      }

      // ======================================================================

      namespace cond
      {
        inline
        Attributes::Attributes (const char* name) :
            Named_object (name)
        {
          ;
        }
      } /* namespace cond */

      // ======================================================================

      inline bool
      Condition_variable::operator== (const Condition_variable& rhs) const
      {
        return this == &rhs;
      }

      // ======================================================================

      namespace semaphore
      {
        inline
        Attributes::Attributes (const char* name) :
            Named_object (name)
        {
          initial_count_ = 0;
          max_count_ = max_count_value;
        }

        inline result_t
        Attributes::get_intial_count (count_t* initial_count) const
        {
          if (initial_count != nullptr)
            {
              *initial_count = initial_count_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_intial_count (count_t initial_count)
        {
          initial_count_ = initial_count;
          return result::ok;
        }

        inline result_t
        Attributes::get_max_count (count_t* max_count) const
        {
          if (max_count != nullptr)
            {
              *max_count = max_count_;
            }
          return result::ok;
        }

        inline result_t
        Attributes::set_max_count (count_t max_count)
        {
          max_count_ = max_count;
          return result::ok;
        }

        inline
        Binary_attributes::Binary_attributes (const char* name) :
            Attributes (name)
        {
          initial_count_ = 0;
          max_count_ = 1;
        }

      } /* namespace semaphore */

      // ======================================================================

      inline bool
      Semaphore::operator== (const Semaphore& rhs) const
      {
        return this == &rhs;
      }

// ------------------------------------------------------------------------

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
