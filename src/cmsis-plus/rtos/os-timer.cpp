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

    namespace timer
    {
      const Attributes once_initializer
        { nullptr };
    } /* namespace timer */

    // ------------------------------------------------------------------------

    /**
     * @details
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
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Timer::Timer (const timer::Attributes& attr, timer::func_t function,
                  timer::func_args_t args) :
        Named_object
          { attr.name () }

    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      assert(function != nullptr);

      type_ = attr.tm_type;
      func_ = function;
      func_args_ = args;

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_TIMER)

      port::Timer::create (this, function, args);

#else

      // TODO

#endif
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Timer::~Timer ()
    {
      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      port::Timer::destroy (this);

#else

      // TODO

#endif
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Timer::start (systicks_t ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d) @%p \n", __func__, ticks, this);

      if (ticks == 0)
        {
          ticks = 1;
        }

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

      return port::Timer::start (this, ticks);

#else

      // TODO
      return result::ok;

#endif
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Timer::stop (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_SEMAPHORE)

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
