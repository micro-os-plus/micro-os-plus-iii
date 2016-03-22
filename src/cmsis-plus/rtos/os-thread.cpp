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
#pragma GCC diagnostic ignored "-Wunused-parameter"

#if 0
    void*
    no_thread_func (void* args);

    void*
    no_thread_func (void* args)
      {
        return nullptr;
      }

    Thread no_thread
      { nullptr, (thread::func_t) no_thread_func, nullptr};
#endif

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
      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      Thread&
      thread (void)
      {
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

        return port::this_thread::thread ();

#else

        // TODO

#endif
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

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

        return port::this_thread::yield ();

#else

        // TODO

#endif
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
      trace::printf ("%s @%p %s\n", __func__, thread, thread->name ());
      thread->exit (thread->func_ (thread->func_args_));
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
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      assert(function != nullptr);
      assert(attr.th_priority != thread::priority::none);

      // Prevent the new thread to execute before all members are set.
      scheduler::Critical_section cs; // ----- Critical section -----

      // Get attributes from user structure.
      prio_ = attr.th_priority;
      stack_size_bytes_ = attr.th_stack_size_bytes;
      stack_addr_ = attr.th_stack_address;

      func_ = function;
      func_args_ = args;

      sig_mask_ = 0;

      joiner_ = nullptr;

      trace::printf ("%s @%p %s %d %d\n", __func__, this, name (), prio_,
                     stack_size_bytes_);

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

      port::Thread::create (this);
      sched_state_ = thread::state::ready;

#else

      // TODO: check min size
      // TODO: align stack
      if (stack_addr_ == nullptr)
        {
          // TODO: alloc default stack size
        }
      state_ = thread::state::inactive;

      scheduler::__register_thread (this);

#endif

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
      // Prevent the main thread to destroy itself while running
      // the exit cleanup code.
      if (this != &this_thread::thread())
        {
          _destroy ();
        }
    }

    void
    Thread::_destroy (void)
    {
      if (sched_state_ == thread::state::destroyed)
        {
          return;
        }

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      sched_state_ = thread::state::destroyed;

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

      port::Thread::destroy (this);
      // Does not return if the current thread.

#else

      // TODO
      scheduler::__unregister_thread (this);

#endif
    }

    /**
     * @details
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    void
    Thread::_sleep (void)
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

      port::Thread::sleep (this);

#else

      // TODO

#endif
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
    Thread::wakeup (void)
    {
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
      wakeup_reason_ = result::ok;

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

      port::Thread::wakeup (this);

#else

      // TODO

#endif
    }

#if 0
    /**
     * @details
     * Internal, no POSIX equivalent, used to notify timeouts or cancels.
     *
     * @note Can be invoked from Interrupt Service Routines.
     */
    void
    Thread::wakeup (result_t reason)
      {
        assert(reason == EINTR || reason == ETIMEDOUT);

        trace::printf ("%s(&d) @%p %s \n", __func__, reason, this, name ());
        wakeup_reason_ = reason;

        // TODO
      }
#endif

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

      trace::printf ("%s(%d) @%p %s\n", __func__, prio, this, name ());

      prio_ = prio;

      result_t res = result::ok;

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

      // The port must perform a context switch.
      res = port::Thread::sched_prio (this, prio);

#else

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

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      while ((sched_state_ != thread::state::terminated)
          && (sched_state_ != thread::state::destroyed))
        {
          joiner_ = this;
          _sleep ();
        }

      if (exit_ptr != nullptr)
        {
          *exit_ptr = func_result_;
        }
      trace::printf ("%s() @%p %s joined\n", __func__, this, name ());

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

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

#if defined(OS_INCLUDE_PORT_RTOS_THREAD)

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

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

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
     * The behaviour of `exit()` is undefined if called from a
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
    Thread::exit (void* exit_ptr)
    {
      assert(!scheduler::in_handler_mode ());

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      if (sched_state_ == thread::state::terminated)
        {
          trace::printf ("%s() @%p %s already terminated\n", __func__, this,
                         name ());
          return; // Already terminated
        }

      func_result_ = exit_ptr;
      sched_state_ = thread::state::terminated;

      if (joiner_ != nullptr)
        {
          joiner_->wakeup ();
        }

      _destroy ();
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

      trace::printf ("%s() @%p %s\n", __func__, this, name ());

      if ((sched_state_ == thread::state::terminated)
          || (sched_state_ == thread::state::destroyed))
        {
          trace::printf ("%s() @%p %s already terminated\n", __func__, this,
                         name ());
          return result::ok; // Already terminated
        }

      func_result_ = nullptr;
      sched_state_ = thread::state::terminated;

      if (joiner_ != nullptr)
        {
          joiner_->wakeup ();
        }

      _destroy ();

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

      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());

      interrupts::Critical_section cs; // ----- Critical section -----

      if (oflags != nullptr)
        {
          *oflags = sig_mask_;
        }

      sig_mask_ |= mask;

      wakeup ();

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

      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());

      interrupts::Critical_section cs; // ----- Critical section -----

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

      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());

      interrupts::Critical_section cs; // ----- Critical section -----

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

      return EAGAIN;
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
    Thread::sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                      flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(0x%X, %d) @%p %s\n", __func__, mask, mode, this,
                     name ());

      Systick_clock::rep prev = Systick_clock::now ();
      Systick_clock::sleep_rep slept_ticks = 0;
      for (;;)
        {
            {
              interrupts::Critical_section cs; // ----- Critical section -----

              if (_try_wait (mask, oflags, mode) == result::ok)
                {
                  slept_ticks =
                      (Systick_clock::sleep_rep) (Systick_clock::now () - prev);
                  trace::printf ("%s(0x%X, %d)=%d @%p %s\n", __func__, mask,
                                 mode, slept_ticks, this, name ());
                  return result::ok;
                }
            }

          _sleep ();

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
    Thread::try_sig_wait (thread::sigset_t mask, thread::sigset_t* oflags,
                          flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(0x%X, %d) @%p %s\n", __func__, mask, mode, this,
                     name ());

      interrupts::Critical_section cs; // ----- Critical section -----

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
    Thread::timed_sig_wait (thread::sigset_t mask, duration_t timeout,
                            thread::sigset_t* oflags, flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      trace::printf ("%s(0x%X, %d, %d) @%p %s\n", __func__, mask, mode, timeout,
                     this, name ());

      if (timeout == 0)
        {
          timeout = 1;
        }

        {
          interrupts::Critical_section cs; // ----- Critical section -----

          if (_try_wait (mask, oflags, mode) == result::ok)
            {
              return result::ok;
            }
        }

      Systick_clock::rep prev = Systick_clock::now ();
      Systick_clock::sleep_rep slept_ticks = 0;

      result_t res = ENOTRECOVERABLE;
      for (;;)
        {
          Systick_clock::wait (timeout - slept_ticks);

          Systick_clock::rep now = Systick_clock::now ();
          slept_ticks += (Systick_clock::sleep_rep) (now - prev);

            {
              interrupts::Critical_section cs; // ----- Critical section -----

              if (_try_wait (mask, oflags, mode) == result::ok)
                {
                  res = result::ok;
                  break;
                }
            }

          if (slept_ticks >= timeout)
            {
              res = ETIMEDOUT;
              break;
            }

          if (interrupted ())
            {
              res = EINTR;
              break;
            }

          prev = now;
        }

#if 1
      trace::printf ("%s(0x%X, %d, %d)=%d @%p %s\n", __func__, mask, mode,
                     timeout, slept_ticks, this, name ());
#endif

      return res;
    }

// --------------------------------------------------------------------------

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */
