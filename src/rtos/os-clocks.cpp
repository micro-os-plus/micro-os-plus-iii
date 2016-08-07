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

using namespace os;
using namespace os::rtos;

// ----------------------------------------------------------------------------

/**
 * @details
 * Must be called from the physical interrupt handler.
 */
void
os_systick_handler (void)
{
  using namespace os::rtos;

#if defined(OS_USE_RTOS_PORT_SCHEDULER)
  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      port::clock_systick::internal_interrupt_service_routine();
    }
#endif

#if defined(OS_TRACE_RTOS_SYSCLOCK_TICK)
  trace::putchar ('.');
#endif

    {
      // ----- Enter critical section -----------------------------------------
      interrupts::critical_section ics;

      sysclock.internal_increment_count ();
      hrclock.internal_increment_count ();
      // ----- Exit critical section ------------------------------------------
    }
  sysclock.internal_check_timestamps ();
  hrclock.internal_check_timestamps ();

#if !defined(OS_INCLUDE_RTOS_REALTIME_CLOCK_DRIVER)

  // Simulate an RTC driver.
  static uint32_t ticks = clock_systick::frequency_hz;

  if (--ticks == 0)
    {
      ticks = clock_systick::frequency_hz;

      os_rtc_handler ();
    }

#endif /* !defined(OS_INCLUDE_RTOS_REALTIME_CLOCK_DRIVER) */

#if !defined(OS_USE_RTOS_PORT_SCHEDULER)

  port::scheduler::reschedule ();

#endif /* !defined(OS_USE_RTOS_PORT_SCHEDULER) */

#if defined(OS_TRACE_RTOS_SYSCLOCK_TICK)
  trace::putchar (',');
#endif
}

/**
 * @details
 * Must be called from the physical RTC interrupt handler.
 */
void
os_rtc_handler (void)
{

#if defined(OS_USE_RTOS_PORT_SCHEDULER)
  // Prevent scheduler actions before starting it.
  if (scheduler::started ())
    {
      port::clock_rtc::internal_interrupt_service_routine();
    }
#endif

#if defined(OS_TRACE_RTOS_RTC_TICK)
  trace_putchar ('!');
#endif

    {
      // ----- Enter critical section -----------------------------------------
      interrupts::critical_section ics;

      rtclock.internal_increment_count ();
      // ----- Exit critical section ------------------------------------------
    }

  rtclock.internal_check_timestamps ();
}

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ========================================================================

    /**
     * @class clock
     * @details
     * Used as base class for all clocks.
     */

    /**
     * @cond ignore
     */

    /**
     * @endcond
     */

    clock::~clock ()
    {
      ;
    }

    /**
     * @details
     * For system clocks it is called from the system startup code.
     */
    void
    clock::start (void)
    {
      ;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    clock::timestamp_t
    clock::now (void)
    {
      // ----- Enter critical section -----------------------------------------
      interrupts::critical_section ics;

      // Prevent inconsistent values using the critical section.
      return steady_count_;
      // ----- Exit critical section ------------------------------------------
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    clock::timestamp_t
    clock::steady_now (void)
    {
      // ----- Enter critical section -----------------------------------------
      interrupts::critical_section ics;

      // Prevent inconsistent values using the critical section.
      return steady_count_;
      // ----- Exit critical section ------------------------------------------
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    clock::sleep_for (duration_t duration)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("%s(%u) %p %s\n", __func__,
                     static_cast<unsigned int> (duration),
                     &this_thread::thread (), this_thread::thread ().name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      clock::timestamp_t timestamp = steady_now () + duration;
      for (;;)
        {
          result_t res;
          res = internal_wait_until_ (timestamp, steady_list_);

          timestamp_t n = steady_now ();
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

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    clock::sleep_until (timestamp_t timestamp)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("%s()\n", __func__);
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      for (;;)
        {
          result_t res;
          res = internal_wait_until_ (timestamp, steady_list_);

          timestamp_t nw = now ();
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

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    clock::wait_for (duration_t timeout)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("%s(%u)\n", __func__, static_cast<unsigned int> (timeout));
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      clock::timestamp_t timestamp = steady_now () + timeout;

      result_t res;
      res = internal_wait_until_ (timestamp, steady_list_);

      timestamp_t nw = steady_now ();
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

    /**
     * @cond ignore
     */

    clock::offset_t
    clock::offset (void)
    {
      return 0;
    }

    clock::offset_t
    clock::offset (offset_t offset __attribute__((unused)))
    {
      return 0;
    }

    result_t
    clock::internal_wait_until_ (timestamp_t timestamp,
                                 internal::clock_timestamps_list& list)
    {
      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::timeout_thread_node node
        { timestamp, crt_thread };

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          // Remove this thread from the ready list, if there.
          port::this_thread::prepare_suspend ();

          // Add this thread to the clock waiting list.
          list.link (node);
          crt_thread.clock_node_ = &node;
          crt_thread.state_ = thread::state::suspended;
          // ----- Exit critical section --------------------------------------
        }

      port::scheduler::reschedule ();

        {
          // ----- Enter critical section -------------------------------------
          interrupts::critical_section ics;

          // Remove the thread from the clock timeout list,
          // if not already removed by the timer.
          crt_thread.clock_node_ = nullptr;
          node.unlink ();
          // ----- Exit critical section --------------------------------------
        }

      return result::ok;
    }

    /**
     * @endcond
     */

    // ========================================================================
    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    adjustable_clock::~adjustable_clock ()
    {
      ;
    }

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    clock::timestamp_t
    adjustable_clock::now (void)
    {
      // Prevent inconsistent values.
      // ----- Enter critical section -----------------------------------------
      interrupts::critical_section ics;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
      return steady_count_ + offset_;
#pragma GCC diagnostic pop
      // ----- Exit critical section ------------------------------------------
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    adjustable_clock::sleep_until (timestamp_t timestamp)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("%s()\n", __func__);
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      for (;;)
        {
          result_t res;
          res = internal_wait_until_ (timestamp, adjusted_list_);

          timestamp_t nw = now ();
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

    /**
     * @details
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    clock::offset_t
    adjustable_clock::offset (void)
    {
      return offset_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    clock::offset_t
    adjustable_clock::offset (offset_t value)
    {
      // ----- Enter critical section -----------------------------------------
      interrupts::critical_section ics;

      offset_t tmp;
      tmp = offset_;
      offset_ = value;

      return tmp;
      // ----- Exit critical section ------------------------------------------
    }

    // ========================================================================

    /**
     * @class clock_systick
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
     *    clock::timestamp_t ticks = sysclock.now();
     *
     *    // Put the current thread to sleep for a given number of ticks.
     *    sysclock.sleep_for(7);
     *
     *    // Put the current thread to sleep for a given number of microseconds.
     *    // For a 1000 Hz clock, the actual value is 4 ticks.
     *    sysclock.sleep_for(clock_systick::ticks_cast((uint64_t)(3500)));
     *
     *    // Do something else.
     * }
     * @endcode
     */

    // ------------------------------------------------------------------------
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    /**
     * @details Kind of singleton instance of the clock_systick class.
     */
    clock_systick sysclock;
#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    clock_systick::clock_systick () :
        clock
          { "sysclock" }
    {
      ;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    clock_systick::~clock_systick ()
    {
      ;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    void
    clock_systick::start (void)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("clock_systick::%s()\n", __func__);
#endif
      port::clock_systick::start ();
    }

    // ------------------------------------------------------------------------

#if defined(OS_USE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)

    result_t
    clock_systick::internal_wait_until_ (timestamp_t timestamp,
        clock_timestamps_list& list __attribute__((unused)))
      {
        result_t res;

        timestamp_t nw = now ();
        if (nw >= timestamp)
          {
            return result::ok;
          }
        duration_t ticks = ((duration_t) (timestamp - nw));
        res = port::clock_systick::wait_for (ticks);
        return res;
      }

#endif

    // ========================================================================

    /**
     * @class clock_rtc
     * @details
     * This clock counts seconds since epoch or startup.
     *
     * The real time clock should be derived from a battery powered
     * second counting RTC, initialised at startup with the number
     * of seconds since the standard POSIX epoch (January 1st, 1970).
     *
     * As any usual clock, it might occasionally be adjusted to match
     * a reference clock, so it cannot be a steady clock.
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
     *    clock::timestamp_t seconds = rtclock.now();
     *
     *    // Put the current thread to sleep for a given number of seconds.
     *    rtclock.sleep_for(7);
     *
     *    // Do something else.
     * }
     * @endcode
     */

    // ------------------------------------------------------------------------
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    /**
     * @brief Kind of singleton instance of the clock_rtc class.
     */
    clock_rtc rtclock;
#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    clock_rtc::clock_rtc () :
        adjustable_clock
          { "rtclock" }
    {
      ;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    clock_rtc::~clock_rtc ()
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
    void
    clock_rtc::start (void)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("clock_rtc::%s()\n", __func__);
#endif
      assert(!interrupts::in_handler_mode ());

      // TODO: Use the RTC driver to initialise the seconds to epoch.
    }

    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wglobal-constructors"
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
    /**
     * @brief Kind of singleton instance of the clock_highres class.
     */
    clock_highres hrclock;
#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    clock_highres::clock_highres () :
        clock
          { "hrclock" }
    {
      ;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    clock_highres::~clock_highres ()
    {
      ;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    void
    clock_highres::start (void)
    {
#if defined(OS_TRACE_RTOS_CLOCKS)
      trace::printf ("clock_highres::%s()\n", __func__);
#endif

      port::clock_highres::start ();
    }

    clock::timestamp_t
    clock_highres::now (void)
    {
      // ----- Enter critical section -----------------------------------------
      interrupts::critical_section ics;

      return steady_count_ + port::clock_highres::cycles_since_tick ();
      // ----- Exit critical section ------------------------------------------
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
