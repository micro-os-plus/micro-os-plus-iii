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
       * @cond ignore
       */

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
      extern top_threads_list top_threads_list_;

#if !defined(OS_INCLUDE_RTOS_PORT_SCHEDULER)
      extern thread* volatile current_thread_;
      extern ready_threads_list ready_threads_list_;
      extern terminated_threads_list terminated_threads_list_;
#endif

      /**
       * @endcond
       */

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
       * @par Returns
       *  Nothing.
       */
      [[noreturn]] void
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
       * @brief Lock the scheduler.
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

      /**
       * @cond ignore
       */

      void
      _create_idle (void);

      /**
       * @endcond
       */

      // ======================================================================
      /**
       * @brief Scheduler critical section [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) helper.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class critical_section
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
        critical_section ();

        /**
         * @cond ignore
         */

        critical_section (const critical_section&) = delete;
        critical_section (critical_section&&) = delete;
        critical_section&
        operator= (const critical_section&) = delete;
        critical_section&
        operator= (critical_section&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit a critical section.
         */
        ~critical_section ();

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
         * @brief Variable to store the initial scheduler status.
         */
        const status_t status_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Scheduler uncritical section [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) helper.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class uncritical_section
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
        uncritical_section ();

        /**
         * @cond ignore
         */

        uncritical_section (const uncritical_section&) = delete;
        uncritical_section (uncritical_section&&) = delete;
        uncritical_section&
        operator= (const uncritical_section&) = delete;
        uncritical_section&
        operator= (uncritical_section&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit a critical section.
         */
        ~uncritical_section ();

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
         * @brief Variable to store the initial scheduler status.
         */
        const status_t status_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief %Scheduler standard locker.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class lockable
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
        lockable ();

        /**
         * @cond ignore
         */

        lockable (const lockable&) = delete;
        lockable (lockable&&) = delete;
        lockable&
        operator= (const lockable&) = delete;
        lockable&
        operator= (lockable&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destroy the lock.
         */
        ~lockable ();

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
         * @cond ignore
         */

        /**
         * @brief Variable to store the initial scheduler status.
         */
        status_t status_ = 0;

        /**
         * @endcond
         */

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
      class critical_section
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
        critical_section ();

        /**
         * @cond ignore
         */

        critical_section (const critical_section&) = delete;
        critical_section (critical_section&&) = delete;
        critical_section&
        operator= (const critical_section&) = delete;
        critical_section&
        operator= (critical_section&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit interrupts critical section.
         */
        ~critical_section ();

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
         * @cond ignore
         */

        /**
         * @brief Variable to store the interrupts status.
         */
        const status_t status_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Interrupts critical section [RAII](https://en.wikipedia.org/wiki/Resource_Acquisition_Is_Initialization) helper.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class uncritical_section
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Enter interrupts uncritical section.
         * @par Parameters
         *  None
         */
        uncritical_section ();

        /**
         * @cond ignore
         */

        uncritical_section (const uncritical_section&) = delete;
        uncritical_section (uncritical_section&&) = delete;
        uncritical_section&
        operator= (const uncritical_section&) = delete;
        uncritical_section&
        operator= (uncritical_section&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Exit interrupts uncritical section.
         */
        ~uncritical_section ();

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        /**
         * @brief Enter interrupts uncritical section.
         * @par Parameters
         *  None
         * @return The current interrupts status register.
         */
        static status_t
        enter (void);

        /**
         * @brief Exit interrupts uncritical section.
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
         * @cond ignore
         */

        /**
         * @brief Variable to store the interrupts status.
         */
        const status_t status_;

        /**
         * @endcond
         */

        /**
         * @}
         */
      };

      // ======================================================================

      /**
       * @brief Interrupts standard locker.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class lockable
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
        lockable ();

        /**
         * @brief Destroy the interrupts lock.
         */
        ~lockable ();

        /**
         * @cond ignore
         */

        lockable (const lockable&) = delete;
        lockable (lockable&&) = delete;
        lockable&
        operator= (const lockable&) = delete;
        lockable&
        operator= (lockable&&) = delete;

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
         * @cond ignore
         */

        /**
         * @brief Variable to store the interrupts status.
         */
        status_t status_;

        /**
         * @endcond
         */

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
      critical_section::critical_section () :
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
      critical_section::~critical_section ()
      {
        unlock (status_);
      }

      /**
       * @details
       * Lock the scheduler and remember the initial scheduler status.
       */
      inline
      uncritical_section::uncritical_section () :
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
      uncritical_section::~uncritical_section ()
      {
        unlock (status_);
      }

      constexpr
      lockable::lockable () :
          status_ (scheduler::init_status)
      {
        ;
      }

      inline
      lockable::~lockable ()
      {
        ;
      }

      inline void
      lockable::lock (void)
      {
        status_ = scheduler::lock ();
      }

      /**
       * @details
       * Somehow redundant, since the lock will always succeed;
       * but used to meet the lockableable requirements.
       */
      inline bool
      lockable::try_lock (void)
      {
        status_ = scheduler::lock ();
        return true;
      }

      inline void
      lockable::unlock (void)
      {
        scheduler::unlock (status_);
      }

    } /* namespace scheduler */

    // ========================================================================

    namespace interrupts
    {
      inline
      critical_section::critical_section () :
          status_ (enter ())
      {
        ;
      }

      inline
      critical_section::~critical_section ()
      {
        exit (status_);
      }

      inline
      uncritical_section::uncritical_section () :
          status_ (enter ())
      {
        ;
      }

      inline
      uncritical_section::~uncritical_section ()
      {
        exit (status_);
      }

      constexpr
      lockable::lockable () :
          status_ (port::interrupts::init_status)
      {
        ;
      }

      inline
      lockable::~lockable ()
      {
        ;
      }

      inline void
      lockable::lock (void)
      {
        status_ = critical_section::enter ();
      }

      /**
       * @details
       * Somehow redundant, since the lock will always succeed;
       * but used to meet the Lockable requirements.
       */
      inline bool
      lockable::try_lock (void)
      {
        status_ = critical_section::enter ();
        return true;
      }

      inline void
      lockable::unlock (void)
      {
        critical_section::exit (status_);
      }

    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_SCHED_H_ */
