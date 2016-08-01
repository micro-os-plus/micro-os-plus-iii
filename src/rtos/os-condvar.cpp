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
     * @class condition_variable::attributes
     * @details
     * Allow to assign a name to the condition variable.
     *
     * @par POSIX compatibility
     *  Inspired by `pthread_condattr_t` from [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  (IEEE Std 1003.1, 2013 Edition).
     */

    /**
     * @details
     * This variable is used by the default constructor.
     */
    const condition_variable::attributes condition_variable::initializer;

    // ------------------------------------------------------------------------

    /**
     * @class condition_variable
     * @details
     * A condition variable is a synchronisation object which allows a thread
     * to suspend execution, repeatedly, until some associated predicate
     * becomes true. A thread whose execution is suspended on a condition
     * variable is said to be blocked on the condition variable.
     *
     * @par Scheduling Behaviour of Condition Variables
     *
     * Synchronisation primitives that attempt to interfere with
     * scheduling policy by specifying an ordering rule are considered
     * undesirable. Threads waiting on mutexes and condition variables
     * are selected to proceed in an order dependent upon the scheduling
     * policy rather than in some fixed order (for example, FIFO or
     * priority). Thus, the scheduling policy determines which
     * thread(s) are awakened and allowed to proceed.
     *
     * @par Cancellation and Condition Wait
     *
     * A condition wait, whether timed or not, is a cancellation
     * point. That is, the functions `wait()`
     * or `timed_wait()` are points where a pending
     * (or concurrent) cancellation request is noticed. The reason
     * for this is that an indefinite wait is possible at these
     * points-whatever event is being waited for, even if the
     * program is totally correct, might never occur; for example,
     * some input data being awaited might never be sent. By
     * making condition wait a cancellation point, the thread can
     * be cancelled and perform its cancellation cleanup handler
     * even though it may be stuck in some indefinite wait.
     *
     * A side-effect of acting on a cancellation request while a
     * thread is blocked on a condition variable is to re-acquire
     * the mutex before calling any of the cancellation cleanup
     * handlers. This is done in order to ensure that the
     * cancellation cleanup handler is executed in the same
     * state as the critical code that lies both before and
     * after the call to the condition wait function. This
     * rule is also required when interfacing to POSIX threads
     * from languages, such as Ada or C++, which may choose
     * to map cancellation onto a language exception; this rule
     * ensures that each exception handler guarding a critical
     * section can always safely depend upon the fact that
     * the associated mutex has already been locked regardless
     * of exactly where within the critical section the
     * exception was raised. Without this rule, there would
     * not be a uniform rule that exception handlers could
     * follow regarding the lock, and so coding would become
     * very cumbersome.
     *
     * Therefore, since some statement has to be made regarding
     * the state of the lock when a cancellation is delivered
     * during a wait, a definition has been chosen that makes
     * application coding most convenient and error free.
     *
     * When acting on a cancellation request while a thread
     * is blocked on a condition variable, the implementation
     * is required to ensure that the thread does not consume
     * any condition signals directed at that condition variable
     * if there are any other threads waiting on that condition
     * variable. This rule is specified in order to avoid deadlock
     * conditions that could occur if these two independent
     * requests (one acting on a thread and the other acting on
     * the condition variable) were not processed independently.
     *
     * @par Performance of Mutexes and Condition Variables
     *
     * Mutexes are expected to be locked only for a few
     * instructions. This practice is almost automatically
     * enforced by the desire of programmers to avoid long
     * serial regions of execution (which would reduce total
     * effective parallelism).
     *
     * When using mutexes and condition variables, one tries
     * to ensure that the usual case is to lock the mutex,
     * access shared data, and unlock the mutex. Waiting on
     * a condition variable should be a relatively rare
     * situation. For example, when implementing a read-write
     * lock, code that acquires a read-lock typically needs
     * only to increment the count of readers
     * (under mutual-exclusion) and return. The calling
     * thread would actually wait on the condition variable
     * only when there is already an active writer. So the
     * efficiency of a synchronisation operation is bounded
     * by the cost of mutex lock/unlock and not by condition
     * wait. Note that in the usual case there is no context switch.
     *
     * This is not to say that the efficiency of condition
     * waiting is unimportant. Since there needs to be at
     * least one context switch per Ada rendezvous, the
     * efficiency of waiting on a condition variable is
     * important. The cost of waiting on a condition variable
     * should be little more than the minimal cost for a context
     * switch plus the time to unlock and lock the mutex.
     *
     * @par Example
     *
     * @code{.cpp}
     * mutex mx;
     * condition_variable cv;
     *
     * void
     * consumer(void)
     * {
     *   // Do something
     *   mx.lock();
     *   // ...
     *   while(!condition())
     *     {
     *       // ...
     *       cv.wait();
     *       // ...
     *     }
     *   // ...
     *   mx.unlock(mx);
     *   // Do something else.
     * }
     *
     * void
     * producer(void)
     * {
     *   // Do something
     *   mx.lock();
     *   // ...
     *   if (some_condition())
     *     {
     *       // ...
     *       cv.signal();
     *       // ...
     *     }
     *   // ...
     *   mx.unlock(mx);
     *   // Do something else.
     * }
     * @endcode
     *
     * @par POSIX compatibility
     *  Inspired by `pthread_cond_t` from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @details
     * This constructor shall initialise a condition variable object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the condition variable attributes shall not be affected.
     *
     * Upon successful initialisation, the state of the
     * condition variable object shall become initialised.
     *
     * Only the condition variable object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default condition variable attributes are
     * appropriate, the variable `condition_variable::initializer`
     * can be used to initialise condition variables.
     * The effect shall be equivalent to creating a condition variables
     * object with the default constructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cond_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_destroy.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */
    condition_variable::condition_variable (const attributes& attr) :
        condition_variable
          { nullptr, attr }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a named condition variable object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the condition variable attributes shall not be affected.
     *
     * Upon successful initialisation, the state of the
     * condition variable object shall become initialised.
     *
     * Only the condition variable object itself may be used for performing
     * synchronisation. It is not allowed to make copies of
     * condition variable objects.
     *
     * In cases where default condition variable attributes are
     * appropriate, the variable `condition_variable::initializer`
     * can be used to initialise condition variables.
     * The effect shall be equivalent to creating a condition variables
     * object with the default constructor.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cond_init()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_destroy.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */
    condition_variable::condition_variable (
        const char* name, const attributes& attr __attribute__((unused))) :
        object_named
          { name }
    {
#if defined(OS_TRACE_RTOS_CONDVAR)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif

      os_assert_throw(!interrupts::in_handler_mode (), EPERM);
    }

    /**
     * @details
     * This destructor shall destroy the condition variable object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * It shall be safe to destroy an initialised condition variable object
     * upon which no threads are currently blocked. Attempting to
     * destroy a condition variable object upon which other threads are
     * currently blocked results in undefined behaviour.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cond_destroy()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_destroy.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */
    condition_variable::~condition_variable ()
    {
#if defined(OS_TRACE_RTOS_CONDVAR)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      assert(list_.empty ());
    }

    /**
     * @details
     * Unblock at least one of the threads that are blocked
     * on the specified condition variable.
     *
     * When each thread unblocked as a result of a
     * `signal()` returns from its call to
     * `wait()` or `timed_wait()`,
     * the thread shall own the mutex with which it called
     * `wait()` or `timed_wait()`.
     * The thread(s) that are unblocked shall contend for
     * the mutex according to the scheduling policy (if applicable),
     * and as if each had called `mutex::lock()`.
     *
     * `signal()` may be called by a thread
     * whether or not it currently owns the mutex that threads
     * calling `wait()` or
     * `timed_wait()` have associated with
     * the condition variable during their waits; however,
     * if predictable scheduling behaviour is required, then
     * that mutex shall be locked by the thread calling
     * `signal()`.
     *
     * The `signal()` function shall
     * have no effect if there are no threads currently
     * blocked on this condition variable.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cond_signal()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_signal.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */
    result_t
    condition_variable::signal ()
    {
#if defined(OS_TRACE_RTOS_CONDVAR)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

      list_.resume_one ();

      return result::ok;
    }

    /**
     * @details
     * Unblock all threads currently blocked on the specified
     * condition variable.
     *
     * If more than one thread are blocked on a condition variable,
     * the scheduling policy shall determine the order in which
     * threads are unblocked.
     *
     * When each thread unblocked as a result of a
     * `broadcast()` returns from its call to
     * `wait()` or `timed_wait()`,
     * the thread shall own the mutex with which it called
     * `wait()` or `timed_wait()`.
     * The thread(s) that are unblocked shall contend for
     * the mutex according to the scheduling policy (if applicable),
     * and as if each had called `mutex::lock()`.
     *
     * `broadcast()` may be called by a thread
     * whether or not it currently owns the mutex that threads
     * calling `wait()` or
     * `timed_wait()` have associated with
     * the condition variable during their waits; however,
     * if predictable scheduling behaviour is required, then
     * that mutex shall be locked by the thread calling
     * `broadcast()`.
     *
     * The `broadcast()` function shall
     * have no effect if there are no threads currently
     * blocked on this condition variable.
     *
     * @par Application usage
     * The `broadcast()` function is used whenever
     * the shared-variable state has been changed in a way that more
     * than one thread can proceed with its task. Consider a single
     * producer/multiple consumer problem, where the producer can
     * insert multiple items on a list that is accessed one item at
     * a time by the consumers. By calling the
     * `broadcast()` function, the producer would
     * notify all consumers that might be waiting, and thereby the
     * application would receive more throughput on a multi-processor.
     * In addition, `broadcast()` makes it easier
     * to implement a read-write lock. The `broadcast()`
     * function is needed in order to wake up all waiting readers when
     * a writer releases its lock. Finally, the two-phase commit
     * algorithm can use this broadcast function to notify all
     * clients of an impending transaction commit.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cond_broadcast()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_broadcast.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */
    result_t
    condition_variable::broadcast ()
    {
#if defined(OS_TRACE_RTOS_CONDVAR)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);

      // Wake-up all threads, if any.
      // Need not be inside the critical section,
      // the list is protected by inner `resume_one()`.
      list_.resume_all ();

      return result::ok;
    }

    /**
     * @details
     * Block on a condition variable. The application shall ensure
     * that this function is called with _mutex_ locked by
     * the calling thread; otherwise, an error (for
     * `mutex::type::errorcheck` and robust mutexes) or
     * undefined behaviour (for other mutexes) results.
     *
     * These functions atomically release _mutex_ and cause the
     * calling thread to block on the condition variable;
     * atomically here means "atomically with respect to access
     * by another thread to the mutex and then the condition variable".
     * That is, if another thread is able to acquire the mutex
     * after the about-to-block thread has released it, then a
     * subsequent call to `broadcast()` or
     * `signal()` in that thread shall behave
     * as if it were issued after the about-to-block thread has blocked.
     *
     * Upon successful return, the mutex shall have been locked and
     * shall be owned by the calling thread. If _mutex_ is a robust
     * mutex where an owner terminated while holding the lock and
     * the state is recoverable, the mutex shall be acquired even
     * though the function returns an error code.
     *
     * When using condition variables there is always a Boolean
     * predicate involving shared variables associated with each
     * condition wait that is true if the thread should proceed.
     * Spurious wakeups from the
     * `wait()` functions may occur. Since
     * the return from
     * `wait()` does not imply anything about
     * the value of this predicate, the predicate should be
     * re-evaluated upon such return.
     *
     * When a thread waits on a condition variable, having specified
     * a particular mutex to either the
     * the `wait()` operation, a dynamic
     * binding is formed between that mutex and condition variable
     * that remains in effect as long as at least one thread is
     * blocked on the condition variable. During this time, the
     * effect of an attempt by any thread to wait on that condition
     * variable using a different mutex is undefined. Once all
     * waiting threads have been unblocked (as by the
     * `broadcast()` operation), the next wait
     * operation on that condition variable shall form a new
     * dynamic binding with the mutex specified by that wait
     * operation. Even though the dynamic binding between
     * condition variable and _mutex_ may be removed or replaced
     * between the time a thread is unblocked from a wait on the
     * condition variable and the time that it returns to the
     * caller or begins cancellation cleanup, the unblocked
     * thread shall always re-acquire the mutex specified in
     * the condition wait operation call from which it is returning.
     *
     * A condition wait (whether timed or not) is a cancellation
     * point. When the cancellability type of a thread is set to
     * PTHREAD_CANCEL_DEFERRED, a side-effect of acting upon a
     * cancellation request while in a condition wait is that
     * the mutex is (in effect) re-acquired before calling the
     * first cancellation cleanup handler. The effect is as if
     * the thread were unblocked, allowed to execute up to the
     * point of returning from the call to
     * `wait()`, but at that point notices
     * the cancellation request and instead of returning to the
     * caller of
     * `wait()`, starts the thread cancellation
     * activities, which includes calling cancellation cleanup handlers.
     *
     * A thread that has been unblocked because it has been cancelled
     * while blocked in a call to
     * `wait()` shall not consume any condition
     * signal that may be directed concurrently at the condition
     * variable if there are other threads blocked on the condition
     * variable.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cond_wait()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_wait.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */
    result_t
    condition_variable::wait (mutex& mutex)
    {
#if defined(OS_TRACE_RTOS_CONDVAR)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      // TODO: validate

      result_t res;
      res = mutex.unlock ();

      if (res != result::ok)
        {
          return res;
        }

        {
          // Add this thread to the condition variable waiting list.
          list_.link (node);
          node.thread_->waiting_node_ = &node;

          res = mutex.lock ();

          // Remove the thread from the node waiting list,
          // if not already removed.
          node.thread_->waiting_node_ = nullptr;
          node.unlink ();
        }

      return res;
    }

    /**
     * @details
     * Block on a condition variable. The application shall ensure
     * that this function is called with _mutex_ locked by
     * the calling thread; otherwise, an error (for
     * `mutex::type::errorcheck` and robust mutexes) or
     * undefined behaviour (for other mutexes) results.
     *
     * These functions atomically release _mutex_ and cause the
     * calling thread to block on the condition variable cond;
     * atomically here means "atomically with respect to access
     * by another thread to the mutex and then the condition variable".
     * That is, if another thread is able to acquire the mutex
     * after the about-to-block thread has released it, then a
     * subsequent call to `broadcast()` or
     * `signal()` in that thread shall behave
     * as if it were issued after the about-to-block thread has blocked.
     *
     * Upon successful return, the mutex shall have been locked and
     * shall be owned by the calling thread. If _mutex_ is a robust
     * mutex where an owner terminated while holding the lock and
     * the state is recoverable, the mutex shall be acquired even
     * though the function returns an error code.
     *
     * When using condition variables there is always a Boolean
     * predicate involving shared variables associated with each
     * condition wait that is true if the thread should proceed.
     * Spurious wakeups from the `timed_wait()`
     * functions may occur. Since
     * the return from `timed_wait()`
     * does not imply anything about
     * the value of this predicate, the predicate should be
     * re-evaluated upon such return.
     *
     * When a thread waits on a condition variable, having specified
     * a particular mutex to either the `timed_wait()`
     * operation, a dynamic
     * binding is formed between that mutex and condition variable
     * that remains in effect as long as at least one thread is
     * blocked on the condition variable. During this time, the
     * effect of an attempt by any thread to wait on that condition
     * variable using a different mutex is undefined. Once all
     * waiting threads have been unblocked (as by the
     * `broadcast()` operation), the next wait
     * operation on that condition variable shall form a new
     * dynamic binding with the mutex specified by that wait
     * operation. Even though the dynamic binding between
     * condition variable and _mutex_ may be removed or replaced
     * between the time a thread is unblocked from a wait on the
     * condition variable and the time that it returns to the
     * caller or begins cancellation cleanup, the unblocked
     * thread shall always re-acquire the mutex specified in
     * the condition wait operation call from which it is returning.
     *
     * A condition wait (whether timed or not) is a cancellation
     * point. When the cancellability type of a thread is set to
     * PTHREAD_CANCEL_DEFERRED, a side-effect of acting upon a
     * cancellation request while in a condition wait is that
     * the mutex is (in effect) re-acquired before calling the
     * first cancellation cleanup handler. The effect is as if
     * the thread were unblocked, allowed to execute up to the
     * point of returning from the call to
     * `timed_wait()`, but at that point notices
     * the cancellation request and instead of returning to the
     * caller of `timed_wait()` or
     * `wait()`, starts the thread cancellation
     * activities, which includes calling cancellation cleanup handlers.
     *
     * A thread that has been unblocked because it has been cancelled
     * while blocked in a call to `timed_wait()`
     * shall not consume any condition
     * signal that may be directed concurrently at the condition
     * variable if there are other threads blocked on the condition
     * variable.
     *
     * The `timed_wait()` function shall be
     * equivalent to `wait()`, except that an
     * error is returned if the timeout specified by _timeout_
     * passes (that is, system time equals or exceeds now() + timeout) before
     * the condition cond is signalled or broadcasted. When such timeouts occur,
     * `timed_wait()` shall nonetheless release
     * and re-acquire the mutex referenced by _mutex_, and may consume
     * a condition signal directed concurrently at the condition variable.
     *
     * The condition variable shall have a clock attribute which
     * specifies the clock that shall be used to measure the time
     * specified by the _ticks_ argument. The
     * `timed_wait()` function is also a
     * cancellation point.
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based (by default the SysTick clock for CMSIS).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cond_timedwait()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cond_timedwait.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */
    result_t
    condition_variable::timed_wait (mutex& mutex, clock::duration_t timeout)
    {
#if defined(OS_TRACE_RTOS_CONDVAR)
      trace::printf ("%s(%u) @%p %s\n", __func__,
                     static_cast<unsigned int> (timeout), this, name ());
#endif

      os_assert_err(!interrupts::in_handler_mode (), EPERM);
      os_assert_err(!scheduler::locked (), EPERM);

      thread& crt_thread = this_thread::thread ();

      // Prepare a list node pointing to the current thread.
      // Do not worry for being on stack, it is temporarily linked to the
      // list and guaranteed to be removed before this function returns.
      internal::waiting_thread_node node
        { crt_thread };

      // TODO: validate

      result_t res;
      res = mutex.unlock ();

      if (res != result::ok)
        {
          return res;
        }

        {
          // Add this thread to the condition variable waiting list.
          list_.link (node);
          node.thread_->waiting_node_ = &node;

          res = mutex.timed_lock (timeout);

          // Remove the thread from the node waiting list,
          // if not already removed.
          node.thread_->waiting_node_ = nullptr;
          node.unlink ();
        }

      return res;
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
