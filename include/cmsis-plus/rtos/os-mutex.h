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

#ifndef CMSIS_PLUS_RTOS_OS_MUTEX_H_
#define CMSIS_PLUS_RTOS_OS_MUTEX_H_

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
     * @brief POSIX compliant **mutex**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-mutex
     */
    class mutex : public internal::object_named
    {
    public:

      /**
       * @brief Type of variables holding mutex protocols.
       * @ingroup cmsis-plus-rtos-mutex
       */
      using protocol_t = uint8_t;

      /**
       * @brief Mutex protocols.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-mutex
       */
      struct protocol
      {
        /**
         * @brief Enumeration of mutex protocols.
         */
        enum
          : protocol_t
            {
              /**
               * @brief Priority and scheduling not affected by mutex ownership.
               */
              none = 0,

              /**
               * @brief Inherit priority from highest priority thread.
               */
              inherit = 1,

              /**
               * @brief Execute at the highest priority.
               */
              protect = 2,

              /**
               * @brief Default value. Differs from POSIX, which uses `none`.
               */
              default_ = inherit,

              /**
               * @brief Maximum value, for validation purposes.
               */
              max_ = protect,
        };
      };

      /**
       * @brief Type of variables holding mutex robustness.
       * @ingroup cmsis-plus-rtos-mutex
       */
      using robustness_t = uint8_t;

      /**
       * @brief Mutex robustness.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-mutex
       */
      struct robustness
      {
        /**
         * @brief Enumeration of mutex robustness.
         */
        enum
          : robustness_t
            {
              /**
               * @brief Normal robustness.
               */
              stalled = 0,

              /**
               * @brief Enhanced robustness at thread termination.
               */
              robust = 1,

              /**
               * @brief Default value.
               */
              default_ = stalled,

              /**
               * @brief Maximum value, for validation purposes.
               */
              max_ = robust,
        };
      };

      /**
       * @brief Type of variables holding mutex behaviours.
       * @ingroup cmsis-plus-rtos-mutex
       */
      using type_t = uint8_t;

      /**
       * @brief Mutex types.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-mutex
       */
      struct type
      {
        /**
         * @brief Mutex types.
         */
        enum
          : type_t
            {
              /**
               * @brief Normal mutex behaviour.
               */
              normal = 0,
              /**
               * @brief Check mutex behaviour.
               */
              errorcheck = 1,
              /**
               * @brief Recursive mutex behaviour.
               */
              recursive = 2,

              /**
               * @brief Default value.
               */
              default_ = normal,

              /**
               * @brief Maximum value, for validation purposes.
               */
              max_ = recursive,
        };
      };

      /**
       * @brief Type of variables holding mutex recursion counters.
       * @ingroup cmsis-plus-rtos-mutex
       */
      using count_t = uint16_t;

      /**
       * @brief Constant with the maximum value for the recursion counter.
       * @ingroup cmsis-plus-rtos-mutex
       */
      static constexpr count_t max_count = 0xFFFF;

      // ======================================================================

      /**
       * @brief %Mutex attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-mutex
       */
      class attributes : public internal::attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a mutex attributes object instance.
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
        attributes (type_t type);

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
         * @brief Destruct the mutex attributes object instance.
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
         * @brief Attribute with the mutex priority ceiling.
         */
        thread::priority_t mx_priority_ceiling = thread::priority::highest;

        /**
         * @brief Attribute with the mutex protocol.
         */
        protocol_t mx_protocol = protocol::default_;

        /**
         * @brief Attribute with the mutex robustness.
         */
        robustness_t mx_robustness = robustness::stalled;

        /**
         * @brief Attribute with the mutex type.
         */
        type_t mx_type = type::default_;

        /**
         * @brief Attribute with the mutex maximum recursive count.
         */
        count_t mx_max_count = max_count;

        // Add more attributes here.

        /**
         * @}
         */

      }; /* class attributes */

      /**
       * @brief Default normal mutex initialiser.
       * @ingroup cmsis-plus-rtos-mutex
       */
      static const attributes initializer_normal;

      // ======================================================================

      /**
       * @brief Recursive mutex attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-mutex
       */
      class attributes_recursive : public attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct a recursive mutex attributes object instance.
         * @par Parameters
         *  None
         */
        constexpr
        attributes_recursive ();

        /**
         * @cond ignore
         */

        attributes_recursive (const attributes_recursive&) = default;
        attributes_recursive (attributes_recursive&&) = default;
        attributes_recursive&
        operator= (const attributes_recursive&) = default;
        attributes_recursive&
        operator= (attributes_recursive&&) = default;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the recursive mutex attributes object instance.
         */
        ~attributes_recursive () = default;

        /**
         * @}
         */

      }; /* class attributes_recursive */

      /**
       * @brief Default recursive mutex initialiser.
       * @ingroup cmsis-plus-rtos-mutex
       */
      static const attributes_recursive initializer_recursive;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a mutex object instance.
       * @param [in] attr Reference to attributes.
       */
      mutex (const attributes& attr = initializer_normal);

      /**
       * @brief Construct a named mutex object instance.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       */
      mutex (const char* name, const attributes& attr = initializer_normal);

      /**
       * @cond ignore
       */

      mutex (const mutex&) = delete;
      mutex (mutex&&) = delete;
      mutex&
      operator= (const mutex&) = delete;
      mutex&
      operator= (mutex&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the mutex object instance.
       */
      ~mutex ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare mutexes.
       * @retval true The given mutex is the same as this mutex.
       * @retval false The mutexes are different.
       */
      bool
      operator== (const mutex& rhs) const;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Lock/acquire the mutex.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The state protected by the mutex is
       *  not recoverable..
       * @retval EAGAIN The mutex could not be acquired because the maximum
       *  number of recursive locks for mutex has been exceeded.
       * @retval EINVAL The mutex was created with the protocol
       *  attribute having the value `mutex::protocol::protect` and the
       *  calling thread's priority is higher than the mutex's
       *  current priority ceiling.
       * @retval EOWNERDEAD The mutex is a robust mutex and the process
       *  containing the previous owning thread terminated while holding
       *  the mutex lock. The mutex lock shall be acquired by the calling
       *  thread and it is up to the new owner to make the state consistent.
       * @retval EDEADLK The mutex type is `mutex::type::errorcheck` and
       *  the current thread already owns the mutex.
       */
      result_t
      lock (void);

      /**
       * @brief Try to lock/acquire the mutex.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The state protected by the mutex is
       *  not recoverable..
       * @retval EAGAIN The mutex could not be acquired because the maximum
       *  number of recursive locks for mutex has been exceeded.
       * @retval EINVAL The mutex was created with the protocol
       *  attribute having the value `mutex::protocol::protect` and the
       *  calling thread's priority is higher than the mutex's
       *  current priority ceiling.
       * @retval EOWNERDEAD The mutex is a robust mutex and the process
       *  containing the previous owning thread terminated while holding
       *  the mutex lock. The mutex lock shall be acquired by the calling
       *  thread and it is up to the new owner to make the state consistent.
       * @retval EDEADLK The mutex type is `mutex::type::errorcheck` and
       *  the current thread already owns the mutex.
       * @retval EWOULDBLOCK The mutex could not be acquired because it was
       *  already locked.
       */
      result_t
      try_lock (void);

      /**
       * @brief Timed attempt to lock/acquire the mutex.
       * @param [in] timeout Timeout to wait, in clock units (ticks or seconds).
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ETIMEDOUT The mutex could not be locked before the
       *  specified timeout expired.
       * @retval ENOTRECOVERABLE The state protected by the mutex
       *  is not recoverable.
       * @retval EAGAIN The mutex could not be acquired because the
       *  maximum number of recursive locks for mutex has been exceeded.
       * @retval EDEADLK The mutex type is `mutex::type::errorcheck`
       *  and the current thread already owns the mutex.
       * @retval EINVAL The process or thread would have blocked, and
       *  the timeout parameter is invalid.
       * @retval EOWNERDEAD The mutex is a robust mutex and the process
       *  containing the previous owning thread terminated while holding
       *  the mutex lock. The mutex lock shall be acquired by the
       *  calling thread and it is up to the new owner to make the
       *  state consistent.
       */
      result_t
      timed_lock (clock::duration_t timeout);

      /**
       * @brief Unlock/release the mutex.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was unlocked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine;
       *  the mutex type is `mutex::type::errorcheck` or
       *  `mutex::type::recursive`, or the mutex is a robust mutex,
       *  and the current thread does not own the mutex.
       * @retval ENOTRECOVERABLE The recursive mutex whose oner
       *   died was not marked `consistent()` before `unlock()`.
       */
      result_t
      unlock (void);

      /**
       * @brief Get the priority ceiling of a mutex.
       * @par Parameters
       *  None
       * @return The priority ceiling.
       */
      thread::priority_t
      prio_ceiling (void) const;

      /**
       * @brief Change the priority ceiling of a mutex.
       * @param [in] prio_ceiling new priority.
       * @param [out] old_prio_ceiling pointer to location where to
       *  store the previous priority; may be `nullptr`.
       * @retval result::ok The priority was changed.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The state protected by the mutex is
       *  not recoverable..
       * @retval EAGAIN The mutex could not be acquired because the maximum
       *  number of recursive locks for mutex has been exceeded.
       * @retval EINVAL The mutex was created with the protocol
       *  attribute having the value `mutex::protocol::protect` and the
       *  calling thread's priority is higher than the mutex's
       *  current priority ceiling.
       * @retval EOWNERDEAD The mutex is a robust mutex and the process
       *  containing the previous owning thread terminated while holding
       *  the mutex lock. The mutex lock shall be acquired by the calling
       *  thread and it is up to the new owner to make the state consistent.
       * @retval EDEADLK The mutex type is `mutex::type::errorcheck` and
       *  the current thread already owns the mutex.
       */
      result_t
      prio_ceiling (thread::priority_t prio_ceiling,
                    thread::priority_t* old_prio_ceiling = nullptr);

      /**
       * @brief Mark mutex as consistent.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was marked as consistent.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The mutex object referenced by mutex is not robust
       *  or does not protect an inconsistent state.
       */
      result_t
      consistent (void);

      /**
       * @brief Get the thread that owns the mutex.
       * @par Parameters
       *  None
       * @return Pointer to thread or `nullptr` if not owned.
       */
      thread*
      owner (void);

      /**
       * @brief Get the mutex type.
       * @return An integer encoding the @ref mutex::type.
       */
      type_t
      type (void);

      /**
       * @brief Get the mutex protocol.
       * @return An integer encoding the @ref mutex::protocol.
       */
      protocol_t
      protocol (void);

      /**
       * @brief Get the mutex robustness.
       * @return An integer encoding the @ref mutex::robustness.
       */
      robustness_t
      robustness (void);

      /**
       * @brief Reset the mutex.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was reset.
       */
      result_t
      reset (void);

      /**
       * @}
       */

    protected:

      friend class thread;

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
      internal_init_ (void);

      /**
       * @brief Internal function used to lock the mutex.
       * @par Parameters
       *  None
       * @retval true The mutex was locked.
       * @retval false The mutex was not locked.
       */
      result_t
      internal_try_lock_ (thread* crt_thread);

      void
      internal_mark_owner_dead_ (void);

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

      // Can be updated in different thread contexts.
      thread* volatile owner_ = nullptr;

#if !defined(OS_USE_RTOS_PORT_MUTEX)
      internal::waiting_threads_list list_;
      clock* clock_ = nullptr;
#endif

    public:

      // Intrusive node used to link this mutex to the owning thread.
      // This is used for priority inheritance and robustness.
      internal::double_list_links owner_links_;

    protected:

#if defined(OS_USE_RTOS_PORT_MUTEX)
      friend class port::mutex;
      os_mutex_port_data_t port_;
#endif

      // Can be updated in different thread contexts.
      volatile count_t count_ = 0;

      // Can be updated in different thread contexts.
      volatile thread::priority_t initial_prio_ceiling_ =
          thread::priority::highest;
      volatile thread::priority_t prio_ceiling_ = thread::priority::highest;
      volatile thread::priority_t boosted_prio_ = thread::priority::none;

      bool owner_dead_ = false;
      bool consistent_ = true;
      bool recoverable_ = true;

      // Constants set during construction.
      const type_t type_; // normal, errorcheck, recursive
      const protocol_t protocol_; // none, inherit, protect
      const robustness_t robustness_; // stalled, robust
      const count_t max_count_;

      // Add more internal data.

      /**
       * @endcond
       */

      /**
       * @}
       */

    };

    /**
     * @brief POSIX compliant **recursive mutex**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-mutex
     */
    class mutex_recursive : public mutex
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct a recursive mutex object instance.
       */
      mutex_recursive (const attributes& attr = initializer_recursive);

      /**
       * @brief Construct a named recursive mutex object instance.
       */
      mutex_recursive (const char* name, const attributes& attr =
                           initializer_recursive);

      /**
       * @cond ignore
       */

      mutex_recursive (const mutex_recursive&) = delete;
      mutex_recursive (mutex_recursive&&) = delete;
      mutex_recursive&
      operator= (const mutex_recursive&) = delete;
      mutex_recursive&
      operator= (mutex_recursive&&) = delete;

      /**
       * @endcond
       */

      /**
       * @brief Destruct the recursive mutex object instance.
       */
      ~mutex_recursive ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare mutexes.
       * @retval true The given mutex is the same as this mutex.
       * @retval false The mutexes are different.
       */
      bool
      operator== (const mutex_recursive& rhs) const;

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
    mutex::attributes::attributes ()
    {
      ;
    }

    constexpr
    mutex::attributes::attributes (type_t type) :
        mx_type (type)
    {
      ;
    }

    // ========================================================================

    constexpr
    mutex::attributes_recursive::attributes_recursive () :
        attributes
          { type::recursive } // Use the protected constructor.
    {
      ;
    }

    // ========================================================================

    /**
     * @details
     * Identical mutexes should have the same memory address.
     */
    inline bool
    mutex::operator== (const mutex& rhs) const
    {
      return this == &rhs;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline thread*
    mutex::owner (void)
    {
      return owner_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline mutex::type_t
    mutex::type (void)
    {
      return type_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline mutex::protocol_t
    mutex::protocol (void)
    {
      return protocol_;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    inline mutex::robustness_t
    mutex::robustness (void)
    {
      return robustness_;
    }

    // ========================================================================

    inline
    mutex_recursive::mutex_recursive (const attributes& attr) :
        mutex
          { attr }
    {
      ;
    }

    inline
    mutex_recursive::mutex_recursive (const char* name, const attributes& attr) :
        mutex
          { name, attr }
    {
      ;
    }

    inline
    mutex_recursive::~mutex_recursive ()
    {
      ;
    }

    /**
     * @details
     * Identical mutexes should have the same memory address.
     */
    inline bool
    mutex_recursive::operator== (const mutex_recursive& rhs) const
    {
      return this == &rhs;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MUTEX_H_ */
