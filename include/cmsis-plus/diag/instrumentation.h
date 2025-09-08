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


// ----------------------------------------------------------------------------

#endif // CMSIS_PLUS_DIAG_INSTRUMENTATION_H_
