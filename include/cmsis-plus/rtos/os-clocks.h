/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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
    class Clock
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      Clock ();

      /**
       * @cond ignore
       */
      Clock (const Clock&) = delete;
      Clock (Clock&&) = delete;
      Clock&
      operator= (const Clock&) = delete;
      Clock&
      operator= (Clock&&) = delete;
      /**
       * @endcond
       */

      virtual
      ~Clock ();

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
      clock::timestamp_t
      now (void);

      clock::timestamp_t
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
      sleep_for (clock::duration_t duration);

      /**
       * @brief Sleep until a timestamp.
       * @param [in] timestamp The absolute moment in time, in clock units.
       * @retval ETIMEDOUT The sleep lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      result_t
      sleep_until (clock::timestamp_t timestamp);

      /**
       * @brief Timed wait for an event.
       * @param [in] timeout The timeout in clock units.
       * @retval result::ok An event occurred before the timeout.
       * @retval ETIMEDOUT The wait lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      result_t
      wait_for (clock::duration_t timeout);

      void
      interrupt_service_routine (void);

#if !defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)
      Clock_timestamps_list&
      steady_list (void);
#endif

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
      _wait_until (clock::timestamp_t timestamp, Clock_timestamps_list& list);

      /**
       * @name Private Member Variables
       * @{
       */

#if !defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR___)
      Clock_timestamps_list steady_list_;
      clock::duration_t volatile sleep_count_;
      Clock_timestamps_list adjusted_list_;
#endif

      /**
       * @brief Monotone ascending count.
       */
      clock::timestamp_t volatile steady_count_;

      /**
       * @brief Adjustable offset to epoch.
       */
      clock::offset_t volatile offset_;

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
    class Systick_clock : public Clock
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
          clock::timestamp_t ticks;

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

      Systick_clock ();

      /**
       * @cond ignore
       */
      Systick_clock (const Systick_clock&) = delete;
      Systick_clock (Systick_clock&&) = delete;
      Systick_clock&
      operator= (const Systick_clock&) = delete;
      Systick_clock&
      operator= (Systick_clock&&) = delete;
      /**
       * @endcond
       */

      virtual
      ~Systick_clock ();

      /**
       * @}
       * @name Public Member Functions
       * @{
       */

      // Enable name lookup in base class (tricky!).
      using Clock::now;

      /**
       * @brief Tell the detailed current time.
       * @param [out] details Pointer to structure to store the clock details.
       * @return The number of SysTick ticks since startup.
       */
      clock::timestamp_t
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
      interrupt_service_routine (void);

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
      _wait_until (clock::timestamp_t timestamp, Clock_timestamps_list& list);

#endif

      /**
       * @}
       */

    };

    extern Systick_clock systick_clock;

    // ========================================================================

    /**
     * @brief Real time clock.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Realtime_clock : public Clock
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

      Realtime_clock ();

      /**
       * @cond ignore
       */
      Realtime_clock (const Realtime_clock&) = delete;
      Realtime_clock (Realtime_clock&&) = delete;
      Realtime_clock&
      operator= (const Realtime_clock&) = delete;
      Realtime_clock&
      operator= (Realtime_clock&&) = delete;
      /**
       * @endcond
       */

      virtual
      ~Realtime_clock ();

      /**
       * @}
       * @name Public Member Functions
       * @{
       */

      // Enable name lookup in base class (tricky!).
      using Clock::now;

      /**
       * @brief Initialise RTC.
       * @par Parameters
       *  None
       * @retval result::ok   The real time clock was initialised.
       * @retval ENOTRECOVERABLE Could not initialise real time clock.
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
      _wait_until (clock::timestamp_t timestamp, Clock_timestamps_list& list);

#endif

      /**
       * @}
       */
    };

    extern Realtime_clock realtime_clock;

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {

    // ========================================================================

#if !defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)

    inline Clock_timestamps_list&
    Clock::steady_list (void)
      {
        return steady_list_;
      }

#endif

    // ========================================================================

    /**
     * @details
     * Round up the microseconds value and convert to a number of
     * ticks, using the SysTick frequency in Hz.
     */
    template<typename Rep_T>
      constexpr clock::systicks_t
      Systick_clock::ticks_cast (Rep_T microsec)
      {
        // TODO: add some restrictions to match only numeric types
        return (clock::systicks_t) ((((microsec) * ((Rep_T) frequency_hz))
            + (Rep_T) 999999ul) / (Rep_T) 1000000ul);
      }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_CLOCKS_H_ */
