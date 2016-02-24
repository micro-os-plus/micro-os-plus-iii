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
 *
 * Major improvements:
 * - no more macros required to define objects
 * - for applications that require it, allows fully static
 *   memory allocations
 * - very close to POSIX (IEEE Std 1003.1, 2013 Edition)
 *   http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html
 * - specifically designed as a convenient implementation for
 *   C++ standard thread library (ISO/IEC 14882:2011)
 * - improved usability, by defining both simple (using defaults)
 *   and complex (using attributes) object constructors (feature
 *   inspired by POSIX threads attributes)
 * - improved readability with explicit three-fold waiting functions
 *   (for example: lock(), try-lock(), timed-lock(), similar to POSIX threads)
 * - POSIX condition variable added
 * - versatile clocks added (Systick_clock, Realtime_clock)
 * - standard POSIX errors definitions used
 */

/*
 * TODO:
 * - make Thread virtual, to allow create_hook/delete_hook functionality
 * - make most classes virtual, to allow post_hook functionality
 * - event timestamps: add derived classes that capture the event timestamp
 * - add object type in base class
 * - add Wait_list in base class
 * - add libc/newlib errno() function
 * - add a separate Stack object
 *
 * Notes:
 * - try_wait(), try_sig_wait() are probably not very inspired.
 */

#ifndef CMSIS_PLUS_RTOS_OS_H_
#define CMSIS_PLUS_RTOS_OS_H_

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

// We definitely use CMSIS++ ;-)
#define OS_USE_CMSIS_PLUS

// Include the CMSIS++ OS implementation declarations. This might further
// include os-config.h, for the application specific definitions.
#include <cmsis-plus/rtos/port/os-decls.h>

#include <cmsis-plus/iso/system_error>

#include <cstdint>
#include <cstddef>

// ----------------------------------------------------------------------------

#ifdef NDEBUG           /* ANSI standard */
#define os_assert_err(__e, __er) \
  do { if (!(__e)) return __er; } while (false)
#else
#define os_assert_err(__e, __er) assert(__e)
#endif

#ifdef NDEBUG           /* ANSI standard */
#define os_assert_throw(__e, __er) \
  do { if (!(__e)) os::estd::__throw_system_error(__er, #__e); } while (false)
#else
#define os_assert_throw(__e, __er) assert(__e)
#endif

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

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

      /*
       * The rest of the errors are those defined by POSIX, in the
       * <errno.h> header. Currently in use:
       * - EPERM
       * - EINVAL
       * - EAGAIN
       * - ENOTRECOVERABLE
       * - EDEADLOCK
       * - EMSGSIZE
       */

      };
    } /* namespace result */

    // ------------------------------------------------------------------------

    using systicks_t = uint32_t;
    using duration_t = uint32_t;

    // ------------------------------------------------------------------------

    namespace scheduler
    {
      using status_t = bool;

      extern status_t is_locked_;
      extern bool is_started_;

      /**
       * @brief Initialise the RTOS.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      initialize (void);

      /**
       * @brief Start the RTOS scheduler.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      start (void);

      /**
       * @brief Check if the scheduler was started.
       * @retval true The scheduler was started.
       * @retval false The scheduler was not started.
       */
      bool
      is_started (void);

      /**
       * @brief Check if the scheduler is locked.
       * @retval true The scheduler is locked.
       * @retval false The scheduler is running (not locked).
       */
      bool
      is_locked (void);

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

      /**
       * @brief Check if in handler mode.
       * @retval true Execution is in an exception handler context.
       * @retval false Execution is in a thread context.
       */
      bool
      in_handler_mode (void);

      // ----------------------------------------------------------------------

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

    // ------------------------------------------------------------------------
    }
    /* namespace scheduler */

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

    //  ==== Thread Management ====

    class Thread;

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
        // This gives a basic range of 16 priorities, easily extensible
        // to 32, 64, 128.
        constexpr uint32_t shift = 0;

        enum
          : priority_t
            {
              //
          none = 0, // undefined
          idle = 1, // system reserved for IDLE thread
          lowest = 2, // lowest available for user code
          low = (2 << shift),
          below_normal = (4 << shift),
          normal = (6 << shift), // default
          above_normal = (8 << shift),
          high = (10 << shift),
          realtime = (12 << shift),
          highest = ((16 << shift) - 3), // highest available for user code
          isr = ((16 << shift) - 2), // system reserved for ISR deferred task
          error = ((16 << shift) - 1) // error
        };
      } /* namespace priority */

      using state_t = enum class state : uint8_t
        {
          //
          inactive = 0,
          ready = 1,
          running = 2,
          waiting = 3,
          terminated = 4
        };

      using sigset_t = uint32_t;

      namespace sig
      {
        enum
          : sigset_t
            {
              //
          any = 0,
          all = 0xFFFFFFFF,
        };
      } /* namespace sig */

      using func_args_t = void*;
      using func_t = void* (*) (func_args_t args);

    } /* namespace thread */

    namespace stack
    {
      using element_t = os::rtos::port::stack::element_t;

    } /* namespace stack */

    namespace this_thread
    {
      /**
       * @brief Get current thread.
       * @return Reference to the current running thread.
       */
      Thread&
      thread (void);

      /**
       * @brief Yield CPU.
       */
      void
      yield (void);

      /**
       * @brief Suspend the current thread.
       */
      void
      suspend (void);

      /**
       * @brief Terminate the current thread.
       * @return -
       */
      void
      exit (void* exit_ptr = nullptr);

      /**
       * @brief Check if wake-up due to timeout.
       * @retval true The previous sleep returned after the entire duration.
       * @retval false The previous sleep returned due to an event.
       */
      bool
      is_timeout (void);

      /**
       * @brief Wait for signals.
       * @param [in] mask The expected signals flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current signals;
       *  may be nullptr.
       * @retval result::ok All expected signal flags are raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EINVAL The signal mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      sig_wait (thread::sigset_t mask, thread::sigset_t* oflags);

      /**
       * @brief Try to wait for signals.
       * @param [in] mask The expected signals flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current signals;
       *  may be nullptr.
       * @retval result::ok All expected signal flags are raised.
       * @retval EINVAL The signal mask is outside of the permitted range.
       * @retval EAGAIN The signal condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      try_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags);

      /**
       * @brief Timed wait for signals.
       * @param [in] mask The expected signals flags (OR-ed bit-mask);
       *  may be zero.
       * @param [out] oflags Pointer where to store the current signals;
       *  may be nullptr.
       * @retval result::ok All expected signals flags are raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ETIMEDOUT The signal condition did not occur during the
       *  entire timeout duration.
       * @retval EINVAL The signal mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      timed_sig_wait (thread::sigset_t mask, thread::sigset_t* ooflagsmask,
                      systicks_t ticks);

    } /* namespace this_thread */

    // ========================================================================

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

    // ========================================================================

    namespace thread
    {

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
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @class Thread
     * @brief POSIX thread.
     * @details
     * Supports terminating functions and a simplified version of
     * signal flags.
     *
     * Compatible with POSIX threads:
     * http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html
     */
    class Thread : public Named_object
    {
    public:

      /**
       * @brief Create a new thread with default settings.
       */
      Thread (thread::func_t function, thread::func_args_t args);

      /**
       * @brief Create a new thread with custom settings.
       */
      Thread (const thread::Attributes& attr, thread::func_t function,
              thread::func_args_t args);

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
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      cancel (void);

      /**
       * @brief Wait for thread termination.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      join (void** exit_ptr = nullptr);

      /**
       * @brief Detach a thread.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      detach (void);

      // Accessors & mutators.

      /**
       * @brief Set dynamic scheduling priority.
       * @retval result::ok.
       * @retval result::einval The value of prio is invalid for the
       *  scheduling policy of the
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

      // TODO: study how to integrate signals and POSIX cancellation.
      bool
      interrupted (void);

      thread::state_t
      sched_state (void) const;

      /**
       * @brief Wake-up the thread.
       *
       * @note Can be invoked from Interrupt Service Routines.
       */
      void
      wakeup (void);

      void*
      function_args (void) const;

#if 0
      // Maybe make it a structure.
      result_t
      wakeup_reason (void) const;
#endif

      /**
       * @brief Get user storage.
       * @return The address of the thread user storage.
       */
      os_thread_user_storage_t*
      user_storage (void);

      /**
       * @brief Raise the thread signals.
       * @param [in] mask The OR-ed signals to raise.
       * @param [out] omask Optional pointer where to store the
       *  previous mask; may be nullptr.
       * @retval result::ok The signals were raised.
       * @retval EINVAL The mask is zero.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      thread::sigset_t
      sig_raise (thread::sigset_t mask, thread::sigset_t* oflags);

      /**
       * @brief Get/clear thread signals.
       * @param [in] mask The OR-ed signals to get/clear; may be zero.
       * @param [in] clear If true, the selected bits are cleared
       *  after read. The default is true.
       * @retval mask The selected bits from the current thread signal mask.
       * @retval sig::error Cannot be invoked from an Interrupt Service Routine.
       */
      thread::sigset_t
      sig_get (thread::sigset_t mask, bool clear = true);

      /**
       * @brief Clear thread signals.
       * @param [in] mask The OR-ed signals to clear.
       * @retval result::ok The signals were cleared.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EINVAL The mask is zero.
       */
      result_t
      sig_clear (thread::sigset_t mask, thread::sigset_t* oflags);

    protected:

      friend void
      this_thread::suspend (void);

      /**
       * @brief Suspend the current thread.
       *
       */
      void
      suspend (void);

      friend void
      this_thread::exit (void* exit_ptr);

      /**
       * @brief Terminate thread.
       * @return -
       */
      void
      exit (void* exit_ptr = nullptr);

      /**
       * @brief Invoke terminating thread function.
       * @param [in] thread The static `this`.
       */
      static void
      _invoke_with_exit (Thread* thread);

      friend result_t
      this_thread::sig_wait (thread::sigset_t mask, thread::sigset_t* oflags);

      result_t
      sig_wait (thread::sigset_t mask, thread::sigset_t* oflags);

      friend result_t
      this_thread::try_sig_wait (thread::sigset_t mask,
                                 thread::sigset_t* oflags);

      result_t
      try_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags);

      friend result_t
      this_thread::timed_sig_wait (thread::sigset_t mask,
                                   thread::sigset_t* oflags, systicks_t ticks);

      result_t
      timed_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                      systicks_t ticks);

      result_t
      _try_wait (thread::sigset_t mask, thread::sigset_t* oflags);

    protected:

      // TODO: group them in a Stack object
      void* stack_addr_;
      thread::func_t func_;
      thread::func_args_t func_args_;
      void* func_result_;

      // Implementation
      void* impl_;
      void* impl_event_flags_;

      std::size_t stack_size_bytes_;
      thread::state_t sched_state_;
      thread::priority_t prio_;

      result_t wakeup_reason_;

      // volatile, but used in critical sections.
      thread::sigset_t sig_mask_;

      os_thread_user_storage_t user_storage_;

      // Add other internal data

    };

#pragma GCC diagnostic pop

    // ========================================================================

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

      /**
       * @brief SysTick detailed timestamp.
       *
       * @details
       * When an accurate timestamp is needed, the current SysTick
       * counter can be sampled to get the count of CPU cycles inside
       * the tick. For a 100 MHz clock, this is a 10 ns resolution.
       *
       * To simplify further processing of this timestamp, the
       * structure also includes the CPU clock and the SysTick divider.
       */
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
       * @tparam Rep_T Type of input, auto deduced (usually uint32_t or uin64_t)
       * @param [in] micros The number of microseconds.
       * @return The number of ticks.
       */
      template<typename Rep_T>
        static constexpr uint32_t
        ticks_cast (Rep_T microsec);

      /**
       * @brief Sleep a number of ticks.
       * @param [in] ticks the number of ticks to sleep.
       * @retval ETIMEDOUT The sleep lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EINTR The sleep was interrupted.
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
       * @retval ETIMEDOUT The sleep lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EINTR The sleep was interrupted.
       */
      static result_t
      sleep_for (sleep_rep secs);

      // TODO: read hardware regs
      // TODO: write hw regs

      // TODO: write alarm (array)
      // TODO: read alarm

      // TODO: capabilities : nr. of alarms
    };

    // ==================--====================================================

    namespace timer
    {
      /**
       * Timer call back function arguments.
       */
      using func_args_t = void*;
      /**
       * Entry point of a timer call back function.
       */
      using func_t = void (*) (func_args_t args);

      /**
       * Timer run type.
       */
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

      extern const Attributes once_initializer;

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
       * @retval ENOTRECOVERABLE Timer could not be started.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      start (systicks_t ticks);

      /**
       * @brief Stop the timer.
       * @retval result::ok The timer has been stopped.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EAGAIN The timer is not yet started.
       * @retval ENOTRECOVERABLE Timer could not be stopped.
       */
      result_t
      stop (void);

    protected:

      timer::func_t func_;
      timer::func_args_t func_args_;
      void* impl_;
      timer::type_t type_;

      // Add more internal data.
    };

#pragma GCC diagnostic pop

    // ========================================================================

    namespace mutex
    {
      using protocol_t = enum class protocol : uint8_t
        {
          //
          none = 0,
          inherit = 1,
          protect = 2
        };

      using robustness_t = enum class robustness : uint8_t
        {
          //
          stalled = 0,
          robust = 1
        };

      using type_t = enum class type : uint8_t
        {
          //
          normal = 0,
          errorcheck = 1,
          recursive = 2,
        };

      using count_t = uint16_t;

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

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class Mutex : public Named_object
    {
    public:

      /**
       * @brief Create a mutex with default attributes.
       */
      Mutex ();
      /**
       * @brief Create a mutex with custom attributes.
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
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ENOTRECOVERABLE The mutex was not locked.
       */
      result_t
      lock (void);

      /**
       * @brief Try to lock the mutex.
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EAGAIN The mutex was not locked.
       */
      result_t
      try_lock (void);

      /**
       * @brief Timed attempt to lock the mutex.
       * @param [in] ticks Number of ticks to wait.
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ETIMEDOUT The mutex could not be locked before the
       *  specified timeout expired.
       * @retval ENOTRECOVERABLE The mutex was not locked.
       */
      result_t
      timed_lock (systicks_t ticks);

      /**
       * @brief Unlock the mutex.
       * @retval result::ok The mutex was unlocked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ENOTRECOVERABLE The mutex was not unlocked.
       */
      result_t
      unlock (void);

      /**
       * @brief Get the priority ceiling of a mutex.
       * @return The priority ceiling.
       */
      thread::priority_t
      prio_ceiling (void) const;

      /**
       * @brief Change the priority ceiling of a mutex.
       * @param [in] prio_ceiling new priority.
       * @param [out] old_prio_ceiling pointer to location where to
       *  store the previous priority; may be nullptr.
       * @retval result::ok The priority was changed.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      prio_ceiling (thread::priority_t prio_ceiling,
                    thread::priority_t* old_prio_ceiling = nullptr);

      /**
       * @brief Mark mutex as consistent.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      consistent (void);

    protected:

      // Can be updated in different thread contexts.
      Thread* volatile owner_;

      void* impl_;

      // Can be updated in different thread contexts.
      volatile mutex::count_t count_;

      // Can be updated in different thread contexts.
      volatile thread::priority_t prio_ceiling_;

      // Constants set during construction.
      const mutex::type_t type_; // normal, errorcheck, recursive
      const mutex::protocol_t protocol_; // none, inherit, protect
      const mutex::robustness_t robustness_; // stalled, robust

      // Add more internal data.
    };

#pragma GCC diagnostic pop

    // ========================================================================

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

    // ========================================================================

    class Condition_variable : public Named_object
    {
    public:

      /**
       * @brief Create a condition variable.
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
       * @retval true The given condition variable is the same as
       *  this condition variable.
       * @retval false The condition variables are different.
       */
      bool
      operator== (const Condition_variable& rhs) const;

      /**
       * @brief Signal a condition.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      signal (void);

      /**
       * @brief Broadcast a condition.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      broadcast (void);

      /**
       * @brief Wait on a condition.
       * @param [in] mutex Reference to the associated mutex.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      wait (Mutex& mutex);

      // Neither POSIX nor ISO define a try_wait(), so... do we need one?

      /**
       * @brief Timed wait on a condition.
       * @param [in] mutex Reference to the associated mutex.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval result::ok.
       */
      result_t
      timed_wait (Mutex& mutex, systicks_t ticks);

    protected:

      // Add more internal data.
    };

    // ========================================================================

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
       * @retval EOVERFLOW The max count was exceeded.
       * @retval ENOTRECOVERABLE The semaphore could not be posted
       *  (extension to POSIX).
       */
      result_t
      post (void);

      /**
       * @brief Lock the semaphore, possibly waiting.
       * @retval result::ok The calling process successfully
       *  performed the semaphore lock operation.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
       * @retval EDEADLK A deadlock condition was detected.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      wait ();

      /**
       * @brief Try to lock  the semaphore.
       * @retval result::ok The calling process successfully
       *  performed the semaphore lock operation.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EAGAIN The semaphore was already locked.
       * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
       * @retval EDEADLK A deadlock condition was detected.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      try_wait ();

      /**
       * @brief Timed wait to lock the semaphore.
       * @param [in] ticks Number of ticks to wait.
       * @retval result::ok The calling process successfully
       *  performed the semaphore lock operation.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval EINVAL Invalid timeout (POSIX limits the timeout
       *  to 1000 million ns)
       * @retval ETIMEDOUT The semaphore could not be locked before
       *  the specified timeout expired.
       * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
       * @retval EDEADLK A deadlock condition was detected.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      timed_wait (systicks_t ticks);

      /**
       * @brief Get the semaphore value.
       * @return The semaphore value.
       */
      semaphore::count_t
      value (void) const;

      /**
       * @brief Reset the semaphore.
       * @retval result::ok The semaphore was reset.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      reset (void);

    protected:

      port::Tasks_list list_;

      void* impl_;

      const semaphore::count_t initial_count_;

      // Can be updated in different contexts (interrupts or threads)
      volatile semaphore::count_t count_;

      // Constant set during construction.
      const semaphore::count_t max_count_;

      // Add more internal data.
    };

#pragma GCC diagnostic pop

    // ========================================================================

    namespace mempool
    {
      using size_t = uint16_t;
      constexpr size_t max_size = (0 - 1);

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

    /**
     * @class Memory_pool
     * @brief Synchronised memory pool.
     * @details
     * Manage a pool of same size blocks. Fast and deterministic allocation
     * and dealocation behaviour, suitable for use even in ISRs.
     *
     * No POSIX similar functionality identified.
     *
     * @note There is no equivalent of calloc(); to initialise memory, use:
     * `memset (block, 0, pool.block_size ());`
     */
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
       * @retval EINVAL The block does not belong to the memory pool.
       */
      result_t
      free (void* block);

      /**
       * @brief Get memory pool capacity.
       * @return The max number of blocks in the pool.
       */
      std::size_t
      capacity (void) const;

      /**
       * @brief Get blocks count.
       * @return The number of blocks used from the queue.
       */
      std::size_t
      count (void) const;

      /**
       * @brief Get block size.
       * @return The block size, in bytes.
       */
      std::size_t
      block_size (void) const;

      /**
       * @brief Check if the memory pool is empty.
       * @retval true The memory pool has no allocated blocks.
       * @retval false The memory pool has allocated blocks.
       */
      bool
      empty (void) const;

      /**
       * @brief Check if the memory pool is full.
       * @retval true All memory blocks are allocated.
       * @retval false There are still memory blocks that can be allocated.
       */
      bool
      full (void) const;

      /**
       * @brief Reset the memory pool.
       * @retval result::ok The memory pool was reset.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      reset (void);

    protected:

      void*
      _try_first (void);

    protected:

      port::Tasks_list list_;

      char* pool_addr_;
      const mempool::size_t blocks_;
      const mempool::size_t block_size_bytes_;

      volatile mempool::size_t count_;

      // All accesses will be done inside a critical section,
      // the volatile may not be needed, the variable remains stable
      // during the critical section and no loops wait for this variable.
      void* volatile first_;

      uint8_t flags_;
      enum
        : uint8_t
          {
            flags_allocated = 1
      };
      // Add more internal data.
    };

#pragma GCC diagnostic pop

    // ========================================================================

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

    /**
     * @class Message_queue
     * @brief POSIX message queue.
     * @details
     * Priority based, fixed size FIFO.
     *
     * Compatible with POSIX message queues.
     * http://pubs.opengroup.org/onlinepubs/9699919799/
     */
    class Message_queue : public Named_object
    {
    public:

      /**
       * @brief Create a message queue.
       * @param [in] msgs The number of messages.
       * @param [in] msg_size_bytes The message size, in bytes.
       */
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
       * @retval false The memory queues are different.
       */
      bool
      operator== (const Message_queue& rhs) const;

      /**
       * @brief Send message to the queue.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @retval result::ok The message was enqueued.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes,
       *  exceeds the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ENOTRECOVERABLE The message could not be enqueue
       *  (extension to POSIX).
       * @retval EINTR The operation was interrupted.
       */
      result_t
      send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio);

      /**
       * @brief Try to send message to the queue.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @retval result::ok The message was enqueued.
       * @retval EAGAIN The specified message queue is full.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes,
       *  exceeds the message size attribute of the message queue.
       * @retval ENOTRECOVERABLE The message could not be enqueue
       *  (extension to POSIX).
       */
      result_t
      try_send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio);

      /**
       * @brief Send message to the queue with timeout.
       * @param [in] msg The address of the message to enqueue.
       * @param [in] nbytes The length of the message. Must be not
       *  higher than the value used when creating the queue.
       * @param [in] mprio The message priority.
       * @param [in] ticks The timeout duration, in SysTick ticks.
       * @retval result::ok The message was enqueued.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes,
       *  exceeds the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ETIMEDOUT The timeout expired before the message
       *  could be added to the queue.
       * @retval ENOTRECOVERABLE The message could not be enqueue
       *  (extension to POSIX).
       * @retval EINTR The operation was interrupted.
       */
      result_t
      timed_send (const char* msg, std::size_t nbytes, mqueue::priority_t mprio,
                  systicks_t ticks);

      /**
       * @brief Receive message from the queue.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority. May be nullptr.
       * @retval result::ok The message was received.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes, is
       *  greater than the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ENOTRECOVERABLE The message could not be dequeued
       *  (extension to POSIX).
       * @retval EBADMSG The implementation has detected a data corruption
       *  problem with the message.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      receive (char* msg, std::size_t nbytes, mqueue::priority_t* mprio);

      /**
       * @brief Try to receive message from the queue.
       * @param [out] msg The address where to store the dequeued message.
       * @param [in] nbytes The size of the destination buffer. Must
       *  be lower than the value used when creating the queue.
       * @param [out] mprio The address where to store the message
       *  priority. May be nullptr.
       * @retval result::ok The message was received.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes, is
       *  greater than the message size attribute of the message queue.
       * @retval ENOTRECOVERABLE The message could not be dequeued
       *  (extension to POSIX).
       * @retval EBADMSG The implementation has detected a data corruption
       *  problem with the message.
       * @retval EAGAIN The specified message queue is empty.
       */
      result_t
      try_receive (char* msg, std::size_t nbytes, mqueue::priority_t* mprio);

      /**
       * @brief Receive message from the queue with timeout.
       * @retval result::ok The message was received.
       * @retval EINVAL A parameter is invalid or outside of a permitted range.
       * @retval EMSGSIZE The specified message length, nbytes, is
       *  greater than the message size attribute of the message queue.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       * @retval ENOTRECOVERABLE The message could not be dequeued
       *  (extension to POSIX).
       * @retval EBADMSG The implementation has detected a data corruption
       *  problem with the message.
       * @retval EINTR The operation was interrupted.
       * @retval ETIMEDOUT No message arrived on the queue before the
       *  specified timeout expired.
       */
      result_t
      timed_receive (char* msg, std::size_t nbytes, mqueue::priority_t* mprio,
                     systicks_t ticks);

      /**
       * @brief Get queue capacity.
       * @return The max number of messages that can be queued.
       */
      std::size_t
      capacity (void) const;

      /**
       * @brief Get queue length.
       * @return The number of messages in the queue.
       */
      std::size_t
      length (void) const;

      /**
       * @brief Get message size.
       * @return The message size, in bytes.
       */
      std::size_t
      msg_size (void) const;

      /**
       * @brief Check if the queue is empty.
       * @retval true The queue has no messages.
       * @retval false The queue has some messages.
       */
      bool
      empty (void) const;

      /**
       * @brief Check if the queue is full.
       * @retval true The queue is full.
       * @retval false The queue is not full.
       */
      bool
      full (void) const;

      /**
       * @brief Reset the message queue.
       * @retval result::ok.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine.
       */
      result_t
      reset (void);

    private:

      port::Tasks_list send_list_;
      port::Tasks_list receive_list_;

      void* queue_addr_;
      void* impl_;

      std::size_t queue_size_bytes_;

      const mqueue::size_t msgs_;
      const mqueue::size_t msg_size_bytes_;

      mqueue::size_t count_;

    };

#pragma GCC diagnostic pop

    // ========================================================================

    namespace evflags
    {
      using mask_t = uint32_t;
      using mode_t = uint32_t;

      namespace mode
      {
        enum
          : mode_t
            {
              //
          all = 1,
          any = 2,
          clear = 4
        };
      }
      ;
      /**
       * @brief Event flags attributes.
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

        // Add more attributes.
      };

      extern const Attributes initializer;

    } /* namespace mqueue */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @class Event_flags
     * @brief Event flags.
     * @details
     * Synchronised set of flags that can be used to notify events
     * between threads or between ISRs and threads.
     *
     * No POSIX similar functionality identified.
     */
    class Event_flags : public Named_object
    {
    public:

      Event_flags (void);
      Event_flags (const evflags::Attributes&attr);

      Event_flags (const Event_flags&) = delete;
      Event_flags (Event_flags&&) = delete;
      Event_flags&
      operator= (const Event_flags&) = delete;
      Event_flags&
      operator= (Event_flags&&) = delete;

      /**
       * @brief Delete the event flags.
       */
      ~Event_flags ();

      /**
       * @brief Compare event flags.
       * @retval true The given event flags object is the same as this
       *  event flags.
       * @retval false The event flags are different.
       */
      bool
      operator== (const Event_flags& rhs) const;

      result_t
      wait (evflags::mask_t mask, evflags::mask_t* oflags,
            evflags::mode_t mode);

      result_t
      try_wait (evflags::mask_t mask, evflags::mask_t* oflags,
                evflags::mode_t mode);

      result_t
      timed_wait (evflags::mask_t mask, evflags::mask_t* oflags,
                  evflags::mode_t mode, systicks_t ticks);

      result_t
      raise (evflags::mask_t mask, evflags::mask_t* oflags);

      result_t
      clear (evflags::mask_t mask, evflags::mask_t* oflags);

      result_t
      get (evflags::mask_t mask, evflags::mask_t* oflags, evflags::mode_t mode);

      bool
      waiting (void);

    private:

      port::Tasks_list list_;
      evflags::mask_t flags_;
    };

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    // ========================================================================

    namespace kernel
    {
      ;
    } /* namespace kernel */

    namespace scheduler
    {
      /**
       * @details
       * Check if the scheduler was started, i.e. if scheduler::start()
       * was called.
       */
      inline bool
      is_started (void)
      {
        return is_started_;
      }

      /**
       * @details
       * Check if the scheduler is locked on the current task or
       * is switching tasks from the ready list.
       */
      inline bool
      is_locked (void)
      {
        return is_locked_;
      }

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

    // ========================================================================

    inline const char*
    Named_object::name (void) const
    {
      return name_;
    }

    // ========================================================================

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

    // ========================================================================

    namespace this_thread
    {
      /**
       * @details
       * Remove the current thread from the ready list and pass
       * control to the next thread that is in \b READY state.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline void
      suspend (void)
      {
        this_thread::thread ().suspend ();
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      sig_wait (thread::sigset_t mask, thread::sigset_t* omask)
      {
        return this_thread::thread ().sig_wait (mask, omask);
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      try_sig_wait (thread::sigset_t mask, thread::sigset_t* omask)
      {
        return this_thread::thread ().try_sig_wait (mask, omask);
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      inline result_t
      timed_sig_wait (thread::sigset_t mask, thread::sigset_t* omask,
                      systicks_t ticks)
      {
        return this_thread::thread ().timed_sig_wait (mask, omask, ticks);
      }

    } /* namespace this_thread */

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
    } /* namespace thread */

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
    Thread::sched_state (void) const
    {
      return sched_state_;
    }

    inline void*
    Thread::function_args (void) const
    {
      return func_args_;
    }

#if 0
    inline result_t
    Thread::wakeup_reason (void) const
      {
        return wakeup_reason_;
      }
#endif

    /**
     * @details
     * The user storage is a custom structure defined in os-app-config.h,
     * which is added to each and every thread storage. Applications
     * can store here any data.
     *
     * Inspired by (actually a generalisation of) µC-OS III task user
     * registers and FreeRTOS thread local storage, which proved useful
     * when implementing CMSIS+ over FreeRTOS.
     */
    inline os_thread_user_storage_t*
    Thread::user_storage (void)
    {
      return &user_storage_;
    }

    // ========================================================================

    namespace timer
    {
      inline
      Attributes::Attributes (const char* name) :
          Named_object
            { name }
      {
        this->tm_type = run::once;
      }

      // ======================================================================

      inline
      Periodic_attributes::Periodic_attributes (const char* name) :
          Attributes
            { name }
      {
        this->tm_type = run::periodic;
      }
    } /* namespace timer */

    // ========================================================================

    /**
     * @details
     * Identical timers should have the same memory address.
     */
    inline bool
    Timer::operator== (const Timer& rhs) const
    {
      return this == &rhs;
    }

    // ========================================================================

    namespace mutex
    {
      inline
      Attributes::Attributes (const char* name) :
          Named_object (name)
      {
        mx_priority_ceiling = thread::priority::highest;
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

    // ========================================================================

    /**
     * @details
     * Identical mutexes should have the same memory address.
     */
    inline bool
    Mutex::operator== (const Mutex& rhs) const
    {
      return this == &rhs;
    }

    // ========================================================================

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

    // ========================================================================

    /**
     * @details
     * Identical condition variables should have the same memory address.
     */
    inline bool
    Condition_variable::operator== (const Condition_variable& rhs) const
    {
      return this == &rhs;
    }

    // ========================================================================

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

    // ========================================================================

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
    Semaphore::value (void) const
    {
      return count_;
    }

    // ========================================================================

    namespace mempool
    {
      inline
      Attributes::Attributes (const char* name) :
          Named_object (name)
      {
        mp_pool_address = nullptr;
      }

    } /* namespace mempool */

    // ========================================================================

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
    Memory_pool::capacity (void) const
    {
      return blocks_;
    }

    inline std::size_t
    Memory_pool::block_size (void) const
    {
      return block_size_bytes_;
    }

    inline std::size_t
    Memory_pool::count (void) const
    {
      return count_;
    }

    inline bool
    Memory_pool::empty (void) const
    {
      return (count () == 0);
    }

    inline bool
    Memory_pool::full (void) const
    {
      return (count () == capacity ());
    }

    // ========================================================================

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

    // ========================================================================

    /**
     * @details
     * Identical message queues should have the same memory address.
     */
    inline bool
    Message_queue::operator== (const Message_queue& rhs) const
    {
      return this == &rhs;
    }

    inline std::size_t
    Message_queue::length (void) const
    {
      return count_;
    }

    inline std::size_t
    Message_queue::capacity (void) const
    {
      return msgs_;
    }

    inline std::size_t
    Message_queue::msg_size (void) const
    {
      return msg_size_bytes_;
    }

    inline bool
    Message_queue::empty (void) const
    {
      return (length () == 0);
    }

    inline bool
    Message_queue::full (void) const
    {
      return (length () == capacity ());
    }

// ------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

extern "C"
{
  void
  os_systick_handler (void);

  void
  os_impl_systick_handler (void);

  void
  os_rtc_handler (void);

  void
  os_impl_rtc_handler (void);

  int
  os_main (int argc, char* argv[]);
}

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
