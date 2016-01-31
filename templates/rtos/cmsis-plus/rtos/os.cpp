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
        status_t
        initialize (void)
        {
          // TODO
          return status::ok;
        }

        status_t
        start (void)
        {
          // TODO
          return status::ok;
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
        strerror (status_t ret)
        {
          const char* str;
          switch (ret)
            {
            case status::event_signal:
              str = "signal event occurred";
              break;

            case status::event_message:
              str = "message event occurred";
              break;

            case status::event_mail:
              str = "mail event occurred";
              break;

            case status::event_timeout:
              str = "timeout occurred";
              break;

            case status::error_parameter:
              str = "mandatory parameter missing or incorrect object";
              break;

            case status::error_resource:
              str = "resource not available";
              break;

            case status::error_timeout_resource:
              str = "resource not available within given time";
              break;

            case status::error_isr:
              str = "not allowed in ISR context";
              break;

            case status::error_isr_recursive:
              str = "function called multiple times from ISR with same object";
              break;

            case status::error_priority:
              str =
                  "system cannot determine priority or thread has illegal priority";
              break;

            case status::error_no_memory:
              str = "system is out of memory";
              break;

            case status::error_value:
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
        { nullptr, (thread_func_t) no_thread_func, nullptr };

      namespace thread
      {
        Thread&
        get_current (void)
        {
          // TODO
          return no_thread;
        }

        status_t
        yield (void)
        {
          // TODO
          return status::ok;
        }

        status_t
        wait (millis_t millisec, event_t* ret)
        {
          // TODO
          return status::ok;
        }

        status_t
        wait_signals (signal_flags_t signals, millis_t millisec,
                      signal_flags_t* ret)
        {
          // TODO
          return status::ok;
        }

#if 0
        status_t
        delay (millis_t millisec)
          {
            // TODO
            return status::ok;
          }
#endif

        status_t
        sleep (sys_ticks_t ticks)
        {
          // TODO
          return status::ok;
        }

      }

      // ======================================================================

      Named_object::Named_object (const char* name) :
          name_ (name != nullptr ? name : "-")
      {
        ;
      }

      // ======================================================================

      Thread::Thread (const thread_attr_t* attr, thread_func_t function,
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
            prio_ = priority::normal;
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

      priority_t
      Thread::get_sched_prio (void)
      {
        return prio_;
      }

      status_t
      Thread::set_sched_prio (priority_t prio)
      {
        prio_ = prio;
        return status::ok;
      }

      status_t
      Thread::join (void** exit_ptr)
      {
        // TODO
        return status::ok;
      }

      status_t
      Thread::detach (void)
      {
        // TODO
        return status::ok;
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

      Timer::Timer (const char* name, timer_func_t function, timer_type_t type,
                    void* args) : //
          Named_object (name)
      {
        // TODO
      }

      Timer::~Timer ()
      {
        // TODO
      }

      status_t
      Timer::start (millis_t millisec)
      {
        // TODO
        return status::ok;
      }

      status_t
      Timer::stop (void)
      {
        // TODO
        return status::ok;
      }

      // ======================================================================

      Mutex::Mutex (const char* name) :
          Named_object (name)
      {
        // TODO
      }

      Mutex::~Mutex ()
      {
        // TODO
      }

      status_t
      Mutex::wait (void)
      {
        return status::ok;
      }

      status_t
      Mutex::try_wait (sys_ticks_t ticks)
      {
        return status::ok;
      }

      status_t
      Mutex::release (void)
      {
        return status::ok;
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

      status_t
      Recursive_mutex::wait (void)
      {
        return status::ok;
      }

      status_t
      Recursive_mutex::try_wait (sys_ticks_t ticks)
      {
        return status::ok;
      }

      status_t
      Recursive_mutex::release (void)
      {
        return status::ok;
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

      status_t
      Condition_variable::notify_one () noexcept
      {
        // TODO
        return status::ok;
      }

      status_t
      Condition_variable::notify_all () noexcept
      {
        // TODO
        return status::ok;
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
        return status::ok;
      }

      status_t
      Semaphore::release (void)
      {
        return status::ok;
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

      status_t
      Pool::free (void* block)
      {
        // TODO
        return status::ok;
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

      status_t
      Message_queue::put (void* info, millis_t millisec)
      {
        // TODO
        return status::ok;
      }

      status_t
      Message_queue::get (millis_t millisec, void** ret)
      {
        // TODO
        return status::ok;
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

      status_t
      Mail_queue::put (void* mail)
      {
        // TODO
        return status::ok;
      }

      status_t
      Mail_queue::get (millis_t millisec, void** ret)
      {
        // TODO
        return status::ok;
      }

      status_t
      Mail_queue::free (void* mail)
      {
        // TODO
        return status::ok;
      }

    // ======================================================================

#pragma GCC diagnostic pop

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */
