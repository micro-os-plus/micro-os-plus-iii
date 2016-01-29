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
        return_t
        initialize (void)
        {
          // TODO
          return os_ok;
        }

        return_t
        start (void)
        {
          // TODO
          return os_ok;
        }

        bool
        is_running (void)
        {
          // TODO
          return true;
        }

        uint64_t
        get_current_systick (Current_systick* details)
        {
          // TODO
          return 1234;
        }

        const char*
        strerror (return_t ret)
        {
          const char* str;
          switch (ret)
            {
            case Return::os_event_signal:
              str = "signal event occurred";
              break;

            case Return::os_event_message:
              str = "message event occurred";
              break;

            case Return::os_event_mail:
              str = "mail event occurred";
              break;

            case Return::os_event_timeout:
              str = "timeout occurred";
              break;

            case Return::os_error_parameter:
              str = "mandatory parameter missing or incorrect object";
              break;

            case Return::os_error_resource:
              str = "resource not available";
              break;

            case Return::os_error_timeout_resource:
              str = "resource not available within given time";
              break;

            case Return::os_error_isr:
              str = "not allowed in ISR context";
              break;

            case Return::os_error_isr_recursive:
              str = "function called multiple times from ISR with same object";
              break;

            case Return::os_error_priority:
              str =
                  "system cannot determine priority or thread has illegal priority";
              break;

            case Return::os_error_no_memory:
              str = "system is out of memory";
              break;

            case Return::os_error_value:
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

      Thread no_thread
        { "none", nullptr, 0, Priority::normal, (Thread_func_vp) nullptr,
            nullptr };

      namespace thread
      {
        Thread&
        get_current (void)
        {
          // TODO
          return no_thread;
        }

        return_t
        yield (void)
        {
          // TODO
          return os_ok;
        }

        return_t
        wait (millis_t millisec, event_t* ret)
        {
          // TODO
          return os_ok;
        }

        return_t
        wait_signals (signals_t signals, millis_t millisec, signals_t* ret)
        {
          // TODO
          return os_ok;
        }

#if 0
        return_t
        delay (millis_t millisec)
          {
            // TODO
            return os_ok;
          }
#endif

        return_t
        sleep (sys_ticks_t ticks)
        {
          // TODO
          return os_ok;
        }

      }

      // ======================================================================

      Named_object::Named_object (const char* name) :
          name_ (name != nullptr ? name : "-")
      {
        ;
      }

      // ======================================================================

      Thread::Thread (const char* name, void* stack,
                      std::size_t stack_size_bytes, Priority prio,
                      Thread_func_vp function,  void* args) : //
          Named_object
            { name }, //
          prio_
            { prio }, //
          func_ptr_
            { function }, //
          args_ptr_
            { (Args_ptr::element_type*) args, nullptr } //
      {
        trace::printf ("%s(\"%s\", %d) @%p \n", __func__, get_name (),
                       stack_size_bytes, this);
      }

      Thread::~Thread ()
      {
        trace::printf ("%s() @%p \n", __func__, this);
      }

      Priority
      Thread::get_priority (void)
      {
        return prio_;
      }

      Priority
      Thread::set_priority (Priority prio)
      {
        Priority ret = prio_;
        prio_ = prio;
        return ret;
      }

      signals_t
      Thread::set_signals (signals_t signals)
      {
        // TODO
        return 0;
      }

      signals_t
      Thread::clear_signals (signals_t signals)
      {
        // TODO
        return 0;
      }

      void
      Thread::join (void)
      {
        // TODO
      }

      void
      Thread::detach (void)
      {
        // TODO
      }

#if defined(TESTING)
      void
      Thread::__run_function (void)
      {
        func_ptr_ (args_ptr_.get ());
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

      return_t
      Timer::start (millis_t millisec)
      {
        // TODO
        return os_ok;
      }

      return_t
      Timer::stop (void)
      {
        // TODO
        return os_ok;
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

      return_t
      Mutex::wait (void)
      {
        return os_ok;
      }

      return_t
      Mutex::try_wait (sys_ticks_t ticks)
      {
        return os_ok;
      }

      return_t
      Mutex::release (void)
      {
        return os_ok;
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

      return_t
      Recursive_mutex::wait (void)
      {
        return os_ok;
      }

      return_t
      Recursive_mutex::try_wait (sys_ticks_t ticks)
      {
        return os_ok;
      }

      return_t
      Recursive_mutex::release (void)
      {
        return os_ok;
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

      return_t
      Condition_variable::notify_one () noexcept
      {
        // TODO
      }

      return_t
      Condition_variable::notify_all () noexcept
      {
        // TODO
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
        return os_ok;
      }

      return_t
      Semaphore::release (void)
      {
        return os_ok;
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

      return_t
      Pool::free (void* block)
      {
        // TODO
        return os_ok;
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

      return_t
      Message_queue::put (void* info, millis_t millisec)
      {
        // TODO
        return os_ok;
      }

      return_t
      Message_queue::get (millis_t millisec, void** ret)
      {
        // TODO
        return os_ok;
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

      return_t
      Mail_queue::put (void* mail)
      {
        // TODO
        return os_ok;
      }

      return_t
      Mail_queue::get (millis_t millisec, void** ret)
      {
        // TODO
        return os_ok;
      }

      return_t
      Mail_queue::free (void* mail)
      {
        // TODO
        return os_ok;
      }

    // ======================================================================

#pragma GCC diagnostic pop

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */
