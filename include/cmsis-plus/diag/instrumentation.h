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

// Base for all custom instrumentation IDs.
#define OS_INTEGER_INSTRUMENTATION_ID_BASE (32u)

// 33
#define OS_INTEGER_INSTRUMENTATION_ID_EXIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 1u)
// 34-41 reserved (2-9).

// 42
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 10u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_UNLOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 11u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_LOCKED_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 12u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_RESCHEDULE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 13u)
#define OS_INTEGER_INSTRUMENTATION_ID_SCHEDULER_PREEMPTIVE_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 14u)

// 47
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 15u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 16u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_YIELD \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 17u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_SUSPEND \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 18u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_RESUME \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 19u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 20u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_PRIORITY_INHERITED_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 21u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_DETACH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 22u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_JOIN \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 23u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_CANCEL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 24u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_INTERRUPT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 25u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_KILL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 26u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_INTERNAL_EXIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 27u)

// 58
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 28u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 328u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TRY_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 29u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TRY_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 329u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 30u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_TIMED_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 330u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_RAISE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 31u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_RAISE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 331u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_CLEAR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 32u)
#define OS_INTEGER_INSTRUMENTATION_ID_THREAD_FLAGS_CLEAR_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 332u)

// 63
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 33u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 333u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 34u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 35u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 335u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 36u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 336u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 37u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 337u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 38u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCK_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 338u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 39u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 339u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_PRIO_CEILING_SET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 40u)
#define OS_INTEGER_INSTRUMENTATION_ID_MUTEX_PRIO_CEILING_SET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 340u)

// 73
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 41u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 341u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 42u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 342u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POST \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 43u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_POST_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 343u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 44u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 344u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 45u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TRY_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 345u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 46u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_TIMED_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 346u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 47u)
#define OS_INTEGER_INSTRUMENTATION_ID_SEMAPHORE_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 347u)

// 80
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 48u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 348u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 49u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 349u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_SEND \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 50u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_SEND_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 350u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_SEND \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 51u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_SEND_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 351u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_SEND \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 52u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_SEND_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 352u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RECEIVE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 53u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RECEIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 353u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_RECEIVE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 54u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TRY_RECEIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 354u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_RECEIVE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 55u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_TIMED_RECEIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 355u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 56u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 356u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 57u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 357u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 58u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_ALLOCATED_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 358u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_INCLUSIVE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 59u)
#define OS_INTEGER_INSTRUMENTATION_ID_MESSAGE_QUEUE_INCLUSIVE_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 359u)

// 92
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 60u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 360u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 61u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 361u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 62u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOC_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 362u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TRY_ALLOC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 63u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TRY_ALLOC_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 363u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TIMED_ALLOC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 64u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_TIMED_ALLOC_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 364u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_FREE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 65u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_FREE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 365u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_RESET \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 66u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_RESET_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 366u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 67u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE_ALLOCATED_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 367u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 68u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_ALLOCATED_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 368u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_INCLUSIVE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 69u)
#define OS_INTEGER_INSTRUMENTATION_ID_MEMORY_POOL_CREATE_INCLUSIVE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 369u)

// 102
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 70u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CREATE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 370u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 71u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_DESTROY_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 371u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 72u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 372u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TRY_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 73u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TRY_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 373u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 74u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_TIMED_WAIT_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 374u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_RAISE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 75u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_RAISE_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 375u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CLEAR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 76u)
#define OS_INTEGER_INSTRUMENTATION_ID_EVENT_FLAGS_CLEAR_VALUES \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 376u)

// 109
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 77u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 78u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_FOR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 79u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_SLEEP_UNTIL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 80u)
#define OS_INTEGER_INSTRUMENTATION_ID_CLOCK_WAIT_FOR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 81u)
#define OS_INTEGER_INSTRUMENTATION_ID_ADJUSTABLE_CLOCK_SLEEP_UNTIL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 82u)

// 115
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 83u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 84u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_SIGNAL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 85u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_BROADCAST \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 86u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 87u)
#define OS_INTEGER_INSTRUMENTATION_ID_CONDITION_VARIABLE_TIMED_WAIT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 88u)

// 121
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 89u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 90u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_START \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 91u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_STOP \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 92u)
#define OS_INTEGER_INSTRUMENTATION_ID_TIMER_CALLBACK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 93u)

// 126
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_VOPEN \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 94u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_VOPEN_OFLAGS \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 394u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 95u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 96u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_CLOSE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 97u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_READ \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 98u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_WRITE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 99u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_WRITEV \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 100u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_VFCNTL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 101u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_ISATTY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 102u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_FSTAT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 103u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_IO_LSEEK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 104u)

// 137
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 105u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 106u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VOPEN \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 107u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VOPEN_OFLAGS \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 108u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_CLOSE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 109u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_VIOCTL \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 110u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_DEVICE_SYNC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 111u)

// 144
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_READ_BLOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 112u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_WRITE_BLOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 113u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_VIOCTL_BLOCK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 114u)

// 147
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_BLOCK_DEVICE_PARTITION_CONFIGURE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 115u)

// 148
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 116u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 117u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_CLOSE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 118u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FTRUNCATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 119u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FSYNC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 120u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_FSTATVFS \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 121u)

// 154
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_MKDIR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 122u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_MKDIR_MORE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 123u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_RMDIR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 124u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_SYNC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 125u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_CHMOD \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 126u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_CHMOD_MORE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 127u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_STAT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 128u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_STAT_MORE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 129u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_TRUNCATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 130u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_TRUNCATE_MORE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 131u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_RENAME \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 132u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_RENAME_MORE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 133u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_UNLINK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 134u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_UTIME \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 135u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_UTIME_MORE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 136u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_STATVFS \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 137u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_STATVFS_MORE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 138u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_OPENDIR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 139u)

// 172
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_CREATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 140u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_DESTROY \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 141u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_VMKFS \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 142u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_VMOUNT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 143u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_UMOUNT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 144u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_VOPEN \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 145u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_VOPEN_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 146u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_OPENDIR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 147u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_OPENDIR_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 148u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_MKDIR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 149u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_MKDIR_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 150u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_RMDIR \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 151u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_RMDIR_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 152u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_SYNC \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 153u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_CHMOD \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 154u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_CHMOD_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 155u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_STAT \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 156u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_STAT_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 157u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_TRUNCATE \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 158u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_TRUNCATE_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 159u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_RENAME \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 160u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_RENAME_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 161u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_RENAME_PATH2 \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 162u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_UNLINK \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 163u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_UNLINK_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 164u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_UTIME \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 165u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_UTIME_PATH \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 166u)
#define OS_INTEGER_INSTRUMENTATION_ID_POSIX_FILE_SYSTEM_STATVFS \
  (OS_INTEGER_INSTRUMENTATION_ID_BASE + 167u)

// 200

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
      yield (os::rtos::thread* thread);

      void
      suspend (os::rtos::thread* thread);

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

    namespace posix
    {
      void
      vopen (const char* path, int oflags);

      void
      vopen_retval (void* io, const char* path = nullptr);

      namespace io
      {
        void
        create (void* io, unsigned int t);

        void
        destroy (void* io);

        void
        close (void* io);

        void
        close_retval (void* io, int res);

        void
        read (void* io, const void* buf, std::size_t nbyte);

        void
        read_retval (void* io, ssize_t res);

        void
        write (void* io, const void* buf, std::size_t nbyte);

        void
        write_retval (void* io, ssize_t res);

        void
        writev (void* io, const void* iov, int iovcnt);

        void
        writev_retval (void* io, ssize_t res);

        void
        vfcntl (void* io, int cmd);

        void
        vfcntl_retval (void* io, int res);

        void
        isatty (void* io);

        void
        isatty_retval (void* io, int res);

        void
        fstat (void* io, void* buf);

        void
        fstat_retval (void* io, int res);

        void
        lseek (void* io, off_t offset, int whence);

        void
        lseek_retval (void* io, off_t offset);

      } // namespace io

      namespace device
      {
        void
        create (void* device, unsigned int t);

        void
        destroy (void* device);

        void
        vopen (void* device, const char* path, int oflags);

        void
        vopen_retval (void* device, int res);

        void
        close (void* device);

        void
        close_retval (void* device, int res);

        void
        vioctl (void* device, int request);

        void
        vioctl_retval (void* device, int res);

        void
        sync (void* device);

        void
        sync_return (void* device);
      } // namespace device

      namespace block_device
      {
        void
        read_block (void* bdev, void* buf, std::size_t block,
                    std::size_t nblocks);

        void
        read_block_retval (void* bdev, ssize_t res);

        void
        write_block (void* bdev, const void* buf, std::size_t block,
                     std::size_t nblocks);

        void
        write_block_retval (void* bdev, ssize_t res);

        void
        vioctl (void* bdev, int request);

        void
        vioctl_retval (void* bdev, int res);
      } // namespace block_device

      namespace block_device_partition
      {
        void
        configure (void* partition, std::size_t offset, std::size_t nblocks);

        void
        configure_return (void* partition);
      } // namespace block_device_partition

      namespace file
      {
        void
        create (void* file, unsigned int t);

        void
        destroy (void* file);

        void
        close (void* file);

        void
        close_retval (void* file, int res);

        void
        ftruncate (void* file, off_t length);

        void
        ftruncate_retval (void* file, int res);

        void
        fsync (void* file);

        void
        fsync_retval (void* file, int res);

        void
        fstatvfs (void* file, void* buf);

        void
        fstatvfs_retval (void* file, int res);
      } // namespace file

      void
      mkdir (const char* path, unsigned int mode);

      void
      mkdir_retval (int res);

      void
      rmdir (const char* path);

      void
      rmdir_retval (int res);

      void
      sync (void);

      void
      sync_return ();

      void
      chmod (const char* path, unsigned int mode);

      void
      chmod_retval (int res);

      void
      stat (const char* path, void* buf);

      void
      stat_retval (int res);

      void
      truncate (const char* path, int length);

      void
      truncate_retval (int res);

      void
      rename (const char* existing, const char* _new);

      void
      rename_retval (int res);

      void
      unlink (const char* path);

      void
      unlink_retval (int res);

      void
      utime (const char* path, const void* times);

      void
      utime_retval (int res);

      void
      statvfs (const char* path, void* buf);

      void
      statvfs_retval (int res);

      void
      opendir (const char* dirpath);

      void
      opendir_retval (void* dirp);

      namespace file_system
      {
        void
        create (void* fs);

        void
        destroy (void* fs);

        void
        vmkfs (void* fs, int args);

        void
        vmkfs_retval (void* fs, int res);

        void
        vmount (void* fs, const char* path, unsigned int flags);

        void
        vmount_retval (void* fs, int res);

        void
        umount (void* fs, unsigned int flags);

        void
        umount_retval (void* fs, int res);

        void
        vopen (void* fs, const char* path, int oflags);

        void
        vopen_retval (void* fs, void* ptr);

        void
        opendir (void* fs, const char* path);

        void
        opendir_retval (void* fs, void* dirp);

        void
        mkdir (void* fs, const char* path, unsigned int mode);

        void
        mkdir_retval (void* fs, int res);

        void
        rmdir (void* fs, const char* path);

        void
        rmdir_retval (void* fs, int res);

        void
        sync (void* fs);

        void
        sync_return (void* fs);

        void
        chmod (void* fs, const char* path, unsigned int mode);

        void
        chmod_retval (void* fs, int res);

        void
        stat (void* fs, const char* path, void* buf);

        void
        stat_retval (void* fs, int res);

        void
        truncate (void* fs, const char* path, int length);

        void
        truncate_retval (void* fs, int res);

        void
        rename (void* fs, const char* existing, const char* _new);

        void
        rename_retval (void* fs, int res);

        void
        unlink (void* fs, const char* path);

        void
        unlink_retval (void* fs, int res);

        void
        utime (void* fs, const char* path, const void* times);

        void
        utime_retval (void* fs, int res);

        void
        statvfs (void* fs, void* buf);

        void
        statvfs_retval (void* fs, int res);

      } // namespace file_system

    } // namespace posix

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
