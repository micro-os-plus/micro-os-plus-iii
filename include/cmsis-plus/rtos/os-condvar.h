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

#ifndef CMSIS_PLUS_RTOS_OS_CONDVAR_H_
#define CMSIS_PLUS_RTOS_OS_CONDVAR_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ========================================================================

    /**
     * @brief POSIX compliant **condition variable**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-condvar
     */
    class condition_variable : public internal::object_named_system
    {
    public:

      // ======================================================================

      /**
       * @brief Condition variable attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-condvar
       */
      class attributes : public internal::attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a condition variable attributes object instance.
         * @par Parameters
         *  None.
         */
        constexpr
        attributes ();

        // The rule of five.
        attributes (const attributes&) = default;
        attributes (attributes&&) = default;
        attributes&
        operator= (const attributes&) = default;
        attributes&
        operator= (attributes&&) = default;

        /**
         * @brief Destruct the condition variable attributes object instance.
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
        // Add more attributes here.
        /**
         * @}
         */

      }; /* class attributes */

      /**
       * @brief Default condition variable initialiser.
       * @ingroup cmsis-plus-rtos-condvar
       */
      static const attributes initializer;

      // ======================================================================

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a condition variable object instance.
       * @param [in] attr Reference to attributes.
       * @par Errors
       *  The constructor shall fail if:
       *  - `EAGAIN` - The system lacked the necessary resources
       *  (other than memory) to create the condition variable.
       *  - `ENOMEM` - Insufficient memory exists to initialise
       *  the condition variable.
       * @par
       *  The constructor shall not fail with an error code of `EINTR`.
       */
      condition_variable (const attributes& attr = initializer);

      /**
       * @brief Construct a named condition variable object instance.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       * @par Errors
       *  The constructor shall fail if:
       *  - `EAGAIN` - The system lacked the necessary resources
       *  (other than memory) to create the condition variable.
       *  - `ENOMEM` - Insufficient memory exists to initialise
       *  the condition variable.
       * @par
       *  The constructor shall not fail with an error code of `EINTR`.
       */
      condition_variable (const char* name,
                          const attributes& attr = initializer);

      /**
       * @cond ignore
       */

      // The rule of five.
      condition_variable (const condition_variable&) = delete;
      condition_variable (condition_variable&&) = delete;
      condition_variable&
      operator= (const condition_variable&) = delete;
      condition_variable&
      operator= (condition_variable&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the condition variable object instance.
       */
      ~condition_variable ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare condition variables.
       * @retval true The given condition variable is the same as
       *  this condition variable.
       * @retval false The condition variables are different.
       */
      bool
      operator== (const condition_variable& rhs) const;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Notify one thread waiting for a condition variable.
       * @par Parameters
       *  None.
       * @retval result::ok The thread was signalled.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @par Errors
       *  The function shall not fail with an error code of `EINTR`.
       */
      result_t
      signal (void);

      /**
       * @brief Notify all threads waiting for a condition variable.
       * @par Parameters
       *  None.
       * @retval result::ok All waiting threads signalled.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @par Errors
       *  The function shall not fail with an error code of `EINTR`.
       */
      result_t
      broadcast (void);

      /**
       * @brief Wait for a condition variable to be notified.
       * @param [in] mutex Reference to the associated mutex.
       * @retval result::ok The condition change was signalled.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines,
       *  or the mutex type is `mutex::type::errorcheck` or the mutex
       *  is a robust mutex, and the current thread does not own the mutex.
       * @retval ENOTRECOVERABLE The state protected by the mutex is
       *  not recoverable.
       * @retval EOWNERDEAD The mutex is a robust mutex and the
       *  process containing the previous owning thread terminated
       *  while holding the mutex lock. The mutex lock shall be
       *  acquired by the calling thread and it is up to the new
       *  owner to make the state consistent.
       * @par Errors
       *  The function shall not fail with an error code of `EINTR`.
       */
      result_t
      wait (mutex& mutex);

      // Neither POSIX nor ISO define a try_wait(), it makes no sense.

      /**
       * @brief Timed wait for a condition variable to be notified.
       * @param [in] mutex Reference to the associated mutex.
       * @param [in] timeout Timeout to wait.
       * @retval result::ok The condition change was signalled.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines,
       *  or the mutex type is `mutex::type::errorcheck` or the mutex
       *  is a robust mutex, and the current thread does not own the mutex.
       * @retval ENOTRECOVERABLE The state protected by the mutex is
       *  not recoverable.
       * @retval EOWNERDEAD The mutex is a robust mutex and the
       *  process containing the previous owning thread terminated
       *  while holding the mutex lock. The mutex lock shall be
       *  acquired by the calling thread and it is up to the new
       *  owner to make the state consistent.
       * @retval ETIMEDOUT The timeout has passed.
       * @par Errors
       *  The function shall not fail with an error code of `EINTR`.
       */
      result_t
      timed_wait (mutex& mutex, clock::duration_t timeout);

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

#if !defined(OS_USE_RTOS_PORT_CONDITION_VARIABLE)
      internal::waiting_threads_list list_;
      // clock& clock_;
#endif

      /**
       * @endcond
       */

      // Add more internal data.
      /**
       * @}
       */

    };

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    constexpr
    condition_variable::attributes::attributes ()
    {
    }

    // ========================================================================

    /**
     * @details
     * Identical condition variables should have the same memory address.
     */
    inline bool
    condition_variable::operator== (const condition_variable& rhs) const
    {
      return this == &rhs;
    }

  } /* namespace rtos */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_CONDVAR_H_ */
