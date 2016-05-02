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
    namespace mutex
    {

      // ======================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief %Mutex attributes.
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
         * @brief Create mutex attributes.
         * @param [in] name Null terminated name. If `nullptr`, "-" is assigned.
         */
        constexpr
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
         * @brief Destroy a mutex attributes.
         */
        ~Attributes () = default;

        /**
         * @}
         */

      protected:

        constexpr
        Attributes (const char* name, mutex::type_t type);

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
        thread::priority_t mx_priority_ceiling;

        /**
         * @brief Mutex protocol attribute.
         */
        mutex::protocol_t mx_protocol;

        /**
         * @brief Mutex protocol attribute.
         */
        mutex::robustness_t mx_robustness;

        /**
         * @brief Mutex type attribute.
         */
        mutex::type_t mx_type;

        /**
         * @brief Mutex maximum recursive count.
         */
        mutex::count_t mx_max_count;

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
       * @brief Default normal mutex initialiser.
       */
      extern const Attributes normal_initializer;

      // ======================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

      /**
       * @brief Recursive mutex attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       */
      class Recursive_attributes : public Attributes
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
        Recursive_attributes (const char* name);

        /**
         * @cond ignore
         */
        Recursive_attributes (const Recursive_attributes&) = default;
        Recursive_attributes (Recursive_attributes&&) = default;
        Recursive_attributes&
        operator= (const Recursive_attributes&) = default;
        Recursive_attributes&
        operator= (Recursive_attributes&&) = default;
        /**
         * @endcond
         */

        /**
         * @brief Destroy a recursive mutex attributes.
         */
        ~Recursive_attributes () = default;

        /**
         * @}
         */
      };

#pragma GCC diagnostic pop

      /**
       * @brief Default recursive mutex initialiser.
       */
      extern const Recursive_attributes recursive_initializer;

    } /* namespace mutex */

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief POSIX compliant **mutex**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos
     */
    class Mutex : public Named_object
    {
    public:

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Create a mutex with default settings.
       * @par Parameters
       *  None
       */
      Mutex ();
      /**
       * @brief Create a mutex with custom settings.
       * @param [in] attr Reference to attributes.
       */
      Mutex (const mutex::Attributes& attr);

      /**
       * @cond ignore
       */
      Mutex (const Mutex&) = delete;
      Mutex (Mutex&&) = delete;
      Mutex&
      operator= (const Mutex&) = delete;
      Mutex&
      operator= (Mutex&&) = delete;
      /**
       * @endcond
       */

      /**
       * @brief Destroy the mutex.
       */
      ~Mutex ();

      /**
       * @}
       * @name Operators
       * @{
       */

      /**
       * @brief Compare mutexes.
       * @retval true The given mutex is the same as this mutex.
       * @retval false The mutexes are different.
       */
      bool
      operator== (const Mutex& rhs) const;

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
      Thread*
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
      _try_lock (Thread* crt_thread);

      /**
       * @}
       */

    protected:

      /**
       * @name Private Member Variables
       * @{
       */

      // Can be updated in different thread contexts.
      Thread* volatile owner_;

#if !defined(OS_INCLUDE_RTOS_PORT_MUTEX)
      Waiting_threads_list list_;
      Clock& clock_;
#endif

#if defined(OS_INCLUDE_RTOS_PORT_MUTEX)
      friend class port::Mutex;
      os_mutex_port_data_t port_;
#endif

      // Can be updated in different thread contexts.
      volatile mutex::count_t count_;

      // Can be updated in different thread contexts.
      volatile thread::priority_t prio_ceiling_;
      volatile thread::priority_t owner_prio_;

      // Constants set during construction.
      const mutex::type_t type_; // normal, errorcheck, recursive
      const mutex::protocol_t protocol_; // none, inherit, protect
      const mutex::robustness_t robustness_; // stalled, robust
      const mutex::count_t max_count_;

      // Add more internal data.

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

  // ========================================================================

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    namespace mutex
    {

      // ======================================================================

      constexpr
      Attributes::Attributes (const char* name) :
          Clocked_attribute
            { name }, //
          mx_priority_ceiling (thread::priority::highest), //
          mx_protocol (protocol::none), //
          mx_robustness (robustness::stalled), //
          mx_type (type::_default), //
          mx_max_count (max_count)
      {
        ;
      }

      constexpr
      Attributes::Attributes (const char* name, mutex::type_t type) :
          Clocked_attribute
            { name }, //
          mx_priority_ceiling (thread::priority::highest), //
          mx_protocol (protocol::none), //
          mx_robustness (robustness::stalled), //
          mx_type (type), //
          mx_max_count (max_count)
      {
        ;
      }

      // ======================================================================

      constexpr
      Recursive_attributes::Recursive_attributes (const char* name) :
          Attributes
            { name, type::recursive } // Use the protected constructor.
      {
        ;
      }

    }

    // ========================================================================

    /**
     * @details
     * Identical mutexes should have the same memory address.
     */
    inline bool
    Mutex::operator== (const Mutex& rhs) const
    {
      return this == &rhs;
    }

    inline Thread*
    Mutex::owner (void)
    {
      return owner_;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_MUTEX_H_ */
