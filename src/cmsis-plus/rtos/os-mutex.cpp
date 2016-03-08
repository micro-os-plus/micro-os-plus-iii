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

    /**
     * @details
     * The os::rtos::mutex namespace groups mutex types, enumerations,
     * attributes and initialisers.
     */
    namespace mutex
    {
      /**
       * @class Attributes
       * @details
       * Allow to assign a name and custom attributes (like priority ceiling,
       * robustness, etc) to the mutex.
       *
       * To simplify access, the member variables are public and do not
       * require accessors or mutators.
       *
       * @par POSIX compatibility
       *  Inspired by `pthread_mutexattr_t`
       *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      /**
       * @class Recursive_attributes
       * @details
       * Allow to assign a name and custom attributes (like priority ceiling,
       * robustness, etc) to the mutex.
       *
       * @par POSIX compatibility
       *  Inspired by `pthread_mutexattr_t`
       *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      /**
       * @var thread::priority_t Attributes::mx_priority_ceiling
       * @details
       * The @ref mx_priority_ceiling attribute defines the priority
       * ceiling of initialised mutexes, which is the minimum priority
       * level at which the critical section guarded by the mutex is
       * executed. In order to avoid priority inversion, the priority
       * ceiling of the mutex shall be set to a priority higher than
       * or equal to the highest priority of all the threads that may
       * lock that mutex. The values of @ref mx_priority_ceiling are
       * within the maximum range of priorities defined under the
       * SCHED_FIFO scheduling policy.
       *
       * @par POSIX compatibility
       *  Inspired by `pthread_mutexattr_setprioceiling()`
       *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      /**
       * @var mutex::protocol_t Attributes::mx_protocol
       * @details
       * The default value of the attribute shall be PTHREAD_PRIO_NONE.
       *
       * When a thread owns a mutex with the PTHREAD_PRIO_NONE
       * protocol attribute, its priority and scheduling shall
       * not be affected by its mutex ownership.
       *
       * When a thread is blocking higher priority threads
       * because of owning one or more robust mutexes with the
       * PTHREAD_PRIO_INHERIT protocol attribute, it shall execute
       * at the higher of its priority or the priority of the highest
       * priority thread waiting on any of the robust mutexes owned
       * by this thread and initialised with this protocol.
       *
       * When a thread is blocking higher priority threads because
       * of owning one or more non-robust mutexes with the
       * PTHREAD_PRIO_INHERIT protocol attribute, it shall execute
       * at the higher of its priority or the priority of the
       * highest priority thread waiting on any of the non-robust
       * mutexes owned by this thread and initialised with this protocol.
       *
       * When a thread owns one or more robust mutexes initialised
       * with the PTHREAD_PRIO_PROTECT protocol, it shall execute
       * at the higher of its priority or the highest of the priority
       * ceilings of all the robust mutexes owned by this thread and
       * initialised with this attribute, regardless of whether other
       * threads are blocked on any of these robust mutexes or not.
       *
       * When a thread owns one or more non-robust mutexes initialised
       * with the PTHREAD_PRIO_PROTECT protocol, it shall execute at
       * the higher of its priority or the highest of the priority
       * ceilings of all the non-robust mutexes owned by this thread
       * and initialised with this attribute, regardless of whether
       * other threads are blocked on any of these non-robust mutexes
       * or not.
       *
       * While a thread is holding a mutex which has been initialised
       * with the PTHREAD_PRIO_INHERIT or PTHREAD_PRIO_PROTECT protocol
       * attributes, it shall not be subject to being moved to the tail
       * of the scheduling queue at its priority in the event that its
       * original priority is changed, such as by a call to
       * sched_setparam(). Likewise, when a thread unlocks a mutex
       * that has been initialised with the PTHREAD_PRIO_INHERIT or
       * PTHREAD_PRIO_PROTECT protocol attributes, it shall not be
       * subject to being moved to the tail of the scheduling queue
       * at its priority in the event that its original priority is changed.
       *
       * If a thread simultaneously owns several mutexes initialised
       * with different protocols, it shall execute at the highest of
       * the priorities that it would have obtained by each of these
       * protocols.
       *
       * When a thread makes a call to Mutex::lock(), the mutex
       * was initialised with the protocol attribute having the
       * value PTHREAD_PRIO_INHERIT, when the calling thread is
       * blocked because the mutex is owned by another thread, that
       * owner thread shall inherit the priority level of the calling
       * thread as long as it continues to own the mutex. The
       * implementation shall update its execution priority to
       * the maximum of its assigned priority and all its
       * inherited priorities. Furthermore, if this owner thread
       * itself becomes blocked on another mutex with the protocol
       * attribute having the value PTHREAD_PRIO_INHERIT, the same
       * priority inheritance effect shall be propagated to this
       * other owner thread, in a recursive manner.
       *
       * @par POSIX compatibility
       *  Inspired by `pthread_mutexattr_setprotocol()`
       *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      /**
       * @var mutex::robustness_t Attributes::mx_robustness
       * @details
       *
       * Valid values for robust include:
       * - PTHREAD_MUTEX_STALLED
       *
       *   No special actions are taken if the owner of the mutex
       *  is terminated while holding the mutex lock. This can
       *  lead to deadlocks if no other thread can unlock the mutex.
       *  This is the default value.
       *
       * - PTHREAD_MUTEX_ROBUST
       *
       *   If the process containing the owning thread of a robust
       *   mutex terminates while holding the mutex lock, the next
       *   thread that acquires the mutex shall be notified about
       *   the termination by the return value [EOWNERDEAD] from
       *   the locking function. If the owning thread of a robust
       *   mutex terminates while holding the mutex lock, the next
       *   thread that acquires the mutex may be notified about the
       *   termination by the return value [EOWNERDEAD]. The notified
       *   thread can then attempt to mark the state protected by
       *   the mutex as consistent again by a call to
       *   pthread_mutex_consistent(). After a subsequent
       *   successful call to pthread_mutex_unlock(), the mutex
       *   lock shall be released and can be used normally by
       *   other threads. If the mutex is unlocked without a
       *   call to pthread_mutex_consistent(), it shall be in a
       *   permanently unusable state and all attempts to lock
       *   the mutex shall fail with the error [ENOTRECOVERABLE].
       *   The only permissible operation on such a mutex is
       *   pthread_mutex_destroy().
       *
       * @par POSIX compatibility
       *  Inspired by [`pthread_mutexattr_setrobust()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_setrobust.html)
       *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      /**
       * @var mutex::type_t Attributes::mx_type
       * @details
       * The default value of the type attribute is PTHREAD_MUTEX_DEFAULT.
       *
       * The type of mutex is contained in the type attribute of
       * the mutex attributes. Valid mutex types include:
       *
       * - PTHREAD_MUTEX_NORMAL
       * - PTHREAD_MUTEX_ERRORCHECK
       * - PTHREAD_MUTEX_RECURSIVE
       * - PTHREAD_MUTEX_DEFAULT
       *
       * The mutex type affects the behaviour of calls which lock
       * and unlock the mutex. See @ref Mutex::lock() for details.
       * An implementation may map PTHREAD_MUTEX_DEFAULT to one of
       * the other mutex types.
       *
       * @par POSIX compatibility
       *  Inspired by [`pthread_mutexattr_settype()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_settype.html)
       *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
       */

      const Attributes normal_initializer
        { nullptr };

      const Recursive_attributes recursive_initializer
        { nullptr };

    } /* namespace mutex */

    // ------------------------------------------------------------------------

    /**
     * @class Mutex
     * @details
     * A synchronisation object used to allow multiple threads to serialise
     * their access to shared data. The name derives from the capability
     * it provides; namely, **mutual-exclusion**. The thread that has locked
     * a mutex becomes its owner and remains the owner until that same
     * thread unlocks the mutex.
     *
     * @par Tradeoff Between Error Checks and Performance Supported
     *
     * Many error conditions that can occur are not required to be
     * detected by the implementation in order to let implementations
     * trade off performance versus degree of error checking according
     * to the needs of their specific applications and execution
     * environment. As a general rule, conditions caused by the
     * system (such as insufficient memory) are required to be
     * detected, but conditions caused by an erroneously coded
     * application (such as failing to provide adequate synchronisation
     * to prevent a mutex from being deleted while in use) are
     * specified to result in undefined behaviour.
     *
     * A wide range of implementations is thus made possible. For
     * example, an implementation intended for application debugging
     * may implement all of the error checks, but an implementation
     * running a single, provably correct application under very tight
     * performance constraints in an embedded computer might implement
     * minimal checks. An implementation might even be provided in two
     * versions, similar to the options that compilers provide: a
     * full-checking, but slower version; and a limited-checking, but
     * faster version. To forbid this optionality would be a disservice
     * to users.
     *
     * By carefully limiting the use of "undefined behaviour" only to
     * things that an erroneous (badly coded) application might do,
     * and by defining that resource-not-available errors are mandatory,
     * POSIX ensures that a fully-conforming application is portable across
     * the full range of implementations, while not forcing all
     * implementations to add overhead to check for numerous things
     * that a correct program never does. When the behaviour is
     * undefined, no error number is specified to be returned on
     * implementations that do detect the condition. This is because
     * undefined behaviour means anything can happen, which includes
     * returning with any value (which might happen to be a valid, but
     * different, error number). However, since the error number might
     * be useful to application developers when diagnosing problems
     * during application development, a recommendation is made in
     * rationale that implementors should return a particular error
     * number if their implementation does detect the condition.
     *
     * @par Static Initialisers
     *
     * Providing for static initialisation of statically allocated
     * synchronisation objects allows modules with private static
     * synchronisation variables to avoid runtime initialisation
     * tests and overhead. Furthermore, it simplifies the coding of
     * self-initialising modules. Such modules are common in C
     * libraries, where for various reasons the design calls for
     * self-initialisation instead of requiring an explicit module
     * initialisation function to be called. An example use of
     * static initialisation:
     *
     * @code{.cpp}
     * // Create a normal mutex. Same as using the default constructor.
     * Mutex mx { normal_initializer };
     *
     * // Create a recursive mutex.
     * Mutex rmx { recursive_initializer };
     * @endcode
     *
     * @par Example
     *
     * @code{.cpp}
     * // Protected resource.
     * typedef struct {
     *   int count;
     * } res_t;
     *
     * res_t res;
     *
     * // Mutex to protect the resource.
     * Mutex mx;
     *
     * void
     * func(void)
     * {
     *   // Do something
     *
     *   mx.lock();
     *   res.count++;
     *   mx.unlock();
     *
     *   // Do something else.
     * }
     * @endcode
     *
     * @par POSIX compatibility
     *  Inspired by `pthread_mutex_t`
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @details
     * Initialise the mutex with default attributes.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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
     * Initialise the mutex with attributes specified by _attr_.
     * Upon successful initialisation, the state of the mutex becomes
     * initialised and unlocked.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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
     * It shall be safe to destroy an initialised mutex that is
     * unlocked. Attempting to destroy a locked mutex or a mutex
     * that is referenced (for example, while being used in a
     * `timed_wait()` or `wait()`) by another
     * thread results in undefined behaviour.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_destroy()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_destroy.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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
     * locked, `lock()` shall behave as described in the
     * **Relock** column of the following table. If a thread
     * attempts to unlock a mutex that it has not locked or a
     * mutex which is unlocked, `unlock()` shall behave as
     * described in the **Unlock When Not Owner** column of the
     * following table.
     *
     * | %Mutex Type | Robustness | Relock    | Unlock When Not Owner |
     * | -----------| ---------- | --------- | ----------------------|
     * | normal     | non-robust | deadlock  | undefined behaviour   |
     * | normal     | robust     | deadlock  | error                 |
     * | errorcheck | either     | error     | error                 |
     * | recursive  | either     | recursive | error                 |
     * | default    | non-robust | undefined | undefined behaviour   |
     * | default    | robust     | undefined | error                 |
     *
     * Where the table indicates recursive behaviour, the mutex
     * shall maintain the concept of a lock count. When a thread
     * successfully acquires a mutex for the first time, the
     * lock count shall be set to one. Every time a thread
     * relocks this mutex, the lock count shall be incremented
     * by one. Each time the thread unlocks the mutex, the
     * lock count shall be decremented by one. When the lock
     * count reaches zero, the mutex shall become available
     * for other threads to acquire.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_lock()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_lock.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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
     * Try to lock the mutex as `lock()`, except that if the
     * mutex object referenced by mutex is currently locked (by
     * any thread, including the current thread), the call shall
     * return immediately. If the mutex type is `mutex::type::recursive`
     * and the mutex is currently owned by the calling thread,
     * the mutex lock count shall be incremented by one and the
     * `try_lock()` function shall immediately return success.
     *
     * If _mutex_ is a robust mutex and the process containing
     * the owning thread terminated while holding the mutex lock,
     * a call to `lock()` shall return the error value
     * `EOWNERDEAD`. If _mutex_ is a robust mutex and the owning
     * thread terminated while holding the mutex lock, a call
     * to Mutex::lock() may return the error value `EOWNERDEAD`
     * even if the process in which the owning thread resides
     * has not terminated. In these cases, the mutex is locked
     * by the thread but the state it protects is marked as
     * inconsistent. The application should ensure that the
     * state is made consistent for reuse and when that is
     * complete call `consistent()`. If the application
     * is unable to recover the state, it should unlock the
     * mutex without a prior call to `consistent()`, after
     * which the mutex is marked permanently unusable.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_trylock()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_trylock.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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
     * mutex becomes available as in the `lock()` function. If the
     * mutex cannot be locked without waiting for another thread to
     * unlock the mutex, this wait shall be terminated when the specified
     * timeout expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based.
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
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_timedlock()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_timedlock.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - the timeout is not expressed as an absolute time point, but
     * as a relative number of timer ticks (by default, the SysTick
     * clock for CMSIS).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Mutex::timed_lock (duration_t timeout)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(%d_ticks) @%p \n", __func__, timeout, this);

#if defined(OS_INCLUDE_PORT_RTOS_MUTEX)

      return port::Mutex::timed_lock (this, timeout);

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
     * object referenced by mutex when `unlock()` is called,
     * resulting in the mutex becoming available, the scheduling
     * policy shall determine which thread shall acquire the mutex.
     *
     * (In the case of `mutex::type::recursive` mutexes, the mutex
     * shall become available when the count reaches zero and the
     * calling thread no longer has any locks on this mutex.)
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_unlock()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_unlock.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_getprioceiling()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_getprioceiling.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    thread::priority_t
    Mutex::prio_ceiling (void) const
    {
      assert (!scheduler::in_handler_mode ());

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
     * Attempt to lock the mutex as if by a call to `lock()`,
     * except that the process of locking the mutex need not adhere
     * to the priority protect protocol. On acquiring the mutex
     * it shall change the mutex's priority ceiling and then
     * release the mutex as if by a call to `unlock()`.
     * When the change is successful, the previous value of
     * the priority ceiling shall be returned in old_ceiling.
     *
     * If `prio_ceiling()` function fails, the mutex
     * priority ceiling shall not be changed.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_setprioceiling()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_setprioceiling.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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
     * `consistent()` function can be used to mark the
     * state protected by the mutex referenced by mutex as
     * consistent again.
     *
     * If an owner of a robust mutex terminates while holding
     * the mutex, the mutex becomes inconsistent and the next
     * thread that acquires the mutex lock shall be notified
     * of the state by the return value `EOWNERDEAD`. In this
     * case, the mutex does not become normally usable again
     * until the state is marked consistent.
     *
     * If the thread which acquired the mutex lock with the
     * return value `EOWNERDEAD` terminates before calling
     * either `consistent()` or `unlock()`, the
     * next thread that acquires the mutex lock shall be
     * notified about the state of the mutex by the return
     * value `EOWNERDEAD`.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_consistent()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_consistent.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
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

    /**
     * @details
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
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
