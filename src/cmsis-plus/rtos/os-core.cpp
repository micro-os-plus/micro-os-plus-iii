/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#pragma GCC diagnostic push
// TODO: remove it when fully implemented
#pragma GCC diagnostic ignored "-Wunused-parameter"

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
       * Modified by `lock()` and restored to previous value
       * by `unlock()`.
       */
      status_t is_locked_ = false;

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

        trace::printf ("%s() \n", __func__);

        return port::scheduler::initialize ();
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
      result_t
      start (void)
      {
        os_assert_err(!scheduler::in_handler_mode (), EPERM);

        trace::printf ("%s() \n", __func__);

        is_started_ = true;
        is_locked_ = false;

        return port::scheduler::start ();
      }

      /**
       * @details
       * Lock the scheduler (prevent it for doing thread switches) and
       * return the previous status, to be restored by `unlock()`.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      status_t
      lock (void)
      {
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

        port::scheduler::lock ();

        status_t tmp = is_locked_;
        is_locked_ = true;

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

        port::scheduler::unlock (status);

        is_locked_ = status;
      }

    /**
     * @class Critical_section
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
     *      scheduler::Critical_section cs;  // Critical section begins here.
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
     * @var const status_t Critical_section::status_
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

#if 0
    namespace scheduler
      {

        void
        __register_thread (Thread* thread)
          {
            // TODO
#if defined(TESTING)
            thread->__run_function ();
#endif
          }

        void
        __unregister_thread (Thread* thread)
          {
            return;
          }

      } /* namespace scheduler */
#endif

    /**
     * @details
     * The os::rtos::interrupts namespace groups interrupts related
     *  types and enumerations.
     */
    namespace interrupts
    {
      /**
       * @class Critical_section
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
       *      interrupts::Critical_section cs;  // Critical section begins here.
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
       * @var const status_t Critical_section::status_
       * @details
       * The variable is constant, after being set by the constructor no
       * further changes are possible.
       *
       * The variable type usually is an unsigned integer where
       * the status register is saved.
       */

      // Enter an IRQ critical section
      status_t
      Critical_section::enter (void)
      {
        return port::interrupts::Critical_section::enter ();
      }

      // Exit an IRQ critical section
      void
      Critical_section::exit (status_t status)
      {
        return port::interrupts::Critical_section::exit (status);
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

    // ========================================================================

    /**
     * @class Named_object
     * @details
     * This class serves as a base class for all objects that have a
     * name (most of the RTOS classes do have a name).
     */

    /**
     * @var const char* const Named_object::name_
     * @details
     * To save space, the null terminated string passed to the
     * constructor is not copied locally. Instead, the pointer to
     * the string is copied, so the
     * caller must ensure that the pointer life cycle
     * is at least as long as the object life cycle. A constant
     * string (stored in flash) is preferred.
     */

#if 1
    /**
     * @details
     * To save space, instead of copying the null terminated string
     * locally, the pointer to the string
     * is copied, so the caller must ensure that the pointer
     * life cycle is at least as long as the object life cycle.
     * A constant string (stored in flash) is preferred.
     */
    Named_object::Named_object (const char* name) :
        name_ (name != nullptr ? name : "-")
    {
      ;
    }
#endif

  // ==========================================================================

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
