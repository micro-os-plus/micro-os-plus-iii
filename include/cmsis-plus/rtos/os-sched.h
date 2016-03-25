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

#ifndef CMSIS_PLUS_RTOS_OS_SCHED_H_
#define CMSIS_PLUS_RTOS_OS_SCHED_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace scheduler
    {
      /**
       * @brief The scheduler status.
       */
      extern status_t is_locked_;

      /**
       * @brief Variable set to true after the scheduler is started.
       */
      extern bool is_started_;

      /**
       * @brief Initialise the RTOS scheduler.
       * @par Parameters
       *  None
       * @retval result::ok The scheduler was initialised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      initialize (void);

      /**
       * @brief Start the RTOS scheduler.
       * @par Parameters
       *  None
       * @retval result::ok The scheduler was started.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      start (void);

      /**
       * @brief Check if the scheduler was started.
       * @par Parameters
       *  None
       * @retval true The scheduler was started.
       * @retval false The scheduler was not started.
       */
      bool
      started (void);

      /**
       * @brief Check if the scheduler is locked.
       * @par Parameters
       *  None
       * @retval true The scheduler is locked.
       * @retval false The scheduler is running (not locked).
       */
      bool
      locked (void);

      /**
       * @brief %Lock the scheduler.
       * @par Parameters
       *  None
       * @return The previous status of the scheduler.
       */
      status_t
      lock (void);

      /**
       * @brief Unlock the scheduler.
       * @param [in] status The new status of the scheduler.
       * @return  Nothing.
       */
      void
      unlock (status_t status);

      /**
       * @brief Check if the CPU is in handler mode.
       * @par Parameters
       *  None
       * @retval true Execution is in an exception handler context.
       * @retval false Execution is in a thread context.
       */
      bool
      in_handler_mode (void);

      // ======================================================================

      /**
       * @brief Scheduler critical section [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) helper.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Critical_section
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Enter a critical section.
         * @par Parameters
         *  None
         */
        Critical_section ();

        /**
         * @cond ignore
         */
        Critical_section (const Critical_section&) = delete;
        Critical_section (Critical_section&&) = delete;
        Critical_section&
        operator= (const Critical_section&) = delete;
        Critical_section&
        operator= (Critical_section&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Exit a critical section.
         */
        ~Critical_section ();

        /**
         * @}
         */
      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Variable to store the initial scheduler status.
         */
        const status_t status_;

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief %Scheduler standard locker.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Lock
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a lock.
         * @par Parameters
         *  None
         */
        constexpr
        Lock ();

        /**
         * @brief Destroy the lock.
         */
        ~Lock ();

        /**
         * @cond ignore
         */
        Lock (const Lock&) = delete;
        Lock (Lock&&) = delete;
        Lock&
        operator= (const Lock&) = delete;
        Lock&
        operator= (Lock&&) = delete;
        /**
         * @endcond
         */

        /**
         * @}
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief %Lock the scheduler.
         * @par Parameters
         *  None
         * @return  Nothing.
         */
        void
        lock (void);

        /**
         * @brief Try to lock the scheduler.
         * @par Parameters
         *  None
         * @retval true The scheduler was locked.
         */
        bool
        try_lock (void);

        /**
         * @brief Unlock the scheduler.
         * @par Parameters
         *  None
         * @return  Nothing.
         */
        void
        unlock (void);

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Variable to store the initial scheduler status.
         */
        status_t status_ = 0;

        /**
         * @}
         */
      };

    // ------------------------------------------------------------------------
    } /* namespace scheduler */

    namespace interrupts
    {
      // TODO: define all levels of critical sections
      // (kernel, real-time(level), complete)

      // TODO: make template, parameter IRQ level

      // ======================================================================

      /**
       * @brief Interrupts critical section [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) helper.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Critical_section
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Enter interrupts critical section.
         * @par Parameters
         *  None
         */
        Critical_section ();

        /**
         * @cond ignore
         */
        Critical_section (const Critical_section&) = delete;
        Critical_section (Critical_section&&) = delete;
        Critical_section&
        operator= (const Critical_section&) = delete;
        Critical_section&
        operator= (Critical_section&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Exit interrupts critical section.
         */
        ~Critical_section ();

        /**
         * @}
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Enter interrupts critical section.
         * @par Parameters
         *  None
         * @return The current interrupts status register.
         */
        static status_t
        enter (void);

        /**
         * @brief Exit interrupts critical section.
         * @param status The value to restore the interrupts status register.
         * @return  Nothing.
         */
        static void
        exit (status_t status);

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Variable to store the initial interrupts status.
         */
        const status_t status_;

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Interrupts standard locker.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Lock
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create an interrupts lock.
         * @par Parameters
         *  None
         */
        constexpr
        Lock ();

        /**
         * @brief Destroy the interrupts lock.
         */
        ~Lock ();

        /**
         * @cond ignore
         */
        Lock (const Lock&) = delete;
        Lock (Lock&&) = delete;
        Lock&
        operator= (const Lock&) = delete;
        Lock&
        operator= (Lock&&) = delete;
        /**
         * @endcond
         */

        /**
         * @}
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Lock the interrupts.
         * @par Parameters
         *  None
         * @return  Nothing.
         */
        void
        lock (void);

        /**
         * @brief Try to lock the interrupts.
         * @par Parameters
         *  None
         * @retval true The interrupts were locked.
         */
        bool
        try_lock (void);

        /**
         * @brief Unlock the interrupts.
         * @par Parameters
         *  None
         * @return  Nothing.
         */
        void
        unlock (void);

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @brief Variable to store the initial interrupts status.
         */
        status_t status_;

        /**
         * @}
         */

      };

    } /* namespace interrupts */

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    namespace scheduler
    {
      /**
       * @details
       * Check if the scheduler was started, i.e. if scheduler::start()
       * was called.
       */
      inline bool
      started (void)
      {
        return is_started_;
      }

      /**
       * @details
       * Check if the scheduler is locked on the current thread or
       * is switching threads from the ready list.
       */
      inline bool
      locked (void)
      {
        return is_locked_;
      }

      /**
       * @details
       * Lock the scheduler and remember the initial scheduler status.
       */
      inline
      Critical_section::Critical_section () :
          status_ (lock ())
      {
        ;
      }

      /**
       * @details
       * Restore the initial scheduler status and possibly unlock
       * the scheduler.
       */
      inline
      Critical_section::~Critical_section ()
      {
        unlock (status_);
      }

      constexpr
      Lock::Lock () :
          status_ (0)
      {
        ;
      }

      inline
      Lock::~Lock ()
      {
        ;
      }

      inline void
      Lock::lock (void)
      {
        status_ = scheduler::lock ();
      }

      /**
       * @details
       * Somehow redundant, since the lock will always succeed;
       * but used to meet the Lockable requirements.
       */
      inline bool
      Lock::try_lock (void)
      {
        status_ = scheduler::lock ();
        return true;
      }

      inline void
      Lock::unlock (void)
      {
        scheduler::unlock (status_);
      }

    } /* namespace scheduler */

    // ========================================================================

    namespace interrupts
    {
      inline
      Critical_section::Critical_section () :
          status_ (enter ())
      {
        ;
      }

      inline
      Critical_section::~Critical_section ()
      {
        exit (status_);
      }

      constexpr
      Lock::Lock () :
          status_ (0)
      {
        ;
      }

      inline
      Lock::~Lock ()
      {
        ;
      }

      inline void
      Lock::lock (void)
      {
        status_ = Critical_section::enter ();
      }

      /**
       * @details
       * Somehow redundant, since the lock will always succeed;
       * but used to meet the Lockable requirements.
       */
      inline bool
      Lock::try_lock (void)
      {
        status_ = Critical_section::enter ();
        return true;
      }

      inline void
      Lock::unlock (void)
      {
        Critical_section::exit (status_);
      }

    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_SCHED_H_ */
