/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_RTOS_OS_TIMER_H_
#define CMSIS_PLUS_RTOS_OS_TIMER_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

    /**
     * @brief User single-shot or periodic **timer**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-timer
     */
    class timer : public internal::object_named_system
    {
    public:

      /**
       * @brief Timer call back function arguments.
       * @ingroup cmsis-plus-rtos-timer
       */
      using func_args_t = void*;

      /**
       * @brief Entry point of a timer call back function.
       * @ingroup cmsis-plus-rtos-timer
       */
      using func_t = void (*) (func_args_t args);

      /**
       * @brief Type of of variables holding timer run types.
       * @ingroup cmsis-plus-rtos-timer
       */
      using type_t = uint8_t;

      /**
       * @brief Timer run types.
       * @ingroup cmsis-plus-rtos-timer
       */
      struct run
      {
        enum
          : type_t
            {
              /**
               * @brief Run only once.
               */
              once = 0,

              /**
               * @brief Run periodically.
               */
              periodic = 1      //
        };
      };

      /**
       * @brief Type of of variables holding timer states.
       * @ingroup cmsis-plus-rtos-timer
       */
      using state_t = uint8_t;

      /**
       * @brief Timer states
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-timer
       */
      struct state
      {
        enum
          : state_t
            {
              /**
               * @brief Used to catch uninitialised threads.
               */
              undefined = 0,
          initialized = 1,
          running = 2,
          completed = 3,
          stopped = 4,
          destroyed = 5
        };
      };

      // ======================================================================

      /**
       * @brief Timer attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-timer
       */
      class attributes : public internal::attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a timer attributes object instance.
         * @par Parameters
         *  None.
         */
        constexpr
        attributes ();

      protected:

        /**
         * @cond ignore
         */

        constexpr
        attributes (type_t type);

        /**
         * @endcond
         */

      public:

        // The rule of five.
        attributes (const attributes&) = default;
        attributes (attributes&&) = default;
        attributes&
        operator= (const attributes&) = default;
        attributes&
        operator= (attributes&&) = default;

        /**
         * @brief Destruct the timer attributes object instance.
         */
        ~attributes () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Variables
         * @{
         */

        // Public members; no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief Timer type attribute.
         */
        type_t tm_type = run::once;

        // Add more attributes.

        /**
         * @}
         */

      }; /* class attributes */

      /**
       * @brief Default one shot timer initialiser.
       */
      static const attributes once_initializer;

      /**
       * @brief Periodic timer attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-timer
       */
      class attributes_periodic : public attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct periodic timer attributes object instance.
         * @par Parameters
         *  None.
         */
        constexpr
        attributes_periodic ();

        // The rule of five.
        attributes_periodic (const attributes_periodic&) = default;
        attributes_periodic (attributes_periodic&&) = default;
        attributes_periodic&
        operator= (const attributes_periodic&) = default;
        attributes_periodic&
        operator= (attributes_periodic&&) = default;

        /**
         * @brief Destruct the periodic timer attributes object instance.
         */
        ~attributes_periodic () = default;

        /**
         * @}
         */

      }; /* class attributes_periodic */

      /**
       * @brief Default periodic timer initialiser.
       * @ingroup cmsis-plus-rtos-timer
       */
      static const attributes_periodic periodic_initializer;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a timer object instance.
       * @param [in] function Pointer to timer function.
       * @param [in] args Pointer to timer function arguments.
       * @param [in] attr Reference to attributes.
       */
      timer (func_t function, func_args_t args, const attributes& attr =
                 once_initializer);

      /**
       * @brief Construct a named timer object instance.
       * @param [in] name Pointer to name.
       * @param [in] function Pointer to timer function.
       * @param [in] args Pointer to timer function arguments.
       * @param [in] attr Reference to attributes.
       */
      timer (const char* name, func_t function, func_args_t args,
             const attributes& attr = once_initializer);

      /**
       * @cond ignore
       */

      timer (const timer&) = delete;
      timer (timer&&) = delete;
      timer&
      operator= (const timer&) = delete;
      timer&
      operator= (timer&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the timer object instance.
       */
      ~timer ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare timers.
       * @retval true The given timer is the same as this timer.
       * @retval false The timers are different.
       */
      bool
      operator== (const timer& rhs) const;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Start or restart the timer.
       * @param [in] period Timer period, in clock units (ticks or seconds).
       * @retval result::ok The timer has been started or restarted.
       * @retval ENOTRECOVERABLE Timer could not be started.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      start (clock::duration_t period);

      /**
       * @brief Stop the timer.
       * @par Parameters
       *  None.
       * @retval result::ok The timer has been stopped.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EAGAIN The timer is not yet started.
       * @retval ENOTRECOVERABLE Timer could not be stopped.
       */
      result_t
      stop (void);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Friends
       * @{
       */

      /**
       * @cond ignore
       */

      friend class internal::timer_node;

      /**
       * @endcond
       */

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @cond ignore
       */

#if !defined(OS_USE_RTOS_PORT_TIMER)

      void
      internal_interrupt_service_routine (void);

#endif

      /**
       * @endcond
       */

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

      func_t func_;
      func_args_t func_args_;

#if !defined(OS_USE_RTOS_PORT_TIMER)
      clock* clock_ = nullptr;
      internal::timer_node timer_node_
        { 0, *this };
      clock::duration_t period_ = 0;
#endif

#if defined(OS_USE_RTOS_PORT_TIMER)
      friend class port::timer;
      os_timer_port_data_t port_;
#endif

      type_t type_ = run::once;
      state_t state_ = state::undefined;

      // Add more internal data.

      /**
       * @endcond
       */

      /**
       * @}
       */

    };

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    // ========================================================================

    constexpr
    timer::attributes::attributes ()
    {
    }

    /**
     * @cond ignore
     */

    constexpr
    timer::attributes::attributes (type_t type) :
        tm_type (type)
    {
    }

    /**
     * @endcond
     */

    // ========================================================================
    constexpr
    timer::attributes_periodic::attributes_periodic () :
        attributes
          { run::periodic }
    {
    }

    /**
     * @details
     * Identical timers should have the same memory address.
     */
    inline bool
    timer::operator== (const timer& rhs) const
    {
      return this == &rhs;
    }

  } /* namespace rtos */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_TIMER_H_ */
