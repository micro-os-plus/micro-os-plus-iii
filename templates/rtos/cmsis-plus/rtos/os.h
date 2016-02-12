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
      using result_t = enum class result : uint32_t
        {
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

          eagain = error_resource,
          etimedout = error_timeout_resource,

          // The above values were preserved for compatibility with legacy
          // CMSIS, but the applications should not make any assumptions
          // on the numeric values of functions results.
          einval = 0x100,
          eintr = 0x101,
          eoverflow = 0x102,

          ///< prevent from enum down-size compiler optimisation.
          /// (Actually redundant in C++ if the underlying type is 32 bits)
          reserved = 0x7FFFFFFF
        };

      // ----------------------------------------------------------------------

      using millis_t = uint32_t;
      using systicks_t = uint32_t;
      using duration_t = uint32_t;

      class Thread;
      class Mail_queue;
      class Message_queue;

      // ----------------------------------------------------------------------

      //  ==== Kernel Control Functions ====

      namespace kernel
      {
        /**
         * @brief Initialise RTOS kernel.
         * @retval result::ok.
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
         * @brief Tell the current time.
         * @return The number of SysTick ticks since startup.
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
         * @return Accurate sampling of SysTick.
         */
        static rep
        now (current_t* details);

        /**
         * @brief Convert microseconds to ticks.
         * @param [in] micros The number of microseconds.
         * @return The number of ticks.
         */
        template<typename Rep_T>
          static constexpr uint32_t
          ticks_cast (Rep_T microsec);

        /**
         * @brief Sleep a number of ticks.
         * @param [in] ticks the number of ticks to sleep.
         * @retval result::etimedout The sleep lasted the entire duration.
         * @retval result::eintr The sleep was interrupted.
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
         * @return The number of seconds since 1 January 1970 00:00:00.
         */
        static uint64_t
        now (void);

        /**
         * @brief Sleep a number of seconds.
         * @param [in] secs the number of seconds to sleep.
         * @retval result::etimedout The sleep lasted the entire duration.
         * @retval result::eintr The sleep was interrupted.
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
         * @retval result::ok.
         */
        result_t
        start (void);

        /**
         * @brief Check if RTOS is running.
         * @retval true The RTOS is running.
         * @retval false The RTOS was not started.
         */
        bool
        is_running (void);

        /**
         * @brief Lock the scheduler.
         * @return The previous status of the scheduler.
         */
        status_t
        lock (void);

        /**
         * @brief Unlock the scheduler.
         * @param [in] status The new status of the scheduler.
         * @return The previous status of the scheduler.
         */
        status_t
        unlock (status_t status);

        class Critical_section
        {
        public:

          Critical_section ();

          Critical_section (const Critical_section&) = delete;
          Critical_section (Critical_section&&) = delete;
          Critical_section&
          operator= (const Critical_section&) = delete;
          Critical_section&
          operator= (Critical_section&&) = delete;

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

      // TODO: make template, parameter IRQ level
      class Critical_section_irq
      {
      public:

        Critical_section_irq ();

        ~Critical_section_irq ();

        Critical_section_irq (const Critical_section_irq&) = delete;
        Critical_section_irq (Critical_section_irq&&) = delete;
        Critical_section_irq&
        operator= (const Critical_section_irq&) = delete;
        Critical_section_irq&
        operator= (Critical_section_irq&&) = delete;

      protected:

        const critical::status_t status_;
      };

      // ----------------------------------------------------------------------

      using event_flags_t = uint32_t;

      // This might require further refinements.

      namespace flags
      {
        /**
         * @brief Clear thread flags.
         * @param [in] thread Reference to the thread.
         * @param [in] flags The flags, as OR-ed bit mask.
         * @param [out] ret Pointer where to store previous flags; may be nullptr.
         * @reval result::ok when successful.
         */
        result_t
        set (Thread& thread, event_flags_t flags, event_flags_t* out_flags);

        /**
         * @brief Set thread flags.
         * @param [in] thread Reference to the thread.
         * @param [in] flags The signal flags, as OR-ed bit mask.
         * @param [out] ret Pointer where to store previous flags, may be nullptr.
         * @reval result::ok when successful.
         */
        result_t
        clear (Thread& thread, event_flags_t flags, event_flags_t* out_flags);

        /**
         * @brief Wait for flags.
         * @param [in] flags The flags, as OR-ed bit mask.
         * @param [out] ret Pointer where to store previous flags, may be nullptr.
         * @reval result::ok The signal condition occurred.
         */
        result_t
        wait (event_flags_t flags, event_flags_t* out_flags);

        /**
         * @brief Wait for flags.
         * @param [in] flags The flags, as OR-ed bit mask.
         * @param [out] ret Pointer where to store previous flags, may be nullptr.
         * @reval result::ok The signal condition occurred.
         */
        result_t
        try_wait (event_flags_t flags, event_flags_t* out_flags);

        /**
         * @brief Wait for flags.
         * @param [in] flags The signal flags, as OR-ed bit mask.
         * @param [out] ret Pointer where to store previous flags, may be nullptr.
         * @reval result::ok The signal condition occurred.
         * @reval result::etimedout The signal condition did not occur during the entire timeout duration.
         */
        result_t
        timed_wait (event_flags_t flags, event_flags_t* out_flags,
                    systicks_t ticks);

      } /* namespace flags */

      // ----------------------------------------------------------------------

      //  ==== Thread Management ====

      class Thread;

      namespace this_thread
      {
        /**
         * @brief Get current thread.
         * @return Reference to the current running thread.
         */
        Thread&
        get (void);

        // TODO: investigate if this can return void, and use a different
        // mechanism for eintr.

        /**
         * @brief Yield CPU to next thread.
         * @retval result::ok when successful.
         * @retval result::eintr when interrupted.
         */
        result_t
        yield (void);

        /**
         * @brief Check if wake-up due to timeout.
         * @retval true The previous sleep returned after the entire duration.
         * @retval false The previous sleep returned due to an event.
         */
        bool
        is_timeout (void);

      } /* namespace this_thread */

      // ======================================================================

      /**
       * @brief Base class for named objects.
       */
      class Named_object
      {
      public:

        Named_object (const char* name);

        Named_object (const Named_object&) = default;
        Named_object (Named_object&&) = default;
        Named_object&
        operator= (const Named_object&) = default;
        Named_object&
        operator= (Named_object&&) = default;

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

        // Explicit namespace used because values are not restricted
        // to the enumeration.
        namespace priority
        {
          enum
            : priority_t
              {
                //
            none = 0,
            idle = 1, ///< priority: idle (lowest)
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

        using state_t = enum class state : uint8_t
          {
            inactive = 0, //
            ready = 1,//
            running = 2,//
            waiting = 3//
          };

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
         * @brief Compare threads.
         * @retval true The given thread is the same as this thread.
         * @retval false The threads are different.
         */
        bool
        operator== (const Thread& rhs) const;

        /**
         * @brief Cancel thread execution.
         * @retval result::ok.
         */
        result_t
        cancel (void);

        /**
         * @brief Wait for thread termination.
         * @retval result::ok.
         */
        result_t
        join (void** exit_ptr);

        /**
         * @brief Detach a thread.
         * @retval result::ok.
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
         * @retval result::ok.
         * @retval result::einval The value of prio is invalid for the scheduling policy of the
         * specified thread.
         */
        result_t
        set_sched_prio (thread::priority_t prio);

        /**
         * @brief Get the current scheduling priority.
         * @return The thread priority.
         */
        thread::priority_t
        get_sched_prio (void);

#if 0
        // ???
        result_t
        set_cancel_state (int, int*);
        result_t
        set_cancel_type (int, int*);

        result_t
        get_sched_param (int*, struct sched_param*);
        result_t
        set_sched_param (int, const struct sched_param*);

        //void test_cancel(void);
#endif

        thread::state_t
        get_state (void);

        void
        wakeup (void);

#if 1
        void
        wakeup (result_t reason);

        //void
        //wakeup (result_t reason, event_value_t value, event_object_t object);

#endif

        void*
        get_function_args (void);

        result_t
        get_wakeup_reason (void);

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

        thread::state_t state_;
        thread::priority_t prio_;

        result_t wakeup_reason_;
        // Add other internal data

      };

#pragma GCC diagnostic pop

      // ======================================================================

      namespace timer
      {
        /// Entry point of a timer call back function.
        using func_args_t = void*;
        using func_t = void (*) (func_args_t args);

        /// Timer type value for the timer definition.
        using type_t = enum class type : uint8_t
          {
            once = 0, //
            periodic = 1//
          };

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        /**
         * @brief Timer attributes.
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
           * @brief Delete the timer attributes.
           */
          ~Attributes () = default;

          type_t
          get_type (void) const;

          void
          set_type (type_t type);

        protected:

          type_t type_;

          // Add more internal data.
        };

#pragma GCC diagnostic pop

        extern const Attributes initializer;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

        class Periodic_attributes : public Attributes
        {
        public:

          Periodic_attributes (const char* name);

          Periodic_attributes (const Periodic_attributes&) = default;
          Periodic_attributes (Periodic_attributes&&) = default;
          Periodic_attributes&
          operator= (const Periodic_attributes&) = default;
          Periodic_attributes&
          operator= (Periodic_attributes&&) = default;

          /**
           * @brief Delete the timer attributes.
           */
          ~Periodic_attributes () = default;

        };

#pragma GCC diagnostic pop

        extern const Periodic_attributes periodic_initializer;

      }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      class Timer : public Named_object
      {
      public:

        Timer (timer::func_t function, timer::func_args_t args);

        Timer (const timer::Attributes& attr, timer::func_t function,
               timer::func_args_t args);

        Timer (const Timer&) = delete;
        Timer (Timer&&) = delete;
        Timer&
        operator= (const Timer&) = delete;
        Timer&
        operator= (Timer&&) = delete;

        ~Timer ();

        /**
         * @brief Compare timers.
         * @retval true The given timer is the same as this timer.
         * @retval false The timers are different.
         */
        bool
        operator== (const Timer& rhs) const;

        /**
         * @brief Start or restart the timer.
         * @param [in] ticks The timer period, in ticks.
         * @retval result::ok The timer has been started or restarted.
         */
        result_t
        start (systicks_t ticks);

        /**
         * @brief Stop the timer.
         * @retval result::ok The timer has been stopped.
         * @retval result::eagain The timer is not yet started.
         */
        result_t
        stop (void);

      protected:

        timer::func_t func_;

        timer::func_args_t func_args_;

        timer::type_t type_;

        // Add more internal data.
      };

#pragma GCC diagnostic pop

      // ======================================================================

      namespace mutex
      {
        using protocol_t = enum class protocol : uint8_t
          {
            //
            none = 0,//
            inherit = 1,//
            protect = 2
          };

        using robustness_t = enum class robustness : uint8_t
          {
            //
            stalled = 0,//
            robust = 1,//
          };

        using type_t = enum class type : uint8_t
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

          // Add more internal data.
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

        /**
         * @brief Compare mutexes.
         * @retval true The given mutex is the same as this mutex.
         * @retval false The mutexes are different.
         */
        bool
        operator== (const Mutex& rhs) const;

        /**
         * @brief Lock the mutex.
         * @retval result::ok.
         */
        result_t
        lock (void);

        /**
         * @brief Try to lock the mutex.
         * @retval result::ok.
         */
        result_t
        try_lock (void);

        /**
         * @brief Timed attempt to lock the mutex.
         * @param [in] ticks Number of ticks to wait.
         * @retval result::ok.
         */
        result_t
        timed_lock (systicks_t ticks);

        /**
         * @brief Unlock the mutex.
         * @retval result::ok.
         */
        result_t
        unlock (void);

        /**
         * @brief Get the priority ceiling of a mutex.
         * @param [out] prio_ceiling Pointer to location where to store the priority.
         * @retval result::ok.
         */
        result_t
        get_prio_ceiling (thread::priority_t* prio_ceiling) const;

        /**
         * @brief Set the priority ceiling of a mutex.
         * @param [in] prio_ceiling new priority.
         * @param [out] old_prio_ceiling pointer to location where to
         * store the previous priority; may be nullptr.
         * @retval result::ok.
         */
        result_t
        set_prio_ceiling (thread::priority_t prio_ceiling,
                          thread::priority_t* old_prio_ceiling = nullptr);

        /**
         * @brief Mark state protected by robust mutex as consistent.
         * @retval result::ok.
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
        const mutex::type_t type_; // normal, errorcheck, recursive
        const mutex::protocol_t protocol_; // none, inherit, protect
        const mutex::robustness_t robustness_; // stalled, robust

        // Add more internal data.

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

          // Add more internal data.
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

        /**
         * @brief Compare condition variables.
         * @retval true The given condition variable is the same as this condition variable.
         * @retval false The condition variables are different.
         */
        bool
        operator== (const Condition_variable& rhs) const;

        /**
         * @brief Signal a condition.
         * @retval result::ok.
         */
        result_t
        signal (void);

        /**
         * @brief Broadcast a condition.
         * @retval result::ok.
         */
        result_t
        broadcast (void);

        /**
         * @brief Wait on a condition.
         * @retval result::ok.
         */
        result_t
        wait (Mutex* mutex);

        /**
         * @brief Wait on a condition with timeout.
         * @retval result::ok.
         */
        result_t
        timed_wait (Mutex* mutex, systicks_t ticks);

      protected:

        // Add more internal data.
      };

      // ======================================================================

      namespace semaphore
      {
        using count_t = int16_t;
        constexpr count_t max_count_value = 0x7FFF;

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

          count_t initial_count_;

          count_t max_count_;

          // Add more internal data.
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

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @class Semaphore
       * @brief POSIX semaphore.
       * @details
       * Supports both counting and binary semaphores.
       *
       * Semaphores should generally be used to synchronise with
       * events occuring on interrupts.
       *
       * For inter-thread synchronisation, to avoid cases of priority
       * inversion, more suitable are mutexes.
       *
       * Compatible with POSIX semaphores.
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

        /**
         * @brief Compare semaphores.
         * @retval true The given semaphore is the same as this semaphore.
         * @retval false The semaphores are different.
         */
        bool
        operator== (const Semaphore& rhs) const;

        /**
         * @brief Post (unlock) the semaphore.
         * @retval result::ok The semaphore was posted.
         * @retval result::eoverflow The max count was exceeded.
         */
        result_t
        post (void);

        /**
         * @brief Lock the semaphore, possibly waiting.
         * @retval result::ok The calling process successfully performed the semaphore lock operation.
         * @retval result::eintr A cancel interrupted this function.
         */
        result_t
        wait ();

        /**
         * @brief Try to lock  the semaphore.
         * @retval result::ok The calling process successfully performed the semaphore lock operation.
         * @retval result::eagain The semaphore was already locked.
         */
        result_t
        try_wait ();

        /**
         * @brief Timed wait to lock the semaphore.
         * @param [in] ticks Number of ticks to wait.
         * @retval result::ok The calling process successfully performed the semaphore lock operation.
         * @retval result::etimedout The semaphore could not be locked before the specified timeout expired.
         */
        result_t
        timed_wait (systicks_t ticks);

        /**
         * @brief Get the semaphore value.
         * @param [out] value Pointer to integer where to store the value.
         * @retval result::ok The value was returned.
         */
        result_t
        get_value (semaphore::count_t* value);

        /**
         * @brief Get the semaphore value.
         * @return The semaphore value.
         */
        semaphore::count_t
        value (void);

        result_t
        reset (void);

      protected:

        impl::Prioritised_list list_;

        const semaphore::count_t initial_count_;

        // Can be updated in different contexts (interrupts or threads)
        volatile semaphore::count_t count_;

        // Constant set during construction.
        const semaphore::count_t max_count_;

        // Add more internal data.
      };

#pragma GCC diagnostic pop

      // ======================================================================

      namespace mempool
      {
        using size_t = uint16_t;

        /**
         * @brief Memory pool attributes.
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
           * @brief Delete the memory pool attributes.
           */
          ~Attributes () = default;

          void*
          get_pool_addr (void) const;

          void
          set_pool_addr (void* addr);

        protected:

          void* pool_addr_;

          // Add more internal data.
        };

        extern const Attributes initializer;

      } /* namespace mempool */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      class Memory_pool : public Named_object
      {
      public:

        /**
         * @brief Create a memory pool with default attributes.
         * @param [in] items The maximum number of items in the pool.
         * @param [in] item_size_bytes The size of an item, in bytes.
         */
        Memory_pool (mempool::size_t blocks, mempool::size_t block_size_bytes);

        /**
         * @brief Create a memory pool with custom attributes.
         * @param [in] attr Reference to attributes object.
         * @param [in] items The maximum number of items in the pool.
         * @param [in] item_size_bytes The size of an item, in bytes.
         */
        Memory_pool (const mempool::Attributes& attr, mempool::size_t blocks,
                     mempool::size_t block_size_bytes);

        Memory_pool (const Memory_pool&) = delete;
        Memory_pool (Memory_pool&&) = delete;
        Memory_pool&
        operator= (const Memory_pool&) = delete;
        Memory_pool&
        operator= (Memory_pool&&) = delete;

        /**
         * @brief Delete the memory pool.
         */
        ~Memory_pool ();

        /**
         * @brief Compare memory pools.
         * @retval true The given memory pool is the same as this memory pool.
         * @retval false The memory pools are different.
         */
        bool
        operator== (const Memory_pool& rhs) const;

        /**
         * @brief Allocate a memory block.
         * @return Pointer to memory block.
         */
        void*
        alloc (void);

        /**
         * @brief Allocate a memory block.
         * @return Pointer to memory block, or `nullptr` if no memory available.
         */
        void*
        try_alloc (void);

        /**
         * @brief Allocate a memory block.
         * @return Pointer to memory block, or `nullptr` if timeout.
         */
        void*
        timed_alloc (systicks_t ticks);

        /**
         * @brief Free the memory block.
         * @retval result::ok The memory block was released.
         * @retval result::error_value The block does not belong to the memory pool.
         */
        result_t
        free (void* block);

        ::std::size_t
        size (void);

        ::std::size_t
        count (void);

        ::std::size_t
        block_size (void);

        bool
        is_empty (void);

        bool
        is_full (void);

        result_t
        reset (void);

      protected:

        impl::Prioritised_list list_;

        void* pool_addr_;
        mempool::size_t blocks_;
        mempool::size_t block_size_bytes_;

        mempool::size_t count_;

        // Add more internal data.
      };

#pragma GCC diagnostic pop

      // ======================================================================

      namespace mqueue
      {
        using size_t = uint16_t;
        using priority_t = uint8_t;

        /**
         * @brief Message queue attributes.
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
           * @brief Delete the timer attributes.
           */
          ~Attributes () = default;

        public:

          // Public members, no accessors and mutators required.

          void* queue_addr;
          ::std::size_t queue_size_bytes;
        };

        extern const Attributes initializer;

      } /* namespace mqueue */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      class Message_queue : public Named_object
      {
      public:
        Message_queue (mqueue::size_t msgs, mqueue::size_t msg_size_bytes);
        Message_queue (const mqueue::Attributes&attr, mqueue::size_t msgs,
                       mqueue::size_t msg_size_bytes);

        Message_queue (const Message_queue&) = delete;
        Message_queue (Message_queue&&) = delete;
        Message_queue&
        operator= (const Message_queue&) = delete;
        Message_queue&
        operator= (Message_queue&&) = delete;

        /**
         * @brief Delete the message queue.
         */
        ~Message_queue ();

        /**
         * @brief Compare memory queues.
         * @retval true The given memory queue is the same as this memory queue.
         * @retval false The memory queus are different.
         */
        bool
        operator== (const Message_queue& rhs) const;

        result_t
        send (const char* msg, ::std::size_t nbytes, mqueue::priority_t mprio);

        result_t
        try_send (const char* msg, ::std::size_t nbytes,
                  mqueue::priority_t mprio);

        result_t
        timed_send (const char* msg, ::std::size_t nbytes,
                    mqueue::priority_t mprio, systicks_t ticks);

        result_t
        receive (const char* msg, ::std::size_t nbytes,
                 mqueue::priority_t* mprio);

        result_t
        try_receive (const char* msg, ::std::size_t nbytes,
                     mqueue::priority_t* mprio);

        result_t
        timed_receive (const char* msg, ::std::size_t nbytes,
                       mqueue::priority_t* mprio, systicks_t ticks);

        ::std::size_t
        length (void);

        ::std::size_t
        size (void);

        ::std::size_t
        msg_size (void);

        bool
        is_empty (void);

        bool
        is_full (void);

        result_t
        reset (void);

      private:

        impl::Prioritised_list send_list_;
        impl::Prioritised_list receive_list_;

        void* queue_addr_;
        ::std::size_t queue_size_bytes_;

        const mqueue::size_t msgs_;
        const mqueue::size_t msg_size_bytes_;

        mqueue::size_t count_;

      };

#pragma GCC diagnostic pop

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
       * Identical threads should have the same memory address.
       *
       * Compatible with POSIX pthread_equal().
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_equal.html
       */
      inline bool
      Thread::operator== (const Thread& rhs) const
      {
        return this == &rhs;
      }

      inline thread::state_t
      Thread::get_state (void)
      {
        return state_;
      }

      inline void*
      Thread::get_function_args (void)
      {
        return func_args_;
      }

      inline result_t
      Thread::get_wakeup_reason (void)
      {
        return wakeup_reason_;
      }

      // ======================================================================

      namespace timer
      {
        inline
        Attributes::Attributes (const char* name) :
            Named_object (name)
        {
          type_ = type::once;
        }

        inline type_t
        Attributes::get_type (void) const
        {
          return type_;
        }

        inline void
        Attributes::set_type (type_t type)
        {
          type_ = type;
        }

        // ====================================================================

        inline
        Periodic_attributes::Periodic_attributes (const char* name) :
            Attributes (name)
        {
          type_ = type::periodic;
        }
      } /* namespace timer */

      // ======================================================================

      inline bool
      Timer::operator== (const Timer& rhs) const
      {
        return this == &rhs;
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

      /**
       * @details
       * Identical mutexes should have the same memory address.
       */
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

      /**
       * @details
       * Identical semaphores should have the same memory address.
       */
      inline bool
      Semaphore::operator== (const Semaphore& rhs) const
      {
        return this == &rhs;
      }

      /**
       * @details
       * If positive, the semaphore value reflects the number of available resources.
       * If negative, it counts the waiting tasks.
       */
      inline semaphore::count_t
      Semaphore::value (void)
      {
        return count_;
      }

      // ======================================================================

      namespace mempool
      {
        inline
        Attributes::Attributes (const char* name) :
            Named_object (name)
        {
          pool_addr_ = nullptr;
        }

        inline void*
        Attributes::get_pool_addr (void) const
        {
          return pool_addr_;
        }

        inline void
        Attributes::set_pool_addr (void* addr)
        {
          pool_addr_ = addr;
        }

      } /* namespace mempool */

      // ======================================================================

      /**
       * @details
       * Identical memory pools should have the same memory address.
       */
      inline bool
      Memory_pool::operator== (const Memory_pool& rhs) const
      {
        return this == &rhs;
      }

      inline ::std::size_t
      Memory_pool::size (void)
      {
        return blocks_;
      }

      inline ::std::size_t
      Memory_pool::block_size (void)
      {
        return block_size_bytes_;
      }

      inline ::std::size_t
      Memory_pool::count (void)
      {
        return count_;
      }

      inline bool
      Memory_pool::is_empty (void)
      {
        return (count () == 0);
      }

      inline bool
      Memory_pool::is_full (void)
      {
        return (count () == size ());
      }

      // ======================================================================

      namespace mqueue
      {
        inline
        Attributes::Attributes (const char* name) :
            Named_object (name)
        {
          queue_addr = nullptr;
          queue_size_bytes = 0;
        }

      } /* namespace mqueue */

      // ======================================================================

      /**
       * @details
       * Identical message queue should have the same memory address.
       */
      inline bool
      Message_queue::operator== (const Message_queue& rhs) const
      {
        return this == &rhs;
      }

      inline ::std::size_t
      Message_queue::length (void)
      {
        return count_;
      }

      inline ::std::size_t
      Message_queue::size (void)
      {
        return msgs_;
      }

      inline ::std::size_t
      Message_queue::msg_size (void)
      {
        return msg_size_bytes_;
      }

      inline bool
      Message_queue::is_empty (void)
      {
        return (length () == 0);
      }

      inline bool
      Message_queue::is_full (void)
      {
        return (length () == size ());
      }

    // ------------------------------------------------------------------------

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
