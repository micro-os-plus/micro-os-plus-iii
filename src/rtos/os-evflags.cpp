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
     * @class event_flags::attributes
     * @details
     * Allow to assign a name to the event flags.
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified, but inspired by POSIX
     *  attributes used in [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  (IEEE Std 1003.1, 2013 Edition).
     */

    /**
     * @details
     * This variable is used by the default constructor.
     */
    const event_flags::attributes event_flags::initializer;

    // ------------------------------------------------------------------------

    /**
     * @class event_flags
     * @details
     * Synchronised set of flags that can be used to notify events
     * between threads or between ISRs and threads.
     *
     * @par Example
     *
     * @code{.cpp}
     * event_flags ev;
     *
     * void
     * consumer(void)
     * {
     *   // Do something
     *   for (; some_condition();)
     *     {
     *       ev.wait(0x3)
     *       // ...
     *       // Both flags were raised
     *     }
     *   // Do something else.
     * }
     *
     * void
     * producer1(void)
     * {
     *   // Do something
     *   ev.raise(0x1);
     *   // ...
     *   // Do something else.
     * }
     *
     * void
     * producer2(void)
     * {
     *   // Do something
     *   ev.raise(0x2);
     *   // ...
     *   // Do something else.
     * }
     * @endcode
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified.
     */

    /**
     * @details
     * This constructor shall initialise an event flags object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the event flags attributes shall not be affected.
     * Upon successful initialisation, the state of the event
     * flags object shall become initialised, with all flags cleared.
     *
     * Only the event flags object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * event flags objects.
     *
     * In cases where default event flags attributes are
     * appropriate, the variable `event_flags::initializer` can be used to
     * initialise event flags.
     * The effect shall be equivalent to creating an event flags object with
     * the default constructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    event_flags::event_flags (const attributes& attr) :
        event_flags
          { nullptr, attr }
    {
      ;
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * @details
     * This constructor shall initialise a named event flags object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the event flags attributes shall not be affected.
     * Upon successful initialisation, the state of the event
     * flags object shall become initialised, with all flags cleared.
     *
     * Only the event flags object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * event flags objects.
     *
     * In cases where default event flags attributes are
     * appropriate, the variable `event_flags::initializer` can be used to
     * initialise event flags.
     * The effect shall be equivalent to creating an event flags object with
     * the default constructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    event_flags::event_flags (const char* name, const attributes& attr) :
        object_named
          { name }
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif

      os_assert_throw(!interrupts::in_handler_mode (), EPERM);

#if !defined(OS_USE_RTOS_PORT_EVENT_FLAGS)
      clock_ = attr.clock != nullptr ? attr.clock : &sysclock;
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      port::event_flags::create (this);

#else

#endif

    }

#pragma GCC diagnostic pop

    /**
     * @details
     * This destructor shall destroy the event flags object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised event flags object
     * upon which no threads are currently blocked. Attempting to
     * destroy an event flags object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    event_flags::~event_flags ()
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      port::event_flags::destroy (this);

#else

      assert(list_.empty ());

#endif
    }

    /**
     * @details
     * If the `flags::mode::all` bit is set, the function expects
     * all requested flags to be raised; otherwise, if the `flags::mode::any`
     * bit is set, the function expects any single flag to be raised.
     *
     * If the expected event flags are
     * raised, the function returns instantly.
     *
     * Otherwise suspend the execution of the current thread until all/any
     * specified event flags are raised.
     *
     * When the parameter mask is 0, the current thread is suspended
     * until any event flag is raised. In this case, if any event flags
     * are already raised, the function returns instantly.
     *
     * If the flags::mode::clear bit is set, the event flags that are
     * returned are automatically cleared.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    event_flags::wait (flags::mask_t mask, flags::mask_t* oflags,
                       flags::mode_t mode)
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X,%u) @%p %s <0x%X\n", __func__, mask, mode, this,
                     name (), event_flags_.mask ());
#endif

      os_assert_throw(!interrupts::in_handler_mode (), EPERM);
      os_assert_throw(!scheduler::locked (), EPERM);

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      return port::event_flags::wait (this, mask, oflags, mode);

#else

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (event_flags_.check_raised (mask, oflags, mode))
            {
#if defined(OS_TRACE_RTOS_EVFLAGS)
              trace::printf ("%s(0x%X,%u) @%p %s >0x%X\n", __func__, mask, mode,
                             this, name (), event_flags_.mask ());
#endif
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

              if (event_flags_.check_raised (mask, oflags, mode))
                {
#if defined(OS_TRACE_RTOS_EVFLAGS)
                  trace::printf ("%s(0x%X,%u) @%p %s >0x%X\n", __func__, mask,
                                 mode, this, name (), event_flags_.mask ());
#endif
                  return result::ok;
                }

              // Add this thread to the event flags waiting list.
              scheduler::internal_link_node (list_, node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

            {
              // ----- Enter critical section ---------------------------------
              interrupts::critical_section ics;

              // Remove the thread from the event flags waiting list,
              // if not already removed by raise().
              scheduler::internal_unlink_node (node);
              // ----- Exit critical section ----------------------------------
            }

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_EVFLAGS)
              trace::printf ("%s(0x%X,%u) EINTR @%p %s\n", __func__, mask, mode,
                             this, name ());
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
     * If the flags::mode::all bit is set, the function expects
     * all requested flags to be raised; otherwise, if the flags::mode::any
     * bit is set, the function expects any single flag to be raised.
     *
     * The function does not blocks, if the expected event flags are
     * not raised, but returns EGAIN.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    event_flags::try_wait (flags::mask_t mask, flags::mask_t* oflags,
                           flags::mode_t mode)
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X,%u) @%p %s <0x%X\n", __func__, mask, mode, this,
                     name (), event_flags_.mask ());
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      return port::event_flags::try_wait (this, mask, oflags, mode);

#else

      assert(port::interrupts::is_priority_valid ());

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (event_flags_.check_raised (mask, oflags, mode))
            {
#if defined(OS_TRACE_RTOS_EVFLAGS)
              trace::printf ("%s(0x%X,%u) @%p %s >0x%X\n", __func__, mask, mode,
                             this, name (), event_flags_.mask ());
#endif
              return result::ok;
            }
          else
            {
#if defined(OS_TRACE_RTOS_EVFLAGS)
              trace::printf ("%s(0x%X,%u) EWOULDBLOCK @%p %s \n", __func__,
                             mask, mode, this, name ());
#endif
              return EWOULDBLOCK;
            }
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

    /**
     * @details
     * If the flags::mode::all bit is set, the function expects
     * all requested flags to be raised; otherwise, if the flags::mode::any
     * bit is set, the function expects any single flag to be raised.
     *
     * If the expected event flags are
     * raised, the function returns instantly.
     *
     * Otherwise suspend the execution of the current thread until all/any
     * specified event flags are raised.
     *
     * When the parameter mask is 0, the current thread is suspended
     * until any event flag is raised. In this case, if any event flags
     * are already raised, the function returns instantly.
     *
     * The wait shall be terminated when the specified timeout
     * expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based (the SysTick clock for CMSIS).
     *
     * Under no circumstance shall the operation fail with a timeout
     * if the event flags are already raised. The validity of
     * the timeout need not be checked if the expected flags
     * are already raised and the call can return immediately.
     *
     * The clock used for timeouts can be specified via the `clock`
     * attribute. By default, the clock derived from the scheduler
     * timer is used, and the durations are expressed in ticks.
     *
     * If the flags::mode::clear bit is set, the event flags that are
     * returned are automatically cleared.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    event_flags::timed_wait (flags::mask_t mask, clock::duration_t timeout,
                             flags::mask_t* oflags, flags::mode_t mode)
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X,%u,%u) @%p %s <0x%X\n", __func__, mask, timeout,
                     mode, this, name (), event_flags_.mask ());
#endif

      os_assert_throw(!interrupts::in_handler_mode (), EPERM);
      os_assert_throw(!scheduler::locked (), EPERM);

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      return port::event_flags::timed_wait (this, mask, timeout, oflags, mode);

#else

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (event_flags_.check_raised (mask, oflags, mode))
            {
#if defined(OS_TRACE_RTOS_EVFLAGS)
              trace::printf ("%s(0x%X,%u,%u) @%p %s >0x%X\n", __func__, mask,
                             timeout, mode, this, name (),
                             event_flags_.mask ());
#endif
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

              if (event_flags_.check_raised (mask, oflags, mode))
                {
#if defined(OS_TRACE_RTOS_EVFLAGS)
                  trace::printf ("%s(0x%X,%u,%u) @%p %s >0x%X\n", __func__,
                                 mask, timeout, mode, this, name (),
                                 event_flags_.mask ());
#endif
                  return result::ok;
                }

              // Add this thread to the event flags waiting list,
              // and the clock timeout list.
              scheduler::internal_link_node (list_, node, clock_list,
                                             timeout_node);
              // state::suspended set in above link().
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the event flags waiting list,
          // if not already removed by raise() and from the clock
          // timeout list, if not already removed by the timer.
          scheduler::internal_unlink_node (node, timeout_node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_EVFLAGS)
              trace::printf ("%s(0x%X,%u,%u) EINTR @%p %s 0x%X \n", __func__,
                             mask, timeout, mode, this, name ());
#endif
              return EINTR;
            }

          if (clock_->steady_now () >= timeout_timestamp)
            {
#if defined(OS_TRACE_RTOS_EVFLAGS)
              trace::printf ("%s(0x%X,%u,%u) ETIMEDOUT @%p %s 0x%X \n",
                             __func__, mask, timeout, mode, this, name ());
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
     * Set more bits in the thread current signal mask.
     * Use OR at bit-mask level.
     * Wake-up all waiting threads, if any.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    event_flags::raise (flags::mask_t mask, flags::mask_t* oflags)
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X) @%p %s <0x%X \n", __func__, mask, this, name (),
                     event_flags_.mask ());
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      os_assert_err(mask != 0, EINVAL);

      return port::event_flags::raise (this, mask, oflags);

#else

      result_t res = event_flags_.raise (mask, oflags);

      // Wake-up all threads, if any.
      // Need not be inside the critical section,
      // the list is protected by inner `resume_one()`.
      list_.resume_all ();

#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X) @%p %s >0x%X\n", __func__, mask, this, name (),
                     event_flags_.mask ());
#endif
      return res;

#endif
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    event_flags::clear (flags::mask_t mask, flags::mask_t* oflags)
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X) @%p %s <0x%X \n", __func__, mask, this, name (),
                     event_flags_.mask ());
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      os_assert_err(mask != 0, EINVAL);

      return port::event_flags::clear (this, mask, oflags);

#else

      result_t res = event_flags_.clear (mask, oflags);

#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X) @%p %s >0x%X\n", __func__, mask, this, name (),
                     event_flags_.mask ());
#endif

      return res;

#endif
    }

    /**
     * @details
     * Select the requested event flags
     * and return them. If requested, clear the selected bits in the
     * event flags.
     *
     * If the mask is `flags::any`, return the status of all event flags,
     * without any masking or subsequent clearing.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    flags::mask_t
    event_flags::get (flags::mask_t mask, flags::mode_t mode)
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X) @%p %s  \n", __func__, mask, this, name ());
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      return port::event_flags::get (this, mask, mode);

#else

      flags::mask_t ret = event_flags_.get (mask, mode);

#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s(0x%X)=0x%X @%p %s \n", __func__, mask,
                     event_flags_.mask (), this, name ());
#endif
      // Return the selected flags.
      return ret;

#endif
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    bool
    event_flags::waiting (void)
    {
#if defined(OS_TRACE_RTOS_EVFLAGS)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)

      return port::event_flags::waiting (this);

#else

      assert(port::interrupts::is_priority_valid ());

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          return !list_.empty ();
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
