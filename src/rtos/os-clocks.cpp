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

using namespace os;
using namespace os::rtos;

// ----------------------------------------------------------------------------

void
os_systick_handler (void)
{
  using namespace os::rtos;

#if defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      port::Systick_clock::_interrupt_service_routine();
    }
#endif

#if defined(OS_TRACE_RTOS_SYSTICK_TICK)
  trace::putchar ('.');
#endif
  systick_clock._interrupt_service_routine ();
#if defined(OS_TRACE_RTOS_SYSTICK_TICK)
  trace::putchar (',');
#endif
}

void
os_rtc_handler (void)
{

#if defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      port::Realtime_clock::_interrupt_service_routine();
    }
#endif

#if defined(OS_TRACE_RTOS_RTC_TICK)
  trace_putchar ('!');
#endif
  realtime_clock._interrupt_service_routine ();
}

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ========================================================================

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
      // Prevent inconsistent values.
      interrupts::Critical_section ics; // ----- Critical section -----

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
      return steady_count_ + offset_;
#pragma GCC diagnostic pop
    }

    clock::timestamp_t
    Clock::steady_now (void)
    {
      interrupts::Critical_section ics; // ----- Critical section -----

      // Prevent inconsistent values using the critical section.
      return steady_count_;
    }

    result_t
    Clock::sleep_for (clock::duration_t duration)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("%s(%d)\n", __func__, duration);
#endif

      clock::timestamp_t timestamp = steady_now () + duration;
      for (;;)
        {
          result_t res;
          res = _wait_until (timestamp, steady_list_);

          clock::timestamp_t n = steady_now ();
          if (n >= timestamp)
            {
              return ETIMEDOUT;
            }

          if (this_thread::thread ().interrupted ())
            {
              return EINTR;
            }

          if (res != result::ok)
            {
              return res;
            }
        }
      return ENOTRECOVERABLE;
    }

    result_t
    Clock::sleep_until (clock::timestamp_t timestamp)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("%s()\n", __func__);
#endif

      for (;;)
        {
          result_t res;
          res = _wait_until (timestamp, adjusted_list_);

          clock::timestamp_t nw = now ();
          if (nw >= timestamp)
            {
              return ETIMEDOUT;
            }

          if (this_thread::thread ().interrupted ())
            {
              return EINTR;
            }

          if (res != result::ok)
            {
              return res;
            }
        }
      return ENOTRECOVERABLE;
    }

    result_t
    Clock::wait_for (clock::duration_t timeout)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("%s(%d_ticks)\n", __func__, timeout);
#endif

      clock::timestamp_t timestamp = steady_now () + timeout;

      result_t res;
      res = _wait_until (timestamp, steady_list_);

      clock::timestamp_t nw = steady_now ();
      if (nw >= timestamp)
        {
          return ETIMEDOUT;
        }

      if (this_thread::thread ().interrupted ())
        {
          return EINTR;
        }

      return res;
    }

    void
    Clock::_interrupt_service_routine (void)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      // trace::putchar ('.');
#endif

        {
          interrupts::Critical_section ics; // ----- Critical section -----
          ++steady_count_;
        }

      steady_list_.check_timestamp (steady_count_);

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
      adjusted_list_.check_timestamp (steady_count_ + offset_);
#pragma GCC diagnostic pop
    }

    result_t
    Clock::_wait_until (clock::timestamp_t timestamp,
                        Clock_timestamps_list& list)
    {
      Thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      Timeout_thread_node node
        { timestamp, crt_thread };

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          // Remove this thread from the ready list, if there.
          port::this_thread::prepare_suspend ();

          // Add this thread to the clock waiting list.
          list.link (node);
          crt_thread.clock_node_ = &node;
          crt_thread.sched_state_ = thread::state::waiting;
        }

      port::scheduler::reschedule ();

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          // Remove the thread from the clock timeout list,
          // if not already removed by the timer.
          crt_thread.clock_node_ = nullptr;
          node.unlink ();
        }

      return result::ok;
    }

    // ========================================================================

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

    // ------------------------------------------------------------------------
    /**
     * @brief Instance of the SysTick clock.
     */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    Systick_clock systick_clock;
#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    Systick_clock::Systick_clock ()
    {
      ;
    }

    Systick_clock::~Systick_clock ()
    {
      ;
    }

    // ------------------------------------------------------------------------

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
      assert (details != nullptr);

#if defined(__ARM_EABI__)
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
          interrupts::Critical_section ics; // ----- Critical section -----

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
#elif defined(__APPLE__) || defined(__linux__)

      details->core_frequency_hz = 1000000000; // Very fast!
      details->divisor = 1;
      details->cycles = 0;
      details->ticks = steady_count_;

      return steady_count_;

#else
#error
#endif
    }

#if defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)

    result_t
    Systick_clock::_wait_until (clock::timestamp_t timestamp,
        Clock_timestamps_list& list __attribute__((unused)))
      {
        result_t res;

        clock::timestamp_t nw = now ();
        if (nw >= timestamp)
          {
            return result::ok;
          }
        clock::duration_t ticks = ((clock::duration_t) (timestamp - nw));
        res = port::Systick_clock::wait_for (ticks);
        return res;
      }

#endif

    void
    Systick_clock::_interrupt_service_routine (void)
    {
      Clock::_interrupt_service_routine ();

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

    // ========================================================================

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

    // ------------------------------------------------------------------------
    /**
     * @brief Instance of the Real Time clock.
     */
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    Realtime_clock realtime_clock;
#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    Realtime_clock::Realtime_clock ()
    {
      ;
    }

    Realtime_clock::~Realtime_clock ()
    {
      ;
    }

    // ------------------------------------------------------------------------

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

  // --------------------------------------------------------------------------

  }
/* namespace rtos */
} /* namespace os */
