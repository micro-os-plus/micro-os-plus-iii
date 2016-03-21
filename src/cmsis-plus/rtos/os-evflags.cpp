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
     * The os::rtos::evflags namespace groups event flags attributes
     * and initialisers.
     */
    namespace evflags
    {
      /**
       * @class Attributes
       * @details
       * Allow to assign a name to the event flags.
       *
       * @par POSIX compatibility
       *  No POSIX similar functionality identified, but inspired by POSIX
       *  attributes used in [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  (IEEE Std 1003.1, 2013 Edition).
       */

      const Attributes initializer
        { nullptr };
    } /* namespace evflags */

    // ------------------------------------------------------------------------

    /**
     * @class Event_flags
     * @details
     * Synchronised set of flags that can be used to notify events
     * between threads or between ISRs and threads.
     *
     * @par Example
     *
     * @code{.cpp}
     * Event_flags ev;
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
     * This constructor shall initialise the event flags object
     * with default settings.
     * The effect shall be equivalent to creating an event flags object
     * referring to the attributes in `evflags::initializer`.
     * Upon successful initialisation, the state of the event
     * flags object shall become initialised, with all flags cleared.
     *
     * Only the event flags object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * event flags objects.
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
     * This constructor shall initialise the event flags object
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
     * appropriate, the variable `evflags::initializer` can be used to
     * initialise event flags.
     * The effect shall be equivalent to creating an event flags object with
     * the default constructor.
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
    Event_flags::~Event_flags ()
    {
      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      port::Event_flags::destroy (this);

#else

      assert(list_.empty());

#endif
    }

    bool
    Event_flags::_try_wait (flags::mask_t mask, flags::mask_t* oflags,
                            flags::mode_t mode)
    {
      interrupts::Critical_section cs; // ----- Critical section -----

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
              return true;
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
              return true;
            }
        }

      return false;
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
    Event_flags::wait (flags::mask_t mask, flags::mask_t* oflags,
                       flags::mode_t mode)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::wait (this, mask, oflags, mode);

#else

      Thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      DoubleListNodeThread node
        { crt_thread };

      for (;;)
        {
          if (_try_wait (mask, oflags, mode))
            {
              return result::ok;
            }

            {
              // Add this thread to the event flags waiting list.
              // It is removed when this block ends (after suspend()).
              Waiting_threads_list_guard<interrupts::Critical_section> lg
                { list_, node };

              this_thread::suspend ();
            }

          if (crt_thread.interrupted ())
            {
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
    Event_flags::try_wait (flags::mask_t mask, flags::mask_t* oflags,
                           flags::mode_t mode)
    {

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::try_wait (this, mask, oflags, mode);

#else

      if (_try_wait (mask, oflags, mode))
        {
          return result::ok;
        }
      else
        {
          return EAGAIN;
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
     * if the event flags are already raised.
     *
     * If the flags::mode::clear bit is set, the event flags that are
     * returned are automatically cleared.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Event_flags::timed_wait (flags::mask_t mask, systicks_t timeout,
                             flags::mask_t* oflags, flags::mode_t mode)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      if (timeout == 0)
        {
          timeout = 1;
        }

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::timed_wait (this, mask, timeout, oflags, mode);

#else

      Thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      DoubleListNodeThread node
        { crt_thread };

      Systick_clock::rep start = Systick_clock::now ();
      for (;;)
        {
          Systick_clock::sleep_rep slept_ticks;

          if (_try_wait (mask, oflags, mode))
            {
              return result::ok;
            }

          Systick_clock::rep now = Systick_clock::now ();
          slept_ticks = (Systick_clock::sleep_rep) (now - start);
          if (slept_ticks >= timeout)
            {
              return ETIMEDOUT;
            }

            {
              // Add this thread to the event flags waiting list.
              // It is removed when this block ends (after wait()).
              Waiting_threads_list_guard<interrupts::Critical_section> lg
                { list_, node };

              Systick_clock::wait (timeout - slept_ticks);
            }

          if (crt_thread.interrupted ())
            {
              return EINTR;
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
    Event_flags::raise (flags::mask_t mask, flags::mask_t* oflags)
    {
      os_assert_err(mask != 0, EINVAL);

#if defined(OS_INCLUDE_PORT_RTOS_EVENT_FLAGS)

      return port::Event_flags::raise (this, mask, oflags);

#else
        {
          interrupts::Critical_section cs; // ----- Critical section -----

          flags_ |= mask;

          if (oflags != nullptr)
            {
              *oflags = flags_;
            }
        }

        {
          interrupts::Critical_section cs; // ----- Critical section -----

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

      interrupts::Critical_section cs; // ----- Critical section -----

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

      interrupts::Critical_section cs; // ----- Critical section -----

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
