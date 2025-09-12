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

// ----------------------------------------------------------------------------

namespace os
{
  namespace instrumentation
  {
    namespace memory_resource
    {
      void
      define (os::rtos::memory::memory_resource* heap, void* base,
              std::size_t heap_size, std::size_t metadata_size)
      {
        SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (heap),
                                     heap->name ());
        SEGGER_SYSVIEW_HeapDefine (heap, base, heap_size, metadata_size);
      }
    } // namespace memory_resource

    namespace mutex
    {
      void
      created (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mutex),
                                     mutex->name ());
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->type ()),
            static_cast<U32> (mutex->protocol ()),
            static_cast<U32> (mutex->robustness ()));
      }

      void
      destroyed (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_DESTROYED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)));
      }

      void
      locked (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()),
            static_cast<U32> (result));
      }

      void
      try_locked (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()),
            static_cast<U32> (result));
      }

      void
      timed_locked (os::rtos::mutex* mutex, unsigned int timeout,
                    os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (timeout),
            static_cast<U32> (mutex->prio_ceiling ()),
            static_cast<U32> (result));
      }

      void
      unlocked (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (mutex->prio_ceiling ()),
            static_cast<U32> (result));
      }

    } // namespace mutex

    namespace semaphore
    {
      void
      created (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (semaphore),
                                     semaphore->name ());
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_CREATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->max_value ()),
            static_cast<U32> (semaphore->value ()));
      }

      void
      destroyed (os::rtos::semaphore* semaphore)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROYED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()));
      }

      void
      posted (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POSTED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()), static_cast<U32> (res));
      }

      void
      waiting (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAITING,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()), static_cast<U32> (res));
      }

      void
      try_waiting (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAITING,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (semaphore->value ()), static_cast<U32> (res));
      }

      void
      timed_waiting (os::rtos::semaphore* semaphore, unsigned int timeout,
                     os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAITING,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (semaphore)),
            static_cast<U32> (timeout), static_cast<U32> (semaphore->value ()),
            static_cast<U32> (res));
      }
    } // namespace semaphore

    namespace message_queue
    {
      void
      created (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mqueue),
                                     mqueue->name ());
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_CREATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->capacity ()),
            static_cast<U32> (mqueue->msg_size ()));
      }

      void
      destroyed (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_DESTROYED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
      }

      void
      sent (os::rtos::message_queue* mqueue, std::size_t nbytes,
            unsigned int mprio, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x5 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_SENT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mprio),
            static_cast<U32> (mqueue->length ()), static_cast<U32> (res));
      }

      void
      try_sent (os::rtos::message_queue* mqueue, std::size_t nbytes,
                unsigned int mprio, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x5 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_SENT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mprio),
            static_cast<U32> (mqueue->length ()), static_cast<U32> (res));
      }

      void
      timed_sent (os::rtos::message_queue* mqueue, std::size_t nbytes,
                  unsigned int mprio, unsigned int timeout,
                  os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x6 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_SENT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mprio),
            static_cast<U32> (timeout), static_cast<U32> (mqueue->length ()),
            static_cast<U32> (res));
      }

      void
      received (os::rtos::message_queue* mqueue, std::size_t nbytes,
                os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_RECEIVED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mqueue->length ()),
            static_cast<U32> (res));
      }

      void
      try_received (os::rtos::message_queue* mqueue, std::size_t nbytes,
                    os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_RECEIVED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mqueue->length ()),
            static_cast<U32> (res));
      }

      void
      timed_received (os::rtos::message_queue* mqueue, std::size_t nbytes,
                      unsigned int timeout, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x5 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_RECEIVED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (timeout),
            static_cast<U32> (mqueue->length ()), static_cast<U32> (res));
      }
    } // namespace message_queue

    namespace memory_pool
    {
      void
      created (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mpool),
                                     mpool->name ());
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_CREATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->capacity ()),
            static_cast<U32> (mpool->block_size ()));
      }

      void
      destroyed (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_DESTROYED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
      }

      void
      allocated (os::rtos::memory_pool* mpool, void* addr)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_ALLOCATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()), reinterpret_cast<U32> (addr));
      }

      void
      try_allocated (os::rtos::memory_pool* mpool, void* addr)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TRY_ALLOCATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()), reinterpret_cast<U32> (addr));
      }

      void
      timed_allocated (os::rtos::memory_pool* mpool, unsigned int timeout,
                       void* addr)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TIMED_ALLOCATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (timeout), static_cast<U32> (mpool->count ()),
            reinterpret_cast<U32> (addr));
      }

      void
      deallocated (os::rtos::memory_pool* mpool, void* block,
                   os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_DEALLOCATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            reinterpret_cast<U32> (block), static_cast<U32> (mpool->count ()),
            static_cast<U32> (res));
      }
    } // namespace memory_pool

    namespace event_flags
    {
      void
      created (os::rtos::event_flags* evflags)
      {
        SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (evflags),
                                     evflags->name ());
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_CREATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)));
      }

      void
      destroyed (os::rtos::event_flags* evflags)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_DESTROYED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
      }

      void
      waiting (os::rtos::event_flags* evflags, unsigned int mask,
               unsigned int mode, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x5 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_WAITING,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (mode),
            evflags->get (0, 0), static_cast<U32> (res));
      }

      void
      try_waiting (os::rtos::event_flags* evflags, unsigned int mask,
                   unsigned int mode, os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x5 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TRY_WAITING,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (mode),
            evflags->get (0, 0), static_cast<U32> (res));
      }

      void
      timed_waiting (os::rtos::event_flags* evflags, unsigned int mask,
                     unsigned int mode, unsigned int timeout,
                     os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x6 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TIMED_WAITING,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (mode),
            static_cast<U32> (timeout), evflags->get (0, 0),
            static_cast<U32> (res));
      }

      void
      raised (os::rtos::event_flags* evflags, unsigned int mask,
              os::rtos::result_t res)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_RAISED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), evflags->get (0, 0),
            static_cast<U32> (res));
      }
    } // namespace event_flags

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
