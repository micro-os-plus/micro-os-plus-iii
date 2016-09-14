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
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
     */
    class clock : public internal::object_named
    {
    public:

      // ----------------------------------------------------------------------

      /**
       * @name Types & Constants
       * @{
       */

      /**
       * @brief Type of variables holding clock durations.
       * @details
       * A numeric type intended to store a clock duration, either in ticks
       * or in seconds.
       * @ingroup cmsis-plus-rtos-clock
       */
      using duration_t = port::clock::duration_t;

      /**
       * @brief Type of variables holding clock time stamps.
       * @details
       * A numeric type intended to store a clock timestamp, either in ticks
       * or in seconds.
       * @ingroup cmsis-plus-rtos-clock
       */
      using timestamp_t = port::clock::timestamp_t;

      /**
       * @brief Type of variables holding clock offsets.
       * @details
       * A numeric type intended to store a clock offset
       * (difference to epoch), either in ticks
       * or in seconds.
       * @ingroup cmsis-plus-rtos-clock
       */
      using offset_t = port::clock::offset_t;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    protected:

      /**
       * @cond ignore
       */

      /**
       * @brief Construct a clock object instance (protected, used in derived classes)
       * @param [in] name Pointer to clock name.
       */
      clock (const char* name);

      /**
       * @endcond
       */

    public:

      /**
       * @cond ignore
       */

      // The rule of five.
      clock (const clock&) = delete;
      clock (clock&&) = delete;
      clock&
      operator= (const clock&) = delete;
      clock&
      operator= (clock&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the clock object instance.
       */
      virtual
      ~clock ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Start the clock.
       * @par Parameters
       *  None.
       * @par Returns
       *  Nothing.
       */
      virtual void
      start (void) = 0;

      /**
       * @brief Tell the current time, possibly adjusted for epoch.
       * @par Parameters
       *  None.
       * @return The clock current timestamp (time units from startup
       * plus the epoch offset).
       */
      virtual timestamp_t
      now (void);

      /**
       * @brief Tell the current time since startup.
       * @par Parameters
       *  None.
       * @return The clock current timestamp (time units from startup).
       */
      timestamp_t
      steady_now (void);

      /**
       * @brief Sleep for a relative duration.
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
      virtual result_t
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

      /**
       * @cond ignore
       */

      /**
       * @brief Get adjustment offset (placeholder).
       * @par Parameters
       *  None.
       * @return 0 for steady clocks.
       */
      virtual offset_t
      offset (void);

      /**
       * @brief Set adjustment offset (placeholder)
       * @param [in] value Ignored for steady clocks.
       * @return 0 for steady clocks.
       */
      virtual offset_t
      offset (offset_t value);

      internal::clock_timestamps_list&
      steady_list (void);

      void
      internal_increment_count (void);

      void
      internal_check_timestamps (void);

      /**
       * @endcond
       */

      /**
       * @}
       */

    protected:

      /**
       * @cond ignore
       */

      /**
       * @brief Queue timestamp and wait for it.
       * @param timestamp The absolute moment in time, in clock units.
       * @param list Reference to the clock list.
       * @retval result::ok The wait was performed.
       * @retval ENOTRECOVERABLE The wait failed.
       */
      virtual result_t
      internal_wait_until_ (timestamp_t timestamp,
                            internal::clock_timestamps_list& list);

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @cond ignore
       */

      internal::clock_timestamps_list steady_list_;
      duration_t volatile sleep_count_ = 0;

      /**
       * @brief Monotone ascending count.
       */
      timestamp_t volatile steady_count_ = 0;

      /**
       * @endcond
       */

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    /**
     * @brief Adjustable (non-steady) clock.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
     */
    class adjustable_clock : public clock
    {
      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    protected:

      /**
       * @cond ignore
       */

      /**
       * @brief Construct a clock object instance (protected, used in
       *  derived classes).
       * @param [in] name Pointer to clock name.
       */
      adjustable_clock (const char* name);

      /**
       * @endcond
       */

    public:

      /**
       * @cond ignore
       */

      // The rule of five.
      adjustable_clock (const adjustable_clock&) = delete;
      adjustable_clock (adjustable_clock&&) = delete;
      adjustable_clock&
      operator= (const adjustable_clock&) = delete;
      adjustable_clock&
      operator= (adjustable_clock&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the clock object instance.
       */
      virtual
      ~adjustable_clock ();

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Tell the current time adjusted for epoch.
       * @par Parameters
       *  None.
       * @return The clock current timestamp (time units from startup
       * plus the epoch offset).
       */
      virtual timestamp_t
      now (void) override;

      /**
       * @brief Sleep until an absolute timestamp.
       * @param [in] timestamp The absolute moment in time, in clock units.
       * @retval ETIMEDOUT The sleep lasted the entire duration.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINTR The sleep was interrupted.
       */
      virtual result_t
      sleep_until (timestamp_t timestamp) override;

      /**
       * @brief Get adjustment offset.
       * @par Parameters
       *  None.
       * @return Integer value representing the offset to epoch.
       */
      virtual offset_t
      offset (void) override;

      /**
       * @brief Set adjustment offset.
       * @param [in] value Integer representing the offset to epoch (positive).
       * @return Integer value representing the previous offset to epoch.
       */
      virtual offset_t
      offset (offset_t value) override;

      void
      internal_check_timestamps (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      /**
       * @cond ignore
       */

      /**
       * @brief Adjustable offset, usually to epoch.
       */
      offset_t volatile offset_ = 0;

      internal::clock_timestamps_list adjusted_list_;

      /**
       * @endcond
       */

      /**
       * @}
       */
    };

    // ========================================================================

    /**
     * @brief SysTick derived clock.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
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

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a SysTick clock object instance.
       */
      clock_systick ();

      /**
       * @cond ignore
       */

      // The rule of five.
      clock_systick (const clock_systick&) = delete;
      clock_systick (clock_systick&&) = delete;
      clock_systick&
      operator= (const clock_systick&) = delete;
      clock_systick&
      operator= (clock_systick&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the SysTick clock object instance.
       */
      virtual
      ~clock_systick ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

      virtual void
      start (void) override;

      /**
       * @brief Convert microseconds to ticks.
       * @tparam Rep_T Type of input, auto deduced (usually uint32_t or uin64_t)
       * @param [in] microsec The number of microseconds.
       * @return The number of ticks.
       */
      template<typename Rep_T>
        static constexpr clock::duration_t
        ticks_cast (Rep_T microsec);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @cond ignore
       */

#if defined(OS_USE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR)

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
      internal_wait_until_ (timestamp_t timestamp, clock_timestamps_list& list);

#endif

      /**
       * @endcond
       */

      /**
       * @}
       */

    };

    /**
     * @brief The system clock object instance.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
     */
    extern clock_systick sysclock;

    // ========================================================================

    /**
     * @brief Real time clock.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
     */
    class clock_rtc : public adjustable_clock
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
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a real time clock object instance.
       */
      clock_rtc ();

      /**
       * @cond ignore
       */

      // The rule of five.
      clock_rtc (const clock_rtc&) = delete;
      clock_rtc (clock_rtc&&) = delete;
      clock_rtc&
      operator= (const clock_rtc&) = delete;
      clock_rtc&
      operator= (clock_rtc&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the real time clock object instance.
       */
      virtual
      ~clock_rtc ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Initialise and make the RTC tick.
       * @par Parameters
       *  None.
       * @retval result::ok   The real time clock was started.
       * @retval ENOTRECOVERABLE Could not start real time clock.
       */
      virtual void
      start (void) override;

      /**
       * @}
       */

      /**
       * @cond ignore
       */

#if defined(OS_USE_RTOS_PORT_REALTIME_CLOCK_SLEEP_FOR)

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
      internal_wait_until_ (timestamp_t timestamp, clock_timestamps_list& list);

#endif

      /**
       * @endcond
       */

    };

    /**
     * @brief The real time clock object instance.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
     */
    extern clock_rtc rtclock;

    // ========================================================================

    /**
     * @brief High Resolution derived clock.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
     */
    class clock_highres : public clock
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a SysTick clock object instance.
       */
      clock_highres ();

      /**
       * @cond ignore
       */

      // The rule of five.
      clock_highres (const clock_highres&) = delete;
      clock_highres (clock_highres&&) = delete;
      clock_highres&
      operator= (const clock_highres&) = delete;
      clock_highres&
      operator= (clock_highres&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the SysTick clock object instance.
       */
      virtual
      ~clock_highres ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

      virtual void
      start (void) override;

      /**
       * @brief Tell the current time.
       * @par Parameters
       *  None.
       * @return The number of SysTick input clocks since startup.
       */
      virtual timestamp_t
      now (void) override;

      uint32_t
      input_clock_frequency_hz (void);

      void
      internal_increment_count (void);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @}
       */

    };

    /**
     * @brief The high resolution clock object instance.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-clock
     */
    extern clock_highres hrclock;

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {

    // ========================================================================

    /**
     * @cond ignore
     */

    inline
    clock::clock (const char* name) :
        internal::object_named
          { name }
    {
      ;
    }

    inline internal::clock_timestamps_list&
    __attribute__((always_inline))
    clock::steady_list (void)
    {
      return steady_list_;
    }

    inline void
    __attribute__((always_inline))
    clock::internal_increment_count (void)
    {
      // Increment the systick count by 1.
      ++steady_count_;
    }

    inline void
    __attribute__((always_inline))
    clock::internal_check_timestamps (void)
    {
      steady_list_.check_timestamp (steady_count_);
    }

    /**
     * @endcond
     */

    // ========================================================================
    /**
     * @cond ignore
     */

    inline
    adjustable_clock::adjustable_clock (const char* name) :
        clock
          { name }
    {
      ;
    }

    inline void
    __attribute__((always_inline))
    adjustable_clock::internal_check_timestamps (void)
    {
      clock::internal_check_timestamps ();

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsign-conversion"
      adjusted_list_.check_timestamp (steady_count_ + offset_);
#pragma GCC diagnostic pop
    }

    /**
     * @endcond
     */

    // ========================================================================
    /**
     * @cond ignore
     */

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
            + static_cast<Rep_T> (1000000ul) - 1)
            / static_cast<Rep_T> (1000000ul));
      }

    /**
     * @endcond
     */
    // ========================================================================
    inline void
    __attribute__((always_inline))
    clock_highres::internal_increment_count (void)
    {
      // Increment the highres count by SysTick divisor.
      steady_count_ += port::clock_highres::cycles_per_tick ();
    }

    inline uint32_t
    __attribute__((always_inline))
    clock_highres::input_clock_frequency_hz (void)
    {
      return port::clock_highres::input_clock_frequency_hz ();
    }

  // ========================================================================

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_CLOCKS_H_ */
