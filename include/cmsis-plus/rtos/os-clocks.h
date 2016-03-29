/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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

    class Double_list_node_clock : public Double_list_node_timestamp
    {
    public:

      Double_list_node_clock (Double_list& lst, clock::timestamp_t ts,
                              Thread& th);

      /**
       * @cond ignore
       */
      Double_list_node_clock (const Double_list_node_clock&) = delete;
      Double_list_node_clock (Double_list_node_clock&&) = delete;
      Double_list_node_clock&
      operator= (const Double_list_node_clock&) = delete;
      Double_list_node_clock&
      operator= (Double_list_node_clock&&) = delete;
      /**
       * @endcond
       */

      ~Double_list_node_clock ();

      virtual void
      action (void) override;

    public:

      Thread& thread;
    };

#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

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
       * @param [in] duration The number of ticks or seconds to sleep.
       * @retval ETIMEDOUT The sleep lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      result_t
      sleep_for (clock::duration_t duration);

      result_t
      sleep_until (clock::timestamp_t timestamp);

      /**
       * @brief Timed wait for an event.
       * @param [in] duration The timeout in clock units.
       * @retval result::ok An event occurred before the timeout.
       * @retval ETIMEDOUT The wait lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      result_t
      wait_for (clock::duration_t duration);

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

      virtual result_t
      _wait (clock::duration_t duration) = 0;

      /**
       * @name Private Member Variables
       * @{
       */

#if !defined(OS_INCLUDE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)
      Clock_timestamps_list sleep_for_list_;
      clock::duration_t volatile sleep_count_;
      Clock_timestamps_list sleep_until_list_;
#endif

      clock::timestamp_t volatile steady_count_;
      clock::offset_t volatile offset_;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    /**
     * @brief SysTick derived **clock**.
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

      /**
       * @brief Internal wait.
       * @param ticks
       * @retval result::ok An event occurred before the timeout.
       * @retval ETIMEDOUT The wait lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      virtual result_t
      _wait (clock::duration_t ticks) override;

      /**
       * @}
       */

    };

    extern Systick_clock systick_clock;

    // ========================================================================

    /**
     * @brief Real time **clock**.
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

      /**
       * @brief Internal wait.
       * @param secs
       * @retval result::ok An event occurred before the timeout.
       * @retval ETIMEDOUT The wait lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      virtual result_t
      _wait (clock::duration_t secs) override;

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
      return sleep_for_list_;
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
