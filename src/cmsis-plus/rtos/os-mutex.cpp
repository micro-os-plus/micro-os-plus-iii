/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cmsis-plus/rtos/port/os-decls.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// ----------------------------------------------------------------------------

#include <cassert>
#include <cerrno>
#include <cstdlib>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
// TODO: remove it when fully implemented
//#pragma GCC diagnostic ignored "-Wunused-parameter"

    namespace mutex
    {
      const Attributes normal_initializer
        { nullptr };

      const Recursive_attributes recursive_initializer
        { nullptr };
    } /* namespace mutex */

    // ------------------------------------------------------------------------

    /**
     * @details
     * Initialise the mutex with default attributes.
     *
     * pthread_mutex_init()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Mutex::Mutex () :
        Mutex
          { mutex::normal_initializer }
    {
      ;
    }

    /**
     * @details
     * Initialize the mutex with attributes specified by _attr_.
     * Upon successful initialization, the state of the mutex becomes
     * initialized and unlocked.
     *
     * pthread_mutex_init()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Mutex::Mutex (const mutex::Attributes& attr) :
        Named_object
          { attr.name () }, //
        type_ (attr.mx_type), //
        protocol_ (attr.mx_protocol), //
        robustness_ (attr.mx_robustness) //
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      prio_ceiling_ = attr.mx_priority_ceiling;
      owner_ = nullptr;
      count_ = 0;

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      port::Mutex::create (this);

#else

      // TODO

#endif
    }

    /**
     * @details
     * Destroy the mutex object.
     * It shall be safe to destroy an initialized mutex that is
     * unlocked. Attempting to destroy a locked mutex or a mutex
     * that is referenced (for example, while being used in a
     * pthread_cond_timedwait() or pthread_cond_wait()) by another
     * thread results in undefined behavior.

     *
     * pthread_mutex_destroy()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Mutex::~Mutex ()
    {
      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      port::Mutex::destroy (this);

#else

      // TODO

#endif
    }

    /**
     * @details
     * If the mutex is free, lock it. If the mutex is already
     * locked by another thread, the calling thread shall block
     * until the mutex becomes available. This operation shall
     * return with the mutex object referenced by mutex in the
     * locked state with the calling thread as its owner. If a
     * thread attempts to relock a mutex that it has already
     * locked, Mutex::lock() shall behave as described in the
     * **Relock** column of the following table. If a thread
     * attempts to unlock a mutex that it has not locked or a
     * mutex which is unlocked, Mutex::unlock() shall behave as
     * described in the **Unlock When Not Owner** column of the
     * following table.
     *
     * TODO: add table
     *
     * Where the table indicates recursive behavior, the mutex
     * shall maintain the concept of a lock count. When a thread
     * successfully acquires a mutex for the first time, the
     * lock count shall be set to one. Every time a thread
     * relocks this mutex, the lock count shall be incremented
     * by one. Each time the thread unlocks the mutex, the
     * lock count shall be decremented by one. When the lock
     * count reaches zero, the mutex shall become available
     * for other threads to acquire.
     *
     * pthread_mutex_lock()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Mutex::lock (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::lock (this);

#else

      // TODO
      return result::ok;

#endif
    }

    /**
     * @details
     * Try to lock the mutex as Mutex::lock(), except that if the
     * mutex object referenced by mutex is currently locked (by
     * any thread, including the current thread), the call shall
     * return immediately. If the mutex type is PTHREAD_MUTEX_RECURSIVE
     * and the mutex is currently owned by the calling thread,
     * the mutex lock count shall be incremented by one and the
     * Mutex::trylock() function shall immediately return success.
     *
     * If _mutex_ is a robust mutex and the process containing
     * the owning thread terminated while holding the mutex lock,
     * a call to Mutex::lock() shall return the error value
     * [EOWNERDEAD]. If _mutex_ is a robust mutex and the owning
     * thread terminated while holding the mutex lock, a call
     * to Mutex::lock() may return the error value [EOWNERDEAD]
     * even if the process in which the owning thread resides
     * has not terminated. In these cases, the mutex is locked
     * by the thread but the state it protects is marked as
     * inconsistent. The application should ensure that the
     * state is made consistent for reuse and when that is
     * complete call Mutex::consistent(). If the application
     * is unable to recover the state, it should unlock the
     * mutex without a prior call to Mutex::consistent(), after
     * which the mutex is marked permanently unusable.
     *
     * pthread_mutex_trylock()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Mutex::try_lock (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::try_lock (this);

#else

      // TODO
      return result::ok;

#endif
    }

    /**
     * @details
     * Try to lock the mutex object referenced by mutex. If the mutex
     * is already locked, the calling thread shall block until the
     * mutex becomes available as in the Mutex::lock() function. If the
     * mutex cannot be locked without waiting for another thread to
     * unlock the mutex, this wait shall be terminated when the specified
     * timeout expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based (the SysTick clock for CMSIS).
     *
     * Under no circumstance shall the function fail with a timeout
     * if the mutex can be locked immediately.
     *
     * As a consequence of the priority inheritance rules (for
     * mutexes initialized with the PRIO_INHERIT protocol),
     * if a timed mutex wait is terminated because its timeout
     * expires, the priority of the owner of the mutex shall be
     * adjusted as necessary to reflect the fact that this thread
     * is no longer among the threads waiting for the mutex.
     *
     * pthread_mutex_timedlock()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_timedlock.html
     *
     * Differences from the standard:
     * - the timeout is not expressed as an absolute time point, but
     * as a relative number of system ticks.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Mutex::timed_lock (systicks_t ticks)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks) @%p \n", __func__, ticks, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::timed_lock (this, ticks);

#else

      // TODO
      return result::ok;

#endif
    }

    /**
     * @details
     * Release the mutex object referenced by _mutex_. The manner
     * in which a mutex is released is dependent upon the mutex's
     * type attribute. If there are threads blocked on the mutex
     * object referenced by mutex when Mutex::unlock() is called,
     * resulting in the mutex becoming available, the scheduling
     * policy shall determine which thread shall acquire the mutex.
     *
     * (In the case of PTHREAD_MUTEX_RECURSIVE mutexes, the mutex
     * shall become available when the count reaches zero and the
     * calling thread no longer has any locks on this mutex.)
     *
     * pthread_mutex_unlock()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Mutex::unlock (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::unlock (this);

#else

      // TODO
      return result::ok;

#endif
    }

    /**
     * @details
     * Return the current priority ceiling of the mutex.
     *
     * pthread_mutex_getprioceiling()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_getprioceiling.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    thread::priority_t
    Mutex::prio_ceiling (void) const
    {
      assert(!scheduler::in_handler_mode ());

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::prio_ceiling (this);

#else

      // TODO
      return prio_ceiling_;

#endif
    }

    /**
     * @details
     * Attempt to lock the mutex as if by a call to Mutex::lock(),
     * except that the process of locking the mutex need not adhere
     * to the priority protect protocol. On acquiring the mutex
     * it shall change the mutex's priority ceiling and then
     * release the mutex as if by a call to Mutex::unlock().
     * When the change is successful, the previous value of
     * the priority ceiling shall be returned in old_ceiling.
     *
     * If Mutex::set_prio_ceiling() function fails, the mutex
     * priority ceiling shall not be changed.
     *
     * pthread_mutex_setprioceiling()
     * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_getprioceiling.html
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Mutex::prio_ceiling (thread::priority_t prio_ceiling,
                         thread::priority_t* old_prio_ceiling)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::prio_ceiling (this, prio_ceiling, old_prio_ceiling);

#else

      thread::priority_t prio;
        {
          // TODO: lock() must not adhere to the priority protocol.
          lock ();
          prio = prio_ceiling_;
          prio_ceiling_ = prio_ceiling;
          unlock ();
        }

      if (old_prio_ceiling != nullptr)
        {
          *old_prio_ceiling = prio;
        }
      return result::ok;

#endif
    }

    /**
     * @details
     * If the robust mutex is in an inconsistent state, the
     * Mutex::consistent() function can be used to mark the
     * state protected by the mutex referenced by mutex as
     * consistent again.
     *
     * If an owner of a robust mutex terminates while holding
     * the mutex, the mutex becomes inconsistent and the next
     * thread that acquires the mutex lock shall be notified
     * of the state by the return value [EOWNERDEAD]. In this
     * case, the mutex does not become normally usable again
     * until the state is marked consistent.
     *
     * If the thread which acquired the mutex lock with the
     * return value [EOWNERDEAD] terminates before calling
     * either Mutex::consistent() or Mutex::unlock(), the
     * next thread that acquires the mutex lock shall be
     * notified about the state of the mutex by the return
     * value [EOWNERDEAD].
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Mutex::consistent (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s() @%p \n", __func__, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::consistent (this);

#else

      // TODO
      return result::ok;

#endif
    }

    result_t
    Mutex::reset (void)
    {
      // TODO wait list?
      count_ = 0;
      return result::ok;
    }

  // ------------------------------------------------------------------------

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
