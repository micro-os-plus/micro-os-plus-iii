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

#include <cassert>

using namespace os::cmsis;

namespace os
{
  namespace cmsis
  {
    namespace rtos
    {

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

      // ======================================================================

      namespace kernel
      {
        /**
         * @details
         * Create all RTOS internal objects and be ready to run.
         */
        result_t
        initialize (void)
        {
          // TODO
          return result::ok;
        }

        const char*
        strerror (result_t res)
        {
          const char* str;
          switch (res)
            {
            case result::event_signal:
              str = "signal event occurred";
              break;

            case result::event_message:
              str = "message event occurred";
              break;

            case result::event_mail:
              str = "mail event occurred";
              break;

            case result::event_timeout:
              str = "timeout occurred";
              break;

            case result::error_parameter:
              str = "mandatory parameter missing or incorrect object";
              break;

            case result::error_resource:
              str = "resource not available";
              break;

            case result::error_timeout_resource:
              str = "resource not available within given time";
              break;

            case result::error_isr:
              str = "not allowed in ISR context";
              break;

            case result::error_isr_recursive:
              str = "function called multiple times from ISR with same object";
              break;

            case result::error_priority:
              str =
                  "system cannot determine priority or thread has illegal priority";
              break;

            case result::error_no_memory:
              str = "system is out of memory";
              break;

            case result::error_value:
              str = "value of a parameter is out of range";
              break;

            default:
              str = "unknown error";
              break;
            }
          return str;
        }

        bool
        is_in_irq (void)
        {
          // TODO

          return false;
        }

      } /* namespace kernel */

      // ======================================================================

      static Systick_clock::rep __systick_now = 12300;

      Systick_clock::rep
      Systick_clock::now (void)
      {
        return __systick_now;
      }

      Systick_clock::rep
      Systick_clock::now (current_t* details)
      {
        assert(details != nullptr);

        details->core_frequency_hz = 168000000;
        details->divisor = details->core_frequency_hz / frequency_hz;
        details->cycles = details->divisor - 1;
        details->ticks = __systick_now;

        return __systick_now;
      }

      /**
       * @details
       * Put the current thread to sleep, until the next n-th
       * SysTick occurs. Depending when the call is issued, the
       * first tick counted may be very short.
       */
      result_t
      Systick_clock::sleep_for (Systick_clock::sleep_rep ticks)
      {
        trace::printf ("Systick_clock::sleep_for(%d_ticks)\n", ticks);
        __systick_now += ticks;
        return result::ok;
      }

      static Realtime_clock::rep __rtc_now = 1000000;

      Realtime_clock::rep
      Realtime_clock::now (void)
      {
        return __rtc_now;
      }

      /**
       * @details
       * Put the current thread to sleep, until the next n-th
       * RTC second occurs. Depending when the call is issued, the
       * first second counted may be very short.
       */
      result_t
      Realtime_clock::sleep_for (Realtime_clock::sleep_rep secs)
      {
        trace::printf ("Realtime_clock::sleep_for(%ds)\n", secs);
        __rtc_now += secs;
        return result::ok;
      }

      namespace scheduler
      {
        status_t sched_running = false;

        /**
         *
         */
        result_t
        start (void)
        {
          // TODO
          sched_running = true;
          return result::ok;
        }

        bool
        is_running (void)
        {
          return sched_running;
        }

        status_t
        lock (void)
        {
          status_t tmp = sched_running;
          sched_running = false;
          return tmp;
        }

        status_t
        unlock (status_t status)
        {
          status_t tmp = sched_running;
          sched_running = status;
          return tmp;
        }

        void
        __register_thread (Thread* thread)
        {
          // TODO
#if defined(TESTING)
          thread->__run_function ();
#endif
        }

        void
        __unregister_thread (Thread* thread)
        {
          // TODO
        }

      } /* namespace scheduler */

      namespace critical
      {
        // Enter an IRQ critical section
        status_t
        enter (void)
        {
          return 0;
        }

        // Exit an IRQ critical section
        status_t
        exit (status_t status)
        {
          return 0;
        }
      }

      // ======================================================================

      void*
      no_thread_func (void* args);

      void*
      no_thread_func (void* args)
      {
        return nullptr;
      }

      Thread no_thread
        { nullptr, (thread::func_t) no_thread_func, nullptr };

      namespace this_thread
      {
        /**
         *
         */
        Thread&
        get (void)
        {
          // TODO
          return no_thread;
        }

        /**
         * @details
         * Pass control to next thread that is in \b READY state.
         */
        result_t
        yield (void)
        {
          assert(!kernel::is_in_irq ());
          // TODO
          return result::ok;
        }

        // Legacy
        result_t
        wait (millis_t millisec, event_t* ret)
        {
          // TODO
          return result::ok;
        }

        // Legacy
        result_t
        wait_signals (signal_flags_t signals, millis_t millisec,
                      signal_flags_t* ret)
        {
          // TODO
          return result::ok;
        }

      } /* namespace this_thread */

      // ======================================================================

      Named_object::Named_object (const char* name) :
          name_ (name != nullptr ? name : "-")
      {
        ;
      }

      // ======================================================================

      namespace thread
      {
        const Attributes initializer
          { nullptr };
      } /* namespace thread */

      /**
       * @details
       *
       * Create a new thread, with default attributes.
       *
       * The thread is created executing function with args as its
       * sole argument. If the start_routine returns, the effect
       * shall be as if there was an implicit call to exit() using
       * the return value of function as the exit status. Note that
       * the thread in which main() was originally invoked differs
       * from this. When it returns from main(), the effect shall
       * be as if there was an implicit call to exit() using the
       * return value of main() as the exit status.
       *
       * Compatible with pthread_create().
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html
       */
      Thread::Thread (thread::func_t function, void* args) :
          Thread (thread::initializer, function, args)
      {
        ;
      }

      /**
       * @details
       *
       * Create a new thread, with attributes specified by attr.
       * If attr is NULL, the default attributes shall be used.
       * If the attributes specified by attr are modified later,
       * the thread's attributes shall not be affected.
       *
       * The thread is created executing function with args as its
       * sole argument. If the start_routine returns, the effect
       * shall be as if there was an implicit call to exit() using
       * the return value of function as the exit status. Note that
       * the thread in which main() was originally invoked differs
       * from this. When it returns from main(), the effect shall
       * be as if there was an implicit call to exit() using the
       * return value of main() as the exit status.
       *
       * Compatible with pthread_create().
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html
       */
      Thread::Thread (const thread::Attributes& attr, thread::func_t function,
                      void* args) :
          Named_object (attr.get_name ())
      {
        assert(function != nullptr);
        assert(!kernel::is_in_irq ());

        // Get attributes from user structure.
        attr.get_priority (&prio_);
        // TODO: check min size
        attr.get_stack_size_bytes (&stack_size_bytes_);
        // TODO: align stack
        attr.get_stack_address (&stack_addr_);

        if (stack_addr_ == nullptr)
          {
            // TODO: alloc default stack size
          }

        func_ = function;
        func_args_ = args;

        trace::printf ("%s(\"%s\", %d) @%p \n", __func__, get_name (),
                       stack_size_bytes_, this);

        scheduler::__register_thread (this);
      }

      /**
       *
       */
      Thread::~Thread ()
      {
        trace::printf ("%s() @%p \n", __func__, this);
        scheduler::__register_thread (this);
      }

      /**
       * No POSIX equivalent.
       */
      thread::priority_t
      Thread::get_sched_prio (void)
      {
        return prio_;
      }

      /**
       * @details
       * Set the scheduling priority for the thread to the value given
       * by prio.
       *
       * pthread_setschedprio()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_setschedprio.html
       *
       * If an implementation detects use of a thread ID after the end
       * of its lifetime, it is recommended that the function should
       * fail and report an [ESRCH] error.
       *
       * The pthread_setschedprio() function shall not return an error
       * code of [EINTR].
       */
      result_t
      Thread::set_sched_prio (thread::priority_t prio)
      {
        prio_ = prio;
        return result::ok;
      }

      /**
       * @details
       * Suspend execution of the calling thread until the target thread
       * terminates, unless the target thread has already terminated.
       * On return from a successful join() call with a non-NULL
       * exit_ptr argument, the value passed to exit() by the
       * terminating thread shall be made available in the location
       * referenced by exit_ptr. When a join() returns successfully,
       * the target thread has been terminated. The results of
       * multiple simultaneous calls to join() specifying the
       * same target thread are undefined. If the thread calling
       * join() is cancelled, then the target thread shall not be
       * detached.
       *
       * pthread_join()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html
       *
       * @note Cannot be called from Interrupt Service Routines.
       *
       * The join() function may fail if:
       * [EDEADLK] A deadlock was detected.
       *
       * The join() function shall not return an error code of [EINTR].
       */
      result_t
      Thread::join (void** exit_ptr)
      {
        assert(!kernel::is_in_irq ());
        // TODO
        return result::ok;
      }

      /**
       * @details
       * Indicate to the implementation that storage for the thread
       * thread can be reclaimed when that thread terminates. If
       * thread has not terminated, detach() shall not cause it
       * to terminate. The behaviour is undefined if the value
       * specified by the thread argument to detach() does not
       * refer to a joinable thread.
       *
       * pthread_detach()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_detach.html
       *
       * @note Cannot be called from Interrupt Service Routines.
       *
       * The detach() function shall not return an error code of [EINTR].
       */
      result_t
      Thread::detach (void)
      {
        assert(!kernel::is_in_irq ());
        // TODO
        return result::ok;
      }

      /**
       * @details
       * pthread_cancel()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cancel.html
       *
       *
       * The cancel() function shall not return an error code of [EINTR].
       * If an implementation detects use of a thread ID after the end
       * of its lifetime, it is recommended that the function should
       * fail and report an [ESRCH] error.
       * error number is returned.
       *
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Thread::cancel (void)
      {
        assert(!kernel::is_in_irq ());
        // TODO
        return result::ok;
      }

      /**
       * @details
       * Terminate the calling thread and make the value value_ptr
       * available to any successful join with the terminating thread.
       * Any cancellation cleanup handlers that have been pushed and
       * not yet popped shall be popped in the reverse order that
       * they were pushed and then executed. After all cancellation
       * cleanup handlers have been executed, if the thread has any
       * thread-specific data, appropriate destructor functions
       * shall be called in an unspecified order. Thread termination
       * does not release any application visible process resources,
       * including, but not limited to, mutexes and file descriptors,
       * nor does it perform any process-level cleanup actions,
       * including, but not limited to, calling any atexit() routines
       * that may exist.
       * An implicit call to exit() is made when a thread other
       * than the thread in which main() was first invoked returns
       * from the start routine that was used to create it.
       * The function's return value shall serve as the thread's
       * exit status.
       * The behaviour of exit() is undefined if called from a
       * cancellation cleanup handler or destructor function that
       * was invoked as a result of either an implicit or explicit
       * call to exit().
       * After a thread has terminated, the result of access to
       * local (auto) variables of the thread is undefined.
       * Thus, references to local variables of the exiting
       * thread should not be used for the exit() value_ptr
       * parameter value.
       *
       * pthread_exit()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_exit.html
       *
       * @note Cannot be called from Interrupt Service Routines.
       */
      void
      Thread::exit (void* value_ptr)
      {
        assert(!kernel::is_in_irq ());
        // TODO
      }

#if defined(TESTING)
      void
      Thread::__run_function (void)
      {
        assert(func_ != nullptr);
        func_ (func_args_);
      }
#endif

      // ======================================================================

      Timer::Timer (const char* name, timer::func_t function,
                    timer::type_t type, void* args) : //
          Named_object (name)
      {
        assert(!kernel::is_in_irq ());
        // TODO
      }

      Timer::~Timer ()
      {
        // TODO
      }

      result_t
      Timer::start (millis_t millisec)
      {
        assert(!kernel::is_in_irq ());
        // TODO
        return result::ok;
      }

      result_t
      Timer::stop (void)
      {
        assert(!kernel::is_in_irq ());
        // TODO
        return result::ok;
      }

      // ======================================================================

      namespace mutex
      {
        const Attributes normal_initializer
          { nullptr };

        const Recursive_attributes recursive_initializer
          { nullptr };
      } /* namespace mutex */

      /**
       * @details
       * Initialise the mutex with default attributes.
       *
       * pthread_mutex_init()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_init.html
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
       */
      Mutex::Mutex (const mutex::Attributes& attr) :
          Named_object (attr.get_name ()), //
          type_ (([&attr]()->mutex::type_t
            { mutex::type_t type; attr.get_type (&type); return type;}) ()), //
          protocol_ (
              ([&attr]()->mutex::protocol_t
                { mutex::protocol_t protocol; attr.get_protocol (&protocol); return protocol;}) ()), //
          robustness_ (
              ([&attr]()->mutex::robustness_t
                { mutex::robustness_t robustness; attr.get_robustness (&robustness); return robustness;}) ())
      {
        assert(!kernel::is_in_irq ());

        attr.get_prio_ceiling (
            const_cast<thread::priority_t*> (&prio_ceiling_));
        owner_ = nullptr;
        count_ = 0;

        trace::printf ("%s() @%p \n", __func__, this);
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
       */
      Mutex::~Mutex ()
      {
        trace::printf ("%s() @%p \n", __func__, this);
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
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Mutex::lock (void)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
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
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Mutex::try_lock (void)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
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
       * The timeout shall expire after the given number of system ticks.
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
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Mutex::timed_lock (systicks_t ticks)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s(%d_ticks) @%p \n", __func__, ticks, this);
        // TODO
        return result::ok;
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
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Mutex::unlock (void)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
      }

      /**
       * @details
       * Return the current priority ceiling of the mutex.
       *
       * pthread_mutex_getprioceiling()
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_mutex_getprioceiling.html
       *
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Mutex::get_prio_ceiling (thread::priority_t* prio_ceiling) const
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        if (prio_ceiling != nullptr)
          {
            *prio_ceiling = prio_ceiling_;
          }
        return result::ok;
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
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Mutex::set_prio_ceiling (thread::priority_t prio_ceiling,
                               thread::priority_t* old_prio_ceiling)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
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
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Mutex::consistent (void)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
      }

      // ======================================================================

      namespace cond
      {
        const Attributes initializer
          { nullptr };
      } /* namespace cond */

      /**
       * @details
       */
      Condition_variable::Condition_variable () :
          Condition_variable (cond::initializer)
      {
        ;
      }

      /**
       * @details
       */
      Condition_variable::Condition_variable (const cond::Attributes& attr) :
          Named_object (attr.get_name ())
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
      }

      /**
       * @details
       * It shall be safe to destroy an initialised condition variable
       * upon which no threads are currently blocked. Attempting to
       * destroy a condition variable upon which other threads are
       * currently blocked results in undefined behaviour.
       */
      Condition_variable::~Condition_variable ()
      {
        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
      }

      /**
       * @details
       * Unblock at least one of the threads that are blocked
       * on the specified condition variable.
       *
       * When each thread unblocked as a result of a
       * Condition_variable::signal() returns from its call to
       * Condition_variable::wait() or Condition_variable::timedwait(),
       * the thread shall own the mutex with which it called
       * Condition_variable::wait() or Condition_variable::timed_wait().
       * The thread(s) that are unblocked shall contend for
       * the mutex according to the scheduling policy (if applicable),
       * and as if each had called Mutex::lock().
       *
       * Condition_variable::signal() may be called by a thread
       * whether or not it currently owns the mutex that threads
       * calling Condition_variable::wait() or
       * Condition_variable::timed_wait() have associated with
       * the condition variable during their waits; however,
       * if predictable scheduling behavior is required, then
       * that mutex shall be locked by the thread calling
       * Condition_variable::broadcast().
       *
       * The Condition_variable::signal() function shall
       * have no effect if there are no threads currently
       * blocked on this condition variable.
       *
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Condition_variable::signal ()
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
      }

      /**
       * @details
       * Unblock all threads currently blocked on the specified
       * condition variable.
       *
       * If more than one thread is blocked on a condition variable,
       * the scheduling policy shall determine the order in which
       * threads are unblocked.
       *
       * When each thread unblocked as a result of a
       * Condition_variable::broadcast() returns from its call to
       * Condition_variable::wait() or Condition_variable::timed_wait(),
       * the thread shall own the mutex with which it called
       * Condition_variable::wait() or Condition_variable::timed_wait().
       * The thread(s) that are unblocked shall contend for
       * the mutex according to the scheduling policy (if applicable),
       * and as if each had called Mutex::lock().
       *
       * Condition_variable::broadcast() may be called by a thread
       * whether or not it currently owns the mutex that threads
       * calling Condition_variable::wait() or
       * Condition_variable::timed_wait() have associated with
       * the condition variable during their waits; however,
       * if predictable scheduling behaviour is required, then
       * that mutex shall be locked by the thread calling
       * Condition_variable::broadcast().
       *
       * The Condition_variable::broadcast() function shall
       * have no effect if there are no threads currently
       * blocked on this condition variable.
       *
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Condition_variable::broadcast ()
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
      }

      /**
       * @details
       * Block on a condition variable. The application shall ensure
       * that this function is called with mutex locked by
       * the calling thread; otherwise, an error (for
       * PTHREAD_MUTEX_ERRORCHECK and robust mutexes) or
       * undefined behaviour (for other mutexes) results.
       *
       * TODO: add more.
       *
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Condition_variable::wait (Mutex* mutex)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
      }

      /**
       * @details
       * Block on a condition variable. The application shall ensure
       * that this function is called with mutex locked by
       * the calling thread; otherwise, an error (for
       * ERRORCHECK and robust mutexes) or
       * undefined behaviour (for other mutexes) results.
       *
       * TODO: add more.
       *
       * @note Cannot be called from Interrupt Service Routines.
       */
      result_t
      Condition_variable::timed_wait (Mutex* mutex, systicks_t ticks)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s(%d_ticks) @%p \n", __func__, ticks, this);
        // TODO
        return result::ok;
      }

      // ======================================================================

      namespace semaphore
      {
        const Attributes counting_initializer
          { nullptr };

        const Binary_attributes binary_initializer
          { nullptr };

      } /* namespace semaphore */

      /**
       * @details
       * Create and initialise a semaphore with default attributes.
       * Initialise the semaphore with count=0 and no limit for count.
       *
       * Compatible with POSIX `sem_init()`.
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html#
       */
      Semaphore::Semaphore () :
          Semaphore (semaphore::counting_initializer)
      {
        ;
      }

      /**
       * @details
       * Create and initialise a semaphore with custom attributes.
       *
       * Compatible with POSIX `sem_init()`.
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_init.html#
       */
      Semaphore::Semaphore (const semaphore::Attributes& attr) :
          Named_object (attr.get_name ()), //
          max_count_ (
              ([&attr]()->semaphore::count_t
                { semaphore::count_t count; attr.get_max_count (&count); return count;}) ())
      {
        attr.get_intial_count (const_cast<semaphore::count_t*> (&count_));
      }

      /**
       * @details
       * It is safe to destroy an initialised semaphore upon which
       * no threads are currently blocked. The effect of destroying
       * a semaphore upon which other threads are currently blocked
       * is undefined.
       *
       * Compatible with POSIX `sem_destroy()`.
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_destroy.html
       */
      Semaphore::~Semaphore ()
      {
        ;
      }

      /**
       * @details
       * Performe a post operation on the semaphore, informing
       * the waiting consumers that one more resource is available.
       * The semaphore count is incremented, up to max_count.
       *
       * If the semaphore count resulting from this operation is
       * positive, then no threads were blocked waiting for the
       * semaphore to become unlocked.
       *
       * If the count of the semaphore resulting from this operation
       * is zero, then one of the threads blocked waiting for the
       * semaphore shall be allowed to return successfully from its
       * call to Semaphore::wait().
       *
       * If the Process Scheduling option is supported, the thread
       * to be unblocked shall be chosen in a manner appropriate to
       * the scheduling policies and parameters in effect for the
       * blocked threads. In the case of the schedulers SCHED_FIFO
       * and SCHED_RR, the highest priority waiting thread shall
       * be unblocked, and if there is more than one highest
       * priority thread blocked waiting for the semaphore, then
       * the highest priority thread that has been waiting the
       * longest shall be unblocked. If the Process Scheduling
       * option is not defined, the choice of a thread to unblock
       * is unspecified. If the scheduling policy is SCHED_SPORADIC,
       * the semantics are as per SCHED_FIFO.
       *
       * The function may be invoked from signal catching functions or
       * Interrupt Service Routines.
       *
       * Compatible with POSIX `sem_post()`
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_post.html
       */
      result_t
      Semaphore::post (void)
      {
        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        ++count_;
        if (count_ == 0)
          {
            // wakeup one thread
          }
        return result::ok;
      }

      /**
       * @details
       * Perform a lock operation on the semaphore.
       *
       * If the current value is positive, it is decremented, and
       * the call returns immediately
       *
       * If the semaphore
       * value is currently zero, then the calling thread shall not
       * return from the call to Semaphore::wait() until it either
       * locks the semaphore or the call is interrupted by a signal.
       *
       * Upon successful return, the state of the semaphore shall
       * be locked and shall remain locked until the Semaphore::post()
       * function is executed and returns successfully.
       *
       * The function is interruptible by the delivery of an external
       * event (signal, thread cancel, etc).
       *
       * Compatible with POSIX `sem_wait()`.
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_wait.html
       */
      result_t
      Semaphore::wait ()
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);

        // Critical section block.
          {
            Critical_section_irq cs;

            // TODO
            --count_;
            if (count_ >= 0)
              {
                return result::ok;
              }

            // TODO: Add current thread to the semaphore waiting  list
          }
        this_thread::yield ();

        return result::ok;
      }

      /**
       * @details
       * Tries to perform a lock operation only if the semaphore is currently
       * not locked; that is, if the semaphore value is currently
       * positive it decrements it and returns success.
       * Otherwise, it shall not lock the semaphore.
       *
       * Upon successful return, the state of the semaphore shall
       * be locked and shall remain locked until the Semaphore::post()
       * function is executed and returns successfully.
       *
       * The function is interruptible by the delivery of an external
       * event (signal, thread cancel, etc).
       *
       * Compatible with POSIX `sem_trywait()`.
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_wait.html
       */
      result_t
      Semaphore::try_wait ()
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s() @%p \n", __func__, this);
        // TODO
        return result::ok;
      }

      /**
       * @details
       * Try to perform a lock operation; if the semaphore cannot be
       * locked without waiting for another process or thread to
       * unlock the semaphore by performing a Semaphore::post()
       * function, this wait shall be terminated when the specified
       * timeout expires.
       *
       * The timeout shall expire after the number of time units (that
       * is when the value of that clock equals or exceeds (now()+duration).
       * The resolution of the timeout shall be the resolution of the
       * clock on which it is based (the SysTick clock for CMSIS).
       *
       * Under no circumstance shall the function fail with a timeout
       * if the semaphore can be locked immediately.
       *
       * Compatible with POSIX `sem_timedwait()`.
       * http://pubs.opengroup.org/onlinepubs/9699919799/functions/sem_timedwait.html
       */
      result_t
      Semaphore::timed_wait (systicks_t ticks)
      {
        assert(!kernel::is_in_irq ());

        trace::printf ("%s(%d_ticks) @%p \n", __func__, ticks, this);
        // TODO
        return result::ok;
      }

      result_t
      Semaphore::get_value (semaphore::count_t* value)
      {
        if (value != nullptr)
          {
            *value = count_;
          }
        return result::ok;
      }

      // ======================================================================

      Pool::Pool (const char* name, std::size_t items, std::size_t item_size,
                  void* mem) :
          Named_object (name)
      {
        // TODO
      }

      Pool::~Pool ()
      {
        // TODO
      }

      void*
      Pool::alloc (void)
      {
        // TODO
        return nullptr;
      }

      void*
      Pool::calloc (void)
      {
        // TODO
        return nullptr;
      }

      result_t
      Pool::free (void* block)
      {
        // TODO
        return result::ok;
      }

      // ======================================================================

      Message_queue::Message_queue (const char* name, std::size_t items,
                                    void* mem, Thread* thread) :
          Named_object (name)
      {
        // TODO
      }

      Message_queue::~Message_queue ()
      {
        // TODO
      }

      result_t
      Message_queue::put (void* info, millis_t millisec)
      {
        // TODO
        return result::ok;
      }

      result_t
      Message_queue::get (millis_t millisec, void** ret)
      {
        // TODO
        return result::ok;
      }

      // ======================================================================

      Mail_queue::Mail_queue (const char* name, std::size_t messages,
                              std::size_t message_size, void* mem,
                              Thread* thread) :
          Named_object (name)
      {
        // TODO
      }

      Mail_queue::~Mail_queue ()
      {
        // TODO
      }

      void*
      Mail_queue::alloc (millis_t millisec)
      {
        // TODO
        return nullptr;
      }

      void*
      Mail_queue::calloc (millis_t millisec)
      {
        // TODO
        return nullptr;
      }

      result_t
      Mail_queue::put (void* mail)
      {
        // TODO
        return result::ok;
      }

      result_t
      Mail_queue::get (millis_t millisec, void** ret)
      {
        // TODO
        return result::ok;
      }

      result_t
      Mail_queue::free (void* mail)
      {
        // TODO
        return result::ok;
      }

// ======================================================================

#pragma GCC diagnostic pop

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */
