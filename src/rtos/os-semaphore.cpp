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

#include <cmsis-plus/diag/trace.h>

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

    /**
     * @class semaphore::attributes
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
     * @var semaphore::count_t semaphore::attributes::sm_initial_value
     * @details
     * This values represents the number of resources initially
     * available to the semaphore.
     */

    /**
     * @var semaphore::count_t semaphore::attributes::sm_max_value
     * @details
     * This values represents the maximum number of resources
     * available to the semaphore.
     */

    /**
     * @class attributes_binary
     * @details
     * Allow to assign a name and custom attributes to the semaphore.
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified, but inspired by POSIX
     *  attributes used in [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @details
     * This variable is used by the default constructor.
     */
    const semaphore::attributes_binary semaphore::initializer_binary
      { 0 };

    // ------------------------------------------------------------------------

    /**
     * @class semaphore
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
     * This constructor shall initialise a named generic semaphore object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the semaphore attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * semaphore object shall become initialised.
     *
     * Only the semaphore object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * semaphore objects.
     *
     * In cases where default semaphore attributes are
     * appropriate, the variable `semaphore::initializer_binary`
     * can be used to
     * initialise semaphores.
     * The effect shall be equivalent to creating a semaphore
     * object with the default constructor.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    semaphore::semaphore (const char* name, const attributes& attr) :
        semaphore
          { name, attr.sm_max_value, attr.sm_initial_value, attr }
    {
      ;
    }

    /**
     * @cond ignore
     */

    semaphore::semaphore (const char* name, const count_t max_value,
                          const count_t initial_value, const attributes& attr) :
        object_named
          { name }, //
        max_value_ (max_value), //
        initial_value_ (initial_value)
    {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s() @%p %s %u %u\n", __func__, this, this->name (),
                     initial_value, max_value_);
#endif

      os_assert_throw(!interrupts::in_handler_mode (), EPERM);

      // The CMSIS validator requires the max_value to be equal to
      // the initial count, which can be 0, but we patch it on the way.
      assert(max_value_ > 0);
      assert(initial_value >= 0);
      assert(initial_value <= max_value_);

      count_ = initial_value;

#if !defined(OS_USE_RTOS_PORT_SEMAPHORE)
      clock_ = attr.clock != nullptr ? attr.clock : &sysclock;
#endif

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)

      port::semaphore::create (this);

#else

      internal_init_ ();

#endif
    }

    /**
     * @endcond
     */

    /**
     * @details
     * This destructor shall destroy the semaphore object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
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
    semaphore::~semaphore ()
    {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)

      port::semaphore::destroy (this);

#else

      assert(list_.empty ());

#endif
    }

    /**
     * @cond ignore
     */

    void
    semaphore::internal_init_ (void)
    {

      count_ = initial_value_;

#if !defined(OS_USE_RTOS_PORT_SEMAPHORE)

      // Wake-up all threads, if any.
      // Need not be inside the critical section,
      // the list is protected by inner `resume_one()`.
      list_.resume_all ();

#endif /* !defined(OS_USE_RTOS_PORT_SEMAPHORE) */
    }

    /*
     * Internal function.
     * Should be called from an interrupts critical section.
     */
    bool
    semaphore::internal_try_wait_ (void)
    {
      if (count_ > 0)
        {
          --count_;
#if defined(OS_TRACE_RTOS_SEMAPHORE)
          trace::printf ("%s() @%p %s >%u\n", __func__, this, name (), count_);
#endif
          return true;
        }

      // Count may be 0.
#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s() @%p %s false\n", __func__, this, name ());
#endif
      return false;
    }

    /**
     * @endcond
     */

    /**
     * @details
     * Perform a post operation on the semaphore, informing
     * the waiting consumers that one more resource is available.
     * The semaphore count is incremented, up to max_value.
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
    semaphore::post (void)
    {

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)

#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      return port::semaphore::post (this);

#else

      assert(port::interrupts::is_priority_valid ());

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (count_ >= this->max_value_)
            {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
              trace::printf ("%s() @%p %s EAGAIN\n", __func__, this, name ());
#endif
              return EAGAIN;
            }

          ++count_;
#if defined(OS_TRACE_RTOS_SEMAPHORE)
          trace::printf ("%s() @%p %s count %u\n", __func__, this, name (),
                         count_);
#endif
          // ----- Exit critical section --------------------------------------
        }

      // Wake-up one thread.
      list_.resume_one ();

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
     * be locked and shall remain locked until the `post()`
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
    semaphore::wait ()
    {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s() @%p %s <%u\n", __func__, this, name (), count_);
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)

      return port::semaphore::wait (this);

#else

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_wait_ ())
            {
              return result::ok;
            }
          // ----- Exit critical section --------------------------------------
        }

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              interrupts::critical_section ics;

              if (internal_try_wait_ ())
                {
                  return result::ok;
                }

              // Add this thread to the semaphore waiting list.
              scheduler::internal_link_node (list_, node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the semaphore waiting list,
          // if not already removed by post().
          scheduler::internal_unlink_node (node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
              trace::printf ("%s() EINTR @%p %s\n", __func__, this, name ());
#endif
              return EINTR;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

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
    semaphore::try_wait ()
    {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s() @%p %s <%u\n", __func__, this, name (), count_);
#endif

      assert(port::interrupts::is_priority_valid ());

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)

      return port::semaphore::try_wait (this);

#else

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_wait_ ())
            {
              return result::ok;
            }
          else
            {
              return EWOULDBLOCK;
            }
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

    /**
     * @details
     * Try to perform a lock operation; if the semaphore cannot be
     * locked without waiting for another process or thread to
     * unlock the semaphore by performing a `post()`
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
     * The clock used for timeouts can be specified via the `clock`
     * attribute. By default, the clock derived from the scheduler
     * timer is used, and the durations are expressed in ticks.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_timedwait()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_timedwait.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - the timeout is not expressed as an absolute time point, but
     * as a relative number of timer ticks (by default, the SysTick
     * clock for CMSIS).
     *  - for consistency reasons, EWOULDBLOCK is used, instead of EAGAIN
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @warning Applications using these functions may be subject to priority inversion.
     */
    result_t
    semaphore::timed_wait (clock::duration_t timeout)
    {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s(%u) @%p %s <%u\n", __func__,
                     static_cast<unsigned int> (timeout), this, name (),
                     count_);
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)

      return port::semaphore::timed_wait (this, timeout);

#else

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (internal_try_wait_ ())
            {
              return result::ok;
            }
          // ----- Exit critical section --------------------------------------
        }

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      internal::clock_timestamps_list& clock_list = clock_->steady_list ();
      clock::timestamp_t timeout_timestamp = clock_->steady_now () + timeout;

      // Prepare a timeout node pointing to the current thread.
      internal::timeout_thread_node timeout_node
        { timeout_timestamp, crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              interrupts::critical_section ics;

              if (internal_try_wait_ ())
                {
                  return result::ok;
                }

              // Add this thread to the semaphore waiting list,
              // and the clock timeout list.
              scheduler::internal_link_node (list_, node, clock_list,
                                             timeout_node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the semaphore waiting list,
          // if not already removed by post() and from the clock
          // timeout list, if not already removed by the timer.
          scheduler::internal_unlink_node (node, timeout_node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
              trace::printf ("%s(%u) EINTR @%p %s\n", __func__,
                             static_cast<unsigned int> (timeout), this,
                             name ());
#endif
              return EINTR;
            }

          if (clock_->steady_now () >= timeout_timestamp)
            {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
              trace::printf ("%s(%u) ETIMEDOUT @%p %s\n", __func__,
                             static_cast<unsigned int> (timeout), this,
                             name ());
#endif
              return ETIMEDOUT;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * The `value()` function shall return the value of the semaphore
     * without affecting the state of the semaphore. The value represents
     * an actual semaphore value that occurred at some unspecified time
     * during the call, but it need not be the actual value of the
     * semaphore when it is returned to the calling process. This
     * value reflects the number of available resources.
     *
     * If the semaphore is locked, then the returned value shall either
     * be set to zero or to a negative number whose absolute value
     * represents the number of processes waiting for the semaphore
     * at some unspecified time during the call.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_getvalue()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_getvalue.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    semaphore::count_t
    semaphore::value (void) const
    {
#if !defined(OS_USE_RTOS_PORT_SEMAPHORE)
      return (count_ > 0) ? count_ : 0;
#else
      return count_;
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
    semaphore::reset (void)
    {
#if defined(OS_TRACE_RTOS_SEMAPHORE)
      trace::printf ("%s() @%p %s <%u\n", __func__, this, name (), count_);
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)

      return port::semaphore::reset (this);

#else

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          internal_init_ ();
          return result::ok;
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
