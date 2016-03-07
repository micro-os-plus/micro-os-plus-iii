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

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis-plus/rtos/port/os-decls.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// ----------------------------------------------------------------------------

#include <cassert>
#include <cerrno>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
// TODO: remove it when fully implemented
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * @details
     * The `os::rtos::condvar` namespace groups condition variable attributes
     * and initialisers.
     */
    namespace condvar
    {
      /**
       * @class Attributes
       * @details
       * Allow to assign a name to the condition variable.
       *
       * @par POSIX compatibility
       *  Inspired by `pthread_condattr_t` from [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  (IEEE Std 1003.1, 2013 Edition).
       */

      const Attributes initializer
        { nullptr };
    } /* namespace condvar */

    // ------------------------------------------------------------------------

    /**
     * @class Condition_variable
     * @details
     * TODO
     *
     * @par POSIX compatibility
     *  Inspired by `pthread_cond_t` from [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  (IEEE Std 1003.1, 2013 Edition).
     *
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     */

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Condition_variable::Condition_variable () :
        Condition_variable
          { condvar::initializer }
    {
      ;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Condition_variable::Condition_variable (const condvar::Attributes& attr) :
        Named_object
          { attr.name () }
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);
    }

    /**
     * @details
     * It shall be safe to destroy an initialised condition variable
     * upon which no threads are currently blocked. Attempting to
     * destroy a condition variable upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Condition_variable::~Condition_variable ()
    {
      trace::printf ("%s() @%p \n", __func__, this);
      // TODO
    }

    /**
     * @details
     * Unblock at least one of the threads that are blocked
     * on the specified condition variable.
     *
     * When each thread unblocked as a result of a
     * Condition_variable::signal() returns from its call to
     * Condition_variable::wait() or Condition_variable::timed_wait(),
     * the thread shall own the mutex with which it called
     * Condition_variable::wait() or Condition_variable::timed_wait().
     * The thread(s) that are unblocked shall contend for
     * the mutex according to the scheduling policy (if applicable),
     * and as if each had called Mutex::lock().
     *
     * Condition_variable::signal() may be called by a thread
     * whether or not it currently owns the mutex that threads
     * calling Condition_variable::wait() or
     * Condition_variable::timed_wait() have associated with
     * the condition variable during their waits; however,
     * if predictable scheduling behavior is required, then
     * that mutex shall be locked by the thread calling
     * Condition_variable::broadcast().
     *
     * The Condition_variable::signal() function shall
     * have no effect if there are no threads currently
     * blocked on this condition variable.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Condition_variable::signal ()
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);
      // TODO
      return result::ok;
    }

    /**
     * @details
     * Unblock all threads currently blocked on the specified
     * condition variable.
     *
     * If more than one thread is blocked on a condition variable,
     * the scheduling policy shall determine the order in which
     * threads are unblocked.
     *
     * When each thread unblocked as a result of a
     * Condition_variable::broadcast() returns from its call to
     * Condition_variable::wait() or Condition_variable::timed_wait(),
     * the thread shall own the mutex with which it called
     * Condition_variable::wait() or Condition_variable::timed_wait().
     * The thread(s) that are unblocked shall contend for
     * the mutex according to the scheduling policy (if applicable),
     * and as if each had called Mutex::lock().
     *
     * Condition_variable::broadcast() may be called by a thread
     * whether or not it currently owns the mutex that threads
     * calling Condition_variable::wait() or
     * Condition_variable::timed_wait() have associated with
     * the condition variable during their waits; however,
     * if predictable scheduling behaviour is required, then
     * that mutex shall be locked by the thread calling
     * Condition_variable::broadcast().
     *
     * The Condition_variable::broadcast() function shall
     * have no effect if there are no threads currently
     * blocked on this condition variable.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Condition_variable::broadcast ()
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);
      // TODO
      return result::ok;
    }

    /**
     * @details
     * Block on a condition variable. The application shall ensure
     * that this function is called with mutex locked by
     * the calling thread; otherwise, an error (for
     * PTHREAD_MUTEX_ERRORCHECK and robust mutexes) or
     * undefined behaviour (for other mutexes) results.
     *
     * TODO: add more.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Condition_variable::wait (Mutex& mutex)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);
      // TODO
      return result::ok;
    }

    /**
     * @details
     * Block on a condition variable. The application shall ensure
     * that this function is called with mutex locked by
     * the calling thread; otherwise, an error (for
     * ERRORCHECK and robust mutexes) or
     * undefined behaviour (for other mutexes) results.
     *
     * TODO: add more.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based (the SysTick clock for CMSIS).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Condition_variable::timed_wait (Mutex& mutex, systicks_t ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks) @%p \n", __func__, ticks, this);
      // TODO
      return result::ok;
    }

  // --------------------------------------------------------------------------

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
