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
#include <new>
#include <memory>

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
     * @cond ignore
     */

    std::size_t thread::stack::min_size_bytes_ = port::stack::min_size_bytes;

    std::size_t thread::stack::default_size_bytes_ =
        port::stack::default_size_bytes;

    /**
     * @endcond
     */

    // ========================================================================
    /**
     * @class thread::attributes
     * @details
     * Allow to assign custom attributes (like stack address,
     * stack size, priority) to the thread.
     *
     * To simplify access, the member variables are public and do not
     * require accessors or mutators.
     *
     * @par POSIX compatibility
     *  Inspired by `pthread_attr_t` from [<pthread.h>](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  (IEEE Std 1003.1, 2013 Edition).
     */

    /**
     * @class thread::context
     * @details
     * The thread context includes the stack object and port
     * specific structures.
     *
     * On some ports (like the Cortex-M) the thread context is
     * stored on the stack and a pointer to the current location
     * is kept in the context object.
     *
     * On other ports (like the synthetic POSIX port), the
     * makecontext() functions create and manage large
     * ucontex_t structures stored in the context object.
     */

    /**
     * @class thread::stack
     * @details
     * This class does not contain the stack space itself, it is
     * allocated outside, but stores the address and the size of
     * the stack.
     *
     * It also manages the global variables storing the min and
     * default stack sizes.
     */

    /**
     * @details
     * This variable is used by the default constructor.
     */
    const thread::attributes thread::initializer;

    /**
     * @class thread
     * @details
     * CMSIS++ threads are inspired by POSIX threads; they support
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
     *   thread th { func, nullptr };
     *
     *   // Do something.
     *
     *   // Wait for thread to terminate.
     *   this_thread::join(th);
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
     * @cond ignore
     */

    void
    thread::stack::initialize (void)
    {
      // Align the bottom of the stack.
      void* pa = bottom_address_;
      bottom_address_ = static_cast<stack::element_t*> (std::align (
          sizeof(stack::allocation_element_t), stack::min_size (), pa,
          size_bytes_));

      // If there is not enough space for the minimal stack, fail.
      os_assert_throw(bottom_address_ != nullptr, ENOMEM);

      element_t* p = bottom_address_;
      element_t* pend = top ();

      // Initialise the entire stack with the magic word.
      for (; p < pend; ++p)
        {
          *p = magic;
        }

      // Compute the actual size. The -1 is to leave space for the magic.
      size_bytes_ = ((static_cast<std::size_t> (p - bottom_address_) - 1)
          * sizeof(element_t));
    }

    /**
     * @details
     * Count the number of words where the magic is still there.
     *
     * @warning: For large stacks it may be an expensive operation.
     */
    std::size_t
    thread::stack::available (void)
    {
      element_t* p = bottom_address_;
      std::size_t count = 0;
      while (*p == magic)
        {
          count += sizeof(element_t);
          ++p;
        }

      return count;
    }

    /**
     * @details
     * Same as in POSIX, thread functions can return, and the behaviour
     * should be as the thread called the exit() function.
     *
     * This requires a proxy to run the thread function, get the result,
     * and explicitly invoke exit().
     */
    void
    thread::_invoke_with_exit (thread* thread)
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, thread, thread->name ());
#endif
      thread->_exit (thread->func_ (thread->func_args_));
    }

    thread::thread ()
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif
    }

    thread::thread (const char* name) :
        named_object
          { name }
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif
    }

    /**
     * @endcond
     */

    /**
     * @details
     * This constructor shall initialise a thread object
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
     * If the attributes define a stack area (via `th_stack_address` and
     * `th_stack_size_bytes`), that stack is used, otherwise
     * the stack is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    thread::thread (func_t function, func_args_t args, const attributes& attr,
                    const Allocator& allocator) :
        thread
          { nullptr, function, args, attr, allocator }
    {
      ;
    }

    /**
     * @details
     * This constructor shall initialise a named thread object
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
     * If the attributes define a stack area (via `th_stack_address` and
     * `th_stack_size_bytes`), that stack is used, otherwise
     * the stack is dynamically allocated using the RTOS specific allocator
     * (`rtos::memory::allocator`).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_create()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_create.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    thread::thread (const char* name, func_t function, func_args_t args,
                    const attributes& attr, const Allocator& allocator) :
        named_object
          { name }
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, this->name ());
#endif

      if (attr.th_stack_address != nullptr
          && attr.th_stack_size_bytes > stack::min_size ())
        {
          _construct (function, args, attr, nullptr, 0);
        }
      else
        {
          using Allocator = memory::allocator<stack::allocation_element_t>;
          allocator_ = &allocator;

          if (attr.th_stack_size_bytes > stack::min_size ())
            {
              allocated_stack_size_elements_ = (attr.th_stack_size_bytes
                  + sizeof(stack::allocation_element_t) - 1)
                  / sizeof(stack::allocation_element_t);
            }
          else
            {
              allocated_stack_size_elements_ = (stack::default_size ()
                  + sizeof(stack::allocation_element_t) - 1)
                  / sizeof(stack::allocation_element_t);
            }

          allocated_stack_address_ =
              reinterpret_cast<stack::element_t*> (const_cast<Allocator&> (allocator).allocate (
                  allocated_stack_size_elements_));

          assert(allocated_stack_address_ != nullptr);

          _construct (
              function,
              args,
              attr,
              allocated_stack_address_,
              allocated_stack_size_elements_
                  * sizeof(stack::allocation_element_t));
        }
    }

    /**
     * @cond ignore
     */

    void
    thread::_construct (func_t function, func_args_t args,
                        const attributes& attr, void* stack_address,
                        std::size_t stack_size_bytes)
    {
      os_assert_throw(!scheduler::in_handler_mode (), EPERM);

      assert(function != nullptr);
      assert(attr.th_priority != priority::none);

      clock_ = attr.clock != nullptr ? attr.clock : &sysclock;

      if (stack_address != nullptr)
        {
          // The attributes should not define any storage in this case.
          if (attr.th_stack_size_bytes > stack::min_size ())
            {
              assert(attr.th_stack_address == nullptr);
            }

          stack ().set (static_cast<stack::element_t*> (stack_address),
                        stack_size_bytes);
        }
      else
        {
          stack ().set (static_cast<stack::element_t*> (attr.th_stack_address),
                        attr.th_stack_size_bytes);
        }

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s p%u stack{%p,%u}\n", __func__, this, name (),
                     attr.th_priority, stack ().bottom_address_,
                     stack ().size_bytes_);
#endif

        {
          // Prevent the new thread to execute before all members are set.
          scheduler::critical_section cs; // ----- Critical section -----

          // Get attributes from user structure.
          prio_ = attr.th_priority;

          func_ = function;
          func_args_ = args;

          parent_ = this_thread::_thread ();
          if (scheduler::started () && (parent_ != nullptr))
            {
              parent_->children_.link (*this);
            }
          else
            {
              scheduler::top_threads_list_.link (*this);
            }

          stack ().initialize ();

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

          port::thread::create (this);
          sched_state_ = state::ready;

#else

          // Create the context.
          port::context::create (&context_,
                                 reinterpret_cast<void*> (_invoke_with_exit),
                                 this);

          if (!scheduler::started ())
            {
              scheduler::current_thread_ = this;
            }

          // Add to ready list, but do not yield yet.
          resume ();

#endif

        }
      // For just in case the new thread has higher priority.
      this_thread::yield ();
    }

    /**
     * @endcond
     */

    /**
     * @details
     * This destructor shall destroy the thread object; the object
     * becomes, in effect, uninitialised. An implementation may cause
     * the destructor to set the object to an invalid value.
     *
     * If the stack was dynamically allocated, it is deallocated
     * using the same allocator.
     *
     * @par POSIX compatibility
     *  No POSIX similar functionality identified.
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    thread::~thread ()
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s \n", __func__, this, name ());
#endif

      // Prevent the main thread to destroy itself while running
      // the exit cleanup code.
      if (this != &this_thread::thread ())
        {
          kill ();
        }
      else
        {
#if defined(OS_TRACE_RTOS_THREAD)
          trace::printf ("%s() @%p %s nop, cannot commit suicide\n", __func__,
                         this, name ());
#endif
        }
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
    thread::resume (void)
    {
#if defined(OS_TRACE_RTOS_THREAD_CONTEXT)
      trace::printf ("%s() @%p %s %u\n", __func__, this, name (), prio_);
#endif

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

        {
          interrupts::critical_section ics; // ----- Critical section -----

          sched_state_ = state::ready;
          port::thread::resume (this);
        }

#else

      assert(port::interrupts::is_priority_valid ());

        {
          interrupts::critical_section ics; // ----- Critical section -----

          if (ready_node_.next () == nullptr)
            {
              scheduler::ready_threads_list_.link (ready_node_);
              // state::ready set in above link().
            }
        }

      port::scheduler::reschedule ();

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
    thread::sched_prio (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), priority::error);

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
    thread::sched_prio (priority_t prio)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);
      os_assert_err(prio < priority::error, EINVAL);
      os_assert_err(prio != priority::none, EINVAL);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s(%u) @%p %s\n", __func__, prio, this, name ());
#endif

      prio_ = prio;

      result_t res = result::ok;

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

      // The port must perform a context switch.
      res = port::thread::sched_prio (this, prio);

#else

      if (sched_state_ == state::ready)
        {
          interrupts::critical_section ics; // ----- Critical section -----

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
    thread::detach (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

      result_t res = port::thread::detach (this);
      if (res != result::ok)
        {
          return res;
        }

#else

      // TODO: implement

#endif

      return result::ok;
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
    thread::join (void** exit_ptr)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

      // Fail if current thread
      assert(this != this_thread::_thread ());

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      while (sched_state_ != state::destroyed)
        {
          joiner_ = this_thread::_thread ();
          this_thread::_thread ()->_wait ();
        }

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s joined\n", __func__, this, name ());
#endif

      if (exit_ptr != nullptr)
        {
          *exit_ptr = func_result_;
        }

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
     * @todo Implement it properly (thread interruption is not yet
     * fully implemented).
     *
     * @par POSIX compatibility
     *  Inspired by [`pthread_cancel()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/pthread_cancel.html)
     *  from [`<pthread.h>`](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/pthread.h.html)
     *  ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html)).
     *
     * @warning Cannot be invoked from Interrupt Service Routines.
     */
    result_t
    thread::cancel (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      // TODO: implement according to POSIX specs.
      return result::ok;
    }

    /**
     * @details
     * If the interrupt flag is true, threads waiting for
     * an event are notified immediately (actually as
     * soon as the thread priority allows it to run).
     *
     * After the thread detects the interrupted condition, it
     * must clear the interrupted flag.
     */
    bool
    thread::interrupt (bool interrupt)
    {
      bool tmp = interrupted_;
      interrupted_ = interrupt;

      resume ();
      return tmp;
    }

    /**
     * @cond ignore
     */

    /**
     * @details
     *
     * @par POSIX compatibility
     *  Extension to standard, no POSIX similar functionality identified.
     */
    void
    thread::_wait (void)
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

        {
          interrupts::critical_section ics; // ----- Critical section -----

          // Remove this thread from the ready list, if there.
          port::this_thread::prepare_suspend ();

          sched_state_ = state::waiting;
        }
      port::scheduler::reschedule ();

    }

    void
    thread::_exit (void* exit_ptr)
    {
      assert(!scheduler::in_handler_mode ());

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

        {
          scheduler::critical_section scs; // ----- Critical section -----

            {
              interrupts::critical_section ics; // ----- Critical section -----

              ready_node_.unlink ();

              child_links_.unlink ();
            }

          assert(children_.empty ());
          parent_ = nullptr;

          assert(acquired_mutexes_ == 0);

          sched_state_ = state::terminated;

          func_result_ = exit_ptr;

        }

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

        {
          interrupts::critical_section ics; // ----- Critical section -----

          // Add to a list of threads to be destroyed by the idle thread.
          scheduler::terminated_threads_list_.link (ready_node_);
        }

      port::thread::destroy_this (this);
      // Does not return if the current thread.

#else

        {
          interrupts::critical_section ics; // ----- Critical section -----

          // Add to a list of threads to be destroyed by the idle thread.
          scheduler::terminated_threads_list_.link (ready_node_);
        }

      port::scheduler::reschedule ();
      assert(true);
      while (true)
        ;
#endif
      // Does not return.
    }

    void
    thread::_check_stack (void)
    {
      if (stack ().size () > 0)
        {
          assert(stack ().check_bottom_magic ());
          assert(stack ().check_top_magic ());

#if defined(OS_TRACE_RTOS_THREAD)
          trace::printf ("%s() @%p %s %u/%u stack bytes unused\n", __func__,
                         this, name (), stack ().available (),
                         stack ().size ());
#endif

          // Clear stack to avoid further checks
          stack ().clear ();
        }
    }

    void
    thread::_destroy (void)
    {
#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

      _check_stack ();

      if (allocated_stack_address_ != nullptr)
        {
          typedef typename std::allocator_traits<Allocator>::pointer pointer;

          static_cast<Allocator*> (const_cast<void*> (allocator_))->deallocate (
              reinterpret_cast<pointer> (allocated_stack_address_),
              allocated_stack_size_elements_);

          allocated_stack_address_ = nullptr;
        }

      sched_state_ = state::destroyed;

      if (joiner_ != nullptr)
        {
          joiner_->resume ();
        }
    }

    /**
     * @endcond
     */

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
    thread::kill (void)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD)
      trace::printf ("%s() @%p %s\n", __func__, this, name ());
#endif

        {
          scheduler::critical_section scs; // ----- Critical section -----

          if (sched_state_ == state::destroyed)
            {
#if defined(OS_TRACE_RTOS_THREAD)
              trace::printf ("%s() @%p %s already gone\n", __func__, this,
                             name ());
#endif
              return result::ok; // Already exited itself
            }

            {
              interrupts::critical_section ics; // ----- Critical section -----

              // Remove thread from the funeral list and kill it here.
              ready_node_.unlink ();

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

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

          port::thread::destroy_other (this);

#endif

          func_result_ = nullptr;

          _destroy ();

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
    thread::sig_raise (flags::mask_t mask, flags::mask_t* oflags)
    {
#if defined(OS_TRACE_RTOS_THREAD_SIG)
      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());
#endif

      os_assert_err(mask != 0, EINVAL);

      assert(port::interrupts::is_priority_valid ());

      interrupts::critical_section ics; // ----- Critical section -----

      if (oflags != nullptr)
        {
          *oflags = sig_mask_;
        }

      sig_mask_ |= mask;

      this->resume ();

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
    flags::mask_t
    thread::sig_get (flags::mask_t mask, flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), sig::all);

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      trace::printf ("%s(0x%X) @%p %s\n", __func__, mask, this, name ());
#endif

      interrupts::critical_section ics; // ----- Critical section -----

      if (mask == 0)
        {
          // Return the entire mask.
          return sig_mask_;
        }

      flags::mask_t ret = sig_mask_ & mask;
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
    thread::sig_clear (flags::mask_t mask, flags::mask_t* oflags)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      trace::printf ("%s(0x%X) @%p %s 0x%X\n", __func__, mask, this, name (),
                     sig_mask_);
#endif

      interrupts::critical_section ics; // ----- Critical section -----

      if (oflags != nullptr)
        {
          *oflags = sig_mask_;
        }

      if (mask == 0)
        {
          // Clear all flags.
          sig_mask_ = 0;
        }
      else
        {
          // Clear the selected bits; leave the rest untouched.
          sig_mask_ &= ~mask;
        }

      return result::ok;
    }

    /**
     * @cond ignore
     */

    /**
     * @details
     *
     * Internal function used to test if the desired signal flags are raised.
     */
    result_t
    thread::_try_wait (flags::mask_t mask, flags::mask_t* oflags,
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

    result_t
    thread::_sig_wait (flags::mask_t mask, flags::mask_t* oflags,
                       flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      trace::printf ("%s(0x%X, %u) @%p %s 0x%X\n", __func__, mask, mode, this,
                     name (), sig_mask_);
#endif

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      clock::timestamp_t prev = clock_->now ();
      clock::duration_t slept_ticks = 0;
#endif
      for (;;)
        {
            {
              interrupts::critical_section ics; // ----- Critical section -----

              if (_try_wait (mask, oflags, mode) == result::ok)
                {
#if defined(OS_TRACE_RTOS_THREAD_SIG)
                  slept_ticks = static_cast<clock::duration_t> (clock_->now ()
                      - prev);
                  trace::printf ("%s(0x%X, %d)=%d @%p %s 0x%X\n", __func__,
                                 mask, mode, slept_ticks, this, name (),
                                 sig_mask_);
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

    result_t
    thread::_try_sig_wait (flags::mask_t mask, flags::mask_t* oflags,
                           flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      trace::printf ("%s(0x%X, %d) @%p %s 0x%X\n", __func__, mask, mode, this,
                     name (), sig_mask_);
#endif

      interrupts::critical_section ics; // ----- Critical section -----

      return _try_wait (mask, oflags, mode);
    }

    result_t
    thread::_timed_sig_wait (flags::mask_t mask, clock::duration_t timeout,
                             flags::mask_t* oflags, flags::mode_t mode)
    {
      os_assert_err(!scheduler::in_handler_mode (), EPERM);

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      trace::printf ("%s(0x%X, %u, %u) @%p %s 0x%X\n", __func__, mask, mode,
                     timeout, this, name (), sig_mask_);
#endif

        {
          interrupts::critical_section ics; // ----- Critical section -----

          if (_try_wait (mask, oflags, mode) == result::ok)
            {
              return result::ok;
            }
        }

      clock_timestamps_list& clock_list = clock_->steady_list ();
      clock::timestamp_t timeout_timestamp = clock_->steady_now () + timeout;

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      clock::timestamp_t begin_timestamp = clock_->steady_now ();
#endif

      // Prepare a timeout node pointing to the current thread.
      timeout_thread_node timeout_node
        { timeout_timestamp, *this };

      result_t res = ENOTRECOVERABLE;
      for (;;)
        {
            {
              interrupts::critical_section ics; // ----- Critical section -----

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

              sched_state_ = state::waiting;
            }

          port::scheduler::reschedule ();

            {
              interrupts::critical_section ics; // ----- Critical section -----

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

          if (clock_->steady_now () >= timeout_timestamp)
            {
              res = ETIMEDOUT;
              break;
            }
        }

#if defined(OS_TRACE_RTOS_THREAD_SIG)
      clock::duration_t slept_ticks =
          static_cast<clock::duration_t> (clock_->steady_now ()
              - begin_timestamp);
      trace::printf ("%s(0x%X, %u, %u)=%u @%p %s 0x%X\n", __func__, mask, mode,
                     timeout, static_cast<unsigned int> (slept_ticks), this,
                     name (), sig_mask_);
#endif

      return res;
    }

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
    /**
     * @details
     * The os::rtos::this_thread namespace groups functions related to
     * the current thread.
     */
    namespace this_thread
    {

      /**
       * @cond ignore
       */

      rtos::thread*
      _thread (void)
      {
        rtos::thread* th;

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

        th = port::this_thread::thread ();

#else

        th = scheduler::current_thread_;

#endif
        return th;
      }

      /**
       * @endcond
       */

      /**
       * @details
       *
       * @warning Cannot be invoked from Interrupt Service Routines.
       */
      rtos::thread&
      thread (void)
      {
        os_assert_throw(!scheduler::in_handler_mode (), EPERM);

        rtos::thread* th;

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
#if defined(OS_TRACE_RTOS_THREAD_CONTEXT)
            trace::printf ("%s() nop %s \n", __func__, _thread ()->name ());
#endif
            return;
          }

#if defined(OS_TRACE_RTOS_THREAD_CONTEXT)
        trace::printf ("%s() from %s\n", __func__, _thread ()->name ());
#endif

#if defined(OS_USE_RTOS_PORT_SCHEDULER)

        port::this_thread::yield ();

#else

        port::scheduler::reschedule ();

#endif

#if defined(OS_TRACE_RTOS_THREAD_CONTEXT)
        trace::printf ("%s() to %s\n", __func__, _thread ()->name ());
#endif
      }

    } /* namespace this_thread */

  // --------------------------------------------------------------------------

  } /* namespace rtos */
} /* namespace os */
