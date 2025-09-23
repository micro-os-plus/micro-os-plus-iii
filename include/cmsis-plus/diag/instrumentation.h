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

#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_SWITCH (1u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_CLOCK (2u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_THREAD_FLAGS (3u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_SEMAPHORE (4u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MUTEX (5u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MESSAGE_QUEUE_SEND (6u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MESSAGE_QUEUE_RECEIVE (7u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MEMORY_POOL (8u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_EVENT_FLAGS (9u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_JOIN (10u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_USER (11u)

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_INSTRUMENTATION)

#include "instrumentation-sysview-micro-os-plus-iii.h"

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

    namespace scheduler
    {
      static void inline __attribute__ ((__always_inline__))
      lock (bool state)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      unlock (bool state)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      locked_set (bool state)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reschedule (void)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      preemptive_set (bool state)
      {
      }
    } // namespace scheduler

    namespace thread
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::thread* thread)
      {
      }

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

      static void inline __attribute__ ((__always_inline__))
      yield (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      suspend (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      resume (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      resume_return (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      priority_set (os::rtos::thread* thread, unsigned int prio)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      priority_set_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      priority_inherited_set (os::rtos::thread* thread, unsigned int prio)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      priority_inherited_set_retval (os::rtos::thread* thread,
                                     os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      detach (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      detach_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      join (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      join_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      cancel (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      cancel_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      interrupt (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      interrupt_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      kill (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      kill_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      internal_exit (os::rtos::thread* thread, void* exit_ptr)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      internal_exit_return (os::rtos::thread* thread)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_wait (os::rtos::thread* thread, unsigned int mask,
                  unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_wait_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_try_wait (os::rtos::thread* thread, unsigned int mask,
                      unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_try_wait_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_timed_wait (os::rtos::thread* thread, unsigned int mask,
                        unsigned int timeout, unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_timed_wait_retval (os::rtos::thread* thread,
                               os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_raise (os::rtos::thread* thread, unsigned int mask)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_raise_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_clear (os::rtos::thread* thread, unsigned int mask)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      flags_clear_retval (os::rtos::thread* thread, os::rtos::result_t res)
      {
      }
    } // namespace thread

    namespace memory_resource
    {
      static void inline __attribute__ ((__always_inline__))
      define (os::rtos::memory::memory_resource* heap, void* base,
              std::size_t heap_size, std::size_t metadata_size)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      allocated (os::rtos::memory::memory_resource* heap, void* user_data,
                 std::size_t size)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      deallocated (os::rtos::memory::memory_resource* heap, void* user_data)
      {
      }
    } // namespace memory_resource

    namespace mutex
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      lock (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      lock_retval (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_lock (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_lock_retval (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_lock (os::rtos::mutex* mutex, unsigned int timeout)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_lock_retval (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      unlock (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      unlock_retval (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset (os::rtos::mutex* mutex)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset_retval (os::rtos::mutex* mutex, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      prio_ceiling_set (os::rtos::mutex* mutex, unsigned int prio)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      prio_ceiling_set_retval (os::rtos::mutex* mutex,
                               os::rtos::result_t result)
      {
      }
    } // namespace mutex

    namespace semaphore
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      post (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      post_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_wait (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_wait_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_wait (os::rtos::semaphore* semaphore, unsigned int timeout)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_wait_retval (os::rtos::semaphore* semaphore,
                         os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset (os::rtos::semaphore* semaphore)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res)
      {
      }
    } // namespace semaphore

    namespace message_queue
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::message_queue* mqueue)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::message_queue* mqueue)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::message_queue* mqueue)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      send (os::rtos::message_queue* mqueue, std::size_t nbytes,
            unsigned int mprio)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      send_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                unsigned int mprio)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_send_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                  unsigned int timeout, unsigned int mprio)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_send_retval (os::rtos::message_queue* mqueue,
                         os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      receive (os::rtos::message_queue* mqueue, std::size_t nbytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      receive_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_receive (os::rtos::message_queue* mqueue, std::size_t nbytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_receive_retval (os::rtos::message_queue* mqueue,
                          os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_receive (os::rtos::message_queue* mqueue, std::size_t nbytes,
                     unsigned int timeout)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_receive_retval (os::rtos::message_queue* mqueue,
                            os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset (os::rtos::message_queue* mqueue)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res)
      {
      }
    } // namespace message_queue

    namespace message_queue_allocated
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::message_queue* mqueue)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::message_queue* mqueue)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::message_queue* mqueue)
      {
      }
    } // namespace message_queue_allocated

    namespace message_queue_inclusive
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::message_queue* mqueue)
      {
      }
    } // namespace message_queue_inclusive

    namespace memory_pool
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      alloc (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_alloc (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_alloc (os::rtos::memory_pool* mpool, unsigned int timeout)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_alloc_retval (os::rtos::memory_pool* mpool, void* addr)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      free (os::rtos::memory_pool* mpool, void* block)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      free_retval (os::rtos::memory_pool* mpool, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      reset_retval (os::rtos::memory_pool* mpool, os::rtos::result_t res)
      {
      }
    } // namespace memory_pool

    namespace memory_pool_allocated
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::memory_pool* mpool)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::memory_pool* mpool)
      {
      }
    } // namespace memory_pool_allocated

    namespace memory_pool_inclusive
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::memory_pool* mpool)
      {
      }
    } // namespace memory_pool_inclusive

    namespace event_flags
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::event_flags* evflags)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::event_flags* evflags)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::event_flags* evflags)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::event_flags* evflags)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait (os::rtos::event_flags* evflags, unsigned int mask,
            unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_wait (os::rtos::event_flags* evflags, unsigned int mask,
                unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      try_wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_wait (os::rtos::event_flags* evflags, unsigned int mask,
                  unsigned int timeout, unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_wait_retval (os::rtos::event_flags* evflags,
                         os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      raise (os::rtos::event_flags* evflags, unsigned int mask)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      raise_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      clear (os::rtos::event_flags* evflags, unsigned int mask)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      clear_retval (os::rtos::event_flags* evflags, os::rtos::result_t res)
      {
      }
    } // namespace event_flags

    namespace clock
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::clock* clock)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::clock* clock)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      sleep_for (os::rtos::clock* clock, unsigned int duration)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      sleep_for_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      sleep_until (os::rtos::clock* clock, unsigned long long timestamp)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      sleep_until_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait_for (os::rtos::clock* clock, unsigned int duration)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait_for_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
      }
    } // namespace clock

    namespace adjustable_clock
    {
      static void inline __attribute__ ((__always_inline__))
      sleep_until (os::rtos::clock* clock, unsigned long long timestamp)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      sleep_until_retval (os::rtos::clock* clock, os::rtos::result_t res)
      {
      }
    } // namespace adjustable_clock

    namespace condition_variable
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::condition_variable* cond)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::condition_variable* cond)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::condition_variable* cond)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::condition_variable* cond)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      signal (os::rtos::condition_variable* cond)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      signal_retval (os::rtos::condition_variable* cond,
                     os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      broadcast (os::rtos::condition_variable* cond)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      broadcast_retval (os::rtos::condition_variable* cond,
                        os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait (os::rtos::condition_variable* cond)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      wait_retval (os::rtos::condition_variable* cond, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_wait (os::rtos::condition_variable* cond, unsigned int timeout)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      timed_wait_retval (os::rtos::condition_variable* cond,
                         os::rtos::result_t res)
      {
      }
    } // namespace condition_variable

    namespace timer
    {
      static void inline __attribute__ ((__always_inline__))
      create (os::rtos::timer* timer, unsigned int type)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      create_return (os::rtos::timer* timer)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy (os::rtos::timer* timer)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      destroy_return (os::rtos::timer* timer)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      start (os::rtos::timer* timer, unsigned int period)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      start_retval (os::rtos::timer* timer, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      stop (os::rtos::timer* timer)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      stop_retval (os::rtos::timer* timer, os::rtos::result_t res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      callback (os::rtos::timer* timer)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      callback_return (os::rtos::timer* timer)
      {
      }
    } // namespace timer

    namespace posix
    {
      static void inline __attribute__ ((__always_inline__))
      vopen (const char* path, int oflags)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      vopen_retval (void* io, const char* path = nullptr)
      {
      }

      namespace io
      {
        static void inline __attribute__ ((__always_inline__))
        create (void* io, unsigned int t)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        destroy (void* io)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        close (void* io)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        close_retval (void* io, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        read (void* io, const void* buf, std::size_t nbyte)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        read_retval (void* io, ssize_t res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        write (void* io, const void* buf, std::size_t nbyte)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        write_retval (void* io, ssize_t res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        writev (void* io, const void* iov, int iovcnt)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        writev_retval (void* io, ssize_t res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vfcntl (void* io, int cmd)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vfcntl_retval (void* io, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        isatty (void* io)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        isatty_retval (void* io, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        fstat (void* io, void* buf)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        fstat_retval (void* io, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        lseek (void* io, off_t offset, int whence)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        lseek_retval (void* io, off_t offset)
        {
        }
      } // namespace io

      namespace device
      {
        static void inline __attribute__ ((__always_inline__))
        create (void* device, unsigned int t)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        destroy (void* device)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vopen (void* device, const char* path, int oflags)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vopen_retval (void* device, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        close (void* device)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        close_retval (void* device, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vioctl (void* device, int request)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vioctl_retval (void* device, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        sync (void* device)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        sync_return (void* device)
        {
        }
      } // namespace device

      namespace block_device
      {
        static void inline __attribute__ ((__always_inline__))
        read_block (void* bdev, void* buf, std::size_t block,
                    std::size_t nblocks)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        read_block_retval (void* bdev, ssize_t res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        write_block (void* bdev, const void* buf, std::size_t block,
                     std::size_t nblocks)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        write_block_retval (void* bdev, ssize_t res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vioctl (void* bdev, int request)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vioctl_retval (void* bdev, int res)
        {
        }

      } // namespace block_device

      namespace block_device_partition
      {
        static void inline __attribute__ ((__always_inline__))
        configure (void* partition, std::size_t offset, std::size_t nblocks)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        configure_return (void* partition)
        {
        }
      } // namespace block_device_partition

      namespace file
      {
        static void inline __attribute__ ((__always_inline__))
        create (void* file, unsigned int t)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        destroy (void* file)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        close (void* file)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        close_retval (void* file, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        ftruncate (void* file, off_t length)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        ftruncate_retval (void* file, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        fsync (void* file)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        fsync_retval (void* file, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        fstatvfs (void* file, void* buf)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        fstatvfs_retval (void* file, int res)
        {
        }
      } // namespace file

      static void inline __attribute__ ((__always_inline__))
      mkdir (const char* path, unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      mkdir_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      rmdir (const char* path)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      rmdir_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      sync (void)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      sync_return (void)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      chmod (const char* path, unsigned int mode)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      chmod_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      stat (const char* path, void* buf)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      stat_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      truncate (const char* path, int length)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      truncate_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      rename (const char* existing, const char* _new)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      rename_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      unlink (const char* path)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      unlink_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      utime (const char* path, const void* times)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      utime_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      statvfs (const char* path, void* buf)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      statvfs_retval (int res)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      opendir (const char* dirpath)
      {
      }

      static void inline __attribute__ ((__always_inline__))
      opendir_retval (void* dirp)
      {
      }

      namespace file_system
      {
        static void inline __attribute__ ((__always_inline__))
        create (void* fs)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        destroy (void* fs)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vmkfs (void* fs, int args)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vmkfs_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vmount (void* fs, const char* path, unsigned int flags)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vmount_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        umount (void* fs, unsigned int flags)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        umount_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vopen (void* fs, const char* path, int oflags)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        vopen_retval (void* fs, void* ptr)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        opendir (void* fs, const char* path)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        opendir_retval (void* fs, void* dirp)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        mkdir (void* fs, const char* path, unsigned int mode)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        mkdir_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        rmdir (void* fs, const char* path)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        rmdir_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        sync (void* fs)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        sync_return (void* fs)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        chmod (void* fs, const char* path, unsigned int mode)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        chmod_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        stat (void* fs, const char* path, void* buf)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        stat_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        truncate (void* fs, const char* path, int length)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        truncate_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        rename (void* fs, const char* existing, const char* _new)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        rename_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        unlink (void* fs, const char* path)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        unlink_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        utime (void* fs, const char* path, const void* times)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        utime_retval (void* fs, int res)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        statvfs (void* fs, void* buf)
        {
        }

        static void inline __attribute__ ((__always_inline__))
        statvfs_retval (void* fs, int res)
        {
        }

      } // namespace file_system

    } // namespace posix

    // ------------------------------------------------------------------------

    static void inline __attribute__ ((__always_inline__))
    exit (int exit_code)
    {
    }

  } // namespace instrumentation
} // namespace os

#endif // defined(__cplusplus)

inline void __attribute__ ((__always_inline__))
os_instrumentation_start (void)
{
}

inline void __attribute__ ((__always_inline__))
os_instrumentation_stop (void)
{
}

inline void __attribute__ ((__always_inline__))
os_instrumentation_exit (int exit_code)
{
}

inline void __attribute__ ((__always_inline__))
os_instrumentation_interrupt_entered (void)
{
}

inline void __attribute__ ((__always_inline__))
os_instrumentation_interrupt_exited (void)
{
}

#endif // defined(OS_INCLUDE_INSTRUMENTATION)

// ----------------------------------------------------------------------------

#endif // CMSIS_PLUS_DIAG_INSTRUMENTATION_H_
