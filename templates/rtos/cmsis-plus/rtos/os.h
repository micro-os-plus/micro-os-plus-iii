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

#if !defined(OS_INTEGER_SYSTICK_FREQUENCY_HZ)
#define OS_INTEGER_SYSTICK_FREQUENCY_HZ (1000)
#endif

namespace os
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
      enum
        : result_t
          {
            //
        /**
         * Function completed; no error or event occurred.
         */
        ok = 0,

      // The rest of the errors are those defined by POSIX, in the
      // <errno.h> header.
      };
    } /* namespace result */

    // ----------------------------------------------------------------------

    using systicks_t = uint32_t;
    using duration_t = uint32_t;

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

      bool
      is_in_irq (void);

    } /* namespace kernel */

    // ----------------------------------------------------------------------

    class Systick_clock
    {
    public:

      static constexpr uint32_t frequency_hz = OS_INTEGER_SYSTICK_FREQUENCY_HZ;
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
      name (void) const;

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
         * @brief Delete thread attributes.
         */
        ~Attributes () = default;

      public:

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        void* th_stack_address;
        std::size_t th_stack_size_bytes;
        priority_t th_priority;

        // Add more attributes.
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

      // Accessors & mutators.

      /**
       * @brief Set dynamic scheduling priority.
       * @retval result::ok.
       * @retval result::einval The value of prio is invalid for the scheduling policy of the
       * specified thread.
       */
      result_t
      sched_prio (thread::priority_t prio);

      /**
       * @brief Get the current scheduling priority.
       * @return The thread priority.
       */
      thread::priority_t
      sched_prio (void);

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
      state (void);

      void
      wakeup (void);

#if 1
      void
      wakeup (result_t reason);

      //void
      //wakeup (result_t reason, event_value_t value, event_object_t object);

#endif

      void*
      function_args (void);

      // Maybe make it a structure.
      result_t
      wakeup_reason (void);

#if defined(TESTING)
      void
      __run_function (void);
#endif

    protected:

      // TODO: group them in a Stack object
      void* stack_addr_;
      std::size_t stack_size_bytes_;

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
      using type_t = enum class run : uint8_t
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
         * @brief Delete timer attributes.
         */
        ~Attributes () = default;

      public:

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        type_t tm_type;

        // Add more attributes.
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
         * @brief Delete timer attributes.
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

      public:

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        thread::priority_t mx_priority_ceiling;
        mutex::protocol_t mx_protocol;
        mutex::robustness_t mx_robustness;
        mutex::type_t mx_type;

        // Add more attributes.
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

    namespace condvar
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

      public:

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.

        // Add more attributes.
      };

      extern const Attributes initializer;

    } /* namespace condvar */

    // ======================================================================

    class Condition_variable : public Named_object
    {
    public:

      /**
       * @brief Create and initialise a mutex.
       */
      Condition_variable ();
      Condition_variable (const condvar::Attributes& attr);

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

      public:

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        count_t sm_initial_count;

        count_t sm_max_count;

        // Add more attributes.
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

      public:

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        void* mp_pool_address;

        // Add more attributes.
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

      std::size_t
      size (void);

      std::size_t
      count (void);

      std::size_t
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
        // Warning: must match the type & order of the C file header.
        void* queue_address;
        std::size_t queue_size_bytes;

        // Add more attributes.
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
      send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio);

      result_t
      try_send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio);

      result_t
      timed_send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio,
                  systicks_t ticks);

      result_t
      receive (const char* msg, std::size_t nbytes, mqueue::priority_t* mprio);

      result_t
      try_receive (const char* msg, std::size_t nbytes,
                   mqueue::priority_t* mprio);

      result_t
      timed_receive (const char* msg, std::size_t nbytes,
                     mqueue::priority_t* mprio, systicks_t ticks);

      std::size_t
      length (void);

      std::size_t
      size (void);

      std::size_t
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
      std::size_t queue_size_bytes_;

      const mqueue::size_t msgs_;
      const mqueue::size_t msg_size_bytes_;

      mqueue::size_t count_;

    };

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */

// ============================================================================
// Inline & template implementations.

namespace os
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
    Named_object::name (void) const
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
        th_stack_address = nullptr;
        th_stack_size_bytes = 0;
        th_priority = thread::priority::normal;
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
    Thread::state (void)
    {
      return state_;
    }

    inline void*
    Thread::function_args (void)
    {
      return func_args_;
    }

    inline result_t
    Thread::wakeup_reason (void)
    {
      return wakeup_reason_;
    }

    // ======================================================================

    namespace timer
    {
      inline
      Attributes::Attributes (const char* name) :
          Named_object
            { name }
      {
        this->tm_type = run::once;
      }

      // ====================================================================

      inline
      Periodic_attributes::Periodic_attributes (const char* name) :
          Attributes
            { name }
      {
        this->tm_type = run::periodic;
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
        mx_priority_ceiling = thread::priority::max;
        mx_protocol = protocol::none;
        mx_robustness = robustness::stalled;
        mx_type = type::normal;
      }

      inline
      Recursive_attributes::Recursive_attributes (const char* name) :
          Attributes (name)
      {
        mx_type = type::recursive;
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

    namespace condvar
    {
      inline
      Attributes::Attributes (const char* name) :
          Named_object
            { name }
      {
        ;
      }
    } /* namespace condvar */

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
          Named_object
            { name }
      {
        sm_initial_count = 0;
        sm_max_count = max_count_value;
      }

      inline
      Binary_attributes::Binary_attributes (const char* name) :
          Attributes (name)
      {
        sm_initial_count = 0;
        sm_max_count = 1;
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
        mp_pool_address = nullptr;
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

    inline std::size_t
    Memory_pool::size (void)
    {
      return blocks_;
    }

    inline std::size_t
    Memory_pool::block_size (void)
    {
      return block_size_bytes_;
    }

    inline std::size_t
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
        queue_address = nullptr;
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

    inline std::size_t
    Message_queue::length (void)
    {
      return count_;
    }

    inline std::size_t
    Message_queue::size (void)
    {
      return msgs_;
    }

    inline std::size_t
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
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
