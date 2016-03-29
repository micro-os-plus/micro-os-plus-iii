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

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

void
os_systick_handler (void)
{
  using namespace os::rtos;

  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      os_port_systick_handler ();
    }
  os::rtos::systick_clock.interrupt_service_routine ();
}

void
os_rtc_handler (void)
{
  using namespace os::rtos;

  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      os_port_rtc_handler ();
    }
  os::rtos::realtime_clock.interrupt_service_routine ();
}

void __attribute__((weak))
os_port_systick_handler (void)
{
  // TODO
}

void __attribute__((weak))
os_port_rtc_handler (void)
{
  // TODO
}

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ========================================================================

#pragma GCC diagnostic push
// TODO: remove it when fully implemented
#pragma GCC diagnostic ignored "-Wunused-parameter"

    Clock::Clock ()
    {
      steady_count_ = 0;
      offset_ = 0;
    }

    Clock::~Clock ()
    {
      ;
    }

    void
    Clock::start (void)
    {
      ;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    clock::timestamp_t
    Clock::now (void)
    {
      interrupts::Critical_section cs; // ----- Critical section -----

      // Prevent inconsistent values using the critical section.
      return steady_count_ + offset_;
    }

    clock::timestamp_t
    Clock::steady_now (void)
    {
      interrupts::Critical_section cs; // ----- Critical section -----

      // Prevent inconsistent values using the critical section.
      return steady_count_;
    }

    result_t
    Clock::sleep_for (clock::duration_t duration)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks)\n", __func__, duration);

      clock::timestamp_t prev = systick_clock.now ();
      clock::duration_t ticks_to_go = duration;
      for (;;)
        {
          result_t res;

          res = _wait (ticks_to_go);

          clock::timestamp_t now = systick_clock.now ();
          clock::duration_t slept_ticks = (clock::duration_t) (now - prev);
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

    result_t
    Clock::sleep_until (clock::timestamp_t timestamp)
    {
      // TODO
      return result::ok;
    }

    result_t
    Clock::wait_for (clock::duration_t duration)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks)\n", __func__, duration);

      result_t res;

      res = _wait (duration);

      if (this_thread::thread ().interrupted ())
        {
          return EINTR;
        }

      return res;
    }

    void
    Clock::interrupt_service_routine (void)
    {
      trace::putchar('.');

      ++steady_count_;

#if !defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)
      sleep_for_list_.check_timestamp (steady_count_);
      sleep_until_list_.check_timestamp (steady_count_ + offset_);
#endif

#if 0
      if (sleep_count_ > 1)
        {
          --sleep_count_;
        }
      else if (sleep_count_ == 1)
        {
          sleep_count_ = 0;

          sleep_for_list_.wakeup_one ();
        }

      if (!sleep_for_list_.empty ())
        {

          for (;;)
            {
              clock::timestamp_t head_ts = sleep_for_list_.head ()->timestamp;
              if (head_ts > steady_count_)
                {
                  sleep_count_ = (clock::duration_t) (head_ts - steady_count_);
                  break;
                }
              sleep_for_list_.wakeup_one ();
            }
        }
#endif

    }

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
     *    clock::timestamp_t ticks = systick_clock.now();
     *
     *    // Put the current thread to sleep for a given number of ticks.
     *    systick_clock.sleep_for(7);
     *
     *    // Put the current thread to sleep for a given number of microseconds.
     *    // For a 1000 Hz clock, the actual value is 4 ticks.
     *    systick_clock.sleep_for(Systick_clock::ticks_cast(3500));
     *
     *    // Do something else.
     * }
     * @endcode
     */

    // ----------------------------------------------------------------------
    Systick_clock systick_clock;

    // ----------------------------------------------------------------------

    Systick_clock::Systick_clock ()
    {
      ;
    }

    Systick_clock::~Systick_clock ()
    {
      ;
    }

    // ----------------------------------------------------------------------

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
    clock::timestamp_t
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
          interrupts::Critical_section cs; // ----- Critical section -----

          // Sample ticks counter inside critical section.
          ticks = steady_count_;

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
    Systick_clock::_wait (clock::duration_t ticks)
    {
      result_t res;

#if defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)
      if (ticks == 0)
        {
          ticks = 1;
        }

      res = port::Systick_clock::wait (ticks);
#else
      Thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      Double_list_node_clock node
        { sleep_for_list_, steady_count_ + ticks, crt_thread };

        {
          // Add this thread to the clock waiting list.
          // It is removed when this block ends (after sleep()).
          Clock_timestamps_list_guard<interrupts::Critical_section> lg
            { node };

          this_thread::sleep ();
        }

      if (crt_thread.interrupted ())
        {
          return EINTR;
        }

      res = ETIMEDOUT;
#endif
      return res;
    }

#if 0
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
    Systick_clock::sleep_for (clock::duration_t ticks)
      {
        os_assert_err(!scheduler::in_handler_mode (), EPERM);

        trace::printf ("%s(%d_ticks)\n", __func__, ticks);

        clock::timestamp_t prev = systick_clock.now ();
        clock::duration_t ticks_to_go = ticks;
        for (;;)
          {
            result_t res;

            res = _wait (ticks_to_go);

            clock::timestamp_t now = systick_clock.now ();
            clock::duration_t slept_ticks = (clock::duration_t) (now
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
    Systick_clock::wait (clock::duration_t ticks)
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
#endif

    void
    Systick_clock::interrupt_service_routine (void)
    {
      Clock::interrupt_service_routine ();

#if !defined(OS_INCLUDE_RTOS_REALTIME_CLOCK_DRIVER)

      // TODO: simulate an RTC driver.
      static uint32_t ticks = Systick_clock::frequency_hz;

      if (--ticks == 0)
        {
          ticks = Systick_clock::frequency_hz;

          os_rtc_handler ();
        }

#endif

    }

    // ======================================================================

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
     *    clock::timestamp_t seconds = realtime_clock.now();
     *
     *    // Put the current thread to sleep for a given number of seconds.
     *    realtime_clock.sleep_for(7);
     *
     *    // Do something else.
     * }
     * @endcode
     */

    // ----------------------------------------------------------------------
    Realtime_clock realtime_clock;

    // ----------------------------------------------------------------------

    Realtime_clock::Realtime_clock ()
    {
      ;
    }

    Realtime_clock::~Realtime_clock ()
    {
      ;
    }

    // ----------------------------------------------------------------------

    /**
     * @details
     * Must be called only once, during inits.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Realtime_clock::start (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      // TODO: Use the RTC driver to initialise the seconds to epoch.

      return result::ok;
    }

#if 0
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
#endif

    result_t
    Realtime_clock::_wait (clock::duration_t secs)
    {
      return ETIMEDOUT;
    }

  // ----------------------------------------------------------------------

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
