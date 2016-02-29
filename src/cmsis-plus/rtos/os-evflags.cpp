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

    namespace evflags
    {
      const Attributes initializer
        { nullptr };
    } /* namespace evflags */

    // ------------------------------------------------------------------------

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Event_flags::Event_flags () :
        Event_flags
          { evflags::initializer }
    {
      ;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Event_flags::Event_flags (const evflags::Attributes& attr) :
        Named_object
          { attr.name () }
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      port::Event_flags::create (this);

#else

      ;

#endif

    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Event_flags::~Event_flags ()
    {
      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      port::Event_flags::destroy (this);

#else

      ;

#endif
    }

    result_t
    Event_flags::_try_wait (flags::mask_t mask, flags::mask_t* oflags,
                            flags::mode_t mode)
    {
      if ((mask != 0) && ((mode & flags::mode::all) != 0))
        {
          // Only if all desires signals are raised we're done.
          if ((flags_ & mask) == mask)
            {
              if (oflags != nullptr)
                {
                  *oflags = flags_;
                }
              // Clear desired signals.
              flags_ &= ~mask;
              return result::ok;
            }
        }
      else if ((mask == 0) || ((mode & flags::mode::any) != 0))
        {
          // Any flag will do it.
          if (flags_ != 0)
            {
              // Possibly return.
              if (oflags != nullptr)
                {
                  *oflags = flags_;
                }
              // Since we returned them all, also clear them all.
              flags_ = 0;
              return result::ok;
            }
        }

      return EAGAIN;
    }

    /**
     * @details
     * If the mode::all bit is set, the function expects
     * all given flags to be raised; otherwise, if the mode::any
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
     * If the mode::clear bit is set, the event flags that are
     * returned are automatically cleared.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Event_flags::wait (flags::mask_t mask, flags::mask_t* oflags,
                       flags::mode_t mode)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::wait (this, mask, oflags, mode);

#else

      for (;;)
        {
            {
              Critical_section_irq cs; // ----- Critical section -----

              if (_try_wait (mask, oflags, mode) == result::ok)
                {
                  return result::ok;
                }
            }

          this_thread::suspend ();

          if (this_thread::thread ().interrupted ())
            {
              return EINTR;
            }
        }
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * If the mode::all bit is set, the function expects
     * all given flags to be raised; otherwise, if the mode::any
     * bit is set, the function expects any single flag to be raised.
     *
     * The function does not blocks, if the expected event flags are
     * not raised, but returns EGAIN.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    Event_flags::try_wait (flags::mask_t mask, flags::mask_t* oflags,
                           flags::mode_t mode)
    {
      Critical_section_irq cs; // ----- Critical section -----

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::try_wait (this, mask, oflags, mode);

#else

      return _try_wait (mask, oflags, mode);

#endif
    }

    /**
     * @details
     * If the mode::all bit is set, the function expects
     * all given flags to be raised; otherwise, if the mode::any
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
     * if the event flags are already raised.
     *
     * If the mode::clear bit is set, the event flags that are
     * returned are automatically cleared.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Event_flags::timed_wait (flags::mask_t mask, flags::mask_t* oflags,
                             flags::mode_t mode, systicks_t ticks)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      if (ticks == 0)
        {
          ticks = 1;
        }

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::timed_wait (this, mask, oflags, mode, ticks);

#else

      Systick_clock::rep start = Systick_clock::now ();
      for (;;)
        {
          Systick_clock::sleep_rep slept_ticks;
            {
              Critical_section_irq cs; // ----- Critical section -----

              if (_try_wait (mask, oflags, mode) == result::ok)
                {
                  return result::ok;
                }
            }

          Systick_clock::rep now = Systick_clock::now ();
          slept_ticks = (Systick_clock::sleep_rep) (now - start);
          if (slept_ticks >= ticks)
            {
              return ETIMEDOUT;
            }

          Systick_clock::wait (ticks - slept_ticks);

          if (this_thread::thread ().interrupted ())
            {
              return EINTR;
            }
        }
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
    Event_flags::raise (flags::mask_t mask, flags::mask_t* oflags)
    {
      os_assert_err(mask != 0, EINVAL);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::raise (this, mask, oflags);

#else

      Critical_section_irq cs; // ----- Critical section -----

      flags_ |= mask;

      if (oflags != nullptr)
        {
          *oflags = flags_;
        }

      if (!list_.empty ())
        {
          // Wake-up all threads, if any.
          list_.wakeup_all ();
        }

      return result::ok;

#endif
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    Event_flags::clear (flags::mask_t mask, flags::mask_t* oflags)
    {
      os_assert_err(mask != 0, EINVAL);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::clear (this, mask, oflags);

#else

      Critical_section_irq cs; // ----- Critical section -----

      if (oflags != nullptr)
        {
          *oflags = flags_;
        }

      // Clear the selected bits; leave the rest untouched.
      flags_ &= ~mask;

      return result::ok;

#endif
    }

    /**
     * @details
     * Select the requested bits from the event flags
     * and return them. If requested, clear the selected bits in the
     * event flags.
     *
     * If the mask is zero, return the status of all event flags,
     * without any masking or subsequent clearing.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    flags::mask_t
    Event_flags::get (flags::mask_t mask, flags::mode_t mode)
    {
#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::get (this, mask, mode);

#else

      Critical_section_irq cs; // ----- Critical section -----

      if (mask == 0)
        {
          // Return the entire mask.
          return flags_;
        }

      flags::mask_t ret = flags_ & mask;
      if ((mode & flags::mode::clear) != 0)
        {
          // Clear the selected bits; leave the rest untouched.
          flags_ &= ~mask;
        }

      // Return the selected bits.
      return ret;

#endif
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    bool
    Event_flags::waiting (void)
    {
#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::waiting (this);

#else

      return !list_.empty ();

#endif
    }

  // --------------------------------------------------------------------------

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
