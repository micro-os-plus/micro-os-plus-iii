/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace
{
#if defined(OS_HAS_INTERRUPTS_STACK)
// Object used to manage the interrupts stack.
  class os::rtos::thread::stack interrupts_stack;
#endif /* defined(OS_HAS_INTERRUPTS_STACK) */
}

namespace os
{
  /**
   * @details
   * The `os::rtos` namespace groups all RTOS specific declarations,
   * either directly or via nested namespaces.
   */
  namespace rtos
  {

    /**
     * @details
     * The `os::rtos::scheduler` namespace groups scheduler types
     * and functions.
     */
    namespace scheduler
    {
      /**
       * @cond ignore
       */

      /**
       * @details
       * No further changes allowed, the scheduler cannot be stopped,
       * in can be only locked.
       */
      bool is_started_ = false;

#if 0
      /**
       * @details
       * Modified by `lock()`/`unlock()` and restored to the previous value
       * by `locked(state_t)`.
       */
      state_t lock_state_ = state::init;
#endif

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
      /**
       * @details
       * This special list is set to empty during BSS initialisation,
       * since it must be available to register the very first statically
       * allocated thread.
       */
      // top_threads_list top_threads_list_;
      thread::threads_list top_threads_list_;

#pragma GCC diagnostic pop

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

      bool is_preemptive_ = false;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif
      // A small kludge to provide a temporary errno before
      // the first real thread is created.
      typedef struct {
        void* vtbl;
        void* name_;
        // errno is the first thread member, so right after the name.
        int errno_;
      } tiny_thread_t;
#pragma GCC diagnostic pop

      // Ensure the tiny thread is large enough to have the errno
      // member in the same location.
#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#endif
      static_assert(offsetof(tiny_thread_t, errno_) == offsetof(thread, errno_), "adjust tiny_thread_t members");
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wmissing-variable-declarations"
#endif
      tiny_thread_t tiny_thread;
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wcast-align"
      thread* volatile current_thread_ = reinterpret_cast<thread*>(&tiny_thread);
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
      internal::ready_threads_list ready_threads_list_;
#pragma GCC diagnostic pop
#endif

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
      internal::terminated_threads_list terminated_threads_list_;
#pragma GCC diagnostic pop

      /**
       * @endcond
       */

      /**
       * @details
       * Initialise all RTOS internal objects and be ready to run.
       *
       * Must be called only once, usually in main().
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      result_t
      initialize (void)
      {
#if defined(OS_TRACE_RTOS_SCHEDULER)
        trace::printf ("scheduler::%s() \n", __func__);
#endif

        // Don't call this from interrupt handlers.
        os_assert_err(!interrupts::in_handler_mode (), EPERM);

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

        return port::scheduler::initialize ();

#else

        port::scheduler::initialize ();

        return result::ok;
#endif
      }

      /**
       * @details
       * The scheduler cannot be stopped, it will run forever, but
       * thread switching can be locked/unlocked.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      [[noreturn]] void
      start (void)
      {
        trace::printf ("scheduler::%s() \n", __func__);

        // Don't call this from interrupt handlers.
        os_assert_throw(!interrupts::in_handler_mode (), EPERM);

        sysclock.start ();
        hrclock.start ();

        rtclock.start ();

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

        scheduler::statistics::context_switches_ = 0;

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

        scheduler::statistics::cpu_cycles_ = 0;
        scheduler::statistics::switch_timestamp_ = hrclock.now ();

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)
        is_preemptive_ = OS_BOOL_RTOS_SCHEDULER_PREEMPTIVE;
#endif /* defined(OS_USE_RTOS_PORT_SCHEDULER) */

        is_started_ = true;

        port::scheduler::start ();
      }

      /**
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      bool
      preemptive (bool state)
      {
#if defined(OS_TRACE_RTOS_SCHEDULER)
        trace::printf ("scheduler::%s(%d) \n", __func__, state);
#endif
        // Don't call this from interrupt handlers.
        os_assert_throw(!interrupts::in_handler_mode (), EPERM);

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

        return port::scheduler::preemptive(state);

#else
        bool tmp;

          {
            // ----- Enter critical section -----------------------------------
            interrupts::critical_section ics;

            tmp = is_preemptive_;
            is_preemptive_ = state;
            // ----- Exit critical section ------------------------------------
          }

        return tmp;
#endif
      }

      /**
       * @details
       * If the input pointer is nullptr, the function returns the
       * list of top level threads.
       */
      thread::threads_list&
      children_threads (thread* th)
      {
        if (th == nullptr)
          {
            return top_threads_list_;
          }
        else
          {
            return th->children_;
          }
      }

    /**
     * @class critical_section
     * @details
     * Use this class to define a critical section
     * protected to scheduler switches. The beginning of the
     * critical section is exactly the place where this class is
     * instantiated (the constructor will lock
     * the scheduler). The end of the critical
     * section is the end of the surrounding block (the destructor will
     * unlock the scheduler).
     *
     * @note Can be nested as many times as required without problems,
     * only the outer call will unlock the scheduler.
     *
     * @par Example
     *
     * @code{.cpp}
     * void
     * func(void)
     * {
     *    // Do something
     *
     *    {
     *      scheduler::critical_section scs;  // Critical section begins here.
     *
     *      // Inside the critical section.
     *      // No scheduler switches will happen here.
     *
     *    } // Critical section ends here.
     *
     *    // Do something else.
     * }
     * @endcode
     */

    /*
     * @var const state_t critical_section::state_
     * @details
     * The variable is constant, after being set by the constructor no
     * further changes are possible.
     *
     * The variable type usually is a `bool`, but a counter is also
     * possible if the scheduler uses a recursive lock.
     */

    /**
     * @class lockable
     * @details
     * Locker meeting the standard `Lockable` requirements (30.2.5.3).
     */

    } /* namespace scheduler */

    namespace scheduler
    {
      /**
       * @cond ignore
       */

      void
      internal_link_node (internal::waiting_threads_list& list,
                          internal::waiting_thread_node& node)
      {
        // Remove this thread from the ready list, if there.
        port::this_thread::prepare_suspend ();

        // Add this thread to the node waiting list.
        list.link (node);
        node.thread_->waiting_node_ = &node;

        node.thread_->state_ = thread::state::suspended;
      }

      void
      internal_unlink_node (internal::waiting_thread_node& node)
      {
          {
            // ----- Enter critical section -----------------------------------
            interrupts::critical_section ics;

            // Remove the thread from the node waiting list,
            // if not already removed.
            node.thread_->waiting_node_ = nullptr;
            node.unlink ();
            // ----- Exit critical section ------------------------------------
          }
      }

      void
      internal_link_node (internal::waiting_threads_list& list,
                          internal::waiting_thread_node& node,
                          internal::clock_timestamps_list& timeout_list,
                          internal::timeout_thread_node& timeout_node)
      {
        // Remove this thread from the ready list, if there.
        port::this_thread::prepare_suspend ();

        // Add this thread to the node waiting list.
        list.link (node);
        node.thread_->waiting_node_ = &node;

        node.thread_->state_ = thread::state::suspended;

        // Add this thread to the clock timeout list.
        timeout_list.link (timeout_node);
        timeout_node.thread.clock_node_ = &timeout_node;
      }

      void
      internal_unlink_node (internal::waiting_thread_node& node,
                            internal::timeout_thread_node& timeout_node)
      {
        // ----- Enter critical section ---------------------------------------
        interrupts::critical_section ics;

        // Remove the thread from the clock timeout list,
        // if not already removed by the timer.
        timeout_node.thread.clock_node_ = nullptr;
        timeout_node.unlink ();

        // Remove the thread from the node waiting list,
        // if not already removed.
        node.thread_->waiting_node_ = nullptr;
        node.unlink ();
        // ----- Exit critical section ----------------------------------------
      }

      // ----------------------------------------------------------------------

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

      void
      internal_switch_threads (void)
      {
#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

        // Get the high resolution timestamp.
        clock::timestamp_t now = hrclock.now ();

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

        // Compute duration since previous context switch.
        // Assume scheduler is not disabled for very long.
        rtos::statistics::duration_t delta =
            static_cast<rtos::statistics::duration_t> (now
                - scheduler::statistics::switch_timestamp_);

#pragma GCC diagnostic pop

        // Accumulate durations to scheduler total.
        scheduler::statistics::cpu_cycles_ += delta;

        // Accumulate durations to old thread.
        scheduler::current_thread_->statistics_.cpu_cycles_ += delta;

        // Remember the timestamp for the next context switch.
        scheduler::statistics::switch_timestamp_ = now;

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

        // The very core of the scheduler, if not locked, re-link the
        // current thread and return the top priority thread.
        if (!locked ())
          {
            // Normally the old running thread must be re-linked to ready.
            scheduler::current_thread_->internal_relink_running_ ();

            // The top of the ready list gives the next thread to run.
            scheduler::current_thread_ =
                scheduler::ready_threads_list_.unlink_head ();
          }

        // ***** Pointer switched to new thread! *****

        // The new thread was marked as running in unlink_head(),
        // so in case the handler is re-entered immediately,
        // the relink_running() will simply reschedule it,
        // otherwise the thread will be lost.

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

        // Increment global context switches.
        scheduler::statistics::context_switches_++;

        // Increment new thread context switches.
        scheduler::current_thread_->statistics_.context_switches_++;

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

      }

#endif /* !defined(OS_USE_RTOS_PORT_SCHEDULER) */

      namespace statistics
      {
#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES)

        rtos::statistics::counter_t context_switches_;

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CONTEXT_SWITCHES) */

#if defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES)

        clock::timestamp_t switch_timestamp_;
        rtos::statistics::duration_t cpu_cycles_;

#endif /* defined(OS_INCLUDE_RTOS_STATISTICS_THREAD_CPU_CYCLES) */

      } /* namespace statistics */

    /**
     * @endcond
     */

    } /* namespace scheduler */

    /**
     * @details
     * The os::rtos::interrupts namespace groups interrupts related
     *  types and enumerations.
     */
    namespace interrupts
    {
      /**
       * @class critical_section
       * @details
       * Use this class to define a critical section
       * protected to interrupts service routines. The beginning of the
       * critical section is exactly the place where this class is
       * instantiated (the constructor will disable interrupts below
       * the scheduler priority). The end of the critical
       * section is the end of the surrounding block (the destructor will
       * enable the interrupts).
       *
       * @note Can be nested as many times as required without problems,
       * only the outer call will re-enable the interrupts.
       *
       * @par Example
       *
       * @code{.cpp}
       * void
       * func(void)
       * {
       *    // Do something
       *
       *    {
       *      interrupts::critical_section ics;  // Critical section begins here.
       *
       *      // Inside the critical section.
       *      // No scheduler switches will happen here.
       *
       *    } // Critical section ends here.
       *
       *    // Do something else.
       * }
       * @endcode
       */

      /*
       * @var const state_t critical_section::state_
       * @details
       * The variable is constant, after being set by the constructor no
       * further changes are possible.
       *
       * The variable type usually is an unsigned integer where
       * the priorities register is saved.
       */

      /**
       * @class lockable
       * @details
       * Locker meeting the standard `Lockable` requirements (30.2.5.3).
       */

      /*
       * @var state_t lockable::state_
       * @details
       * The variable type usually is an unsigned integer where
       * the priorities register is saved.
       */

#if defined(OS_HAS_INTERRUPTS_STACK) || defined(__DOXYGEN__)

      /**
       * @details
       * The interrupts stack can be manipulated in a way similar to
       * the thread stack.
       *
       * @note This function is available only on platforms that
       *  support a separate interrupt stack (like Cortex-M).
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      class thread::stack*
      stack (void)
        {
          return &interrupts_stack;
        }

#endif /* defined(OS_HAS_INTERRUPTS_STACK) */

    } /* namespace interrupts */

    // ========================================================================
    namespace internal
    {
      /**
       * @class object_named
       * @details
       * This class serves as a base class for all objects that have a
       * name (most of the RTOS classes do have a name).
       *
       * Attributes use a separate constexpr object.
       */

      /*
       * @var const char* const object_named::name_
       * @details
       * To save space, the null terminated string passed to the
       * constructor is not copied locally. Instead, the pointer to
       * the string is copied, so the
       * caller must ensure that the pointer life cycle
       * is at least as long as the object life cycle. A constant
       * string (stored in flash) is preferred.
       */

      /**
       *
       */
      object_named::object_named ()
      {
      }

      /**
       * @details
       * Prefer the given name, otherwise
       * default to '-'.
       *
       * To save space, instead of copying the null terminated string
       * locally, the pointer to the string
       * is copied, so the caller must ensure that the pointer
       * life cycle is at least as long as the object life cycle.
       * A constant string (stored in flash) is preferred.
       */
      object_named::object_named (const char* name) :
          name_ (name != nullptr ? name : "-")
      {
      }

    } /* namespace internal */

  // ==========================================================================
  } /* namespace rtos */
} /* namespace os */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wreserved-identifier"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-decls"
#endif
int*
__errno (void);
#pragma GCC diagnostic pop

/**
 * @name errno getter/setter
 * @{
 */

/**
 * @brief Per-thread error support.
 * @ingroup cmsis-plus-rtos-c
 * @details
 * Standard C libraries define `errno` as a macro to a function returning
 * a pointer. This function returns such a pointer, specific to each
 * thread.
 * @return Pointer to per-thread errno value.
 */
int*
__errno (void)
{
  return os::rtos::this_thread::__errno ();
}

/**
 * @}
 */

// ----------------------------------------------------------------------------
