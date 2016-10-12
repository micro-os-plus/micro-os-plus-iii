/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include "cmsis_os.h"

// There is no functionality at all, the purpose of these definitions
// is only to keep the linker happy when building the tests.

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

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

