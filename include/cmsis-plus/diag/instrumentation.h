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
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MESSAGE_QUEUE_SEND (6u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MESSAGE_QUEUE_RECEIVE (7u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_MEMORY_POOL (8u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_EVENT_FLAGS (9u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_JOIN (10u)
#define OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_USER (11u)

// ----------------------------------------------------------------------------

// Base for all custom instrumentation IDs.
#define OS_INTEGER_INSTRUMENTATION_ID_BASE (32u)

#define OS_INTEGER_INSTRUMENTATION_ID_EXIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 1u)

// 34
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 2u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 202u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 3u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 4u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 204u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 5u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 205u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 6u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 206u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 7u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 207u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 79u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 279u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_PRIO_CEILING_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 82u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_PRIO_CEILING_SET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 282u)

// 40
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 8u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 208u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 9u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 209u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POST \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 10u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POST_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 210u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 11u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 211u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 12u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 212u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 13u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 213u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 81u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 281u)

// 46
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 14u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 214u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 15u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 215u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_SEND \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 16u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_SEND_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 216u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_SEND \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 17u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_SEND_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 217u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_SEND \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 18u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_SEND_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 218u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RECEIVE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 19u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RECEIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 219u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_RECEIVE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 20u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_RECEIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 220u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_RECEIVE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 21u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_RECEIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 221u)

// 54
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 22u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 222u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 23u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 223u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 24u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOC_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 224u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TRY_ALLOC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 25u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TRY_ALLOC_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 225u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TIMED_ALLOC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 26u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TIMED_ALLOC_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 226u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_FREE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 27u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_FREE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 227u)

// 60
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 28u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 228u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 29u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 229u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 30u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 230u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TRY_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 31u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TRY_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 231u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 32u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TIMED_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 232u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_RAISE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 33u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_RAISE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 233u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CLEAR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 58u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CLEAR_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 58u)

// 66
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 34u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 234u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TRY_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 35u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TRY_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 235u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 36u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TIMED_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 236u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_RAISE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 37u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_RAISE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 237u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_CLEAR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 59u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_CLEAR_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 259u)

// 70
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 38u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 39u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_FOR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 40u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_UNTIL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 41u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_WAIT_FOR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 42u)

#define OS_INTEGER_INSTRUMENTATION_ID_ADJUSTABLE_CLOCK_SLEEP_UNTIL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 43u)

// 76
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 44u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 244u)

// 77
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 45u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 245u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 46u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 246u)

// 79
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_INCLUSIVE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 47u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_INCLUSIVE_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 247u)

// 80
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 48u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 248u)

// 81
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 49u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE_ALLOCATED_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 249u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 50u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 250u)

// 83
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_INCLUSIVE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 51u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE_INCLUSIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 251u)

// 84
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 52u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 53u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_SIGNAL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 54u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_BROADCAST \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 55u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 56u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 57u)

// 92
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 60u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 61u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_START \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 62u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_STOP \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 63u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_CALLBACK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 64u)

// 97
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 65u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 66u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_RESUME \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 67u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 68u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_INHERITED_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 69u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_DETACH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 70u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_JOIN \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 71u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_CANCEL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 72u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_INTERRUPT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 73u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_KILL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 74u)

// 107
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 75u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_UNLOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 76u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_LOCKED_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 77u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_RESCHEDULE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 78u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_PREEMPTIVE_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 80u)

// ----------------------------------------------------------------------------

#if defined(OS_INCLUDE_INSTRUMENTATION)

// Invoke SEGGER SystemView functions.

#if defined(__cplusplus)

namespace os
{
  namespace instrumentation
  {
    void
    configure (void);

    void
    start (void);

    void
    stop (void);

    namespace interrupt
    {
      void
      entered (void);

      void
      exited (void);
    } // namespace interrupt

    namespace scheduler
    {
      void
      lock (bool state);

      void
      unlock (bool state);

      void
      locked_set (bool state);

      void
      reschedule (void);

      void
      preemptive_set (bool state);
    } // namespace scheduler

    namespace thread
    {
      void
      create (os::rtos::thread* thread);

      void
      create_return (os::rtos::thread* thread);

      void
      destroy (os::rtos::thread* thread);

      void
      destroy_return (os::rtos::thread* thread);

      // ----------------------------------------------------------------------
      // Specific API calls.

      void
      created (os::rtos::thread* thread);

      void
      ready (os::rtos::thread* thread);

      void
      suspended (os::rtos::thread* thread, unsigned int cause = 0);

      void
      active (os::rtos::thread* thread);

      void
      idle (void);

      void
      terminated (os::rtos::thread* thread);

      // ----------------------------------------------------------------------

      void
      resume (os::rtos::thread* thread);

      void
      resume_return (os::rtos::thread* thread);

      void
      priority_set (os::rtos::thread* thread, unsigned int prio);

      void
      priority_set_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      priority_inherited_set (os::rtos::thread* thread, unsigned int prio);

      void
      priority_inherited_set_retval (os::rtos::thread* thread,
                                     os::rtos::result_t res);

      void
      detach (os::rtos::thread* thread);

      void
      detach_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      join (os::rtos::thread* thread);

      void
      join_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      cancel (os::rtos::thread* thread);

      void
      cancel_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      interrupt (os::rtos::thread* thread);

      void
      interrupt_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      kill (os::rtos::thread* thread);

      void
      kill_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      internal_exit (os::rtos::thread* thread, void* exit_ptr);

      void
      internal_exit_return (os::rtos::thread* thread);

      void
      flags_raise (os::rtos::thread* thread, unsigned int mask);

      void
      flags_raise_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      flags_wait (os::rtos::thread* thread, unsigned int mask,
                  unsigned int mode);

      void
      flags_wait_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      flags_try_wait (os::rtos::thread* thread, unsigned int mask,
                      unsigned int mode);

      void
      flags_try_wait_retval (os::rtos::thread* thread, os::rtos::result_t res);

      void
      flags_timed_wait (os::rtos::thread* thread, unsigned int mask,
                        unsigned int mode, unsigned int timeout);

      void
      flags_timed_wait_retval (os::rtos::thread* thread,
                               os::rtos::result_t res);

      void
      flags_clear (os::rtos::thread* thread, unsigned int mask);

      void
      flags_clear_retval (os::rtos::thread* thread, os::rtos::result_t res);
    } // namespace thread

    namespace memory_resource
    {
      void
      define (os::rtos::memory::memory_resource* heap, void* base,
              std::size_t heap_size, std::size_t metadata_size);

      static void inline __attribute__ ((__always_inline__))
      allocated (os::rtos::memory::memory_resource* heap, void* user_data,
                 std::size_t size)
      {
        SEGGER_SYSVIEW_HeapAlloc (heap, user_data, size);
      }

      static void inline __attribute__ ((__always_inline__))
      deallocated (os::rtos::memory::memory_resource* heap, void* user_data)
      {
        SEGGER_SYSVIEW_HeapFree (heap, user_data);
      }
    } // namespace memory_resource

    namespace mutex
    {
      void
      create (os::rtos::mutex* mutex);

      void
      create_return (os::rtos::mutex* mutex);

      void
      destroy (os::rtos::mutex* mutex);

      void
      destroy_return (os::rtos::mutex* mutex);

      void
      lock (os::rtos::mutex* mutex);

      void
      lock_retval (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      try_lock (os::rtos::mutex* mutex);

      void
      try_lock_retval (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      timed_lock (os::rtos::mutex* mutex, unsigned int timeout);

      void
      timed_lock_retval (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      unlock (os::rtos::mutex* mutex);

      void
      unlock_retval (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      reset (os::rtos::mutex* mutex);

      void
      reset_retval (os::rtos::mutex* mutex, os::rtos::result_t result);

      void
      prio_ceiling_set (os::rtos::mutex* mutex, unsigned int prio);

      void
      prio_ceiling_set_retval (os::rtos::mutex* mutex,
                               os::rtos::result_t result);
    } // namespace mutex

    namespace semaphore
    {
      void
      create (os::rtos::semaphore* semaphore);

      void
      create_return (os::rtos::semaphore* semaphore);

      void
      destroy (os::rtos::semaphore* semaphore);

      void
      destroy_return (os::rtos::semaphore* semaphore);

      void
      post (os::rtos::semaphore* semaphore);

      void
      post_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res);

      void
      wait (os::rtos::semaphore* semaphore);

      void
      wait_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res);

      void
      try_wait (os::rtos::semaphore* semaphore);

      void
      try_wait_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res);

      void
      timed_wait (os::rtos::semaphore* semaphore, unsigned int timeout);

      void
      timed_wait_retval (os::rtos::semaphore* semaphore,
                         os::rtos::result_t res);

      void
      reset (os::rtos::semaphore* semaphore);

      void
      reset_retval (os::rtos::semaphore* semaphore, os::rtos::result_t res);
    } // namespace semaphore

    namespace message_queue
    {
      void
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes);

      void
      create_return (os::rtos::message_queue* mqueue);

      void
      destroy (os::rtos::message_queue* mqueue);

      void
      destroy_return (os::rtos::message_queue* mqueue);

      void
      send (os::rtos::message_queue* mqueue, std::size_t nbytes,
            unsigned int mprio);

      void
      send_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res);

      void
      try_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                unsigned int mprio);

      void
      try_send_retval (os::rtos::message_queue* mqueue,
                       os::rtos::result_t res);

      void
      timed_send (os::rtos::message_queue* mqueue, std::size_t nbytes,
                  unsigned int timeout, unsigned int mprio);

      void
      timed_send_retval (os::rtos::message_queue* mqueue,
                         os::rtos::result_t res);

      void
      receive (os::rtos::message_queue* mqueue, std::size_t nbytes);

      void
      receive_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res);

      void
      try_receive (os::rtos::message_queue* mqueue, std::size_t nbytes);

      void
      try_receive_retval (os::rtos::message_queue* mqueue,
                          os::rtos::result_t res);

      void
      timed_receive (os::rtos::message_queue* mqueue, std::size_t nbytes,
                     unsigned int timeout);

      void
      timed_receive_retval (os::rtos::message_queue* mqueue,
                            os::rtos::result_t res);

      void
      reset (os::rtos::message_queue* mqueue);

      void
      reset_retval (os::rtos::message_queue* mqueue, os::rtos::result_t res);
    } // namespace message_queue

    namespace message_queue_allocated
    {
      void
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes);

      void
      create_return (os::rtos::message_queue* mqueue);

      void
      destroy (os::rtos::message_queue* mqueue);

      void
      destroy_return (os::rtos::message_queue* mqueue);
    } // namespace message_queue_allocated

    namespace message_queue_inclusive
    {
      void
      create (os::rtos::message_queue* mqueue, std::size_t msgs,
              std::size_t msg_size_bytes);

      void
      create_return (os::rtos::message_queue* mqueue);
    } // namespace message_queue_inclusive

    namespace memory_pool
    {
      void
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes);

      void
      create_return (os::rtos::memory_pool* mpool);

      void
      destroy (os::rtos::memory_pool* mpool);

      void
      destroy_return (os::rtos::memory_pool* mpool);

      void
      alloc (os::rtos::memory_pool* mpool);

      void
      alloc_retval (os::rtos::memory_pool* mpool, void* addr);

      void
      try_alloc (os::rtos::memory_pool* mpool);

      void
      try_alloc_retval (os::rtos::memory_pool* mpool, void* addr);

      void
      timed_alloc (os::rtos::memory_pool* mpool, unsigned int timeout);

      void
      timed_alloc_retval (os::rtos::memory_pool* mpool, void* addr);

      void
      free (os::rtos::memory_pool* mpool, void* block);

      void
      free_retval (os::rtos::memory_pool* mpool, os::rtos::result_t res);

      void
      reset (os::rtos::memory_pool* mpool);

      void
      reset_retval (os::rtos::memory_pool* mpool, os::rtos::result_t res);
    } // namespace memory_pool

    namespace memory_pool_allocated
    {
      void
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes);

      void
      create_return (os::rtos::memory_pool* mpool);

      void
      destroy (os::rtos::memory_pool* mpool);

      void
      destroy_return (os::rtos::memory_pool* mpool);
    } // namespace memory_pool_allocated

    namespace memory_pool_inclusive
    {
      void
      create (os::rtos::memory_pool* mpool, std::size_t blocks,
              std::size_t block_size_bytes);

      void
      create_return (os::rtos::memory_pool* mpool);
    } // namespace memory_pool_inclusive

    namespace event_flags
    {
      void
      create (os::rtos::event_flags* evflags);

      void
      create_return (os::rtos::event_flags* evflags);

      void
      destroy (os::rtos::event_flags* evflags);

      void
      destroy_return (os::rtos::event_flags* evflags);

      void
      wait (os::rtos::event_flags* evflags, unsigned int mask,
            unsigned int mode);

      void
      wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res);

      void
      try_wait (os::rtos::event_flags* evflags, unsigned int mask,
                unsigned int mode);

      void
      try_wait_retval (os::rtos::event_flags* evflags, os::rtos::result_t res);

      void
      timed_wait (os::rtos::event_flags* evflags, unsigned int mask,
                  unsigned int mode, unsigned int timeout);

      void
      timed_wait_retval (os::rtos::event_flags* evflags,
                         os::rtos::result_t res);

      void
      raise (os::rtos::event_flags* evflags, unsigned int mask);

      void
      raise_retval (os::rtos::event_flags* evflags, os::rtos::result_t res);

      void
      clear (os::rtos::event_flags* evflags, unsigned int mask);

      void
      clear_retval (os::rtos::event_flags* evflags, os::rtos::result_t res);
    } // namespace event_flags

    namespace clock
    {
      void
      create (os::rtos::clock* clock);

      void
      destroy (os::rtos::clock* clock);

      void
      sleep_for (os::rtos::clock* clock, unsigned int duration);

      void
      sleep_for_retval (os::rtos::clock* clock, os::rtos::result_t res);

      void
      sleep_until (os::rtos::clock* clock, unsigned long long timestamp);

      void
      sleep_until_retval (os::rtos::clock* clock, os::rtos::result_t res);

      void
      wait_for (os::rtos::clock* clock, unsigned int duration);

      void
      wait_for_retval (os::rtos::clock* clock, os::rtos::result_t res);

    } // namespace clock

    namespace adjustable_clock
    {
      void
      sleep_until (os::rtos::clock* clock, unsigned long long timestamp);

      void
      sleep_until_retval (os::rtos::clock* clock, os::rtos::result_t res);

    } // namespace adjustable_clock

    namespace condition_variable
    {
      void
      create (os::rtos::condition_variable* cond);

      void
      create_return (os::rtos::condition_variable* cond);

      void
      destroy (os::rtos::condition_variable* cond);

      void
      destroy_return (os::rtos::condition_variable* cond);

      void
      signal (os::rtos::condition_variable* cond);

      void
      signal_retval (os::rtos::condition_variable* cond,
                     os::rtos::result_t res);

      void
      broadcast (os::rtos::condition_variable* cond);

      void
      broadcast_retval (os::rtos::condition_variable* cond,
                        os::rtos::result_t res);

      void
      wait (os::rtos::condition_variable* cond);

      void
      wait_retval (os::rtos::condition_variable* cond, os::rtos::result_t res);

      void
      timed_wait (os::rtos::condition_variable* cond, unsigned int timeout);

      void
      timed_wait_retval (os::rtos::condition_variable* cond,
                         os::rtos::result_t res);
    } // namespace condition_variable

    namespace timer
    {
      void
      create (os::rtos::timer* timer, unsigned int type);

      void
      create_return (os::rtos::timer* timer);

      void
      destroy (os::rtos::timer* timer);

      void
      destroy_return (os::rtos::timer* timer);

      void
      start (os::rtos::timer* timer, unsigned int period);

      void
      start_retval (os::rtos::timer* timer, os::rtos::result_t res);

      void
      stop (os::rtos::timer* timer);

      void
      stop_retval (os::rtos::timer* timer, os::rtos::result_t res);

      void
      callback (os::rtos::timer* timer);

      void
      callback_return (os::rtos::timer* timer);
    } // namespace timer

    // ------------------------------------------------------------------------

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

    // ------------------------------------------------------------------------

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
