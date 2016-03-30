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

#ifndef CMSIS_PLUS_RTOS_OS_CONDVAR_H_
#define CMSIS_PLUS_RTOS_OS_CONDVAR_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace condvar
    {

      // ======================================================================

      /**
       * @brief Condition variable attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Attributes : public Clocked_attribute
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create condition variable attributes.
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
         * @brief Destroy condition variable attributes.
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
        //
        // TODO: add clock ID.
        //
        // Add more attributes.
        /**
         * @}
         */
      };

      /**
       * @brief Default condition variable initialiser.
       */
      extern const Attributes initializer;

    } /* namespace condvar */

    // ========================================================================

    /**
     * @brief POSIX compliant **condition variable**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Condition_variable : public Named_object
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a condition variable with default settings.
       * @par Parameters
       *  None
       * @par Errors
       *  The constructor shall fail if:
       *  - `EAGAIN` - The system lacked the necessary resources
       *  (other than memory) to create the condition variable.
       *  - `ENOMEM` - Insufficient memory exists to initialise
       *  the condition variable.
       * @par
       *  The constructor shall not fail with an error code of `EINTR`.
       */
      Condition_variable ();

      /**
       * @brief Create a condition variable with custom settings.
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
      Condition_variable (const condvar::Attributes& attr);

      /**
       * @cond ignore
       */
      Condition_variable (const Condition_variable&) = delete;
      Condition_variable (Condition_variable&&) = delete;
      Condition_variable&
      operator= (const Condition_variable&) = delete;
      Condition_variable&
      operator= (Condition_variable&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy a condition variable.
       */
      ~Condition_variable ();

      /**
       * @}
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
      operator== (const Condition_variable& rhs) const;

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
       *  None
       * @retval result::ok The thread was signaled.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @par Errors
       *  The function shall not fail with an error code of `EINTR`.
       */
      result_t
      signal (void);

      /**
       * @brief Notify all threads waiting for a condition variable.
       * @par Parameters
       *  None
       * @retval result::ok All waiting threads signaled.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @par Errors
       *  The function shall not fail with an error code of `EINTR`.
       */
      result_t
      broadcast (void);

      /**
       * @brief Wait for a condition variable to be notified.
       * @param [in] mutex Reference to the associated mutex.
       * @retval result::ok The condition change was signaled.
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
      wait (Mutex& mutex);

      // Neither POSIX nor ISO define a try_wait(), it makes no sense.

      /**
       * @brief Timed wait for a condition variable to be notified.
       * @param [in] mutex Reference to the associated mutex.
       * @param [in] timeout Timeout to wait.
       * @retval result::ok The condition change was signaled.
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
      timed_wait (Mutex& mutex, clock::duration_t timeout);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

#if !defined(OS_INCLUDE_RTOS_PORT_CONDITION_VARIABLE)
      Waiting_threads_list list_;
      // Clock& clock_;
#endif

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
    namespace condvar
    {
      inline
      Attributes::Attributes (const char* name) :
          Clocked_attribute
            { name }
      {
        ;
      }
    } /* namespace condvar */

    // ========================================================================

    /**
     * @details
     * Identical condition variables should have the same memory address.
     */
    inline bool
    Condition_variable::operator== (const Condition_variable& rhs) const
    {
      return this == &rhs;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_CONDVAR_H_ */
