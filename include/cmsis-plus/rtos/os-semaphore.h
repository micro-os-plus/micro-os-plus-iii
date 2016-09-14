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
     * @ingroup cmsis-plus-rtos-semaphore
     */
    class semaphore : public internal::object_named_system
    {
    public:

      /**
       * @brief Type of semaphore counter storage.
       * @details
       * A numeric value enough to hold the semaphore counter,
       * usually a 16-bits signed value.
       * @ingroup cmsis-plus-rtos-semaphore
       */
      using count_t = int16_t;

      /**
       * @brief Maximum semaphore value.
       * @details
       * Used to validate the semaphore initial count and max count.
       * @ingroup cmsis-plus-rtos-semaphore
       */
      static constexpr count_t max_count_value = 0x7FFF;

      // ======================================================================

      /**
       * @brief Semaphore attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-semaphore
       */
      class attributes : public internal::attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a semaphore attributes object instance.
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
        attributes (count_t max_value, count_t initial_value = 0);

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
         * @brief Destruct the semaphore attributes object instance.
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
         * @brief Semaphore max count value.
         */
        count_t sm_max_value = 1;

        /**
         * @brief Semaphore initial count value.
         */
        count_t sm_initial_value = 0;

        // Add more attributes here.

        /**
         * @}
         */

      }; /* class attributes */

      // ======================================================================
      /**
       * @brief Binary semaphore attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-semaphore
       */
      class attributes_binary : public attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a binary semaphore attributes object instance.
         * @param [in] initial_value Initial count value.
         */
        constexpr
        attributes_binary (count_t initial_value);

        // The rule of five.
        attributes_binary (const attributes_binary&) = default;
        attributes_binary (attributes_binary&&) = default;
        attributes_binary&
        operator= (const attributes_binary&) = default;
        attributes_binary&
        operator= (attributes_binary&&) = default;

        /**
         * @brief Destruct the semaphore attributes object instance.
         */
        ~attributes_binary () = default;

        /**
         * @}
         */

      }; /* class attributes_binary */

      /**
       * @brief Default binary semaphore initialiser.
       * @ingroup cmsis-plus-rtos-semaphore
       */
      static const attributes_binary initializer_binary;

      // ======================================================================

      /**
       * @brief Counting semaphore attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-semaphore
       */
      class attributes_counting : public attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a counting semaphore attributes object instance.
         * @param [in] max_value Maximum count value.
         * @param [in] initial_value Initial count value.
         */
        constexpr
        attributes_counting (count_t max_value, count_t initial_value);

        // The rule of five.
        attributes_counting (const attributes_counting&) = default;
        attributes_counting (attributes_counting&&) = default;
        attributes_counting&
        operator= (const attributes_counting&) = default;
        attributes_counting&
        operator= (attributes_counting&&) = default;

        /**
         * @brief Destruct the semaphore attributes object instance.
         */
        ~attributes_counting () = default;

        /**
         * @}
         */

      }; /* class attributes_counting */

      // ======================================================================
      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a semaphore object instance.
       * @param [in] attr Reference to attributes.
       */
      semaphore (const attributes& attr = initializer_binary);

      /**
       * @brief Construct a named semaphore object instance.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       */
      semaphore (const char* name, const attributes& attr = initializer_binary);

    protected:

      /**
       * @cond ignore
       */

      semaphore (const char* name, const count_t max_value,
                 const count_t initial_value, const attributes& attr =
                     initializer_binary);

      /**
       * @endcond
       */

    public:

      /**
       * @cond ignore
       */

      // The rule of five.
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
       * @brief Destruct the semaphore object instance.
       */
      ~semaphore ();

      /**
       * @}
       */

      /**
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
       *  None.
       * @retval result::ok The semaphore was posted.
       * @retval EAGAIN The maximum count value was exceeded.
       * @retval ENOTRECOVERABLE The semaphore could not be posted
       *  (extension to POSIX).
       */
      result_t
      post (void);

      /**
       * @brief Lock the semaphore, possibly waiting.
       * @par Parameters
       *  None.
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
       *  None.
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
       * @brief Get the semaphore count value.
       * @par Parameters
       *  None.
       * @return The semaphore count value.
       */
      count_t
      value (void) const;

      /**
       * @brief Reset the semaphore.
       * @par Parameters
       *  None.
       * @retval result::ok The semaphore was reset.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       */
      result_t
      reset (void);

      /**
       * @brief Get the semaphore initial count value.
       * @par Parameters
       *  None.
       * @return The numeric value set from attributes.
       */
      count_t
      initial_value (void) const;

      /**
       * @brief Get the semaphore maximum count value.
       * @par Parameters
       *  None.
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
       *  None.
       */
      void
      internal_init_ (void);

      bool
      internal_try_wait_ (void);

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

#if !defined(OS_USE_RTOS_PORT_SEMAPHORE)
      internal::waiting_threads_list list_;
      clock* clock_ = nullptr;
#endif

#if defined(OS_USE_RTOS_PORT_SEMAPHORE)
      friend class port::semaphore;
      os_semaphore_port_data_t port_;
#endif

      // Constant set during construction.
      const count_t max_value_ = max_count_value;

      const count_t initial_value_ = 0;

      // Can be updated in different contexts (interrupts or threads)
      volatile count_t count_ = 0;

      // Add more internal data.

      /**
       * @endcond
       */

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief POSIX compliant **binary semaphore**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-semaphore
     */
    class semaphore_binary : public semaphore
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a binary semaphore object instance.
       * @param [in] initial_value Initial count value; 0 if missing.
       */
      semaphore_binary (const count_t initial_value);

      /**
       * @brief Construct a named binary semaphore object instance.
       * @param [in] name Pointer to name.
       * @param [in] initial_value Initial count value; 0 if missing.
       */
      semaphore_binary (const char* name, const count_t initial_value);

      /**
       * @cond ignore
       */

      // The rule of five.
      semaphore_binary (const semaphore_binary&) = delete;
      semaphore_binary (semaphore_binary&&) = delete;
      semaphore_binary&
      operator= (const semaphore_binary&) = delete;
      semaphore_binary&
      operator= (semaphore_binary&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the semaphore object instance.
       */
      ~semaphore_binary ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare semaphores.
       * @retval true The given semaphore is the same as this semaphore.
       * @retval false The semaphores are different.
       */
      bool
      operator== (const semaphore_binary& rhs) const;

      /**
       * @}
       */

    };

    // ========================================================================

    /**
     * @brief POSIX compliant **counting semaphore**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-semaphore
     */
    class semaphore_counting : public semaphore
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a binary semaphore object instance.
       * @param [in] max_value Maximum count value.
       * @param [in] initial_value Initial count value; 0 if missing.
       */
      semaphore_counting (const count_t max_value, const count_t initial_value);

      /**
       * @brief Construct a named binary semaphore object instance.
       * @param [in] name Pointer to name.
       * @param [in] max_value Maximum count value.
       * @param [in] initial_value Initial count value; 0 if missing.
       */
      semaphore_counting (const char* name, const count_t max_value,
                          const count_t initial_value);

      /**
       * @cond ignore
       */

      // The rule of five.
      semaphore_counting (const semaphore_counting&) = delete;
      semaphore_counting (semaphore_counting&&) = delete;
      semaphore_counting&
      operator= (const semaphore_counting&) = delete;
      semaphore_counting&
      operator= (semaphore_counting&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the semaphore object instance.
       */
      ~semaphore_counting ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare semaphores.
       * @retval true The given semaphore is the same as this semaphore.
       * @retval false The semaphores are different.
       */
      bool
      operator== (const semaphore_counting& rhs) const;

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

    /**
     * @cond ignore
     */

    constexpr
    semaphore::attributes::attributes (count_t max_value, count_t initial_value) :
        sm_max_value (max_value), //
        sm_initial_value (initial_value)
    {
      ;
    }

    /**
     * @endcond
     */

    // ========================================================================
    constexpr
    semaphore::attributes_binary::attributes_binary (count_t initial_value) :
        attributes
          { 1, initial_value } // Use the protected constructor.
    {
      ;
    }

    // ========================================================================

    constexpr
    semaphore::attributes_counting::attributes_counting (count_t max_value,
                                                         count_t initial_value) :
        attributes
          { max_value, initial_value } // Use the protected constructor.
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a generic semaphore object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the semaphore attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * semaphore object shall become initialised.
     *
     * Only the semaphore object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * semaphore objects.
     *
     * In cases where default semaphore attributes are
     * appropriate, the variable `semaphore::initializer_binary`
     * can be used to
     * initialise semaphores.
     * The effect shall be equivalent to creating a semaphore
     * object with the default constructor.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    semaphore::semaphore (const attributes& attr) :
        semaphore
          { nullptr, attr }
    {
      ;
    }

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
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline semaphore::count_t
    semaphore::initial_value (void) const
    {
      return initial_value_;
    }

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    inline semaphore::count_t
    semaphore::max_value (void) const
    {
      return max_value_;
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a binary semaphore object
     * with the given _initial_value_.
     * Upon successful initialisation, the state of the
     * semaphore object shall become initialised.
     *
     * Only the semaphore object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * semaphore objects.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    semaphore_binary::semaphore_binary (const count_t initial_value) :
        semaphore
          { nullptr, 1, initial_value, initializer_binary }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a named binary semaphore object
     * with the given _initial_value_.
     * Upon successful initialisation, the state of the
     * semaphore object shall become initialised.
     *
     * Only the semaphore object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * semaphore objects.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    semaphore_binary::semaphore_binary (const char* name,
                                        const count_t initial_value) :
        semaphore
          { name, 1, initial_value }
    {
      ;
    }

    /**
     * @details
     * This destructor shall destroy the semaphore object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It is safe to destroy an initialised semaphore upon which
     * no threads are currently blocked. The effect of destroying
     * a semaphore upon which other threads are currently blocked
     * is undefined.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_destroy()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    semaphore_binary::~semaphore_binary ()
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * This constructor shall initialise a counting semaphore object
     * with the given _max_value_ and _initial_value_.
     * Upon successful initialisation, the state of the
     * semaphore object shall become initialised.
     *
     * Only the semaphore object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * semaphore objects.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    semaphore_counting::semaphore_counting (const count_t max_value,
                                            const count_t initial_value) :
        semaphore
          { nullptr, max_value, initial_value }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a named counting semaphore object
     * with the given _max_value_ and _initial_value_.
     * Upon successful initialisation, the state of the
     * semaphore object shall become initialised.
     *
     * Only the semaphore object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * semaphore objects.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    semaphore_counting::semaphore_counting (const char* name,
                                            const count_t max_value,
                                            const count_t initial_value) :
        semaphore
          { name, max_value, initial_value }
    {
      ;
    }

    /**
     * @details
     * This destructor shall destroy the semaphore object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It is safe to destroy an initialised semaphore upon which
     * no threads are currently blocked. The effect of destroying
     * a semaphore upon which other threads are currently blocked
     * is undefined.
     *
     * @par POSIX compatibility
     *  Inspired by [`sem_destroy()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html)
     *  from [`<semaphore.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/semaphore.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline
    semaphore_counting::~semaphore_counting ()
    {
      ;
    }

  // ========================================================================

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_SEMAPHORE_H_ */
