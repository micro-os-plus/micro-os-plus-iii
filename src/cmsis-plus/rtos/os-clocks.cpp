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
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    Systick_clock::rep
    Systick_clock::now (void)
    {
      Critical_section_irq cs; // ---- Critical section -----

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
      assert(details != nullptr);

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
          Critical_section_irq cs; // ----- Critical section -----

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

    /**
     * @details
     * Put the current thread to sleep, until the next n-th
     * SysTick occurs. Depending when the call is issued, the
     * first tick counted may be very short.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Systick_clock::sleep_for (Systick_clock::sleep_rep ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("Systick_clock::sleep_for(%d_ticks)\n", ticks);

#if defined(OS_INCLUDE_PORT_RTOS_SYSTICK_CLOCK_SLEEP_FOR)
      return port::Systick_clock::sleep_for (ticks);
#else
      // TODO
      return result::ok;
#endif
    }

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
