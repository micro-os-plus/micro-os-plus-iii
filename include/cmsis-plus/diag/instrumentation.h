/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit.
 */

#ifndef CMSIS_PLUS_DIAG_INSTRUMENTATION_H_
#define CMSIS_PLUS_DIAG_INSTRUMENTATION_H_

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_INSTRUMENTATION)

#include "SEGGER_SYSVIEW.h"
#include "SEGGER_SYSVIEW_micro-os-plus.h"

#endif // defined(INSTRUMENTATION)

#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_SWITCH (1u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_CLOCK (2u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_THREAD_FLAGS (3u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_SEMAPHORE (4u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MUTEX (5u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MQUEUE_SEND (6u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MQUEUE_RECEIVE (7u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MEMPOOL (8u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_EVENT_FLAGS (9u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_JOIN (10u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_USER (11u)

#define OS_INTEGER_INSTRUMENTATION_ID_OFFSET (32u)
#define OS_INTEGER_INSTRUMENTATION_ID_EXIT \
  (OS_INTEGER_INSTRUMENTATION_ID_OFFSET + 1u)

#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATED \
  (OS_INTEGER_INSTRUMENTATION_ID_OFFSET + 2u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_DESTROYED \
  (OS_INTEGER_INSTRUMENTATION_ID_OFFSET + 3u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCKED \
  (OS_INTEGER_INSTRUMENTATION_ID_OFFSET + 4u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCKED \
  (OS_INTEGER_INSTRUMENTATION_ID_OFFSET + 5u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCKED \
  (OS_INTEGER_INSTRUMENTATION_ID_OFFSET + 6u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCKED \
  (OS_INTEGER_INSTRUMENTATION_ID_OFFSET + 7u)

#if defined(OS_INCLUDE_INSTRUMENTATION)

// Invoke SEGGER SystemView functions.

#if defined(__cplusplus)

namespace os
{
  namespace instrumentation
  {
    static void inline __attribute__ ((__always_inline__))
    configure (void)
    {
      SEGGER_SYSVIEW_Conf ();
    }

    static void inline __attribute__ ((__always_inline__))
    start (void)
    {
      SEGGER_SYSVIEW_Start ();
    }

    static void inline __attribute__ ((__always_inline__))
    stop (void)
    {
      SEGGER_SYSVIEW_Stop ();
    }

    namespace interrupt
    {
      static void inline __attribute__ ((__always_inline__))
      entered (void)
      {
        SEGGER_SYSVIEW_RecordEnterISR ();
      }

      static void inline __attribute__ ((__always_inline__))
      exited (void)
      {
        // On Cortex-M, the interrupt may return or may be chained to PendSV,
        // to invoke the scheduler.
        // Since the PendSV bit is write only, it is not easy to determine
        // if the scheduler is invoked without additional logic.
        // Since most of the time the scheduler is invoked, use
        // this variant of the function.
        SEGGER_SYSVIEW_RecordExitISRToScheduler ();
        // SEGGER_SYSVIEW_RecordExitISR ();
      }
    } // namespace interrupt

    namespace thread
    {
      static void inline __attribute__ ((__always_inline__))
      created (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskCreate (reinterpret_cast<U32> (thread));
        SYSVIEW_SendTaskInfo (thread);
      }

      static void inline __attribute__ ((__always_inline__))
      ready (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskStartReady (reinterpret_cast<U32> (thread));
      }

      static void inline __attribute__ ((__always_inline__))
      suspended (os::rtos::thread* thread, unsigned int cause = 0)
      {
        SEGGER_SYSVIEW_OnTaskStopReady (reinterpret_cast<U32> (thread), cause);
      }

      static void inline __attribute__ ((__always_inline__))
      active (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskStartExec (reinterpret_cast<U32> (thread));
      }

      static void inline __attribute__ ((__always_inline__))
      idle (void)
      {
        SEGGER_SYSVIEW_OnIdle ();
      }

      static void inline __attribute__ ((__always_inline__))
      terminated (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskTerminate (reinterpret_cast<U32> (thread));
      }
    } // namespace thread

    namespace heap
    {
      static void inline __attribute__ ((__always_inline__))
      define (void* heap, void* base, std::size_t heap_size,
              std::size_t metadata_size)
      {
        SEGGER_SYSVIEW_HeapDefine (heap, base, heap_size, metadata_size);
      }

      static void inline __attribute__ ((__always_inline__))
      allocated (void* heap, void* user_data, std::size_t size)
      {
        SEGGER_SYSVIEW_HeapAlloc (heap, user_data, size);
      }

      static void inline __attribute__ ((__always_inline__))
      deallocated (void* heap, void* user_data)
      {
        SEGGER_SYSVIEW_HeapFree (heap, user_data);
      }
    } // namespace heap

    namespace mutex
    {
      void
      created (os::rtos::mutex* mutex);

      void
      destroyed (os::rtos::mutex* mutex);

      void
      locked (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      try_locked (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      timed_locked (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      unlocked (os::rtos::mutex* mutex, os::rtos::result_t result);
    } // namespace mutex

    void
    exit (int exit_code);

  } // namespace instrumentation
} // namespace os

#endif // defined(__cplusplus)

#if defined(__cplusplus)
extern "C"
{
#endif

  inline void __attribute__ ((__always_inline__))
  os_instrumentation_stop (void)
  {
    SEGGER_SYSVIEW_Stop ();
  }

  extern void
  os_instrumentation_exit (int exit_code);

#if defined(__cplusplus)
}
#endif

// ----------------------------------------------------------------------------
#else // defined(OS_INCLUDE_INSTRUMENTATION)

#if defined(__cplusplus)

// All functions have empty content.
namespace os
{
  namespace instrumentation
  {
    static void inline __attribute__ ((__always_inline__))
    configure (void)
    {
    }

    static void inline __attribute__ ((__always_inline__))
    start (void)
    {
    }

    static void inline __attribute__ ((__always_inline__))
    stop (void)
    {
    }

    namespace interrupt
    {
      static void inline __attribute__ ((__always_inline__))
      entered (void)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      exited (void)
      {
      }
    } // namespace interrupt

    namespace thread
    {
      static void inline __attribute__ ((__always_inline__))
      created (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      ready (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      suspended (os::rtos::thread* thread, unsigned int cause = 0)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      active (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      idle (void)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      terminated (os::rtos::thread* thread)
      {
      }
    } // namespace thread

    namespace heap
    {
      static void inline __attribute__ ((__always_inline__))
      define (void* heap, void* base, std::size_t heap_size,
              std::size_t metadata_size)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      allocated (void* heap, void* user_data, std::size_t size)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      deallocated (void* heap, void* user_data)
      {
      }
    } // namespace heap

    namespace mutex
    {
      static void inline __attribute__ ((__always_inline__))
      created (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroyed (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      locked (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_locked (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_locked (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      unlocked (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }
    } // namespace mutex

    static void inline __attribute__ ((__always_inline__))
    exit (int exit_code)
    {
    }

  } // namespace instrumentation
} // namespace os

#endif // defined(__cplusplus)

inline void __attribute__ ((__always_inline__))
os_instrumentation_stop (void)
{
}

inline void __attribute__ ((__always_inline__))
os_instrumentation_exit (int exit_code)
{
}

#endif // defined(OS_INCLUDE_INSTRUMENTATION)

// ----------------------------------------------------------------------------

#endif // CMSIS_PLUS_DIAG_INSTRUMENTATION_H_
