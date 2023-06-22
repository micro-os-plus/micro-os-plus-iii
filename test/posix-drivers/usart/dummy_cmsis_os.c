/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include "cmsis_os.h"

// There is no functionality at all, the purpose of these definitions
// is only to keep the linker happy when building the tests.

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

osSemaphoreId
osSemaphoreCreate (const osSemaphoreDef_t *semaphore_def, int32_t count)
{
  return 0;
}

int32_t
osSemaphoreWait (osSemaphoreId semaphore_id, uint32_t millisec)
{
  return 0;
}

osStatus
osSemaphoreRelease (osSemaphoreId semaphore_id)
{
  return 0;
}

osStatus
osSemaphoreDelete (osSemaphoreId semaphore_id)
{
  return 0;
}

#pragma GCC diagnostic pop

