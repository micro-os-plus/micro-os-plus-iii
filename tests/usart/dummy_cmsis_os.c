/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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

