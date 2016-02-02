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
        result_t
        initialize (void)
        {
          // TODO
          return result::ok;
        }

        result_t
        start (void)
        {
          // TODO
          return result::ok;
        }

        bool
        is_running (void)
        {
          // TODO
          return true;
        }

        uint64_t
        get_current_systick (current_systick_t* details)
        {
          // TODO
          return 1234;
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

      } /* namespace kernel */

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

      namespace thread
      {
        Thread&
        get_current (void)
        {
          // TODO
          return no_thread;
        }

        result_t
        yield (void)
        {
          // TODO
          return result::ok;
        }

        result_t
        wait (millis_t millisec, event_t* ret)
        {
          // TODO
          return result::ok;
        }

        result_t
        wait_signals (signal_flags_t signals, millis_t millisec,
                      signal_flags_t* ret)
        {
          // TODO
          return result::ok;
        }

#if 0
        result_t
        delay (millis_t millisec)
          {
            // TODO
            return result::ok;
          }
#endif

        result_t
        sleep (sys_ticks_t ticks)
        {
          // TODO
          return result::ok;
        }

      }

      // ======================================================================

      Named_object::Named_object (const char* name) :
          name_ (name != nullptr ? name : "-")
      {
        ;
      }

      // ======================================================================

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
      Thread::Thread (const thread::attr_t* attr, thread::func_t function,
                      void* args) :
          Named_object (attr != nullptr ? attr->name : nullptr)
      {
        assert(function != nullptr);

        if (attr != nullptr)
          {
            // Get attributes from user structure.
            prio_ = attr->priority;
            // TODO: check min size
            stack_size_bytes_ = attr->stack_size_bytes;
            // TODO: align stack
            stack_addr_ = attr->stack_addr;
          }
        else
          {
            // Default attributes.
            prio_ = thread::priority::normal;
            stack_size_bytes_ = 0;
            stack_addr_ = nullptr;
          }

        if (stack_addr_ == nullptr)
          {
            // TODO: alloc default stack size
          }

        func_ = function;
        func_args_ = args;

        trace::printf ("%s(\"%s\", %d) @%p \n", __func__, get_name (),
                       stack_size_bytes_, this);
      }

      Thread::~Thread ()
      {
        trace::printf ("%s() @%p \n", __func__, this);
      }

      /**
       *
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
       */
      result_t
      Thread::join (void** exit_ptr)
      {
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
       */
      result_t
      Thread::detach (void)
      {
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
        // TODO
      }

      Timer::~Timer ()
      {
        // TODO
      }

      result_t
      Timer::start (millis_t millisec)
      {
        // TODO
        return result::ok;
      }

      result_t
      Timer::stop (void)
      {
        // TODO
        return result::ok;
      }

      // ======================================================================

      namespace mutex
      {
        const attr_t normal_initializer
          {
          //
              nullptr,//
              thread::priority::max, //
              protocol::none, //
              robustness::stalled, //
              type::normal //
          };
        const attr_t recursive_initializer
          {
          //
              nullptr,//
              thread::priority::max, //
              protocol::none, //
              robustness::stalled, //
              type::recursive //
          };
      }

      Mutex::Mutex (const mutex::attr_t* attr) :
          Named_object (attr != nullptr ? attr->name : nullptr)
      {
        // TODO
        if (attr == nullptr)
          {
            attr = &mutex::normal_initializer;
          }

        prio_ceiling_ = attr->priority_ceiling;
        protocol_ = attr->protocol;
        robustness_ = attr->robustness;
        type_ = attr->type;
      }

      Mutex::~Mutex ()
      {
        // TODO
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
      Mutex::timed_lock (sys_ticks_t ticks)
      {
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
      Mutex::get_prio_ceiling (thread::priority_t* prio_ceiling)
      {
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
        return result::ok;
      }

      // ======================================================================

      Recursive_mutex::Recursive_mutex (const char* name) :
          Named_object (name)
      {
        // TODO
      }

      Recursive_mutex::~Recursive_mutex ()
      {
        // TODO
      }

      result_t
      Recursive_mutex::wait (void)
      {
        return result::ok;
      }

      result_t
      Recursive_mutex::try_wait (sys_ticks_t ticks)
      {
        return result::ok;
      }

      result_t
      Recursive_mutex::release (void)
      {
        return result::ok;
      }

      // ======================================================================

      Condition_variable::Condition_variable (const char* name) :
          Named_object (name)
      {
        // TODO
      }

      Condition_variable::~Condition_variable ()
      {
        // TODO
      }

      result_t
      Condition_variable::notify_one () noexcept
      {
        // TODO
        return result::ok;
      }

      result_t
      Condition_variable::notify_all () noexcept
      {
        // TODO
        return result::ok;
      }

      // ======================================================================

      Semaphore::Semaphore (const char* name, int32_t count, uint32_t max_count) :
          Named_object (name)
      {
        // TODO
      }

      Semaphore::~Semaphore ()
      {
        // TODO
      }

      int32_t
      Semaphore::wait (millis_t millisec)
      {
        return result::ok;
      }

      result_t
      Semaphore::release (void)
      {
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
