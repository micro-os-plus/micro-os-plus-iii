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
    static Systick_clock::rep __systick_now;
    static Realtime_clock::rep __rtc_now;

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

void
os_systick_handler (void)
{
  using namespace os::rtos;

  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      os_impl_systick_handler ();
    }
  __systick_now++;

#if !defined(OS_INCLUDE_REALTIME_CLOCK_DRIVER)
  static uint32_t ticks = Systick_clock::frequency_hz;

  if (--ticks == 0)
    {
      ticks = Systick_clock::frequency_hz;

      os_rtc_handler ();
    }
#endif
}

void
os_rtc_handler (void)
{
  using namespace os::rtos;

  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      os_impl_rtc_handler ();
    }
  ++__rtc_now;
}

void __attribute__((weak))
os_impl_systick_handler (void)
{
  // TODO
}

void __attribute__((weak))
os_impl_rtc_handler (void)
{
  // TODO
}

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

#pragma GCC diagnostic push
// TODO: remove it when fully implemented
//#pragma GCC diagnostic ignored "-Wunused-parameter"

// ======================================================================

    /**
     * @class Systick_clock
     * @details
     * This clock counts SysTick interrupts since startup.
     *
     * The SysTick clock should be a steady clock, i.e. the total
     * count of ticks should be monotone ascending (in other words no
     * adjustments to the past should be performed).
     *
     * For Cortex-M implementations using the standard SysTick, this
     * clock is able to provide accuracy at CPU cycle level, by
     * sampling the SysTick internal counter. For a CPU clock of 100 MHz,
     * this gives a 10 ns resolution, quite high for accurate timing.
     *
     * @par Example
     *
     * @code{.cpp}
     * void
     * func(void)
     * {
     *    // Do something
     *
     *    // Get the current ticks counter.
     *    Systick_clock::rep ticks = Systick_clock::now();
     *
     *    // Put the current thread to sleep for a given number of ticks.
     *    Systick_clock::sleep_for(7);
     *
     *    // Put the current thread to sleep for a given number of microseconds.
     *    // For a 1000 Hz clock, the actual value is 4 ticks.
     *    Systick_clock::sleep_for(Systick_clock::ticks_cast(3500));
     *
     *    // Do something else.
     * }
     * @endcode
     */

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    Systick_clock::rep
    Systick_clock::now (void)
    {
      interrupts::Critical_section cs; // ----- Critical section -----

      // Prevent inconsistent values using the critical section.
      return __systick_now;
    }

    /**
     * @details
     * Return the very accurate current time, using the internal
     * SysTick cycle counter, which, at 100 MHz and 1000 ticks/sec
     * allows a 10 ns resolution.
     *
     * The function adjust for border conditions, when the SysTick
     * counter is reloaded during the call.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    Systick_clock::rep
    Systick_clock::now (current_t* details)
    {
      assert (details != nullptr);

      // The core frequency can be returned right away, since
      // is not expected to change during this call.
      details->core_frequency_hz = SystemCoreClock;

      // The timer reload value, it is the divisor - 1.
      uint32_t load = SysTick->LOAD;
      // Can also be returned right away.
      details->divisor = load + 1;

      // The ticks and cycles must be read atomically, use a critical
      // section and adjust for border condition, when the event
      // came exactly after entering the critical section.
      uint64_t ticks;
      uint32_t val;
        {
          interrupts::Critical_section cs; // ----- Critical section -----

          // Sample ticks counter inside critical section.
          ticks = __systick_now;

          // Initial sample of the decrementing counter.
          // Might happen before the event, will be used as such.
          val = SysTick->VAL;

          // Check overflow. If the exception is pending, it means the
          // ticks counter was not yet updated, but the counter was reloaded.
          if (SysTick->CTRL & SCB_ICSR_PENDSTSET_Msk)
            {
              // Sample the decrementing counter again to validate the
              // initial sample.
              uint32_t val_update = SysTick->VAL;
              if (val_update > val)
                {
                  // The second sample is more accurate.
                  val = val_update;
                }
              ticks++; // Adjust to next tick.
            }
        }
      details->cycles = load - val;
      details->ticks = ticks;

      return ticks;
    }

    result_t
    Systick_clock::_wait (Systick_clock::sleep_rep ticks)
    {
      if (ticks == 0)
        {
          ticks = 1;
        }

      result_t res;

#if defined(OS_INCLUDE_PORT_RTOS_SYSTICK_CLOCK_SLEEP_FOR)
      res = port::Systick_clock::wait (ticks);
#else
      // TODO
      res = result::ok;
#endif
      return res;
    }

    /**
     * @details
     * Put the current thread to sleep until the given number of
     * SysTick cycles are counted. The first tick counted does
     * not represent a full SysTick clock period, but only the remaining
     * part in the current period, which might be almost nothing.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Systick_clock::sleep_for (Systick_clock::sleep_rep ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks)\n", __func__, ticks);

      Systick_clock::rep prev = Systick_clock::now ();
      Systick_clock::sleep_rep ticks_to_go = ticks;
      for (;;)
        {
          result_t res;

          res = _wait (ticks_to_go);

          Systick_clock::rep now = Systick_clock::now ();
          Systick_clock::sleep_rep slept_ticks = (Systick_clock::sleep_rep) (now
              - prev);
          if (slept_ticks >= ticks_to_go)
            {
              return ETIMEDOUT;
            }

          if (this_thread::thread ().interrupted ())
            {
              return EINTR;
            }

          if (res != ETIMEDOUT)
            {
              return res;
            }

          prev = now;
          ticks_to_go -= slept_ticks;
        }
      return ENOTRECOVERABLE;
    }

    /**
     * @details
     * Put the current thread to sleep until a resume(). If the
     * resume() does not occur until the next n-th
     * SysTick, the wait() returns with a timeout.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Systick_clock::wait (Systick_clock::sleep_rep ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks)\n", __func__, ticks);

      result_t res;

      res = _wait (ticks);

      if (this_thread::thread ().interrupted ())
        {
          return EINTR;
        }

      return res;
    }

    /**
     * @class Realtime_clock
     * @details
     * This clock counts seconds since epoch or startup.
     *
     * The real time clock should be derived from a battery powered
     * second counting RTC, initialised at startup with the number
     * of seconds since the standard POSIX epoch (January 1st, 1970).
     *
     * As any usual clock, it might occasionally be adjusted to match
     * a reference clock, so i cannot be a steady clock.
     *
     * For systems that do not have a hardware RTC, it can be derived from
     * SysTick, but in this case it must be externally initialised with
     * the epoch.
     *
     * @par Example
     *
     * @code{.cpp}
     * void
     * func(void)
     * {
     *    // Do something
     *
     *    // Get the current seconds counter.
     *    Realtime_clock::rep seconds = Realtime_clock::now();
     *
     *    // Put the current thread to sleep for a given number of seconds.
     *    Realtime_clock::sleep_for(7);
     *
     *    // Do something else.
     * }
     * @endcode
     */

    /**
     * @details
     * Must be called only once, during inits.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Realtime_clock::initialize (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      // TODO: Use the RTC driver to initialise the seconds to epoch.

      return result::ok;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    Realtime_clock::rep
    Realtime_clock::now (void)
    {
      return __rtc_now;
    }

    /**
     * @details
     * Put the current thread to sleep, until the next n-th
     * RTC second occurs. Depending when the call is issued, the
     * first second counted may be very short.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Realtime_clock::sleep_for (Realtime_clock::sleep_rep secs)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("Realtime_clock::sleep_for(%ds)\n", secs);

      // TODO
      __rtc_now += secs;
      return result::ok;
    }

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
