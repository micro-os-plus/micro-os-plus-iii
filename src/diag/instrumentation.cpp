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

    namespace mutex
    {
      void
      created (os::rtos::mutex* mutex)
      {
        SEGGER_SYSVIEW_NameResource (reinterpret_cast<U32> (mutex),
                                     mutex->name ());
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_CREATED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            mutex->type ());
      }

      void
      locked (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_LOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (result));
      }

      void
      try_locked (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TRY_LOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (result));
      }

      void
      timed_locked (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_TIMED_LOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (result));
      }

      void
      unlocked (os::rtos::mutex* mutex, os::rtos::result_t result)
      {
        SEGGER_SYSVIEW_RecordU32x2 (
            OS_INTEGER_INSTRUMENTATION_ID_MUTEX_UNLOCKED,
            SEGGER_SYSVIEW_ShrinkId (reinterpret_cast<U32> (mutex)),
            static_cast<U32> (result));
      }

    } // namespace mutex

    void
    exit (int exit_code)
    {
      // SEGGER_SYSVIEW_OnTaskStopReady (
        //   reinterpret_cast<U32> (&os::rtos::this_thread::thread ()),
        //   OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_SWITCH);
      SEGGER_SYSVIEW_RecordU32 (OS_INTEGER_INSTRUMENTATION_ID_EXIT,
                                static_cast<U32> (exit_code));
    }

  } // namespace instrumentation
} // namespace os

void
os_instrumentation_exit (int exit_code)
{
  // SEGGER_SYSVIEW_OnTaskStopReady (
    //   reinterpret_cast<U32> (&os::rtos::this_thread::thread ()),
    //   OS_INTEGER_INSTRUMENTATION_SUSPEND_CAUSE_SWITCH);
  SEGGER_SYSVIEW_RecordU32 (OS_INTEGER_INSTRUMENTATION_ID_EXIT,
                            static_cast<U32> (exit_code));
}

#endif // defined(OS_INCLUDE_INSTRUMENTATION)

// ----------------------------------------------------------------------------
