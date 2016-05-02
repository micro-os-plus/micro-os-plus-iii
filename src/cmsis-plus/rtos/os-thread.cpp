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

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    // ------------------------------------------------------------------------

    /**
     * @details
     * The os::rtos::stack namespace groups declarations related to
     * the thread stack.
     */
    namespace stack
    {

    } /* namespace stack */

    /**
     * @details
     * The os::rtos::this_thread namespace groups functions related to
     * the current thread.
     */
    namespace this_thread
    {

      Thread*
      _thread (void)
      {
        Thread* th;

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

        th = port::this_thread::thread ();

#else

        th = scheduler::current_thread_;

#endif
        return th;
      }

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      Thread&
      thread (void)
      {
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

        Thread* th;

        th = _thread ();

        assert(th != nullptr);
        return (*th);
      }

      /**
       * @details
       * Pass control to next thread that is in \b READY state.
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      void
      yield (void)
      {
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

        if (!scheduler::started ())
          {
            return;
          }

        return port::this_thread::yield ();
      }

    } /* namespace this_thread */

    // ======================================================================

    /**
     * @details
     * The os::rtos::thread namespace groups thread types, enumerations,
     * attributes and initialisers.
     */
    namespace thread
    {
      /**
       * @class Attributes
       * @details
       * Allow to assign a name and custom attributes (like stack address,
       * stack size, priority) to the thread.
       *
       * To simplify access, the member variables are public and do not
       * require accessors or mutators.
       *
       * @par POSIX compatibility
       *  Inspired by `pthread_attr_t` from [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
       *  (IEEE Std 1003.1, 2013 Edition).
       */

      const Attributes initializer
        { nullptr };

    } /* namespace thread */

    /**
     * @class Thread
     * @details
     * CMSIS++ threads are inspired by POSIX threads and support
     * functions that terminate and a simplified version of
     * signal flags.
     *
     * @par Example
     *
     * @code{.cpp}
     * #include <cmsis-plus/rtos/os.h>
     * #include <cstdlib>
     *
     * using namespace os::rtos;
     *
     * // Thread function.
     * void*
     * func(void* args)
     * {
     *   // Do something.
     *   ...
     *   return nullptr;
     * }
     *
     * int
     * os_main(int argc, char* argv[])
     * {
     *   // Create new thread, with function and no arguments.
     *   Thread th { func, nullptr };
     *
     *   // Do something.
     *
     *   // Wait for thread to terminate.
     *   th.join();
     *
     *   return 0;
     * }
     * @endcode
     *
     * @par POSIX compatibility
     *  Inspired by `pthread_t`
     *  from [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     */

    /**
     * @details
     * Same as in POSIX, thread functions can return, and the behaviour
     * should be as the thread called the exit() function.
     *
     * This requires a proxy to run the thread function, get the result,
     * and explicitly invoke exit().
     */
    void
    Thread::_invoke_with_exit (Thread* thread)
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s @%p %s\n", __func__, thread, thread->name ());
#endif
      thread->_exit (thread->func_ (thread->func_args_));
    }

    /**
     * @details
     * This constructor shall initialise the thread object
     * with default settings.
     * The effect shall be equivalent to creating a thread object
     * referring to the attributes in `thread::initializer`.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * condition variable objects.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     *
     * For default thread objects, the stack is dynamically allocated,
     * using the default size.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Thread::Thread (thread::func_t function, thread::func_args_t args) :
        Thread
          { thread::initializer, function, args }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise the thread object
     * with attributes referenced by _attr_.
     * If the attributes specified by _attr_ are modified later,
     * the thread attributes shall not be affected.
     * Upon successful initialisation, the state of the
     * thread object shall become initialised, and the thread is
     * added to the ready list.
     *
     * Only the thread object itself may be used for running the
     * function. It is not allowed to make copies of
     * thread objects.
     *
     * In cases where default thread attributes are
     * appropriate, the variable `thread::initializer` can be used to
     * initialise threads.
     * The effect shall be equivalent to creating a thread
     * object with the default constructor.
     *
     * The thread is created to execute _function_ with _args_ as its
     * sole argument. If the function returns, the effect
     * shall be as if there was an implicit call to `exit()` using
     * the return value of function as the exit status. Note that
     * the thread in which `main()` was originally invoked differs
     * from this. When it returns from `main()`, the effect shall
     * be as if there was an implicit call to `exit()` using the
     * return value of `main()` as the exit status.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Thread::Thread (const thread::Attributes& attr, thread::func_t function,
                    thread::func_args_t args) :
        Named_object
          { attr.name () }
#if !defined(OS_INCLUDE_RTOS_PORT_THREAD)
            , ready_node_
              { *this }
#endif
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      assert(function != nullptr);
      assert(attr.th_priority != thread::priority::none);

      context_.stack_.size_bytes_ = attr.th_stack_size_bytes;
      context_.stack_.bottom_address_ = attr.th_stack_address;

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s @%p %s %d %d\n", __func__, this, name (), prio_,
                     context_.stack_.size_bytes_);
#endif

        {
          // Prevent the new thread to execute before all members are set.
          scheduler::Critical_section cs; // ----- Critical section -----

          // Get attributes from user structure.
          prio_ = attr.th_priority;

          func_ = function;
          func_args_ = args;
          func_result_ = nullptr;

          sig_mask_ = 0;

          joiner_ = nullptr;
          waiting_node_ = nullptr;

          acquired_mutexes_ = 0;

          clock_node_ = nullptr;

          parent_ = this_thread::_thread ();
          if (parent_ != nullptr)
            {
              parent_->children_.link (*this);
            }
          else
            {
              scheduler::top_threads_list_.link (*this);
            }

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

          port::Thread::create (this);
          sched_state_ = thread::state::ready;

#else

          // TODO: check min size
          // TODO: align stack
          if (context_.stack_.bottom () == nullptr)
            {
              // TODO: alloc default stack size
            }

          // Create context
          port::thread::Context::create (
              &context_, reinterpret_cast<void*> (_invoke_with_exit), this);

          // Allow for next resume()
          resume ();

#endif

        }

      // For just in case the new thread has higher priority.
      this_thread::yield ();
    }

    /**
     * @details
     * This destructor shall destroy the thread object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * If the thread was created with dynamic stack, it is freed.
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    Thread::~Thread ()
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s @%p %s \n", __func__, this, name ());
#endif

      // Prevent the main thread to destroy itself while running
      // the exit cleanup code.
      if (this != &this_thread::thread ())
        {
          kill ();
        }
    }

#if 0
    void
    Thread::_destroy (void)
      {

#if defined(OS_TRACE_RTOS_THREAD)
        trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

          {
            interrupts::Critical_section ics; // ----- Critical section -----

            if (sched_state_ == thread::state::destroyed)
              {
                return;
              }

#if !defined(OS_INCLUDE_RTOS_PORT_THREAD)
            ready_node_.unlink ();
#endif

            child_links_.unlink ();
            assert(children_.empty ());
            parent_ = nullptr;

            assert(acquired_mutexes_ == 0);

            sched_state_ = thread::state::destroyed;
            trace::puts (">");
          }

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

        port::Thread::destroy (this);
        // Does not return if the current thread.

#else

        // port::Thread::clean (this);

        // Pass control to the next thread.
        // If current, skip saving current context.
        if (this != scheduler::current_thread_)
          {
            this_thread::yield ();
          }
        else
          {
            port::scheduler::reschedule (false);
          }

        assert(true);
#endif
      }
#endif

    /**
     * @details
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    void
    Thread::_wait (void)
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          // Remove this thread from the ready list, if there.
          port::this_thread::prepare_suspend ();
        }
      port::scheduler::reschedule ();

    }

    /**
     * @details
     * Internal, used in the implementation of synchronisation objects.
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    void
    Thread::resume (void)
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s %d\n", __func__, this, name (), prio_);
#endif

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          sched_state_ = thread::state::ready;
          port::Thread::resume (this);
        }

#else

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          if (ready_node_.next == nullptr)
            {
              scheduler::ready_threads_list_.link (ready_node_);
              // Ready state set in above link().
            }
        }

#endif

    }

    /**
     * @details
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    thread::priority_t
    Thread::sched_prio (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), thread::priority::error);

      // trace::printf ("%s() @%p %s\n", __func__, this, name ());
      return prio_;
    }

    /**
     * @details
     * Set the scheduling priority for the thread to the value given
     * by _prio_.
     *
     * If an implementation detects use of a thread ID after the end
     * of its lifetime, it is recommended that the function should
     * fail and report an `ESRCH` error.
     *
     * The `sched_prio()` function shall not return an error
     * code of `EINTR`.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_setschedprio()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_setschedprio.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::sched_prio (thread::priority_t prio)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);
      os_assert_err(prio < thread::priority::error, EINVAL);
      os_assert_err(prio != thread::priority::none, EINVAL);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(%d) @%p %s\n", __func__, prio, this, name ());
#endif

      prio_ = prio;

      result_t res = result::ok;

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

      // The port must perform a context switch.
      res = port::Thread::sched_prio (this, prio);

#else

      if (sched_state_ == thread::state::ready)
        {
          interrupts::Critical_section ics; // ----- Critical section -----

          // Remove from initial location and reinsert according
          // to new priority.
          ready_node_.unlink ();
          scheduler::ready_threads_list_.link (ready_node_);
        }

      // Mandatory, the priority might have been raised, the
      // task must be scheduled to run.
      this_thread::yield ();

#endif

      return res;
    }

    /**
     * @details
     * Suspend execution of the calling thread until the target thread
     * terminates, unless the target thread has already terminated.
     * On return from a successful `join()` call with a non-NULL
     * _exit_ptr_ argument, the value passed to exit() by the
     * terminating thread shall be made available in the location
     * referenced by _exit_ptr_. When a `join()` returns successfully,
     * the target thread has been terminated. The results of
     * multiple simultaneous calls to `join()` specifying the
     * same target thread are undefined. If the thread calling
     * `join()` is cancelled, then the target thread shall not be
     * detached.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_join()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_join.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     *
     * The join() function may fail if:
     * [EDEADLK] A deadlock was detected.
     *
     * The `join()` function shall not return an error code of [EINTR].
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::join (void** exit_ptr)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      // TODO: Must fail if current thread

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      while ((sched_state_ != thread::state::terminated)
          && (sched_state_ != thread::state::destroyed))
        {
          joiner_ = this;
          _wait ();
        }

      if (exit_ptr != nullptr)
        {
          *exit_ptr = func_result_;
        }
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s joined\n", __func__, this, name ());
#endif

      return result::ok;
    }

    /**
     * @details
     * Indicate to the implementation that storage for the thread
     * thread can be reclaimed when that thread terminates. If
     * thread has not terminated, `detach()` shall not cause it
     * to terminate. The behaviour is undefined if the value
     * specified by the thread argument to `detach()` does not
     * refer to a joinable thread.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_detach()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_detach.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * The `detach()` function shall not return an error code of `EINTR`.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::detach (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

      result_t res = port::Thread::detach (this);
      if (res != result::ok)
        {
          return res;
        }

#else

      // TODO

#endif

      return result::ok;
    }

    /**
     * @details
     *
     * The `cancel()` function shall not return an error code of `EINTR`.
     * If an implementation detects use of a thread ID after the end
     * of its lifetime, it is recommended that the function should
     * fail and report an `ESRCH` error.
     * error number is returned.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cancel()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cancel.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::cancel (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      // TODO
      return result::ok;
    }

    bool
    Thread::interrupted (void)
    {
      return false;
    }

    /**
     * @details
     * Terminate the calling thread and make the value _value_ptr_
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
     * including, but not limited to, calling any `atexit()` routines
     * that may exist.
     * An implicit call to exit() is made when a thread other
     * than the thread in which main() was first invoked returns
     * from the start routine that was used to create it.
     * The function's return value shall serve as the thread's
     * exit status.
     * The behaviour of `_exit()` is undefined if called from a
     * cancellation cleanup handler or destructor function that
     * was invoked as a result of either an implicit or explicit
     * call to `exit()`.
     * After a thread has terminated, the result of access to
     * local (auto) variables of the thread is undefined.
     * Thus, references to local variables of the exiting
     * thread should not be used for the `exit()` _value_ptr_
     * parameter value.
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_exit()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_exit.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    void
    Thread::_exit (void* exit_ptr)
    {
      assert(!scheduler::in_handler_mode ());

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

        {
          scheduler::Critical_section scs; // ----- Critical section -----

            {
              interrupts::Critical_section ics; // ----- Critical section -----

#if !defined(OS_INCLUDE_RTOS_PORT_THREAD)
              ready_node_.unlink ();
#endif

              child_links_.unlink ();
            }

          assert(children_.empty ());
          parent_ = nullptr;

          assert(acquired_mutexes_ == 0);

          sched_state_ = thread::state::destroyed;

          func_result_ = exit_ptr;

          if (joiner_ != nullptr)
            {
              joiner_->resume ();
            }
        }

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

      port::Thread::destroy_this (this);
      // Does not return if the current thread.

#else

      // TODO: add to funeral list

      port::scheduler::reschedule (false);
      assert(true);
      for (;;)
        ;
#endif
      // Does not return.
    }

    /**
     * @details
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_kill()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_kill.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::kill (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

        {
          scheduler::Critical_section scs; // ----- Critical section -----

          if (sched_state_ == thread::state::destroyed)
            {
#if defined(OS_TRACE_RTOS_THREAD)
              trace::printf ("%s() @%p %s already gone\n", __func__, this,
                             name ());
#endif
              return result::ok; // Already exited itself
            }

          if (sched_state_ == thread::state::terminated)
            {
              // TODO remove thread from the funeral list and kill it here.
            }

            {
              interrupts::Critical_section ics; // ----- Critical section -----

#if !defined(OS_INCLUDE_RTOS_PORT_THREAD)
              ready_node_.unlink ();
#endif

              // If the thread is waiting on an event, remove it from the list.
              if (waiting_node_ != nullptr)
                {
                  waiting_node_->unlink ();
                }

              // If the thread is waiting on a timeout, remove it from the list.
              if (clock_node_ != nullptr)
                {
                  clock_node_->unlink ();
                }

              child_links_.unlink ();
            }

          assert(children_.empty ());
          parent_ = nullptr;

          assert(acquired_mutexes_ == 0);

#if defined(OS_INCLUDE_RTOS_PORT_THREAD)

          port::Thread::destroy_other (this);

#else

          port::Thread::clean (this);

#endif

          sched_state_ = thread::state::destroyed;
          func_result_ = nullptr;

          // All funeral services completed, the thread is gone.
          if (joiner_ != nullptr)
            {
              joiner_->resume ();

              port::this_thread::yield ();
            }
          // ----- End of critical section -----
        }

      return result::ok;
    }

    /**
     * @details
     * Set more bits in the thread current signal flags mask.
     * Use OR at bit-mask level.
     * Wake-up the thread to evaluate the signal flags.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::sig_raise (thread::sigset_t mask, thread::sigset_t* oflags)
    {
      os_assert_err(mask != 0, EINVAL);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());
#endif

      interrupts::Critical_section ics; // ----- Critical section -----

      if (oflags != nullptr)
        {
          *oflags = sig_mask_;
        }

      sig_mask_ |= mask;

      this->resume ();

      if (!scheduler::in_handler_mode ())
        {
          port::this_thread::yield ();
        }

      return result::ok;
    }

    /**
     * @details
     * Select the requested bits from the thread current signal mask
     * and return them. If requested, clear the selected bits in the
     * thread signal mask.
     *
     * If the mask is zero, return the full thread signal mask,
     * without any masking or subsequent clearing.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    thread::sigset_t
    Thread::sig_get (thread::sigset_t mask, flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), thread::sig::all);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());
#endif

      interrupts::Critical_section ics; // ----- Critical section -----

      if (mask == 0)
        {
          // Return the entire mask.
          return sig_mask_;
        }

      thread::sigset_t ret = sig_mask_ & mask;
      if ((mode & flags::mode::clear) != 0)
        {
          // Clear the selected bits; leave the rest untouched.
          sig_mask_ &= ~mask;
        }

      // Return the selected bits.
      return ret;
    }

    /**
     * @details
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::sig_clear (thread::sigset_t mask, thread::sigset_t* oflags)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);
      os_assert_err(mask != 0, EINVAL);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());
#endif

      interrupts::Critical_section ics; // ----- Critical section -----

      if (oflags != nullptr)
        {
          *oflags = sig_mask_;
        }

      // Clear the selected bits; leave the rest untouched.
      sig_mask_ &= ~mask;

      return result::ok;
    }

    /**
     * @details
     *
     * Internal function used to test if the desired signal flags are raised.
     */
    result_t
    Thread::_try_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                       flags::mode_t mode)
    {
      if ((mask != 0) && ((mode & flags::mode::all) != 0))
        {
          // Only if all desires signals are raised we're done.
          if ((sig_mask_ & mask) == mask)
            {
              if (oflags != nullptr)
                {
                  *oflags = sig_mask_;
                }
              // Clear desired signals.
              sig_mask_ &= ~mask;
              return result::ok;
            }
        }
      else if ((mask == 0) || ((mode & flags::mode::any) != 0))
        {
          // Any signal will do it.
          if (sig_mask_ != 0)
            {
              // Possibly return .
              if (oflags != nullptr)
                {
                  *oflags = sig_mask_;
                }
              // Since we returned them all, also clear them all.
              sig_mask_ = 0;
              return result::ok;
            }
        }

      return EWOULDBLOCK;
    }

    /**
     * @details
     * If the flags::mode::all bit is set, the function expects
     * all given flags to be raised; otherwise, if the flags::mode::any
     * bit is set, the function expects any single flag to be raised.
     *
     * If the expected signal flags are
     * raised, the function returns instantly.
     *
     * Otherwise suspend the execution of the current thread until all/any
     * specified signal flags are raised.
     *
     * When the parameter mask is 0, the thread is suspended
     * until any signal flag is raised. In this case, if any signal flags
     * are already raised, the function returns instantly.
     *
     * If the flags::mode::clear bit is set, the signal flags that are
     * returned are automatically cleared.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                       flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(0x%X, %d) @%p %s\n", __func__, mask, mode, this,
                     name ());
#endif

#if defined(OS_TRACE_RTOS_THREAD)
      clock::timestamp_t prev = systick_clock.now ();
      clock::duration_t slept_ticks = 0;
#endif
      for (;;)
        {
            {
              interrupts::Critical_section ics; // ----- Critical section -----

              if (_try_wait (mask, oflags, mode) == result::ok)
                {
#if defined(OS_TRACE_RTOS_THREAD)
                  slept_ticks =
                      static_cast<clock::duration_t> (systick_clock.now ()
                          - prev);
                  trace::printf ("%s(0x%X, %d)=%d @%p %s\n", __func__, mask,
                                 mode, slept_ticks, this, name ());
#endif
                  return result::ok;
                }
            }

          _wait ();

          if (interrupted ())
            {
              return EINTR;
            }
        }
      return ENOTRECOVERABLE;
    }

    /**
     * @details
     * If the flags::mode::all bit is set, the function expects
     * all given flags to be raised; otherwise, if the flags::mode::any
     * bit is set, the function expects any single flag to be raised.
     *
     * The function does not blocks, if the expected signal flags are
     * not raised, but returns `EGAIN`.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::_try_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                           flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(0x%X, %d) @%p %s\n", __func__, mask, mode, this,
                     name ());
#endif

      interrupts::Critical_section ics; // ----- Critical section -----

      return _try_wait (mask, oflags, mode);
    }

    /**
     * @details
     * If the flags::mode::all bit is set, the function expects
     * all given flags to be raised; otherwise, if the flags::mode::any
     * bit is set, the function expects any single flag to be raised.
     *
     * If the expected signal flags are
     * raised, the function returns instantly.
     *
     * Otherwise suspend the execution of the thread until all/any
     * specified signal flags are raised.
     *
     * When the parameter mask is 0, the thread is suspended
     * until any event flag is raised. In this case, if any signal flags
     * are already raised, the function returns instantly.
     *
     * The wait shall be terminated when the specified timeout
     * expires.
     *
     * The timeout shall expire after the number of time units (that
     * is when the value of that clock equals or exceeds (now()+duration).
     * The resolution of the timeout shall be the resolution of the
     * clock on which it is based (the SysTick clock for CMSIS).
     *
     * Under no circumstance shall the operation fail with a timeout
     * if the signal flags are already raised.
     *
     * If the flags::mode::clear bit is set, the signal flags that are
     * returned are automatically cleared.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    Thread::_timed_sig_wait (thread::sigset_t mask, clock::duration_t timeout,
                             thread::sigset_t* oflags, flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(0x%X, %d, %d) @%p %s\n", __func__, mask, mode, timeout,
                     this, name ());
#endif

        {
          interrupts::Critical_section ics; // ----- Critical section -----

          if (_try_wait (mask, oflags, mode) == result::ok)
            {
              return result::ok;
            }
        }

      Clock& clock = systick_clock;
      Clock_timestamps_list& clock_list = clock.steady_list ();
      clock::timestamp_t timeout_timestamp = clock.steady_now () + timeout;

#if defined(OS_TRACE_RTOS_THREAD)
      clock::timestamp_t begin_timestamp = clock.steady_now ();
#endif

      // Prepare a timeout node pointing to the current thread.
      Timeout_thread_node timeout_node
        { timeout_timestamp, *this };

      result_t res = ENOTRECOVERABLE;
      for (;;)
        {
            {
              interrupts::Critical_section ics; // ----- Critical section -----

              if (_try_wait (mask, oflags, mode) == result::ok)
                {
                  res = result::ok;
                  break;
                }

              // Remove this thread from the ready list, if there.
              port::this_thread::prepare_suspend ();

              // Add this thread to the clock timeout list.
              clock_list.link (timeout_node);
              timeout_node.thread.clock_node_ = &timeout_node;
            }

          port::scheduler::reschedule ();

            {
              interrupts::Critical_section ics; // ----- Critical section -----

              // Remove the thread from the clock timeout list,
              // if not already removed by the timer.
              timeout_node.thread.clock_node_ = nullptr;
              timeout_node.unlink ();
            }

          if (interrupted ())
            {
              res = EINTR;
              break;
            }

          if (clock.steady_now () >= timeout_timestamp)
            {
              res = ETIMEDOUT;
              break;
            }
        }

#if defined(OS_TRACE_RTOS_THREAD)
      clock::duration_t slept_ticks =
          static_cast<clock::duration_t> (clock.steady_now () - begin_timestamp);
      trace::printf ("%s(0x%X, %d, %d)=%d @%p %s\n", __func__, mask, mode,
                     timeout, slept_ticks, this, name ());
#endif

      return res;
    }

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
