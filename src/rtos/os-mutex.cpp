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

#include <cassert>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    /**
     * @class mutex::attributes
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
     * @class mutex::attributes_recursive
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
     * @var thread::priority_t mutex::attributes::mx_max_count
     * @details
     * The @ref mx_max_count attribute defines the upper limit of
     * recursion for a recursive mutex. Further attempts to lock
     * the mutex will result in `EAGAIN`.
     */

    /**
     * @var thread::priority_t mutex::attributes::mx_priority_ceiling
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
     *  Inspired by [`pthread_mutexattr_setprioceiling()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_getprioceiling.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @var mutex::protocol_t mutex::attributes::mx_protocol
     * @details
     * The default value of this attribute shall be `mutex::protocol::inherit`.
     *
     * @see mutex::protocol
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutexattr_setprotocol()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_getprotocol.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *  <br>Differences from the standard:
     *  - the POSIX default is `mutex::protocol::none`.
     */

    /**
     * @var mutex::robustness_t mutex::attributes::mx_robustness
     * @details
     * The default value of this attribute shall be
     * `mutex::robustness::stalled`.
     *
     * @see mutex::robustness
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutexattr_setrobust()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_setrobust.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @var mutex::type_t mutex::attributes::mx_type
     * @details
     * The default value of this attribute shall be `mutex::type::default_`.
     *
     * @see mutex::type
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutexattr_settype()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_settype.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @class mutex::protocol
     * @details
     * Allow to define the behaviour of the mutex
     * related to scheduling.
     *
     * The mutex protocol is defined via the `mx_protocol` attribute of
     * the `mutex::attributes` class. Valid values are:
     *
     * - `mutex::protocol::none`
     * - `mutex::protocol::inherit`
     * - `mutex::protocol::protect`
     *
     * When a thread owns a mutex with the `mutex::protocol::none`
     * protocol attribute, its priority and scheduling shall
     * not be affected by its mutex ownership.
     *
     * When a thread is blocking higher priority threads
     * because of owning one or more robust mutexes with the
     * `mutex::protocol::inherit` protocol attribute, it shall execute
     * at the higher of its priority or the priority of the highest
     * priority thread waiting on any of the robust mutexes owned
     * by this thread and initialised with this protocol.
     *
     * When a thread is blocking higher priority threads
     * because of owning one or more non-robust mutexes with the
     * `mutex::protocol::inherit` protocol attribute, it shall execute
     * at the higher of its priority or the priority of the highest
     * priority thread waiting on any of the non-robust mutexes owned
     * by this thread and initialised with this protocol.
     *
     * When a thread owns one or more robust mutexes initialised
     * with the `mutex::protocol::protect` protocol, it shall execute
     * at the higher of its priority or the highest of the priority
     * ceilings of all the robust mutexes owned by this thread and
     * initialised with this attribute, regardless of whether other
     * threads are blocked on any of these robust mutexes or not.
     *
     * When a thread owns one or more non-robust mutexes initialised
     * with the `mutex::protocol::protect` protocol, it shall execute
     * at the higher of its priority or the highest of the priority
     * ceilings of all the non-robust mutexes owned by this thread and
     * initialised with this attribute, regardless of whether other
     * threads are blocked on any of these non-robust mutexes or not.
     *
     * If a thread simultaneously owns several mutexes initialised
     * with different protocols, it shall execute at the highest of
     * the priorities that it would have obtained by each of these
     * protocols (this requirement makes most of the specific max
     * requirements above irrelevant, a global max is easier to compute).
     *
     * While a thread is holding a mutex which has been initialised with
     * the `mutex::protocol::inherit` or `mutex::protocol::protect` protocol
     * attributes, it shall not be subject to being moved to the tail
     * of the scheduling queue at its priority in the event that its
     * original priority is changed, such as by a call to
     * thread::priority(int). Likewise, when a thread unlocks a mutex
     * that has been initialised with the `mutex::protocol::inherit` or
     * `mutex::protocol::protect` protocol attributes, it shall not be
     * subject to being moved to the tail of the scheduling queue
     * at its priority in the event that its original priority is changed
     * (ILG: not sure what this means, currently after `unlock()` the thread
     * priority returns to the assigned priority).
     *
     * When a thread makes a call to `mutex::lock()`, the mutex
     * was initialised with the protocol attribute having the
     * value `mutex::protocol::inherit`, when the calling thread is
     * blocked because the mutex is owned by another thread, that
     * owner thread shall inherit the priority level of the calling
     * thread as long as it continues to own the mutex. The
     * implementation shall update its execution priority to
     * the maximum of its assigned priority and all its
     * inherited priorities. Furthermore, if this owner thread
     * itself becomes blocked on another mutex with the protocol
     * attribute having the value `mutex::protocol::inherit`, the same
     * priority inheritance effect shall be propagated to this
     * other owner thread, in a recursive manner.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutexattr_setprotocol()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_getprotocol.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @class mutex::robustness
     * @details
     *
     * Allow to define the behaviour when the thread
     * owning a mutex terminates.
     *
     * The mutex robustness is defined via the `mx_robustness` attribute of
     * the `mutex::attributes` class. Valid values are:
     *
     * - `mutex::robustness::stalled`
     *
     *   No special actions are taken if the owner of the mutex
     *  is terminated while holding the mutex lock. This can
     *  lead to deadlocks if no other thread can unlock the mutex.
     *  This is the default value.
     *
     * - `mutex::robustness::robust`
     *
     *   If the owning thread of a robust
     *   mutex terminates while holding the mutex lock, the next
     *   thread that acquires the mutex may be notified about the
     *   termination by the return value `EOWNERDEAD`. The notified
     *   thread can then attempt to mark the state protected by
     *   the mutex as consistent again by a call to
     *   `mutex::consistent()`. After a subsequent
     *   successful call to `mutex::unlock()`, the mutex
     *   lock shall be released and can be used normally by
     *   other threads. If the mutex is unlocked without a
     *   call to `mutex::consistent()`, it shall be in a
     *   permanently unusable state and all attempts to lock
     *   the mutex shall fail with the error `ENOTRECOVERABLE`.
     *   The only permissible operations on such a mutex are
     *   `mutex::reset()` and the mutex destruction.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutexattr_setrobust()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_setrobust.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @class mutex::type
     * @details
     *
     * Allow to define the behaviour of calls which lock
     * and unlock the mutex. See `mutex::lock()` for details.
     *
     * The mutex type is defined via the `mx_type` attribute of
     * the `mutex::attributes` class. Valid mutex types are:
     *
     * - `mutex::type::normal`
     * - `mutex::type::errorcheck`
     * - `mutex::type::recursive`
     * - `mutex::type::default_`
     *
     * An implementation may map `mutex::type::default_` to one of
     * the other mutex types.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutexattr_settype()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutexattr_settype.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @details
     * This variable is used by the normal mutex default constructor.
     */
    const mutex::attributes mutex::initializer_normal;

    /**
     * @details
     * This variable is used by the recursive mutex default constructor.
     */
    const mutex::attributes_recursive mutex::initializer_recursive;

    // ------------------------------------------------------------------------

    using mutexes_list = internal::intrusive_list<
    mutex, internal::double_list_links, &mutex::owner_links_>;

    // ------------------------------------------------------------------------

    /**
     * @class mutex
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
     * // Construct a normal mutex. Same as using the default constructor.
     * mutex mx { "mx1" };
     *
     * // Construct a recursive mutex.
     * mutex_recursive rmx { "mx2" };
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
     * mutex mx;
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
     * This constructor shall initialise a mutex object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the mutex attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * mutex object shall become initialised.
     *
     * Only the mutex object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default mutex attributes are
     * appropriate, the variables `mutex::initializer_normal`
     * or `mutex::initializer_recursive` can be used to
     * initialise mutex objects.
     * The effect shall be equivalent to creating a mutex
     * object with the default constructor.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    mutex::mutex (const attributes& attr) :
        mutex
          { nullptr, attr }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a mutex object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the mutex attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * mutex object shall become initialised.
     *
     * Only the mutex object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default mutex attributes are
     * appropriate, the variables `mutex::initializer_normal`
     * or `mutex::initializer_recursive` can be used to
     * initialise mutex objects.
     * The effect shall be equivalent to creating a mutex
     * object with the default constructor.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    mutex::mutex (const char* name, const attributes& attr) :
        object_named
          { name }, //
        type_ (attr.mx_type), //
        protocol_ (attr.mx_protocol), //
        robustness_ (attr.mx_robustness), //
        max_count_ ((attr.mx_type == type::recursive) ? attr.mx_max_count : 1)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif

      os_assert_throw(!interrupts::in_handler_mode (), EPERM);

      os_assert_throw(type_ <= type::max_, EINVAL);
      os_assert_throw(protocol_ <= protocol::max_, EINVAL);
      os_assert_throw(robustness_ <= robustness::max_, EINVAL);

#if !defined(OS_USE_RTOS_PORT_MUTEX)
      clock_ = attr.clock != nullptr ? attr.clock : &sysclock;
#endif

      os_assert_throw(attr.mx_priority_ceiling >= thread::priority::lowest,
                      EINVAL);
      os_assert_throw(attr.mx_priority_ceiling <= thread::priority::highest,
                      EINVAL);

      initial_prio_ceiling_ = attr.mx_priority_ceiling;
      prio_ceiling_ = attr.mx_priority_ceiling;

#if defined(OS_USE_RTOS_PORT_MUTEX)

      count_ = 0;
      port::mutex::create (this);

#else

      internal_init_ ();

#endif
    }

    /**
     * @details
     * This destructor shall destroy the mutex object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised mutex that is
     * unlocked. Attempting to destroy a locked mutex
     * results in undefined behaviour (for example it may trigger an assert).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_destroy()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_destroy.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    mutex::~mutex ()
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if defined(OS_USE_RTOS_PORT_MUTEX)

      port::mutex::destroy (this);

#else

      assert(owner_ == nullptr);
      assert(list_.empty ());

#endif
    }

    /**
     * @cond ignore
     */

    void
    mutex::internal_init_ (void)
    {
      owner_ = nullptr;
      owner_links_.unlink ();
      count_ = 0;
      prio_ceiling_ = initial_prio_ceiling_;
      boosted_prio_ = thread::priority::none;
      owner_dead_ = false;
      consistent_ = true;
      recoverable_ = true;

#if !defined(OS_USE_RTOS_PORT_MUTEX)

      // Wake-up all threads, if any.
      // Need not be inside the critical section,
      // the list is protected by inner `resume_one()`.
      list_.resume_all ();

#endif
    }

    /*
     * Internal function.
     * Should be called from a scheduler critical section.
     */
    result_t
    mutex::internal_try_lock_ (thread* crt_thread)
    {
      // Save the initial owner for later protocol tests.
      thread* saved_owner = owner_;

      // First lock.
      if (owner_ == nullptr)
        {
          // If the mutex has no owner, own it.
          owner_ = crt_thread;

          // For recursive mutexes, initialise counter.
          count_ = 1;

          // When the mutex is acquired, some more actions are
          // required, according to mutex attributes.

          if (robustness_ == robustness::robust)
            {
              mutexes_list* th_list =
                  reinterpret_cast<mutexes_list*> (&owner_->mutexes_);
              th_list->link (*this);
            }
          else
            {
              // Count the number of non-robust mutexes acquired by the thread.
              // Terminating a thread with locked non-robust mutexes
              // will trigger an assert.
              ++(crt_thread->acquired_mutexes_);
            }

          if (protocol_ == protocol::protect)
            {
              if (crt_thread->priority () > prio_ceiling_)
                {
                  // Prio ceiling must be at least the priority of the
                  // highest priority thread.
                  return EINVAL;
                }

              // POSIX: When a thread owns one or more mutexes
              // initialised with the mutex::protocol::protect protocol,
              // it shall execute at the higher of its priority or the
              // highest of the priority ceilings of all the mutexes
              // owned by this thread and initialised with this
              // attribute, regardless of whether other threads are
              // blocked on any of these robust mutexes or not.

              // Boost priority.
              boosted_prio_ = prio_ceiling_;
              if (boosted_prio_ > owner_->priority_inherited ())
                {
                  // ----- Enter uncritical section ---------------------------
                  scheduler::uncritical_section sucs;

                  owner_->priority_inherited (boosted_prio_);
                  // ----- Exit uncritical section ----------------------------
                }
            }

#if defined(OS_TRACE_RTOS_MUTEX)
          trace::printf ("%s() @%p %s by %p %s LCK\n", __func__, this, name (),
                         crt_thread, crt_thread->name ());
#endif
          // If the owning thread of a robust mutex terminates while
          // holding the mutex lock, the next thread that acquires the
          // mutex may be notified about the termination by the return
          // value EOWNERDEAD.
          if (owner_dead_)
            {
              return EOWNERDEAD;
            }
          return result::ok;
        }

      // Relock? (lock by the same thread).
      if (saved_owner == crt_thread)
        {
          // The mutex was requested again by the same thread.
          if (type_ == type::recursive)
            {
              if (count_ >= max_count_)
                {
                  // The recursive mutex reached its limit.
#if defined(OS_TRACE_RTOS_MUTEX)
                  trace::printf ("%s() @%p %s EAGAIN\n", __func__, this,
                                 name ());
#endif
                  return EAGAIN;
                }

              // Increment the recursion depth counter.
              ++count_;

#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() @%p %s by %p %s >%u\n", __func__, this,
                             name (), crt_thread, crt_thread->name (), count_);
#endif
              return result::ok;
            }
          else if (type_ == type::errorcheck)
            {
              // Errorcheck mutexes do not block, but return an error.
#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() @%p %s EDEADLK\n", __func__, this, name ());
#endif
              return EDEADLK;
            }
          else if (type_ == type::normal)
            {
#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() @%p %s deadlock\n", __func__, this, name ());
#endif
              return EWOULDBLOCK;
            }

          return EWOULDBLOCK;
        }
      else
        {
          // Try to lock when not owner (another thread requested the mutex).

          // POSIX: When a thread makes a call to mutex::lock(), the mutex was
          // initialised with the protocol attribute having the value
          // mutex::protocol::inherit, when the calling thread is blocked
          // because the mutex is owned by another thread, that owner thread
          // shall inherit the priority level of the calling thread as long
          // as it continues to own the mutex. The implementation shall
          // update its execution priority to the maximum of its assigned
          // priority and all its inherited priorities.
          // Furthermore, if this owner thread itself becomes blocked on
          // another mutex with the protocol attribute having the value
          // mutex::protocol::inherit, the same priority inheritance effect
          // shall be propagated to this other owner thread, in a recursive
          // manner.
          if (protocol_ == protocol::inherit)
            {
              thread::priority_t prio = crt_thread->priority ();
              boosted_prio_ = prio;

              mutexes_list* th_list =
                  reinterpret_cast<mutexes_list*> (&owner_->mutexes_);
              if (owner_links_.unlinked ())
                {
                  th_list->link (*this);
                }

              // Boost owner priority.
              if ((boosted_prio_ > owner_->priority_inherited ()))
                {
                  // ----- Enter uncritical section ---------------------------
                  scheduler::uncritical_section sucs;

                  owner_->priority_inherited (boosted_prio_);
                  // ----- Exit uncritical section ----------------------------
                }

#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() @%p %s boost %u by %p %s \n", __func__, this,
                             name (), boosted_prio_, crt_thread,
                             crt_thread->name ());
#endif

              return EWOULDBLOCK;
            }
        }

      // Block anyway.
      return EWOULDBLOCK;
    }

    // Called from thread termination, in a critical section.
    void
    mutex::internal_mark_owner_dead_ (void)
    {
      if (robustness_ == mutex::robustness::robust)
        {
          // If the owning thread of a robust mutex terminates
          // while holding the mutex lock, the next thread that
          // acquires the mutex may be notified about the termination
          // by the return value EOWNERDEAD.
          owner_dead_ = true;
          consistent_ = false;

          // Actually delayed until end of critical section.
          list_.resume_one ();
        }
    }

    /**
     * @endcond
     */

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
     * | ------------| ---------- | --------- | ----------------------|
     * | normal      | non-robust | deadlock  | undefined behaviour   |
     * | normal      | robust     | deadlock  | error                 |
     * | errorcheck  | either     | error     | error                 |
     * | recursive   | either     | recursive | error                 |
     * | default     | non-robust | undefined | undefined behaviour   |
     * | default     | robust     | undefined | error                 |
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
    mutex::lock (void)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s by %p %s\n", __func__, this, name (),
                     &this_thread::thread (), this_thread::thread ().name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      if (!recoverable_)
        {
          return ENOTRECOVERABLE;
        }

#if defined(OS_USE_RTOS_PORT_MUTEX)

      return port::mutex::lock (this);

#else

      thread& crt_thread = this_thread::thread ();

      result_t res;
        {
          // ----- Enter critical section -------------------------------------
          scheduler::critical_section scs;

          res = internal_try_lock_ (&crt_thread);
          if (res != EWOULDBLOCK)
            {
              return res;
            }
          // ----- Exit critical section --------------------------------------
        }

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              scheduler::critical_section scs;

              res = internal_try_lock_ (&crt_thread);
              if (res != EWOULDBLOCK)
                {
                  return res;
                }

                {
                  // ----- Enter critical section -----------------------------
                  interrupts::critical_section ics;

                  // Add this thread to the mutex waiting list.
                  scheduler::internal_link_node (list_, node);
                  // state::suspended set in above link().
                  // ----- Exit critical section ------------------------------
                }
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the semaphore waiting list,
          // if not already removed by unlock().
          scheduler::internal_unlink_node (node);

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() EINTR @%p %s\n", __func__, this, name ());
#endif
              return EINTR;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * Try to lock the mutex as `lock()`, except that if the
     * mutex is currently locked (by
     * any thread, including the current thread), the call shall
     * return immediately. If the mutex type is `mutex::type::recursive`
     * and the mutex is currently owned by the calling thread,
     * the mutex lock count shall be incremented by one and the
     * `try_lock()` function shall immediately return success.
     *
     * If the mutex is robust and the owning
     * thread terminated while holding the mutex lock, a call
     * to `try_lock()` may return the error value `EOWNERDEAD`.
     * In these cases, the mutex is locked
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
     *  <br>Differences from the standard:
     *  - for consistency reasons, EWOULDBLOCK is used, instead of EBUSY
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    mutex::try_lock (void)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s by %p %s\n", __func__, this, name (),
                     &this_thread::thread (), this_thread::thread ().name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

      if (!recoverable_)
        {
          return ENOTRECOVERABLE;
        }

#if defined(OS_USE_RTOS_PORT_MUTEX)

      return port::mutex::try_lock (this);

#else

      thread& crt_thread = this_thread::thread ();

        {
          // ----- Enter critical section -------------------------------------
          scheduler::critical_section scs;

          return internal_try_lock_ (&crt_thread);
          // ----- Exit critical section --------------------------------------
        }

#endif
    }

    /**
     * @details
     * If the mutex
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
     * if the mutex can be locked immediately. The validity of
     * the timeout need not be checked if the mutex can be
     * locked immediately.
     *
     * The clock used for timeouts can be specified via the `clock`
     * attribute. By default, the clock derived from the scheduler
     * timer is used, and the durations are expressed in ticks.
     *
     * As a consequence of the priority inheritance rules (for
     * mutexes initialized with `mutex::protocol::inherit`),
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
     * clock for Cortex-M).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    mutex::timed_lock (clock::duration_t timeout)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s(%u) @%p %s by %p %s\n", __func__,
                     static_cast<unsigned int> (timeout), this, name (),
                     &this_thread::thread (), this_thread::thread ().name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      if (!recoverable_)
        {
          return ENOTRECOVERABLE;
        }

#if defined(OS_USE_RTOS_PORT_MUTEX)

      return port::mutex::timed_lock (this, timeout);

#else

      thread& crt_thread = this_thread::thread ();

      result_t res;

      // Extra test before entering the loop, with its inherent weight.
      // Trade size for speed.
        {
          // ----- Enter critical section -------------------------------------
          scheduler::critical_section scs;

          res = internal_try_lock_ (&crt_thread);
          if (res != EWOULDBLOCK)
            {
              return res;
            }
          // ----- Exit critical section --------------------------------------
        }

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      internal::clock_timestamps_list& clock_list = clock_->steady_list ();
      clock::timestamp_t timeout_timestamp = clock_->steady_now () + timeout;

      // Prepare a timeout node pointing to the current thread.
      internal::timeout_thread_node timeout_node
        { timeout_timestamp, crt_thread };

      for (;;)
        {
            {
              // ----- Enter critical section ---------------------------------
              scheduler::critical_section scs;

              res = internal_try_lock_ (&crt_thread);
              if (res != EWOULDBLOCK)
                {
                  return res;
                }

                {
                  // ----- Enter critical section -----------------------------
                  interrupts::critical_section ics;

                  // Add this thread to the mutex waiting list,
                  // and the clock timeout list.
                  scheduler::internal_link_node (list_, node, clock_list,
                                                 timeout_node);
                  // state::suspended set in above link().
                  // ----- Exit critical section ------------------------------
                }
              // ----- Exit critical section ----------------------------------
            }

          port::scheduler::reschedule ();

          // Remove the thread from the semaphore waiting list,
          // if not already removed by unlock() and from the clock
          // timeout list, if not already removed by the timer.
          scheduler::internal_unlink_node (node, timeout_node);

          res = result::ok;

          if (crt_thread.interrupted ())
            {
#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() EINTR @%p %s \n", __func__, this, name ());
#endif
              res = EINTR;
            }
          else if (clock_->steady_now () >= timeout_timestamp)
            {
#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() ETIMEDOUT @%p %s \n", __func__, this,
                             name ());
#endif
              res = ETIMEDOUT;
            }
          if (res != result::ok)
            {
              if (boosted_prio_ != thread::priority::none)
                {
                  // If the priority was boosted, it must be restored
                  // to the highest priority of the waiting threads, if any.

                  thread::priority_t max_prio = thread::priority::none;

                  for (auto&& th : list_)
                    {
                      thread::priority_t prio = th.priority ();
                      if (prio > max_prio)
                        {
                          max_prio = prio;
                        }
                    }

                  if (max_prio != thread::priority::none)
                    {
                      boosted_prio_ = max_prio;
                      owner_->priority (boosted_prio_);
                    }
                }
              return res;
            }
        }

      /* NOTREACHED */
      return ENOTRECOVERABLE;

#endif
    }

    /**
     * @details
     * The manner
     * in which a mutex is released is dependent upon the mutex's
     * type attribute. If there are threads blocked on the mutex
     * object referenced by mutex when `unlock()` is called,
     * resulting in the mutex becoming available, the scheduling
     * policy shall determine which thread shall acquire the mutex.
     *
     * In the case of `mutex::type::recursive` mutexes, the mutex
     * shall become available when the count reaches zero and the
     * calling thread no longer has any locks on this mutex.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_mutex_unlock()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_unlock.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    mutex::unlock (void)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s by %p %s\n", __func__, this, name (),
                     &this_thread::thread (), this_thread::thread ().name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

#if defined(OS_USE_RTOS_PORT_MUTEX)

      return port::mutex::unlock (this);

#else

      if (!recoverable_)
        {
          return ENOTRECOVERABLE;
        }

      thread* crt_thread = &this_thread::thread ();

        {
          // ----- Enter critical section -------------------------------------
          scheduler::critical_section scs;

          // Is the rightful owner?
          if (owner_ == crt_thread)
            {
              if ((type_ == type::recursive) && (count_ > 1))
                {
                  --count_;
#if defined(OS_TRACE_RTOS_MUTEX)
                  trace::printf ("%s() @%p %s >%u\n", __func__, this, name (),
                                 count_);
#endif
                  return result::ok;
                }

              if (robustness_ != robustness::robust)
                {
                  --(owner_->acquired_mutexes_);
                }

              // Remove this mutex from the thread list; ineffective if
              // not linked.
              owner_links_.unlink ();

              if (boosted_prio_ != thread::priority::none)
                {
                  mutexes_list* thread_mutexes =
                      reinterpret_cast<mutexes_list*> (&owner_->mutexes_);

                  if (thread_mutexes->empty ())
                    {
                      // If the owner thread has no more mutexes,
                      // clear the inherited priority,
                      // and the assigned priority will take precedence.
                      boosted_prio_ = thread::priority::none;
                    }
                  else
                    {
                      // If the owner thread acquired other mutexes too,
                      // compute the maximum boosted priority.
                      thread::priority_t max_prio = 0;
                      for (auto&& mx : *thread_mutexes)
                        {
                          if (mx.boosted_prio_ > max_prio)
                            {
                              max_prio = mx.boosted_prio_;
                            }
                        }
                      boosted_prio_ = max_prio;
                    }
                  // Delayed until end of critical section.
                  owner_->priority_inherited (boosted_prio_);
                }

              // Delayed until end of critical section.
              list_.resume_one ();

              // Finally release the mutex.
              owner_ = nullptr;
              count_ = 0;

#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() @%p %s ULCK\n", __func__, this, name ());
#endif

              // POSIX: If a robust mutex whose owner died is unlocked without
              // a call to consistent(), it shall be in a permanently
              // unusable state and all attempts to lock the mutex
              // shall fail with the error ENOTRECOVERABLE.

              if (owner_dead_)
                {
                  owner_dead_ = false;

                  if (!consistent_)
                    {
                      recoverable_ = false;
                      return ENOTRECOVERABLE;
                    }
                }

              return result::ok;
            }

          // Not owner, or not locked.
          if (type_ == type::errorcheck || type_ == type::recursive
              || robustness_ == robustness::robust)
            {
#if defined(OS_TRACE_RTOS_MUTEX)
              trace::printf ("%s() EPERM @%p %s \n", __func__, this, name ());
#endif
              return EPERM;
            }

          // Normal no-robust mutexes owned by other threads have
          // undefined behaviour.

#if defined(OS_TRACE_RTOS_MUTEX)
          trace::printf ("%s() ENOTRECOVERABLE @%p %s \n", __func__, this,
                         name ());
#endif
          return ENOTRECOVERABLE;
          // ----- Exit critical section --------------------------------------
        }

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
    mutex::prio_ceiling (void) const
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      assert(!interrupts::in_handler_mode ());

#if defined(OS_USE_RTOS_PORT_MUTEX)

      return port::mutex::prio_ceiling (this);

#else

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
     * the priority ceiling shall be returned in `old_prio_ceiling`.
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
    mutex::prio_ceiling (thread::priority_t prio_ceiling,
                         thread::priority_t* old_prio_ceiling)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

#if defined(OS_USE_RTOS_PORT_MUTEX)

      return port::mutex::prio_ceiling (this, prio_ceiling, old_prio_ceiling);

#else

      // TODO: lock() must not adhere to the priority protocol.
      result_t res = lock ();
      if (res != result::ok)
        {
          return res;
        }

      if (old_prio_ceiling != nullptr)
        {
          *old_prio_ceiling = prio_ceiling_;
        }

      prio_ceiling_ = prio_ceiling;

      unlock ();

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
    mutex::consistent (void)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(robustness_ == robustness::robust, EINVAL);
      os_assert_err(!consistent_, EINVAL);

#if defined(OS_USE_RTOS_PORT_MUTEX)

      return port::mutex::consistent (this);

#else

      // Update status to consistent.
      consistent_ = true;
      return result::ok;

#endif
    }

    /**
     * @details
     * Return the mutex to the state right after creation. If there were threads
     * waiting for this mutex, wakeup all, then clear the waiting list.
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    mutex::reset (void)
    {
#if defined(OS_TRACE_RTOS_MUTEX)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

        {
          // ----- Enter critical section -------------------------------------
          scheduler::critical_section scs;

          internal_init_ ();
          return result::ok;
          // ----- Exit critical section --------------------------------------
        }

    }

  // ==========================================================================

  /**
   * @fn mutex_recursive::mutex_recursive (const attributes& attr)
   * @copydetails mutex::mutex(const attributes&)
   */

  /**
   * @fn mutex_recursive::mutex_recursive (const char* name, const attributes& attr)
   * @copydetails mutex::mutex(const char* name, const attributes&)
   */

  /**
   * @fn mutex_recursive::~mutex_recursive ()
   * @copydetails mutex::~mutex()
   */

  // --------------------------------------------------------------------------
  } /* namespace rtos */
} /* namespace os */
