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
     * @ingroup cmsis-plus-rtos
     */
    class mutex : public named_object
    {
    public:

      /**
       * @brief Type of mutex protocol.
       */
      using protocol_t = enum class protocol : uint8_t
        {
          /**
           * @brief Priority and scheduling not affected by mutex ownership.
           */
          none = 0,

          /**
           * @brief Inherit from highest priority thread.
           * @details
           * TODO: add
           */
          inherit = 1,

          /**
           * @brief Protect.
           * @details
           * TODO: add
           */
          protect = 2
        };

      /**
       * @brief Type of mutex robustness.
       */
      using robustness_t = enum class robustness : uint8_t
        {
          /**
           * @brief Normal robustness.
           */
          stalled = 0,
          /**
           * @brief Enhanced robustness.
           */
          robust = 1
        };

      /**
       * @brief Type of mutex behaviour.
       */
      using type_t = enum class type : uint8_t
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

          _default = normal,
        };

      /**
       * @brief Type of mutex recursion counter.
       */
      using count_t = uint16_t;

      static constexpr count_t max_count = 0xFFFF;

      // ======================================================================

      /**
       * @brief %Mutex attributes.
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
         * @brief Create mutex attributes.
         * @param [in] name Null terminated name. If `nullptr`, "-" is assigned.
         */
        constexpr
        attributes (const char* name);

        /**
         * @cond ignore
         */
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
         * @brief Destroy a mutex attributes.
         */
        ~attributes () = default;

        /**
         * @}
         */

      protected:

        constexpr
        attributes (const char* name, type_t type);

      public:

        /**
         * @name Public Member Variables
         * @{
         */

        // Public members, no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        /**
         * @brief Mutex priority ceiling.
         */
        thread::priority_t mx_priority_ceiling = thread::priority::highest;

        /**
         * @brief Mutex protocol attribute.
         */
        protocol_t mx_protocol = protocol::none;

        /**
         * @brief Mutex protocol attribute.
         */
        robustness_t mx_robustness = robustness::stalled;

        /**
         * @brief Mutex type attribute.
         */
        type_t mx_type = type::_default;

        /**
         * @brief Mutex maximum recursive count.
         */
        count_t mx_max_count = max_count;

        //
        // TODO: add clock ID.
        //
        // Add more attributes.

        /**
         * @}
         */
      }; /* class attributes */

      /**
       * @brief Default normal mutex initialiser.
       */
      static const attributes normal_initializer;

      // ======================================================================

      /**
       * @brief Recursive mutex attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class recursive_attributes : public attributes
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Create recursive mutex attributes.
         * @param [in] name Null terminated name. If `nullptr`, "-" is assigned.
         */
        constexpr
        recursive_attributes (const char* name);

        /**
         * @cond ignore
         */
        recursive_attributes (const recursive_attributes&) = default;
        recursive_attributes (recursive_attributes&&) = default;
        recursive_attributes&
        operator= (const recursive_attributes&) = default;
        recursive_attributes&
        operator= (recursive_attributes&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destroy a recursive mutex attributes.
         */
        ~recursive_attributes () = default;

        /**
         * @}
         */
      }; /* class recursive_attributes */

      /**
       * @brief Default recursive mutex initialiser.
       */
      static const recursive_attributes recursive_initializer;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a mutex with default settings.
       * @par Parameters
       *  None
       */
      mutex ();

      /**
       * @brief Create a named mutex with default settings.
       * @param [in] name Pointer to name.
       */
      mutex (const char* name);

      /**
       * @brief Create a mutex with custom settings.
       * @param [in] attr Reference to attributes.
       */
      mutex (const attributes& attr);

      /**
       * @brief Create a named mutex with custom settings.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       */
      mutex (const char* name, const attributes& attr);

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
       * @brief Destroy the mutex.
       */
      ~mutex ();

      /**
       * @}
       */

      /*
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
       * @brief Lock the mutex.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The state protected by the mutex is
       *  not recoverable..
       * @retval EAGAIN The mutex could not be acquired because the maximum
       *  number of recursive locks for mutex has been exceeded.
       * @retval EINVAL The mutex was created with the protocol
       *  attribute having the value PTHREAD_PRIO_PROTECT and the
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
       * @brief Try to lock the mutex.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was locked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ENOTRECOVERABLE The state protected by the mutex is
       *  not recoverable..
       * @retval EAGAIN The mutex could not be acquired because the maximum
       *  number of recursive locks for mutex has been exceeded.
       * @retval EINVAL The mutex was created with the protocol
       *  attribute having the value PTHREAD_PRIO_PROTECT and the
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
       * @brief Timed attempt to lock the mutex.
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
       *  the abstime parameter specified a nanoseconds field value
       *  less than zero or greater than or equal to 1000 million.
       * @retval EOWNERDEAD The mutex is a robust mutex and the process
       *  containing the previous owning thread terminated while holding
       *  the mutex lock. The mutex lock shall be acquired by the
       *  calling thread and it is up to the new owner to make the
       *  state consistent.
       */
      result_t
      timed_lock (clock::duration_t timeout);

      /**
       * @brief Unlock the mutex.
       * @par Parameters
       *  None
       * @retval result::ok The mutex was unlocked.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routine;
       *  the mutex type is `mutex::type::errorcheck` or
       *  `mutex::type::recursive`, or the mutex is a robust mutex,
       *  and the current thread does not own the mutex.
       * @retval ENOTRECOVERABLE The mutex was not unlocked.
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

      /**
       * @name Private Member Functions
       * @{
       */

      /**
       * @brief Internal initialisation.
       * @par Parameters
       *  None
       */
      void
      _init (void);

      /**
       * @brief Internal function used to lock the mutex.
       * @par Parameters
       *  None
       * @retval true The mutex was locked.
       * @retval false The mutex was not locked.
       */
      result_t
      _try_lock (thread* crt_thread);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      // Can be updated in different thread contexts.
      thread* volatile owner_ = nullptr;

#if !defined(OS_INCLUDE_RTOS_PORT_MUTEX)
      waiting_threads_list list_;
      clock* clock_ = nullptr;
#endif

#if defined(OS_INCLUDE_RTOS_PORT_MUTEX)
      friend class port::Mutex;
      os_mutex_port_data_t port_;
#endif

      // Can be updated in different thread contexts.
      volatile count_t count_ = 0;

      // Can be updated in different thread contexts.
      volatile thread::priority_t prio_ceiling_ = thread::priority::highest;
      volatile thread::priority_t owner_prio_ = 0;

      // Constants set during construction.
      const type_t type_; // normal, errorcheck, recursive
      const protocol_t protocol_; // none, inherit, protect
      const robustness_t robustness_; // stalled, robust
      const count_t max_count_;

      // Add more internal data.

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
    mutex::attributes::attributes (const char* name) :
        clocked_attributes
          { name }
    {
      ;
    }

    constexpr
    mutex::attributes::attributes (const char* name, type_t type) :
        clocked_attributes
          { name }, //
        mx_type (type)
    {
      ;
    }

    // ========================================================================

    constexpr
    mutex::recursive_attributes::recursive_attributes (const char* name) :
        attributes
          { name, type::recursive } // Use the protected constructor.
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

    inline thread*
    mutex::owner (void)
    {
      return owner_;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MUTEX_H_ */
