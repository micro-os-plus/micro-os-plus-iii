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

/*
 * The initial CMSIS++ RTOS API was inspired by CMSIS RTOS API v1.x,
 * Copyright (c) 2013 ARM LIMITED.
 */

#ifndef CMSIS_PLUS_RTOS_OS_CLOCKS_H_
#define CMSIS_PLUS_RTOS_OS_CLOCKS_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Generic clock.
     */
    class clock
    {
    public:

      /**
       * @brief Type of variables holding timer durations.
       * @details
       * A numeric type intended to hold a generic duration, either in ticks
       * or in seconds.
       */
      using duration_t = port::clock::duration_t;

      /**
       * @brief Type of variables holding time stamps.
       * @details
       * A numeric type intended to hold a generic timestamp, either in ticks
       * or in seconds.
       */
      using timestamp_t = port::clock::timestamp_t;

      using offset_t = port::clock::offset_t;

      /**
       * @name Constructors & Destructor
       * @{
       */

      clock ();

      /**
       * @cond ignore
       */
      clock (const clock&) = delete;
      clock (clock&&) = delete;
      clock&
      operator= (const clock&) = delete;
      clock&
      operator= (clock&&) = delete;
      /**
       * @endcond
       */

      virtual
      ~clock ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      void
      start (void);

      /**
       * @brief Tell the current time.
       * @par Parameters
       *  None
       * @return The clock specific timestamp.
       */
      timestamp_t
      now (void);

      timestamp_t
      steady_now (void);

      /**
       * @brief Sleep for a duration.
       * @param [in] duration The number of clock units
       *  (ticks or seconds) to sleep.
       * @retval ETIMEDOUT The sleep lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      result_t
      sleep_for (duration_t duration);

      /**
       * @brief Sleep until an absolute timestamp.
       * @param [in] timestamp The absolute moment in time, in clock units.
       * @retval ETIMEDOUT The sleep lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      result_t
      sleep_until (timestamp_t timestamp);

      /**
       * @brief Timed wait for an event.
       * @param [in] timeout The timeout in clock units.
       * @retval result::ok An event occurred before the timeout.
       * @retval ETIMEDOUT The wait lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      result_t
      wait_for (duration_t timeout);

      void
      _interrupt_service_routine (void);

      clock_timestamps_list&
      steady_list (void);

      /**
       * @}
       */

    protected:

      /**
       * @brief Queue timestamp and wait for it.
       * @param timestamp The absolute moment in time, in clock units.
       * @param list Reference to the clock list.
       * @retval result::ok The wait was performed.
       * @retval ENOTRECOVERABLE The wait failed.
       */
      virtual result_t
      _wait_until (timestamp_t timestamp, clock_timestamps_list& list);

      /**
       * @name Private Member Variables
       * @{
       */

      clock_timestamps_list steady_list_;
      duration_t volatile sleep_count_;
      clock_timestamps_list adjusted_list_;

      /**
       * @brief Monotone ascending count.
       */
      timestamp_t volatile steady_count_;

      /**
       * @brief Adjustable offset to epoch.
       */
      offset_t volatile offset_;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    /**
     * @brief SysTick derived clock.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class clock_systick : public clock
    {
    public:

      /**
       * @name Types & Constants
       * @{
       */

      /**
       * @brief SysTick frequency in Hz.
       */
      static constexpr uint32_t frequency_hz = OS_INTEGER_SYSTICK_FREQUENCY_HZ;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief SysTick detailed timestamp.
       *
       * @details
       * When an accurate timestamp is needed, the current SysTick
       * counter can be sampled to get the count of CPU cycles inside
       * the tick. For a 100 MHz clock, this gives a 10 ns resolution.
       *
       * To simplify further processing of this timestamp, the
       * structure also includes the CPU clock and the SysTick divider.
       */
      using current_t = struct
        {
          /**
           * @brief Count of SysTick ticks since core reset.
           */
          timestamp_t ticks;

          /**
           * @brief Count of SysTick cycles since timer reload (24 bits).
           */
          uint32_t cycles;

          /**
           * @brief SysTick reload value (24 bits).
           */
          uint32_t divisor;

          /**
           * @brief CPU clock frequency Hz.
           */
          uint32_t core_frequency_hz;
        };

#pragma GCC diagnostic pop

      /**
       * @name Constructors & Destructor
       * @{
       */

      clock_systick ();

      /**
       * @cond ignore
       */
      clock_systick (const clock_systick&) = delete;
      clock_systick (clock_systick&&) = delete;
      clock_systick&
      operator= (const clock_systick&) = delete;
      clock_systick&
      operator= (clock_systick&&) = delete;
      /**
       * @endcond
       */

      virtual
      ~clock_systick ();

      /**
       * @}
       * @name Public Member Functions
       * @{
       */

      // Enable name lookup in base class (tricky!).
      using clock::now;

      /**
       * @brief Tell the detailed current time.
       * @param [out] details Pointer to structure to store the clock details.
       * @return The number of SysTick ticks since startup.
       */
      timestamp_t
      now (current_t* details);

      /**
       * @brief Convert microseconds to ticks.
       * @tparam Rep_T Type of input, auto deduced (usually uint32_t or uin64_t)
       * @param [in] microsec The number of microseconds.
       * @return The number of ticks.
       */
      template<typename Rep_T>
        static constexpr uint32_t
        ticks_cast (Rep_T microsec);

      void
      _interrupt_service_routine (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

#if defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)

      /**
       * @brief Internal wait.
       * @param timestamp
       * @param list
       * @retval result::ok An event occurred before the timeout.
       * @retval ETIMEDOUT The wait lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      virtual result_t
      _wait_until (timestamp_t timestamp, clock_timestamps_list& list);

#endif

      /**
       * @}
       */

    };

    extern clock_systick sysclock;

    // ========================================================================

    /**
     * @brief Real time clock.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class clock_rtc : public clock
    {
    public:

      /**
       * @name Types & Constants
       * @{
       */

      /**
       * @brief Real time clock frequency in Hz.
       */
      static constexpr uint32_t frequency_hz = 1;

      /**
       * @name Constructors & Destructor
       * @{
       */

      clock_rtc ();

      /**
       * @cond ignore
       */
      clock_rtc (const clock_rtc&) = delete;
      clock_rtc (clock_rtc&&) = delete;
      clock_rtc&
      operator= (const clock_rtc&) = delete;
      clock_rtc&
      operator= (clock_rtc&&) = delete;
      /**
       * @endcond
       */

      virtual
      ~clock_rtc ();

      /**
       * @}
       * @name Public Member Functions
       * @{
       */

      // Enable name lookup in base class (tricky!).
      using clock::now;

      /**
       * @brief Initialise and make the RTC tick.
       * @par Parameters
       *  None
       * @retval result::ok   The real time clock was started.
       * @retval ENOTRECOVERABLE Could not start real time clock.
       */
      result_t
      start (void);

#if defined(OS_INCLUDE_RTOS_PORT_REALTIME_CLOCK_SLEEP_FOR)

      /**
       * @brief Internal wait.
       * @param timestamp
       * @param list
       * @retval result::ok An event occurred before the timeout.
       * @retval ETIMEDOUT The wait lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      virtual result_t
      _wait_until (timestamp_t timestamp, clock_timestamps_list& list);

#endif

      /**
       * @}
       */
    };

    extern clock_rtc rtclock;

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {

    // ========================================================================

    inline clock_timestamps_list&
    clock::steady_list (void)
    {
      return steady_list_;
    }

    // ========================================================================

    /**
     * @details
     * Round up the microseconds value and convert to a number of
     * ticks, using the SysTick frequency in Hz.
     */
    template<typename Rep_T>
      constexpr clock::duration_t
      clock_systick::ticks_cast (Rep_T microsec)
      {
        // TODO: add some restrictions to match only numeric types
        return static_cast<clock::duration_t> ((((microsec)
            * (static_cast<Rep_T> (frequency_hz)))
            + static_cast<Rep_T> (999999ul)) / static_cast<Rep_T> (1000000ul));
      }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_CLOCKS_H_ */
