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
        initialise (void)
        {
          return os_ok;
        }

        return_t
        start (void)
        {
          return os_ok;
        }

        bool
        is_running (void)
        {
          return true;
        }

        uint32_t
        get_ticks (void)
        {
          return 1234;
        }

      } /* namespace kernel */

      // ======================================================================

      Thread no_thread
        { "none", Priority::normal, nullptr, 0, (thread_func_cvp_t) nullptr, nullptr };

      namespace thread
      {
        Thread&
        get_current (void)
        {
          return no_thread;
        }

        return_t
        yield (void)
        {
          return os_ok;
        }

        return_t
        wait (millis_t millisec, event_t* ret)
        {
          return os_ok;
        }

        return_t
        wait_signals (signals_t signals, millis_t millisec, signals_t* ret)
        {
          return os_ok;
        }

        return_t
        delay (millis_t millisec)
        {
          return os_ok;
        }

        void
        sleep_for_ticks (uint32_t)
        {
          ;
        }

      }

      // ======================================================================

      Named_object::Named_object (const char* name) :
          name_ (name != nullptr ? name : "-")
      {
        ;
      }

      // ======================================================================

      Thread::Thread (const char* name, Priority prio, void* stack,
                      std::size_t stack_size_bytes, thread_func_cvp_t function,
                      const void* args) : //
          Named_object
            { name }, //
          prio_
            { prio }, //
          func_
            { function }, //
          args_
            { args } //
      {
#if defined(OS_INCLUDE_CMSIS_THREAD_VARIADICS)
        has_binding_ = false;
#endif
      }

//      Thread::Thread (thread_func_t function, const void* args) : //
//          Named_object
//            { nullptr }, //
//          prio_
//            { Priority::normal }, //
//          func_
//            { function }, //
//          args_
//            { args } //
//      {
//#if defined(OS_INCLUDE_CMSIS_THREAD_VARIADICS)
//        has_binding_ = false;
//#endif
//      }

      Thread::~Thread ()
      {
#if defined(OS_INCLUDE_CMSIS_THREAD_VARIADICS)
        if (has_binding_ && args_ != nullptr)
          {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdelete-incomplete"
            delete args_;
#pragma GCC diagnostic pop
          }
#endif
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
        return 0;
      }

      signals_t
      Thread::clear_signals (signals_t signals)
      {
        return 0;
      }

#if defined(TESTING)
      void
      Thread::__run_function (void)
      {
        func_ (args_);
      }
#endif

      // ======================================================================

      Timer::Timer (const char* name, timer_func_t function, timer_type_t type,
                    void* args) : //
          Named_object (name)
      {
        ;
      }

      Timer::~Timer ()
      {
        ;
      }

      return_t
      Timer::start (millis_t millisec)
      {
        return os_ok;
      }

      return_t
      Timer::stop (void)
      {
        return os_ok;
      }

      // ======================================================================

      Mutex::Mutex (const char* name) :
          Named_object (name)
      {
        ;
      }

      Mutex::~Mutex ()
      {
        ;
      }

      return_t
      Mutex::wait (millis_t millisec)
      {
        return os_ok;
      }

      return_t
      Mutex::release (void)
      {
        return os_ok;
      }

      // ======================================================================

      Semaphore::Semaphore (const char* name, int32_t count, uint32_t max_count) :
          Named_object (name)
      {
        ;
      }

      Semaphore::~Semaphore ()
      {
        ;
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
        ;
      }

      Pool::~Pool ()
      {
        ;
      }

      void*
      Pool::alloc (void)
      {
        return nullptr;
      }

      void*
      Pool::calloc (void)
      {
        return nullptr;
      }

      return_t
      Pool::free (void* block)
      {
        return os_ok;
      }

      // ======================================================================

      Message_queue::Message_queue (const char* name, std::size_t items,
                                    void* mem, Thread* thread) :
          Named_object (name)
      {
        ;
      }

      Message_queue::~Message_queue ()
      {
        ;
      }

      return_t
      Message_queue::put (void* info, millis_t millisec)
      {
        return os_ok;
      }

      return_t
      Message_queue::get (millis_t millisec, void** ret)
      {
        return os_ok;
      }

      // ======================================================================

      Mail_queue::Mail_queue (const char* name, std::size_t messages,
                              std::size_t message_size, void* mem,
                              Thread* thread) :
          Named_object (name)
      {
        ;
      }

      Mail_queue::~Mail_queue ()
      {
        ;
      }

      void*
      Mail_queue::alloc (millis_t millisec)
      {
        return nullptr;
      }

      void*
      Mail_queue::calloc (millis_t millisec)
      {
        return nullptr;
      }

      return_t
      Mail_queue::put (void* mail)
      {
        return os_ok;
      }

      return_t
      Mail_queue::get (millis_t millisec, void** ret)
      {
        return os_ok;
      }

      return_t
      Mail_queue::free (void* mail)
      {
        return os_ok;
      }

    // ======================================================================

#pragma GCC diagnostic pop

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */
