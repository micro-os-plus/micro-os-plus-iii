/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code is inspired by ARM CMSIS cmsis_os.h file, v1.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_RTOS_OS_H_
#define CMSIS_PLUS_RTOS_OS_H_

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <cstddef>

namespace os
{
  namespace cmsis
  {
    namespace rtos
    {
      using priority_t = uint32_t;

      enum class Priority
        : priority_t
          {
            //
        idle = -3, ///< priority: idle (lowest)
        low = -2, ///< priority: low
        below_normal = -1, ///< priority: below normal
        normal = 0, ///< priority: normal (default)
        above_normal = +1, ///< priority: above normal
        high = +2, ///< priority: high
        realtime = +4, ///< priority: realtime (highest)
        error = 0x84 ///< system cannot determine priority or thread has illegal priority
      };

      using return_t = uint32_t;

      constexpr return_t RETURN_OK = 0;

      enum class Event
        : return_t
          {
            //
        signal = 0x08, ///< function completed; signal event occurred.
        message = 0x10, ///< function completed; message event occurred.
        mail = 0x20, ///< function completed; mail event occurred.
        timeout = 0x40, ///< function completed; timeout occurred.
      };

      enum class Error
        : return_t
          {
            //
        parameter = 0x80, ///< parameter error: a mandatory parameter was missing or specified an incorrect object.
        resource = 0x81, ///< resource not available: a specified resource was not available.
        timeout_resource = 0xC1, ///< resource not available within given time: a specified resource was not available within the timeout period.
        isr = 0x82, ///< not allowed in ISR context: the function cannot be called from interrupt service routines.
        isr_recursive = 0x83, ///< function called multiple times from ISR with same object.
        priority = 0x84, ///< system cannot determine priority or thread has illegal priority.
        no_memory = 0x85, ///< system is out of memory: it was impossible to allocate or reserve memory for the operation.
        value = 0x86, ///< value of a parameter is out of range.
        os = 0xFF, ///< unspecified RTOS error: run-time error but no other error message fits.
        reserved = 0x7FFFFFFF ///< prevent from enum down-size compiler optimization.
      };

      using timer_type_t = uint32_t;

      enum class Timer_type
        : timer_type_t
          {
            //
        once = 0, //
        periodic = 1 //
      };

      constexpr uint32_t SYS_TICK_FREQUENCY_HZ = 1000; // TODO: Param

      constexpr uint32_t WAIT_FOREVER = 0xFFFFFFFF;

      constexpr uint32_t
      compute_sys_ticks (uint32_t microsec)
      {
        return (((uint64_t) microsec) * SYS_TICK_FREQUENCY_HZ) / 1000000;
      }

      constexpr uint32_t MAX_SEMAPHORE_COUNT = 0xFFFFFFFF;

      class MailQueue;
      class MessageQueue;

      using signals_t = int32_t;

      using event_t = struct
        {
          return_t status; ///< status code: event or error information
          union
            {
              uint32_t v; ///< message as 32-bit value
              void* p;///< message or mail as void pointer
              signals_t signals;///< signal flags
            }value; ///< event value
          union
            {
              MailQueue* mail_id;     ///< mail id obtained by \ref osMailCreate
              MessageQueue* message_id;///< message id obtained by \ref osMessageCreate
            }def;                               ///< event definition
        };

      namespace kernel
      {
        return_t
        initialise (void);

        return_t
        start (void);

        bool
        is_running (void);

        uint32_t
        get_ticks (void);

      } /* namespace kernel */

      class Thread;

      namespace thread
      {
        Thread&
        get_current (void);

        void
        yield (void);

        return_t
        wait (uint32_t millisec, event_t* ret);

        return_t
        wait_signals (signals_t signals, uint32_t millisec, event_t* ret);

        return_t
        delay (uint32_t millisec);
      }

      typedef void
      (*thread_func_t) (const void* args);

      class Thread
      {
      public:

        Thread (const char* name, thread_func_t function, priority_t prio,
                void* stack, std::size_t stack_size_bytes,
                uint32_t max_instances, const void* args);

        return_t
        terminate (void);

        priority_t
        get_priority (void);

        priority_t
        set_priority (priority_t prio);

        signals_t
        set_signals (signals_t signals);

        signals_t
        clear_signals (signals_t signals);
      };

      typedef void
      (*timer_func_t) (const void* args);

      class Timer
      {
      public:

        Timer (const char* name, timer_func_t function, timer_type_t type,
               void* args);

        return_t
        start (uint32_t millisec);

        return_t
        stop (void);
      };

      class Mutex
      {
      public:

        Mutex (const char* name);

        return_t
        wait (uint32_t millisec);

        return_t
        release (void);
      };

      class Semaphore
      {
      public:

        Semaphore (const char* name, int32_t count, uint32_t max_count =
                       MAX_SEMAPHORE_COUNT);

        int32_t
        wait (uint32_t millisec);

        return_t
        release (void);
      };

      class Pool
      {
      public:

        Pool (const char* name, std::size_t items, uint32_t item_size,
              void* mem); // template?

        void*
        alloc (void);

        void*
        calloc (void);

        return_t
        free (void* block);
      };

      class MessageQueue
      {
      public:
        MessageQueue (const char* name, std::size_t items, void* mem,
                      Thread* thread);

        return_t
        put (void* info, uint32_t millisec);

        return_t
        get (uint32_t millisec, event_t* ret);
      };

      class MailQueue
      {
      public:
        MailQueue (const char* name, std::size_t messages,
                   std::size_t message_size, void* mem, Thread* thread);

        void*
        alloc (uint32_t millisec);

        void*
        calloc (uint32_t millisec);

        return_t
        put (void* mail);

        return_t
        get (uint32_t millisec, event_t* ret);

        return_t
        free (void* mail);
      };

    } /* namespace rtos */
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
