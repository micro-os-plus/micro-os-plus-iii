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

#include <cassert>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

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
       * @details
       * No further changes allowed, the scheduler cannot be stopped,
       * in can be only locked.
       */
      status_t is_started_ = false;

      /**
       * @details
       * Modified by `lock()` and restored to the previous value
       * by `unlock()`.
       */
      status_t is_locked_ = false;

      /**
       * @details
       * This special list is set to empty during BSS initialisation,
       * since it must be available to register the very first statically
       * allocated thread.
       */
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
      top_threads_list top_threads_list_;
#pragma GCC diagnostic pop

#if !defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
      thread* volatile current_thread_;
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
      ready_threads_list ready_threads_list_;
      terminated_threads_list terminated_threads_list_;
#pragma GCC diagnostic pop
#endif

      /**
       * @details
       * Create all RTOS internal objects and be ready to run.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      result_t
      initialize (void)
      {
        os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_SCHEDULER)
        trace::printf ("%s() \n", __func__);
#endif

#if defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)

        return port::scheduler::initialize ();

#else

        port::scheduler::initialize ();

        scheduler::_create_idle ();

        return result::ok;
#endif
      }

      /**
       *
       * @note Can be invoked from Interrupt Service Routines (obviously).
       */
      bool
      in_handler_mode (void)
      {
        return port::scheduler::in_handler_mode ();
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
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_SCHEDULER)
        trace::printf ("%s() \n", __func__);
#endif

        is_started_ = true;
        is_locked_ = false;

        port::clock_systick::start ();

        port::scheduler::start ();
      }

      /**
       * @details
       * Lock the scheduler (prevent it for doing thread switches) and
       * return the previous status, to be restored by `unlock()`.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      status_t
      lock (status_t status)
      {
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

        status_t tmp;

          {
            interrupts::critical_section ics;

            tmp = is_locked_;
            is_locked_ = status;
          }

        port::scheduler::lock (is_locked_);

        return tmp;
      }

      /**
       * @details
       * Actually restore the scheduler status based on the given
       * parameter, usually returned by a `lock()`. This allows for
       * embedded critical sections to preserve the locked status
       * until the outer one completes and invokes `unlock()`.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      void
      unlock (status_t status)
      {
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

        is_locked_ = status;

        port::scheduler::lock (is_locked_);

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
     *      scheduler::critical_section cs;  // Critical section begins here.
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

    /**
     * @var const status_t critical_section::status_
     * @details
     * The variable is constant, after being set by the constructor no
     * further changes are possible.
     *
     * The variable type usually is a `bool`, but a counter is also
     * possible if the scheduler uses a recursive lock.
     */

    /**
     * @class Lock
     * @details
     * Locker meeting the standard `Lockable` requirements (30.2.5.3).
     */

    /**
     * @var status_t Lock::status_
     * @details
     * The variable type usually is a `bool`, but a counter is also
     * possible if the scheduler uses a recursive lock.
     */

    } /* namespace scheduler */

    namespace scheduler
    {

      void
      _link_node (waiting_threads_list& list, waiting_thread_node& node)
      {
        // Remove this thread from the ready list, if there.
        port::this_thread::prepare_suspend ();

        // Add this thread to the node waiting list.
        list.link (node);
        node.thread_.waiting_node_ = &node;

        node.thread_.sched_state_ = thread::state::waiting;
      }

      void
      _unlink_node (waiting_thread_node& node)
      {
          {
            interrupts::critical_section ics; // ----- Critical section -----

            // Remove the thread from the node waiting list,
            // if not already removed.
            node.thread_.waiting_node_ = nullptr;
            node.unlink ();
          }
      }

      void
      _link_node (waiting_threads_list& list, waiting_thread_node& node,
                  clock_timestamps_list& timeout_list,
                  timeout_thread_node& timeout_node)
      {
        // Remove this thread from the ready list, if there.
        port::this_thread::prepare_suspend ();

        // Add this thread to the node waiting list.
        list.link (node);
        node.thread_.waiting_node_ = &node;

        node.thread_.sched_state_ = thread::state::waiting;

        // Add this thread to the clock timeout list.
        timeout_list.link (timeout_node);
        timeout_node.thread.clock_node_ = &timeout_node;
      }

      void
      _unlink_node (waiting_thread_node& node,
                    timeout_thread_node& timeout_node)
      {
        interrupts::critical_section ics; // ----- Critical section -----

        // Remove the thread from the clock timeout list,
        // if not already removed by the timer.
        timeout_node.thread.clock_node_ = nullptr;
        timeout_node.unlink ();

        // Remove the thread from the node waiting list,
        // if not already removed.
        node.thread_.waiting_node_ = nullptr;
        node.unlink ();
      }

    } /* namespace this_thread */

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
       * protected to interrupts service routines. The begining of the
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

      /**
       * @var const status_t critical_section::status_
       * @details
       * The variable is constant, after being set by the constructor no
       * further changes are possible.
       *
       * The variable type usually is an unsigned integer where
       * the status register is saved.
       */

      // Enter an IRQ critical section
      status_t
      critical_section::enter (void)
      {
        return port::interrupts::critical_section::enter ();
      }

      // Exit an IRQ critical section
      void
      critical_section::exit (status_t status)
      {
        port::interrupts::critical_section::exit (status);
      }

      // Enter an IRQ uncritical section
      status_t
      uncritical_section::enter (void)
      {
        return port::interrupts::uncritical_section::enter ();
      }

      // Exit an IRQ uncritical section
      void
      uncritical_section::exit (status_t status)
      {
        port::interrupts::uncritical_section::exit (status);
      }

    /**
     * @class Lock
     * @details
     * Locker meeting the standard `Lockable` requirements (30.2.5.3).
     */

    /**
     * @var status_t Lock::status_
     * @details
     * The variable type usually is an unsigned integer where
     * the status register is saved.
     */

    } /* namespace interrupts */

    // ========================================================================

    /**
     * @details
     * The os::rtos::flags namespace groups event types and enumerations.
     */
    namespace flags
    {

    } /* namespace flags */

    // ==========================================================================

    /**
     * @class named_object
     * @details
     * This class serves as a base class for all objects that have a
     * name (most of the RTOS classes do have a name).
     *
     * Attributes use a separate constexpr object.
     */

    /**
     * @var const char* const named_object::name_
     * @details
     * To save space, the null terminated string passed to the
     * constructor is not copied locally. Instead, the pointer to
     * the string is copied, so the
     * caller must ensure that the pointer life cycle
     * is at least as long as the object life cycle. A constant
     * string (stored in flash) is preferred.
     */

    named_object::named_object () :
        name_ ("-")
    {
      ;
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
    named_object::named_object (const char* name) :
        name_ (name != nullptr ? name : "-")
    {
      ;
    }

    /**
     * @details
     * Prefer the given name, then the attribute name, otherwise
     * default to '-'.
     *
     * To save space, instead of copying the null terminated string
     * locally, the pointer to the string
     * is copied, so the caller must ensure that the pointer
     * life cycle is at least as long as the object life cycle.
     * A constant string (stored in flash) is preferred.
     */
    named_object::named_object (const char* given_name, const char* attr_name) :
        name_ (
            given_name != nullptr ?
                given_name : (attr_name != nullptr ? attr_name : "-"))
    {
      ;
    }

  // ==========================================================================
  } /* namespace rtos */
} /* namespace os */

int*
__error (void);

/**
 * @brief Per-thread error support.
 * @details
 * Standard C libraries define `errno` as macro to a function returning
 * a pointer. This function returns such a pointer, specific to each
 * thread.
 * @return Pointer to per-thread errno value.
 */
int*
__error (void)
{
  return os::rtos::this_thread::error ();
}
