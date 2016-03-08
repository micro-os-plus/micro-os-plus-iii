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

    /**
     * @details
     * The os::rtos::semaphore namespace groups semaphore types,
     * attributes and initialisers.
     */
    namespace semaphore
    {
      /**
       * @class Attributes
       * @details
       * Allow to assign a name and custom attributes (like initial count,
       * max count) to the semaphore.
       *
       * To simplify access, the member variables are public and do not
       * require accessors or mutators.
       *
       * @par POSIX compatibility
       *  No POSIX similar functionality identified, but inspired by POSIX
       *  attributes used in [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      /**
       * @var count_t Attributes::sm_initial_count
       * @details
       * This values represents the number of resources initially
       * available to the semaphore.
       */

      /**
       * @var count_t Attributes::sm_max_count
       * @details
       * This values represents the maximum number of resources
       * available to the semaphore.
       */

      /**
       * @class Binary_attributes
       * @details
       * Allow to assign a name and custom attributes to the semaphore.
       *
       * @par POSIX compatibility
       *  No POSIX similar functionality identified, but inspired by POSIX
       *  attributes used in [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      const Attributes counting_initializer
        { nullptr };

      const Binary_attributes binary_initializer
        { nullptr };

    } /* namespace semaphore */

    // ------------------------------------------------------------------------

    /**
     * @class Semaphore
     * @details
     * Semaphores allow threads and interrupts to synchronise their actions.
     *
     * A useful way to think of a semaphore as used in the real-world
     * systems is as a record of how many units of a particular resource
     * are available, coupled with operations to safely (i.e., without
     * race conditions) adjust that record as units are required or
     * become free, and, if necessary, wait until a unit of the resource
     * becomes available. Semaphores are a useful tool in the prevention
     * of race conditions; however, their use is by no means a guarantee
     * that a program is free from these problems. Semaphores which allow
     * an arbitrary resource count are called counting semaphores, while
     * semaphores which are restricted to the values 0 and 1 (or
     * locked/unlocked, unavailable/available) are called binary semaphores.
     *
     * Semaphores should generally be used to synchronise threads with
     * events occurring on interrupts.
     *
     * For inter-thread synchronisation, to avoid cases of priority
     * inversion, more suitable are mutexes.
     *
     * @par POSIX compatibility
     *  Inspired by `sem_t`
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @details
     * Create and initialise a semaphore with default attributes
     * (count=0 and no limit for count).
     *
     * Compatible with POSIX `sem_init()`.
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html#
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Semaphore::Semaphore () :
        Semaphore
          { semaphore::counting_initializer }
    {
      ;
    }

    /**
     * @details
     * Create and initialise a semaphore with custom attributes
     * (initial and max count from user defined attributes).
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Semaphore::Semaphore (const semaphore::Attributes& attr) :
        Named_object
          { attr.name () }, //
        initial_count_ (attr.sm_initial_count), //
        max_count_ (attr.sm_max_count)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      // The CMSIS validator requires the max_count to be equal to
      // the initial count, which can be 0, but we patch it on the way.
      assert (max_count_ > 0);
      assert (attr.sm_initial_count >= 0);
      assert (attr.sm_initial_count <= max_count_);

      count_ = attr.sm_initial_count;

      trace::printf ("%s() @%p %s %d %d\n", __func__, this, name (), count_,
                     max_count_);

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      port::Semaphore::create (this);

#else

      // TODO

#endif
    }

    /**
     * @details
     * It is safe to destroy an initialised semaphore upon which
     * no threads are currently blocked. The effect of destroying
     * a semaphore upon which other threads are currently blocked
     * is undefined.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_destroy()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Semaphore::~Semaphore ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      port::Semaphore::destroy (this);

#else

      // TODO

#endif
    }

    /**
     * @details
     * Perform a post operation on the semaphore, informing
     * the waiting consumers that one more resource is available.
     * The semaphore count is incremented, up to max_count.
     *
     * If the semaphore count resulting from this operation is
     * positive, then no threads were blocked waiting for the
     * semaphore to become unlocked.
     *
     * If the count of the semaphore resulting from this operation
     * is zero, then one of the threads blocked waiting for the
     * semaphore shall be allowed to return successfully from its
     * call to `wait()`.
     *
     * If the Process Scheduling option is supported, the thread
     * to be unblocked shall be chosen in a manner appropriate to
     * the scheduling policies and parameters in effect for the
     * blocked threads. In the case of the schedulers SCHED_FIFO
     * and SCHED_RR, the highest priority waiting thread shall
     * be unblocked, and if there is more than one highest
     * priority thread blocked waiting for the semaphore, then
     * the highest priority thread that has been waiting the
     * longest shall be unblocked. If the Process Scheduling
     * option is not defined, the choice of a thread to unblock
     * is unspecified. If the scheduling policy is SCHED_SPORADIC,
     * the semantics are as per SCHED_FIFO.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_post()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_post.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @note Can be invoked from Interrupt Service Routines.
     *
     * @warning Applications using these functions may be subject to priority inversion.
     */
    result_t
    Semaphore::post (void)
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      return port::Semaphore::post (this);

#else

      interrupts::Critical_section cs; // ----- Critical section -----
      if (count_ >= this->max_count_)
        {
          return EOVERFLOW;
        }

      ++count_;

      if (count_ == 0)
        {
          // Wakeup one thread
          if (list_.get_length () > 0)
            {
              list_.get_top ()->wakeup ();
            }
        }
      return result::ok;

#endif
    }

    /**
     * @details
     * Perform a lock operation on the semaphore.
     *
     * If the current value is positive, it is decremented, and
     * the call returns immediately
     *
     * If the semaphore
     * value is currently zero, then the calling thread shall not
     * return from the call to wait() until it either
     * locks the semaphore or the call is interrupted by a signal.
     *
     * Upon successful return, the state of the semaphore shall
     * be locked and shall remain locked until `the post()`
     * function is executed and returns successfully.
     *
     * The function is interruptible by the delivery of an external
     * event (signal, thread cancel, etc).
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_wait()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_wait.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @warning Applications using these functions may be subject to priority inversion.
     */
    result_t
    Semaphore::wait ()
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      return port::Semaphore::wait (this);

#else
        {
          interrupts::Critical_section cs; // ----- Critical section ------

          --count_;
          if (count_ >= 0)
            {
              return result::ok;
            }

          // Add current thread to the semaphore waiting  list
          list_.add (&this_thread::thread ());
          // `count_` is negative.
        }
      this_thread::yield ();

      // Return EINTR?
      return result::ok;

#endif
    }

    /**
     * @details
     * Tries to perform a lock operation only if the semaphore is currently
     * not locked; that is, if the semaphore value is currently
     * positive it decrements it and returns success.
     * Otherwise, it shall not lock the semaphore.
     *
     * Upon successful return, the state of the semaphore shall
     * be locked and shall remain locked until the `post()`
     * function is executed and returns successfully.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_trywait()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_trywait.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @note Can be invoked from Interrupt Service Routines.
     *
     * @warning Applications using these functions may be subject to priority inversion.
     */
    result_t
    Semaphore::try_wait ()
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      return port::Semaphore::try_wait (this);

#else

        {
          interrupts::Critical_section cs; // ----- Critical section -----

          if (count_ > 0)
            {
              --count_;
              return result::ok;
            }

          // Count may be 0 or negative
          return EAGAIN;
        }

#endif
    }

    /**
     * @details
     * Try to perform a lock operation; if the semaphore cannot be
     * locked without waiting for another process or thread to
     * unlock the semaphore by performing a Semaphore::post()
     * function, this wait shall be terminated when the specified
     * timeout expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based.
     *
     * Under no circumstance shall the function fail with a timeout
     * if the semaphore can be locked immediately. The validity of
     * the timeout need not be checked if the semaphore can be
     * locked immediately.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_timedwait()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_timedwait.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - the timeout is not expressed as an absolute time point, but
     * as a relative number of timer ticks (by default, the SysTick
     * clock for CMSIS).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @warning Applications using these functions may be subject to priority inversion.
     */
    result_t
    Semaphore::timed_wait (systicks_t timeout)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks) @%p %s\n", __func__, timeout, this, name ());

      if (timeout == 0)
        {
          timeout = 1;
        }

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      return port::Semaphore::timed_wait (this, timeout);

#else

        {
          interrupts::Critical_section cs; // ----- Critical section -----

          --count_;
          if (count_ >= 0)
            {
              return result::ok;
            }

          // Add current thread to the semaphore waiting list.
          list_.add (&this_thread::thread ());
        }
      Systick_clock::wait (timeout);

      // TODO: return EINTR
      return ETIMEDOUT;

#endif
    }

    /**
     * @details
     * Reset the counter to the initial value.
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Semaphore::reset (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      return port::Semaphore::reset (this);

#else

      interrupts::Critical_section cs; // ----- Critical section -----

      if (count_ < 0)
        {
          // There are waiting tasks
          return EAGAIN;
        }

      count_ = initial_count_;
      return result::ok;

#endif
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
