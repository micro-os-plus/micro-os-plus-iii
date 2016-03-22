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

#include "timers.h"

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
//#pragma GCC diagnostic ignored "-Wunused-parameter"

    /**
     * @details
     * The os::rtos::timer namespace groups timer types, enumerations,
     * attributes and initialisers.
     */
    namespace timer
    {
      /**
       * @class Attributes
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
       * @class Periodic_attributes
       * @details
       * Allow to assign a name to the timer.
       */

      const Attributes once_initializer
        { nullptr };

    } /* namespace timer */

    // ------------------------------------------------------------------------

    /**
     * @class Timer
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
     *   // Create new thread, with function and no arguments.
     *   Timer tm { func, nullptr };
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

    /**
     * @details
     * This constructor shall initialise the timer object
     * with default settings.
     * The effect shall be equivalent to creating a timer object
     * referring to the attributes in `timer::once_initializer`.
     * Upon successful initialisation, the state of the
     * timer object shall become initialised.
     *
     * Only the timer object itself may be used for running
     * the function. It is not allowed to make copies of
     * timer objects.
     *
     * The default timer is a single run timer which uses the
     * `Systick_clock`; the period is expressed
     * in scheduler ticks.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Timer::Timer (timer::func_t function, timer::func_args_t args) :
        Timer
          { timer::once_initializer, function, args }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise the timer object
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
    Timer::Timer (const timer::Attributes& attr, timer::func_t function,
                  timer::func_args_t args) :
        Named_object
          { attr.name () }

    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);
      os_assert_throw(function != nullptr, EINVAL);

      type_ = attr.tm_type;
      func_ = function;
      func_args_ = args;

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_RTOS_PORT_TIMER)

      port::Timer::create (this, function, args);

#else

      // TODO

#endif
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
    Timer::~Timer ()
    {
      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_RTOS_PORT_TIMER)

      port::Timer::destroy (this);

#else

      // TODO

#endif
    }

    /**
     * @details
     * If the period is 0, it is automatically adjusted to 1.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Timer::start (duration_t period)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d) @%p \n", __func__, period, this);

      if (period == 0)
        {
          period = 1;
        }

#if defined(OS_INCLUDE_RTOS_PORT_TIMER)

      return port::Timer::start (this, period);

#else

      // TODO
      return result::ok;

#endif
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
    Timer::stop (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_RTOS_PORT_TIMER)

      return port::Timer::stop (this);

#else

      // TODO
      return result::ok;

#endif
    }

  // ------------------------------------------------------------------------

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
