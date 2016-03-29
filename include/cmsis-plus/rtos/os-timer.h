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
       * @brief Timer attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Attributes : public Named_object
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
        type_t tm_type;

        //
        // TODO: add clock ID.
        //
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

    class Double_list_node_timer : public Double_list_node_timestamp
    {
    public:

      Double_list_node_timer (Double_list& lst, clock::timestamp_t ts,
                              Timer& tm);

      /**
       * @cond ignore
       */
      Double_list_node_timer (const Double_list_node_timer&) = delete;
      Double_list_node_timer (Double_list_node_timer&&) = delete;
      Double_list_node_timer&
      operator= (const Double_list_node_timer&) = delete;
      Double_list_node_timer&
      operator= (Double_list_node_timer&&) = delete;
      /**
       * @endcond
       */

      ~Double_list_node_timer ();

      virtual void
      action (void) override;

    public:

      Timer& timer;
    };

#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief User single-shot or periodic **timer**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Timer : public Named_object
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
       * @brief Create a timer with custom settings.
       * @param [in] attr Reference to attributes.
       * @param [in] function Pointer to timer function.
       * @param [in] args Pointer to arguments.
       */
      Timer (const timer::Attributes& attr, timer::func_t function,
             timer::func_args_t args);

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

      friend class Double_list_node_timer;

#if !defined(OS_INCLUDE_RTOS_PORT_TIMER)

      void
      interrupt_service_routine(void);

#endif

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      timer::func_t func_;
      timer::func_args_t func_args_;

#if !defined(OS_INCLUDE_RTOS_PORT_TIMER)
      Double_list_node_timer timer_node_;
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
      inline
      Attributes::Attributes (const char* name) :
          Named_object
            { name }
      {
        this->tm_type = run::once;
      }

      // ======================================================================

      inline
      Periodic_attributes::Periodic_attributes (const char* name) :
          Attributes
            { name }
      {
        this->tm_type = run::periodic;
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
