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
    namespace thread
    {
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
    } // namespace thread

    namespace thread
    {

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
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (msgs), static_cast<U32> (msg_size_bytes));
      }

      void
      create_return (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_CREATE);
      }

      void
      destroy (os::rtos::message_queue* mqueue)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)));
      }

      void
      destroy_return (os::rtos::message_queue* mqueue)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_DESTROY_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_DESTROY);
      }

      void
      send (os::rtos::message_queue* mqueue, std::size_t nbytes,
            unsigned int mprio)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_SEND,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mprio));
      }

      void
      send_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_SEND_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_SEND, static_cast<U32> (res));
      }

      void
      try_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                unsigned int mprio)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_SEND,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (mprio));
      }

      void
      try_send_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_SEND_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_SEND,
            static_cast<U32> (res));
      }

      void
      timed_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                  unsigned int timeout, unsigned int mprio)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_SEND,
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
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_SEND_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_SEND,
            static_cast<U32> (res));
      }

      void
      receive (os::rtos::message_queue* mqueue, std::size_t nbytes)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_RECEIVE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes));
      }

      void
      receive_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_RECEIVE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_RECEIVE,
            static_cast<U32> (res));
      }

      void
      try_receive (os::rtos::message_queue* mqueue, std::size_t nbytes)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_RECEIVE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes));
      }

      void
      try_receive_retval (os::rtos::message_queue* mqueue,
                          os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_RECEIVE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TRY_RECEIVE,
            static_cast<U32> (res));
      }

      void
      timed_receive (os::rtos::message_queue* mqueue, std::size_t nbytes,
                     unsigned int timeout)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_RECEIVE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (nbytes), static_cast<U32> (timeout));
      }

      void
      timed_receive_retval (os::rtos::message_queue* mqueue,
                            os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_RECEIVE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mqueue)),
            static_cast<U32> (mqueue->length ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MQUEUE_TIMED_RECEIVE,
            static_cast<U32> (res));
      }

    } // namespace message_queue

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
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (blocks), static_cast<U32> (block_size_bytes));
      }

      void
      create_return (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_CREATE);
      }

      void
      destroy (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      destroy_return (os::rtos::memory_pool* mpool)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_DESTROY_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_DESTROY);
      }

      void
      alloc (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_ALLOC,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_ALLOC_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_ALLOC,
            reinterpret_cast<U32> (addr));
      }

      void
      try_alloc (os::rtos::memory_pool* mpool)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TRY_ALLOC,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)));
      }

      void
      try_alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TRY_ALLOC_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TRY_ALLOC,
            reinterpret_cast<U32> (addr));
      }

      void
      timed_alloc (os::rtos::memory_pool* mpool, unsigned int timeout)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TIMED_ALLOC,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (timeout));
      }

      void
      timed_alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TIMED_ALLOC_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_TIMED_ALLOC,
            reinterpret_cast<U32> (addr));
      }

      void
      free (os::rtos::memory_pool* mpool, void* block)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_FREE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            reinterpret_cast<U32> (block));
      }

      void
      free_retval (os::rtos::memory_pool* mpool, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_FREE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mpool)),
            static_cast<U32> (mpool->count ()));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_MEMPOOL_FREE,
            static_cast<U32> (res));
      }
    } // namespace memory_pool

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
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_CREATE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)));
      }

      void
      create_return (os::rtos::event_flags* evflags)
      {
        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_CREATE);
      }

      void
      destroy (os::rtos::event_flags* evflags)
      {
        SEGGER_SYSVIEW_RecordU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_DESTROY,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)));
      }

      void
      destroy_return (os::rtos::event_flags* evflags)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_DESTROY_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCall (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_DESTROY);
      }

      void
      wait (os::rtos::event_flags* evflags, unsigned int mask,
            unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (mode));
      }

      void
      wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_WAIT,
            static_cast<U32> (res));
      }

      void
      try_wait (os::rtos::event_flags* evflags, unsigned int mask,
                unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x3 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TRY_WAIT,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask), static_cast<U32> (mode));
      }

      void
      try_wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TRY_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TRY_WAIT,
            static_cast<U32> (res));
      }

      void
      timed_wait (os::rtos::event_flags* evflags, unsigned int mask,
                  unsigned int timeout, unsigned int mode)
      {
        SEGGER_SYSVIEW_RecordU32x4 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TIMED_WAIT,
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
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TIMED_WAIT_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_TIMED_WAIT,
            static_cast<U32> (res));
      }

      void
      raise (os::rtos::event_flags* evflags, unsigned int mask)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_RAISE,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            static_cast<U32> (mask));
      }

      void
      raise_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
#if defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_RAISE_VALUES,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (evflags)),
            evflags->get (0, 0));
#endif // defined(OS_INCLUDE_INSTRUMENTATION_EXTRA_VALUES)

        SEGGER_SYSVIEW_RecordEndCallU32 (
            OS_INTEGER_INSTRUMENTATION_ID_EVFLAGS_RAISE,
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
