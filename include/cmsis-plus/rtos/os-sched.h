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
       * @brief Head of the list with all top level and detached threads.
       */
      extern Top_threads_list top_threads_list_;

#if !defined(OS_INCLUDE_RTOS_PORT_THREAD)
      extern Thread* current_thread_;
      extern Ready_threads_list ready_threads_list_;
      extern Terminated_threads_list terminated_threads_list_;
#endif

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
       * @param [in] status The new status of the scheduler (true for locked).
       * @return The previous status of the scheduler.
       */
      status_t
      lock (status_t status = true);

      /**
       * @brief Unlock the scheduler.
       * @param [in] status The new status of the scheduler (false for unlocked).
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

      void
      _create_idle (void);

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
       * @brief Scheduler uncritical section [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) helper.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Uncritical_section
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
        Uncritical_section ();

        /**
         * @cond ignore
         */
        Uncritical_section (const Uncritical_section&) = delete;
        Uncritical_section (Uncritical_section&&) = delete;
        Uncritical_section&
        operator= (const Uncritical_section&) = delete;
        Uncritical_section&
        operator= (Uncritical_section&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Exit a critical section.
         */
        ~Uncritical_section ();

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
         * @brief Destroy the lock.
         */
        ~Lock ();

        /**
         * @}
         */

      public:

        /**
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
         */

      public:

        /**
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
         * @brief Variable to store the interrupts status.
         */
        const status_t status_;

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Interrupts critical section [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) helper.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Uncritical_section
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
        Uncritical_section ();

        /**
         * @cond ignore
         */
        Uncritical_section (const Uncritical_section&) = delete;
        Uncritical_section (Uncritical_section&&) = delete;
        Uncritical_section&
        operator= (const Uncritical_section&) = delete;
        Uncritical_section&
        operator= (Uncritical_section&&) = delete;
        /**
         * @endcond
         */

        /**
         * @brief Exit interrupts critical section.
         */
        ~Uncritical_section ();

        /**
         * @}
         */

      public:

        /**
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
         * @brief Variable to store the interrupts status.
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
         */

      public:

        /**
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
         * @brief Variable to store the interrupts status.
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

      /**
       * @details
       * Lock the scheduler and remember the initial scheduler status.
       */
      inline
      Uncritical_section::Uncritical_section () :
          status_ (lock (false))
      {
        ;
      }

      /**
       * @details
       * Restore the initial scheduler status and possibly unlock
       * the scheduler.
       */
      inline
      Uncritical_section::~Uncritical_section ()
      {
        unlock (status_);
      }

      constexpr
      Lock::Lock () :
          status_ (scheduler::init_status)
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

      inline
      Uncritical_section::Uncritical_section () :
          status_ (enter ())
      {
        ;
      }

      inline
      Uncritical_section::~Uncritical_section ()
      {
        exit (status_);
      }

      constexpr
      Lock::Lock () :
          status_ (port::interrupts::init_status)
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
