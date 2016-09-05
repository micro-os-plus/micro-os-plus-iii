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

#include <cmsis-plus/rtos/os.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    /**
     * @class timer::attributes
     * @details
     * Allow to assign a name to the timer.
     *
     * To simplify access, the member variables are public and do not
     * require accessors or mutators.
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified, but inspired by POSIX
     *  attributes used in [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  (IEEE Std 1003.1, 2013 Edition).
     */

    /**
     * @details
     * This variable can be used to create a single run timer.
     */
    const timer::attributes timer::once_initializer;

    /**
     * @class attributes_periodic
     * @details
     * Allow to assign a name to the timer.
     */

    /**
     * @details
     * This variable can be used to create a periodic timer.
     */
    const timer::attributes_periodic timer::periodic_initializer;

    // ------------------------------------------------------------------------

    /**
     * @class timer
     * @details
     * The CMISIS++ timer schedules the execution of a user function after
     * a programmable interval. If the timer is periodic, the function is
     * rescheduled automatically until the timer is stopped.
     *
     * @par Example
     *
     * @code{.cpp}
     * #include <cmsis-plus/rtos/os.h>
     * #include <cstdlib>
     *
     * using namespace os::rtos;
     *
     * // Thread function.
     * void
     * func(void* args)
     * {
     *   // Do something.
     *   ...
     * }
     *
     * int
     * os_main(int argc, char* argv[])
     * {
     *   // Construct new thread, with function and no arguments.
     *   timer tm { func, nullptr };
     *
     *   // Schedule func() to be executed after 100 ticks.
     *   tm.start(100);
     *
     *   // Do something.
     *
     *   return 0;
     * }
     * @endcode
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified.
     */

    // ========================================================================
    /**
     * @details
     * This constructor shall initialise a timer object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the timer attributes shall not be affected.
     *
     * Upon successful initialisation, the state of the
     * timer object shall become initialised.
     *
     * Only the timer object itself may be used for running
     * the function. It is not allowed to make copies of
     * timer objects.
     *
     * In cases where default condition variable attributes are
     * appropriate, the variables `timer::once_initializer`
     * or `timer::periodic_initializer` can be used to
     * initialise timers.
     * The effect shall be equivalent to creating a timer
     * object with the simple constructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    timer::timer (func_t function, func_args_t args, const attributes& attr) :
        timer
          { nullptr, function, args, attr }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a named timer object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the timer attributes shall not be affected.
     *
     * Upon successful initialisation, the state of the
     * timer object shall become initialised.
     *
     * Only the timer object itself may be used for running
     * the function. It is not allowed to make copies of
     * timer objects.
     *
     * In cases where default condition variable attributes are
     * appropriate, the variables `timer::once_initializer`
     * or `timer::periodic_initializer` can be used to
     * initialise timers.
     * The effect shall be equivalent to creating a timer
     * object with the simple constructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    timer::timer (const char* name, func_t function, func_args_t args,
                  const attributes& attr) :
        object_named_system
          { name }
    {
#if defined(OS_TRACE_RTOS_TIMER)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif

      os_assert_throw(!interrupts::in_handler_mode (), EPERM);
      os_assert_throw(function != nullptr, EINVAL);

      type_ = attr.tm_type;
      func_ = function;
      func_args_ = args;

#if !defined(OS_USE_RTOS_PORT_TIMER)
      clock_ = attr.clock != nullptr ? attr.clock : &sysclock;
#endif

#if defined(OS_USE_RTOS_PORT_TIMER)

      port::timer::create (this, function, args);

#else

      period_ = 0;

#endif
      state_ = state::initialized;
    }

    /**
     * @details
     * This destructor shall destroy the timer object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * If the timer is running, it must be automatically stopped.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    timer::~timer ()
    {
#if defined(OS_TRACE_RTOS_TIMER)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if defined(OS_USE_RTOS_PORT_TIMER)

      port::timer::destroy (this);

#else

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          if (state_ == state::running)
            {
              timer_node_.unlink ();
            }
          // ----- Exit critical section --------------------------------------
        }

#endif
      state_ = state::destroyed;
    }

    /**
     * @details
     * If the period is 0, it is automatically adjusted to 1.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    timer::start (clock::duration_t period)
    {
#if defined(OS_TRACE_RTOS_TIMER)
      trace::printf ("%s(%u) @%p %s\n", __func__,
                     static_cast<unsigned int> (period), this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

      if (period == 0)
        {
          period = 1;
        }

      result_t res;

#if defined(OS_USE_RTOS_PORT_TIMER)

      res = port::timer::start (this, period);

#else

      period_ = period;

      timer_node_.timestamp = clock_->steady_now () + period;

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          // If started, stop.
          timer_node_.unlink ();

          clock_->steady_list ().link (timer_node_);
          // ----- Exit critical section --------------------------------------
        }
      res = result::ok;

#endif

      if (res == result::ok)
        {
          state_ = state::running;
        }
      return res;
    }

    /**
     * @details
     * Remove the timer from the clock schedule list, so that the next
     * execution of the function is cancelled.
     *
     * A stopped timer can be restarted later with start()`.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    timer::stop (void)
    {
#if defined(OS_TRACE_RTOS_TIMER)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

      if (state_ != state::running)
        {
          return EAGAIN;
        }

      result_t res;

#if defined(OS_USE_RTOS_PORT_TIMER)

      res = port::timer::stop (this);

#else

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          timer_node_.unlink ();
          // ----- Exit critical section --------------------------------------
        }
      res = result::ok;

#endif

      state_ = state::stopped;
      return res;
    }

#if !defined(OS_USE_RTOS_PORT_TIMER)

    /**
     * @cond ignore
     */

    void
    timer::internal_interrupt_service_routine (void)
    {

      if (type_ == run::periodic)
        {
          // Re-arm the timer for the next period.
          timer_node_.timestamp += period_;

          // No need for critical section in ISR.
          clock_->steady_list ().link (timer_node_);
        }
      else
        {
          state_ = state::completed;
        }

#if defined(OS_USE_RTOS_PORT_TIMER)
      trace::puts (name ());
#endif

      // Call the user function.
      func_ (func_args_);
    }

  /**
   * @endcond
   */

#endif

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
