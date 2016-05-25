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

#ifndef CMSIS_PLUS_RTOS_OS_SEMAPHORE_H_
#define CMSIS_PLUS_RTOS_OS_SEMAPHORE_H_

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
     * @brief POSIX compliant **semaphore**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class semaphore : public named_object
    {
    public:

      /**
       * @brief Type of semaphore counter storage.
       * @details
       * A numeric value enough to hold the semaphore counter,
       * usually a 16-bits signed value.
       */
      using count_t = int16_t;

      /**
       * @brief Maximum semaphore value.
       * @details
       * Used to validate the semaphore initial count and max count.
       */
      static constexpr count_t max_count_value = 0x7FFF;

      // ======================================================================

      /**
       * @brief Semaphore attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class attributes : public clocked_attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a semaphore attributes object.
         * @par Parameters
         *  None
         */
        constexpr
        attributes ();

        /**
         * @cond ignore
         */

      protected:

        constexpr
        attributes (count_t max_count);

      public:

        attributes (const attributes&) = default;
        attributes (attributes&&) = default;
        attributes&
        operator= (const attributes&) = default;
        attributes&
        operator= (attributes&&) = default;

        /**
         * @endcond
         */

        /**
         * @brief Destroy the semaphore attributes object.
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

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief Semaphore initial count.
         */
        count_t sm_initial_count = 0;

        /**
         * @brief Semaphore max count.
         */
        count_t sm_max_count = max_count_value;

        // Add more attributes here.

        /**
         * @}
         */

      }; /* class attributes */

      /**
       * @brief Default counting semaphore initialiser.
       */
      static const attributes counting_initializer;

      // ======================================================================

      /**
       * @brief Binary semaphore attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class binary_attributes : public attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create a binary semaphore attributes object.
         * @par Parameters
         *  None
         */
        constexpr
        binary_attributes ();

        /**
         * @cond ignore
         */

        binary_attributes (const binary_attributes&) = default;
        binary_attributes (binary_attributes&&) = default;
        binary_attributes&
        operator= (const binary_attributes&) = default;
        binary_attributes&
        operator= (binary_attributes&&) = default;

        /**
         * @endcond
         */

        /**
         * @brief Destroy the semaphore attributes object.
         */
        ~binary_attributes () = default;

        /**
         * @}
         */

      }; /* class binary_attributes */

      /**
       * @brief Default binary semaphore initialiser.
       */
      static const binary_attributes binary_initializer;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a semaphore object.
       * @param [in] attr Reference to attributes.
       */
      semaphore (const attributes& attr = counting_initializer);

      /**
       * @brief Create a named semaphore object.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       */
      semaphore (const char* name,
                 const attributes& attr = counting_initializer);

      /**
       * @cond ignore
       */

      semaphore (const semaphore&) = delete;
      semaphore (semaphore&&) = delete;
      semaphore&
      operator= (const semaphore&) = delete;
      semaphore&
      operator= (semaphore&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destroy the semaphore object.
       */
      ~semaphore ();

      /**
       * @}
       */

      /*
       * @name Operators
       * @{
       */

      /**
       * @brief Compare semaphores.
       * @retval true The given semaphore is the same as this semaphore.
       * @retval false The semaphores are different.
       */
      bool
      operator== (const semaphore& rhs) const;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Post (unlock) the semaphore.
       * @par Parameters
       *  None
       * @retval result::ok The semaphore was posted.
       * @retval EAGAIN The max count was exceeded.
       * @retval ENOTRECOVERABLE The semaphore could not be posted
       *  (extension to POSIX).
       */
      result_t
      post (void);

      /**
       * @brief Lock the semaphore, possibly waiting.
       * @par Parameters
       *  None
       * @retval result::ok The calling process successfully
       *  performed the semaphore lock operation.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
       * @retval EDEADLK A deadlock condition was detected.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      wait (void);

      /**
       * @brief Try to lock the semaphore.
       * @par Parameters
       *  None
       * @retval result::ok The calling process successfully
       *  performed the semaphore lock operation.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EWOULDBLOCK The semaphore was already locked.
       * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
       * @retval EDEADLK A deadlock condition was detected.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      try_wait (void);

      /**
       * @brief Timed wait to lock the semaphore.
       * @param [in] timeout Timeout to wait.
       * @retval result::ok The calling process successfully
       *  performed the semaphore lock operation.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL Invalid timeout (POSIX limits the timeout
       *  to 1000 million ns)
       * @retval ETIMEDOUT The semaphore could not be locked before
       *  the specified timeout expired.
       * @retval ENOTRECOVERABLE Semaphore wait failed (extension to POSIX).
       * @retval EDEADLK A deadlock condition was detected.
       * @retval EINTR The operation was interrupted.
       */
      result_t
      timed_wait (clock::duration_t timeout);

      /**
       * @brief Get the semaphore value.
       * @par Parameters
       *  None
       * @return The semaphore value.
       */
      count_t
      value (void) const;

      /**
       * @brief Reset the semaphore.
       * @par Parameters
       *  None
       * @retval result::ok The semaphore was reset.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      reset (void);

      /**
       * @brief Get the semaphore initial value.
       * @par Parameters
       *  None
       * @return The numeric value set from attributes.
       */
      count_t
      initial_value (void) const;

      /**
       * @brief Get the semaphore count max value.
       * @par Parameters
       *  None
       * @return The numeric value set from attributes.
       */
      count_t
      max_value (void) const;

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

      /**
       * @brief Internal initialisation.
       * @par Parameters
       *  None
       */
      void
      _init (void);

      bool
      _try_wait (void);

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

#if !defined(OS_INCLUDE_RTOS_PORT_SEMAPHORE)
      waiting_threads_list list_;
      clock* clock_ = nullptr;
#endif

#if defined(OS_INCLUDE_RTOS_PORT_SEMAPHORE)
      friend class port::semaphore;
      os_semaphore_port_data_t port_;
#endif

      const count_t initial_count_;

      // Can be updated in different contexts (interrupts or threads)
      volatile count_t count_ = 0;

      // Constant set during construction.
      const count_t max_count_ = max_count_value;

      // Add more internal data.

      /**
       * @endcond
       */

      /**
       * @}
       */

    };

#pragma GCC diagnostic pop

  // ==========================================================================

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    // ========================================================================

    constexpr
    semaphore::attributes::attributes ()
    {
      ;
    }

    constexpr
    semaphore::attributes::attributes (count_t max_count) :
        sm_max_count (max_count)
    {
      ;
    }

    // ========================================================================

    constexpr
    semaphore::binary_attributes::binary_attributes () :
        attributes
          { 1 } // Use the protected constructor.
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * Identical semaphores should have the same memory address.
     */
    inline bool
    semaphore::operator== (const semaphore& rhs) const
    {
      return this == &rhs;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline semaphore::count_t
    semaphore::initial_value (void) const
    {
      return initial_count_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    inline semaphore::count_t
    semaphore::max_value (void) const
    {
      return max_count_;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_SEMAPHORE_H_ */
