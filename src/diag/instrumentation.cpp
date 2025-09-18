/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#if defined(OS_INCLUDE_INSTRUMENTATION)

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/instrumentation.h>

#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix-io/device.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace instrumentation
  {
    void
    configure (void)
    {
      SEGGER_SYSVIEW_Conf ();
    }

    void
    start (void)
    {
      SEGGER_SYSVIEW_Start ();
    }

    void
    stop (void)
    {
      SEGGER_SYSVIEW_Stop ();
    }

    namespace interrupt
    {
      static bool exit_isr_to_scheduler = false;

      void
      entered (void)
      {
        SEGGER_SYSVIEW_RecordEnterISR ();
      }

      void
      exited (void)
      {
        // On Cortex-M, the interrupt may return or may be chained to PendSV,
        // to invoke the scheduler.
        // Since the PendSV bit is write only, it is not easy to determine
        // if the scheduler is invoked without additional logic.
        // This flag is set on reschedule() and cleared in active()
        if (exit_isr_to_scheduler)
          {
            SEGGER_SYSVIEW_RecordExitISRToScheduler ();
          }
        else
          {
            SEGGER_SYSVIEW_RecordExitISR ();
          }
      }
    } // namespace interrupt

    namespace scheduler
    {
      void
      lock (bool state)
      {
        SEGGER_SYSVIEW_RecordU32 (OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_LOCK,
                                  static_cast<U32> (state));
      }

      void
      unlock (bool state)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_UNLOCK,
            static_cast<U32> (state));
      }

      void
      locked_set (bool state)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_LOCKED_SET,
            static_cast<U32> (state));
      }

      // Must always be called, otherwise the ISR logic fails.
      void
      reschedule (void)
      {
        SEGGER_SYSVIEW_RecordVoid (
            OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_RESCHEDULE);
        interrupt::exit_isr_to_scheduler = true;
      }

      void
      preemptive_set (bool state)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_PREEMPTIVE_SET,
            static_cast<U32> (state));
      }
    } // namespace scheduler

    namespace thread
    {
      void
      create (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      create_return (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_CREATE);
      }

      void
      destroy (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      destroy_return (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_DESTROY);
      }

      // ----------------------------------------------------------------------
      // Specific API calls.

      void
      created (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskCreate (reinterpret_cast<U32> (thread));
        SYSVIEW_SendTaskInfo (thread);
      }

      void
      ready (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskStartReady (reinterpret_cast<U32> (thread));
      }

      void
      suspended (os::rtos::thread* thread, unsigned int cause)
      {
        SEGGER_SYSVIEW_OnTaskStopReady (reinterpret_cast<U32> (thread), cause);
      }

      void
      active (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskStartExec (reinterpret_cast<U32> (thread));
        interrupt::exit_isr_to_scheduler = false;
      }

      void
      idle (void)
      {
        SEGGER_SYSVIEW_OnIdle ();
      }

      void
      terminated (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_OnTaskTerminate (reinterpret_cast<U32> (thread));
      }

      // ----------------------------------------------------------------------

      void
      yield (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_YIELD,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      suspend (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_SUSPEND,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      resume (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_RESUME,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      resume_return (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_RESUME);
      }

      void
      priority_set (os::rtos::thread* thread, unsigned int prio)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_SET,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            static_cast<U32> (prio));
      }

      void
      priority_set_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_SET,
            static_cast<U32> (res));
      }

      void
      priority_inherited_set (os::rtos::thread* thread, unsigned int prio)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_INHERITED_SET,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            static_cast<U32> (prio));
      }

      void
      priority_inherited_set_retval (os::rtos::thread* thread,
                                     os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_INHERITED_SET,
            static_cast<U32> (res));
      }

      void
      detach (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_DETACH,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      detach_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_DETACH,
            static_cast<U32> (res));
      }

      void
      join (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_JOIN,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      join_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_JOIN, static_cast<U32> (res));
      }

      void
      cancel (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_CANCEL,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      cancel_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_CANCEL,
            static_cast<U32> (res));
      }

      void
      interrupt (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_INTERRUPT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      interrupt_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_INTERRUPT,
            static_cast<U32> (res));
      }

      void
      kill (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_KILL,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)));
      }

      void
      kill_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_KILL, static_cast<U32> (res));
      }

      void
      internal_exit (os::rtos::thread* thread, void* exit_ptr)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_INTERNAL_EXIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            reinterpret_cast<U32> (exit_ptr));
      }

      void
      internal_exit_return (os::rtos::thread* thread)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_INTERNAL_EXIT);
      }

      void
      flags_wait (os::rtos::thread* thread, unsigned int mask,
                  unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            static_cast<U32> (mask), static_cast<U32> (mode));
      }

      void
      flags_wait_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            thread->flags_get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_WAIT,
            static_cast<U32> (res));
      }

      void
      flags_try_wait (os::rtos::thread* thread, unsigned int mask,
                      unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TRY_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            static_cast<U32> (mask), static_cast<U32> (mode));
      }

      void
      flags_try_wait_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TRY_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            thread->flags_get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TRY_WAIT,
            static_cast<U32> (res));
      }

      void
      flags_timed_wait (os::rtos::thread* thread, unsigned int mask,
                        unsigned int timeout, unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TIMED_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            static_cast<U32> (mask), static_cast<U32> (timeout),
            static_cast<U32> (mode));
      }

      void
      flags_timed_wait_retval (os::rtos::thread* thread,
                               os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TIMED_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            thread->flags_get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TIMED_WAIT,
            static_cast<U32> (res));
      }

      void
      flags_raise (os::rtos::thread* thread, unsigned int mask)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_RAISE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            static_cast<U32> (mask));
      }

      void
      flags_raise_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_RAISE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            thread->flags_get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_RAISE,
            static_cast<U32> (res));
      }

      void
      flags_clear (os::rtos::thread* thread, unsigned int mask)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_CLEAR,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            static_cast<U32> (mask));
      }

      void
      flags_clear_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_CLEAR_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (thread)),
            thread->flags_get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_CLEAR,
            static_cast<U32> (res));
      }
    } // namespace thread

    namespace memory_resource
    {
      void
      define (os::rtos::memory::memory_resource* heap, void* base,
              std::size_t heap_size, std::size_t metadata_size)
      {
        if (heap->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (heap),
                                         heap->name ());
          }
        SEGGER_SYSVIEW_HeapDefine (heap, base, heap_size, metadata_size);
      }
    } // namespace memory_resource

    namespace mutex
    {
      void
      create (os::rtos::mutex* mutex)
      {
        if (mutex->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mutex),
                                         mutex->name ());
          }
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->type ()),
            static_cast<U32> (mutex->protocol ()),
            static_cast<U32> (mutex->robustness ()));
      }

      void
      create_return (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATE);
      }

      void
      destroy (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)));
      }

      void
      destroy_return (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_DESTROY);
      }

      void
      lock (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCK,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)));
      }

      void
      lock_retval (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCK_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCK,
            static_cast<U32> (result));
      }

      void
      try_lock (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCK,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)));
      }

      void
      try_lock_retval (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCK_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCK,
            static_cast<U32> (result));
      }

      void
      timed_lock (os::rtos::mutex* mutex, unsigned int timeout)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCK,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (timeout));
      }

      void
      timed_lock_retval (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCK_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCK,
            static_cast<U32> (result));
      }

      void
      unlock (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCK,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)));
      }

      void
      unlock_retval (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCK_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCK,
            static_cast<U32> (result));
      }

      void
      reset (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_RESET,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)));
      }

      void
      reset_retval (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_RESET_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_RESET,
            static_cast<U32> (result));
      }

      void
      prio_ceiling_set (os::rtos::mutex* mutex, unsigned int prio)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_PRIO_CEILING_SET,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (prio));
      }

      void
      prio_ceiling_set_retval (os::rtos::mutex* mutex,
                               os::rtos::result_t result)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_PRIO_CEILING_SET_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_PRIO_CEILING_SET,
            static_cast<U32> (result));
      }
    } // namespace mutex

    namespace semaphore
    {
      void
      create (os::rtos::semaphore* semaphore)
      {
        if (semaphore->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (semaphore),
                                         semaphore->name ());
          }
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->max_value ()),
            static_cast<U32> (semaphore->value ()));
      }

      void
      create_return (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_CREATE);
      }

      void
      destroy (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)));
      }

      void
      destroy_return (os::rtos::semaphore* semaphore)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROY_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROY);
      }

      void
      post (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POST,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)));
      }

      void
      post_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POST_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POST,
            static_cast<U32> (res));
      }

      void
      wait (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)));
      }

      void
      wait_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAIT,
            static_cast<U32> (res));
      }

      void
      try_wait (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)));
      }

      void
      try_wait_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAIT,
            static_cast<U32> (res));
      }

      void
      timed_wait (os::rtos::semaphore* semaphore, unsigned int timeout)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (timeout));
      }

      void
      timed_wait_retval (os::rtos::semaphore* semaphore,
                         os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAIT,
            static_cast<U32> (res));
      }

      void
      reset (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_RESET,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)));
      }

      void
      reset_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_RESET_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_RESET,
            static_cast<U32> (res));
      }
    } // namespace semaphore

    namespace message_queue
    {
      void
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes)
      {
        if (mqueue->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mqueue),
                                         mqueue->name ());
          }
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (msgs), static_cast<U32> (msg_size_bytes));
      }

      void
      create_return (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_CREATE);
      }

      void
      destroy (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)));
      }

      void
      destroy_return (os::rtos::message_queue* mqueue)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_DESTROY_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_DESTROY);
      }

      void
      send (os::rtos::message_queue* mqueue, std::size_t nbytes,
            unsigned int mprio)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_SEND,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mprio));
      }

      void
      send_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_SEND_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_SEND,
            static_cast<U32> (res));
      }

      void
      try_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                unsigned int mprio)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_SEND,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mprio));
      }

      void
      try_send_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_SEND_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_SEND,
            static_cast<U32> (res));
      }

      void
      timed_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                  unsigned int timeout, unsigned int mprio)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_SEND,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (timeout),
            static_cast<U32> (mprio));
      }

      void
      timed_send_retval (os::rtos::message_queue* mqueue,
                         os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_SEND_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_SEND,
            static_cast<U32> (res));
      }

      void
      receive (os::rtos::message_queue* mqueue, std::size_t nbytes)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RECEIVE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes));
      }

      void
      receive_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RECEIVE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RECEIVE,
            static_cast<U32> (res));
      }

      void
      try_receive (os::rtos::message_queue* mqueue, std::size_t nbytes)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_RECEIVE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes));
      }

      void
      try_receive_retval (os::rtos::message_queue* mqueue,
                          os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_RECEIVE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_RECEIVE,
            static_cast<U32> (res));
      }

      void
      timed_receive (os::rtos::message_queue* mqueue, std::size_t nbytes,
                     unsigned int timeout)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_RECEIVE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (timeout));
      }

      void
      timed_receive_retval (os::rtos::message_queue* mqueue,
                            os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_RECEIVE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_RECEIVE,
            static_cast<U32> (res));
      }

      void
      reset (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RESET,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)));
      }

      void
      reset_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RESET_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RESET,
            static_cast<U32> (res));
      }
    } // namespace message_queue

    namespace message_queue_allocated
    {
      void
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes)
      {
        if (mqueue->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mqueue),
                                         mqueue->name ());
          }
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (msgs), static_cast<U32> (msg_size_bytes));
      }

      void
      create_return (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_CREATE);
      }

      void
      destroy (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)));
      }

      void
      destroy_return (os::rtos::message_queue* mqueue)
      {
        // Not needed, since it is displayed by the parent destructor.
        // #if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        //         SEGGER_SYSVIEW_RecordU32x2 (
        //             OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_DESTROY_VALUES,
        //             SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32>
        //             (mqueue)), static_cast<U32> (mqueue->length ()));
        // #endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_DESTROY);
      }
    } // namespace message_queue_allocated

    namespace message_queue_inclusive
    {
      void
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes)
      {
        if (mqueue->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mqueue),
                                         mqueue->name ());
          }
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_INCLUSIVE_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (msgs), static_cast<U32> (msg_size_bytes));
      }

      void
      create_return (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_INCLUSIVE_CREATE);
      }
    } // namespace message_queue_inclusive

    namespace memory_pool
    {
      void
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes)
      {
        if (mpool->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mpool),
                                         mpool->name ());
          }
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (blocks), static_cast<U32> (block_size_bytes));
      }

      void
      create_return (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE);
      }

      void
      destroy (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      destroy_return (os::rtos::memory_pool* mpool)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_DESTROY_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_DESTROY);
      }

      void
      alloc (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOC,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOC_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOC,
            reinterpret_cast<U32> (addr));
      }

      void
      try_alloc (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TRY_ALLOC,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      try_alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TRY_ALLOC_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TRY_ALLOC,
            reinterpret_cast<U32> (addr));
      }

      void
      timed_alloc (os::rtos::memory_pool* mpool, unsigned int timeout)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TIMED_ALLOC,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (timeout));
      }

      void
      timed_alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TIMED_ALLOC_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TIMED_ALLOC,
            reinterpret_cast<U32> (addr));
      }

      void
      free (os::rtos::memory_pool* mpool, void* block)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_FREE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            reinterpret_cast<U32> (block));
      }

      void
      free_retval (os::rtos::memory_pool* mpool, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_FREE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_FREE,
            static_cast<U32> (res));
      }

      void
      reset (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_RESET,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      reset_retval (os::rtos::memory_pool* mpool, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_RESET_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_RESET,
            static_cast<U32> (res));
      }
    } // namespace memory_pool

    namespace memory_pool_allocated
    {
      void
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes)
      {
        if (mpool->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mpool),
                                         mpool->name ());
          }
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (blocks), static_cast<U32> (block_size_bytes));
      }

      void
      create_return (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_CREATE);
      }

      void
      destroy (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      destroy_return (os::rtos::memory_pool* mpool)
      {
        // Not needed, since it is displayed by the parent destructor.
        // #if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        //       SEGGER_SYSVIEW_RecordU32x2 (
        //           OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_DESTROY_VALUES,
        //           SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
        //           static_cast<U32> (mpool->count ()));
        // #endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_DESTROY);
      }
    } // namespace memory_pool_allocated

    namespace memory_pool_inclusive
    {
      void
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes)
      {
        if (mpool->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mpool),
                                         mpool->name ());
          }
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_INCLUSIVE_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (blocks), static_cast<U32> (block_size_bytes));
      }

      void
      create_return (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_INCLUSIVE_CREATE);
      }
    } // namespace memory_pool_inclusive

    namespace event_flags
    {
      void
      create (os::rtos::event_flags* evflags)
      {
        if (evflags->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (evflags),
                                         evflags->name ());
          }
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)));
      }

      void
      create_return (os::rtos::event_flags* evflags)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CREATE);
      }

      void
      destroy (os::rtos::event_flags* evflags)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)));
      }

      void
      destroy_return (os::rtos::event_flags* evflags)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_DESTROY_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_DESTROY);
      }

      void
      wait (os::rtos::event_flags* evflags, unsigned int mask,
            unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (mode));
      }

      void
      wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_WAIT,
            static_cast<U32> (res));
      }

      void
      try_wait (os::rtos::event_flags* evflags, unsigned int mask,
                unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TRY_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (mode));
      }

      void
      try_wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TRY_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TRY_WAIT,
            static_cast<U32> (res));
      }

      void
      timed_wait (os::rtos::event_flags* evflags, unsigned int mask,
                  unsigned int timeout, unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TIMED_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (timeout),
            static_cast<U32> (mode));
      }

      void
      timed_wait_retval (os::rtos::event_flags* evflags,
                         os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TIMED_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TIMED_WAIT,
            static_cast<U32> (res));
      }

      void
      raise (os::rtos::event_flags* evflags, unsigned int mask)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_RAISE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask));
      }

      void
      raise_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_RAISE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_RAISE,
            static_cast<U32> (res));
      }

      void
      clear (os::rtos::event_flags* evflags, unsigned int mask)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CLEAR,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask));
      }

      void
      clear_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CLEAR_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CLEAR,
            static_cast<U32> (res));
      }
    } // namespace event_flags

    namespace clock
    {
      void
      create (os::rtos::clock* clock)
      {
        if (clock->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (clock),
                                         clock->name ());
          }
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (clock)));
      }

      void
      destroy (os::rtos::clock* clock)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (clock)));
      }

      void
      sleep_for (os::rtos::clock* clock, unsigned int duration)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_FOR,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (clock)),
            static_cast<U32> (duration));
      }

      void
      sleep_for_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_FOR,
            static_cast<U32> (res));
      }

      void
      sleep_until (os::rtos::clock* clock, unsigned long long timestamp)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_UNTIL,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (clock)),
            static_cast<U32> (timestamp >> 32),
            static_cast<U32> (timestamp & 0xFFFFFFFF));
      }

      void
      sleep_until_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_UNTIL,
            static_cast<U32> (res));
      }

      void
      wait_for (os::rtos::clock* clock, unsigned int duration)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_WAIT_FOR,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (clock)),
            static_cast<U32> (duration));
      }

      void
      wait_for_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CLOCK_WAIT_FOR,
            static_cast<U32> (res));
      }

    } // namespace clock

    namespace adjustable_clock
    {
      void
      sleep_until (os::rtos::clock* clock, unsigned long long timestamp)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_ADJUSTABLE_CLOCK_SLEEP_UNTIL,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (clock)),
            static_cast<U32> (timestamp >> 32),
            static_cast<U32> (timestamp & 0xFFFFFFFF));
      }

      void
      sleep_until_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_ADJUSTABLE_CLOCK_SLEEP_UNTIL,
            static_cast<U32> (res));
      }
    } // namespace adjustable_clock

    namespace condition_variable
    {
      void
      create (os::rtos::condition_variable* cond)
      {
        if (cond->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (cond),
                                         cond->name ());
          }
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (cond)));
      }

      void
      create_return (os::rtos::condition_variable* cond)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_CREATE);
      }

      void
      destroy (os::rtos::condition_variable* cond)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (cond)));
      }

      void
      destroy_return (os::rtos::condition_variable* cond)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_DESTROY);
      }

      void
      signal (os::rtos::condition_variable* cond)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_SIGNAL,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (cond)));
      }

      void
      signal_retval (os::rtos::condition_variable* cond,
                     os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_SIGNAL,
            static_cast<U32> (res));
      }

      void
      broadcast (os::rtos::condition_variable* cond)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_BROADCAST,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (cond)));
      }

      void
      broadcast_retval (os::rtos::condition_variable* cond,
                        os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_BROADCAST,
            static_cast<U32> (res));
      }

      void
      wait (os::rtos::condition_variable* cond)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (cond)));
      }

      void
      wait_retval (os::rtos::condition_variable* cond, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_WAIT,
            static_cast<U32> (res));
      }

      void
      timed_wait (os::rtos::condition_variable* cond, unsigned int timeout)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_TIMED_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (cond)),
            static_cast<U32> (timeout));
      }

      void
      timed_wait_retval (os::rtos::condition_variable* cond,
                         os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_TIMED_WAIT,
            static_cast<U32> (res));
      }
    } // namespace condition_variable

    namespace timer
    {
      void
      create (os::rtos::timer* timer, unsigned int type)
      {
        if (timer->has_name ())
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (timer),
                                         timer->name ());
          }
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (timer)),
            static_cast<U32> (type));
      }

      void
      create_return (os::rtos::timer* timer)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_CREATE);
      }

      void
      destroy (os::rtos::timer* timer)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (timer)));
      }

      void
      destroy_return (os::rtos::timer* timer)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_DESTROY);
      }

      void
      start (os::rtos::timer* timer, unsigned int period)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_START,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (timer)),
            static_cast<U32> (period));
      }

      void
      start_retval (os::rtos::timer* timer, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_START, static_cast<U32> (res));
      }

      void
      stop (os::rtos::timer* timer)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_STOP,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (timer)));
      }

      void
      stop_retval (os::rtos::timer* timer, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_STOP, static_cast<U32> (res));
      }

      void
      callback (os::rtos::timer* timer)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_CALLBACK,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (timer)));
      }

      void
      callback_return (os::rtos::timer* timer)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_TIMER_CALLBACK);
      }
    } // namespace timer

    namespace posix
    {
      void
      vopen (const char* path, int oflags)
      {
        SEGGER_SYSVIEW_RecordString (OS_INTEGER_INSTRUMENTATION_ID_POSIX_VOPEN,
                                     path);
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_POSIX_VOPEN_OFLAGS,
            static_cast<U32> (oflags));
      }

      void
      vopen_retval (void* io, const char* path)
      {
        if (io != nullptr && path != nullptr)
          {
            SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (io), path);
          }

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_POSIX_VOPEN);
      }

      namespace io
      {
        void
        create (void* io, unsigned int t)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_CREATE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)),
              static_cast<U32> (t));
        }

        void
        destroy (void* io)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_DESTROY,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)));
        }

        void
        close (void* io)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_CLOSE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)));
        }

        void
        close_retval (void* io, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_CLOSE,
              static_cast<U32> (res));
        }

        void
        read (void* io, const void* buf, std::size_t nbyte)
        {
          SEGGER_SYSVIEW_RecordU32x3 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_READ,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)),
              reinterpret_cast<U32> (buf), static_cast<U32> (nbyte));
        }

        void
        read_retval (void* io, ssize_t res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_READ,
              static_cast<U32> (res));
        }

        void
        write (void* io, const void* buf, std::size_t nbyte)
        {
          SEGGER_SYSVIEW_RecordU32x3 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_WRITE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)),
              reinterpret_cast<U32> (buf), static_cast<U32> (nbyte));
        }

        void
        write_retval (void* io, ssize_t res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_WRITE,
              static_cast<U32> (res));
        }

        void
        writev (void* io, const void* iov, int iovcnt)
        {
          SEGGER_SYSVIEW_RecordU32x3 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_WRITEV,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)),
              reinterpret_cast<U32> (iov), static_cast<U32> (iovcnt));
        }

        void
        writev_retval (void* io, ssize_t res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_WRITEV,
              static_cast<U32> (res));
        }

        void
        vfcntl (void* io, int cmd)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_VFCNTL,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)),
              static_cast<U32> (cmd));
        }

        void
        vfcntl_retval (void* io, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_VFCNTL,
              static_cast<U32> (res));
        }

        void
        isatty (void* io)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_ISATTY,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)));
        }

        void
        isatty_retval (void* io, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_ISATTY,
              static_cast<U32> (res));
        }

        void
        fstat (void* io, void* buf)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_FSTAT,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)),
              reinterpret_cast<U32> (buf));
        }

        void
        fstat_retval (void* io, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_FSTAT,
              static_cast<U32> (res));
        }

        void
        lseek (void* io, off_t offset, int whence)
        {
          SEGGER_SYSVIEW_RecordU32x3 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_LSEEK,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (io)),
              static_cast<U32> (offset), static_cast<U32> (whence));
        }

        void
        lseek_retval (void* io, off_t offset)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_LSEEK,
              static_cast<U32> (offset));
        }
      } // namespace io

      namespace device
      {
        void
        create (void* device, unsigned int t)
        {
          os::posix::device* dev
              = reinterpret_cast<os::posix::device*> (device);

          SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (device),
                                       dev->name ());

          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_CREATE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (device)),
              static_cast<U32> (t));
        }

        void
        destroy (void* device)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_DESTROY,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (device)));
        }

        void
        vopen (void* device, const char* path, int oflags)
        {
          if (path != nullptr && path[0] != '\0')
            {
              SEGGER_SYSVIEW_RecordString (
                  OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VOPEN, path);
            }

          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VOPEN_OFLAGS,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (device)),
              static_cast<U32> (oflags));
        }

        void
        vopen_retval (void* device, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VOPEN,
              static_cast<U32> (res));
        }

        void
        close (void* device)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_CLOSE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (device)));
        }

        void
        close_retval (void* device, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_CLOSE,
              static_cast<U32> (res));
        }

        void
        vioctl (void* device, int request)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VIOCTL,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (device)),
              static_cast<U32> (request));
        }

        void
        vioctl_retval (void* device, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VIOCTL,
              static_cast<U32> (res));
        }

        void
        sync (void* device)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_SYNC,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (device)));
        }

        void
        sync_return (void* device)
        {
          SEGGER_SYSVIEW_RecordEndCall (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_SYNC);
        }
      } // namespace device

      namespace block_device
      {
        void
        read_block (void* bdev, void* buf, std::size_t block,
                    std::size_t nblocks)
        {
          SEGGER_SYSVIEW_RecordU32x4 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_READ_BLOCK,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (bdev)),
              reinterpret_cast<U32> (buf), static_cast<U32> (block),
              static_cast<U32> (nblocks));
        }

        void
        read_block_retval (void* bdev, ssize_t res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_READ_BLOCK,
              static_cast<U32> (res));
        }

        void
        write_block (void* bdev, const void* buf, std::size_t block,
                     std::size_t nblocks)
        {
          SEGGER_SYSVIEW_RecordU32x4 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_WRITE_BLOCK,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (bdev)),
              reinterpret_cast<U32> (buf), static_cast<U32> (block),
              static_cast<U32> (nblocks));
        }

        void
        write_block_retval (void* bdev, ssize_t res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_WRITE_BLOCK,
              static_cast<U32> (res));
        }

        void
        vioctl (void* bdev, int request)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_VIOCTL_BLOCK,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (bdev)),
              static_cast<U32> (request));
        }

        void
        vioctl_retval (void* bdev, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_VIOCTL_BLOCK,
              static_cast<U32> (res));
        }
      } // namespace block_device

      namespace block_device_partition
      {
        void
        configure (void* partition, std::size_t offset, std::size_t nblocks)
        {
          SEGGER_SYSVIEW_RecordU32x3 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_PARTITION_CONFIGURE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (partition)),
              static_cast<U32> (offset), static_cast<U32> (nblocks));
        }

        void
        configure_return (void* partition)
        {
          SEGGER_SYSVIEW_RecordEndCall (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_PARTITION_CONFIGURE);
        }
      } // namespace block_device_partition

      namespace file
      {
        void
        create (void* file, unsigned int t)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_CREATE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (file)),
              static_cast<U32> (t));
        }

        void
        destroy (void* file)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_DESTROY,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (file)));
        }

        void
        close (void* file)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_CLOSE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (file)));
        }

        void
        close_retval (void* file, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_CLOSE,
              static_cast<U32> (res));
        }

        void
        ftruncate (void* file, off_t length)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FTRUNCATE,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (file)),
              static_cast<U32> (length));
        }

        void
        ftruncate_retval (void* file, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FTRUNCATE,
              static_cast<U32> (res));
        }

        void
        fsync (void* file)
        {
          SEGGER_SYSVIEW_RecordU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FSYNC,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (file)));
        }

        void
        fsync_retval (void* file, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FSYNC,
              static_cast<U32> (res));
        }

        void
        fstatvfs (void* file, void* buf)
        {
          SEGGER_SYSVIEW_RecordU32x2 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FSTATVFS,
              SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (file)),
              reinterpret_cast<U32> (buf));
        }

        void
        fstatvfs_retval (void* file, int res)
        {
          SEGGER_SYSVIEW_RecordEndCallU32 (
              OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FSTATVFS,
              static_cast<U32> (res));
        }
      } // namespace file

    } // namespace posix

    // ------------------------------------------------------------------------

    void
    exit (int exit_code)
    {
      SEGGER_SYSVIEW_RecordU32 (OS_INTEGER_INSTRUMENTATION_ID_EXIT,
                                static_cast<U32> (exit_code));
    }

  } // namespace instrumentation
} // namespace os

// ----------------------------------------------------------------------------

void
os_instrumentation_exit (int exit_code)
{
  SEGGER_SYSVIEW_RecordU32 (OS_INTEGER_INSTRUMENTATION_ID_EXIT,
                            static_cast<U32> (exit_code));
}

#endif // defined(OS_INCLUDE_INSTRUMENTATION)

// ----------------------------------------------------------------------------
