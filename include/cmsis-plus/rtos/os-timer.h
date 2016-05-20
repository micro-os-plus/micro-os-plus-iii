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

#ifndef CMSIS_PLUS_RTOS_OS_TIMER_H_
#define CMSIS_PLUS_RTOS_OS_TIMER_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace timer
    {

      // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief %Timer attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Attributes : public clocked_attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create timer attributes.
         * @param [in] name Null terminated name. If `nullptr`, "-" is assigned.
         */
        constexpr
        Attributes (const char* name);

        /**
         * @cond ignore
         */
        Attributes (const Attributes&) = default;
        Attributes (Attributes&&) = default;
        Attributes&
        operator= (const Attributes&) = default;
        Attributes&
        operator= (Attributes&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destroy timer attributes.
         */
        ~Attributes () = default;

        /**
         * @}
         */

      protected:

        constexpr
        Attributes (const char* name, type_t type);

      public:

        /**
         * @name Public Member Variables
         * @{
         */

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief Timer type attribute.
         */
        type_t tm_type = run::once;

        // Add more attributes.

        /**
         * @}
         */
      };

#pragma GCC diagnostic pop

      /**
       * @brief Default one shot timer initialiser.
       */
      extern const Attributes once_initializer;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief Periodic timer attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Periodic_attributes : public Attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create periodic timer attributes.
         * @param [in] name Null terminated name. If `nullptr`, "-" is assigned.
         */
        constexpr
        Periodic_attributes (const char* name);

        /**
         * @cond ignore
         */
        Periodic_attributes (const Periodic_attributes&) = default;
        Periodic_attributes (Periodic_attributes&&) = default;
        Periodic_attributes&
        operator= (const Periodic_attributes&) = default;
        Periodic_attributes&
        operator= (Periodic_attributes&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destroy timer attributes.
         */
        ~Periodic_attributes () = default;

        /**
         * @}
         */

      };

#pragma GCC diagnostic pop

      /**
       * @brief Default periodic timer initialiser.
       */
      extern const Periodic_attributes periodic_initializer;
    }

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief User single-shot or periodic **timer**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Timer : public named_object
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a timer with default settings.
       * @param [in] function Pointer to timer function.
       * @param [in] args Pointer to arguments.
       */
      Timer (timer::func_t function, timer::func_args_t args);

      /**
       * @brief Create a named timer with default settings.
       * @param [in] name Pointer to name.
       * @param [in] function Pointer to timer function.
       * @param [in] args Pointer to arguments.
       */
      Timer (const char* name, timer::func_t function, timer::func_args_t args);

      /**
       * @brief Create a timer with custom settings.
       * @param [in] attr Reference to attributes.
       * @param [in] function Pointer to timer function.
       * @param [in] args Pointer to arguments.
       */
      Timer (const timer::Attributes& attr, timer::func_t function,
             timer::func_args_t args);

      /**
       * @brief Create a named timer with custom settings.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       * @param [in] function Pointer to timer function.
       * @param [in] args Pointer to arguments.
       */
      Timer (const char* name, const timer::Attributes& attr,
             timer::func_t function, timer::func_args_t args);

      /**
       * @cond ignore
       */
      Timer (const Timer&) = delete;
      Timer (Timer&&) = delete;
      Timer&
      operator= (const Timer&) = delete;
      Timer&
      operator= (Timer&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the timer.
       */
      ~Timer ();

      /**
       * @}
       */

      /*
       * @name Operators
       * @{
       */

      /**
       * @brief Compare timers.
       * @retval true The given timer is the same as this timer.
       * @retval false The timers are different.
       */
      bool
      operator== (const Timer& rhs) const;

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
       *  None
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

      friend class Timer_node;

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Functions
       * @{
       */

#if !defined(OS_INCLUDE_RTOS_PORT_TIMER)

      void
      _interrupt_service_routine (void);

#endif

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      timer::func_t func_;
      timer::func_args_t func_args_;

#if !defined(OS_INCLUDE_RTOS_PORT_TIMER)
      Clock* clock_;
      Timer_node timer_node_
        { 0, *this };
      clock::duration_t period_;
#endif

#if defined(OS_INCLUDE_RTOS_PORT_TIMER)
      friend class port::Timer;
      os_timer_port_data_t port_;
#endif

      timer::type_t type_;
      timer::state_t state_;

      // Add more internal data.

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
    namespace timer
    {
      // ======================================================================

      constexpr
      Attributes::Attributes (const char* name) :
          clocked_attributes
            { name }
      {
        ;
      }

      constexpr
      Attributes::Attributes (const char* name, type_t type) :
          clocked_attributes
            { name }, //
          tm_type (type)
      {
        ;
      }

      // ======================================================================

      constexpr
      Periodic_attributes::Periodic_attributes (const char* name) :
          Attributes
            { name, run::periodic }
      {
        ;
      }

    } /* namespace timer */

    // ========================================================================

    /**
     * @details
     * Identical timers should have the same memory address.
     */
    inline bool
    Timer::operator== (const Timer& rhs) const
    {
      return this == &rhs;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_TIMER_H_ */
