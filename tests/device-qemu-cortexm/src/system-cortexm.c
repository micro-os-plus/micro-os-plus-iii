/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2022 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/MIT/.
 */

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include "cmsis_device.h"

// ----------------------------------------------------------------------------

#define SYSTEM_CLOCK (16000000UL)
uint32_t SystemCoreClock = SYSTEM_CLOCK;

// ----------------------------------------------------------------------------

void
SystemInit (void)
{
  // FPU settings
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
  // set CP10 and CP11 Full Access
  SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
#endif
}

void
SystemCoreClockUpdate (void)
{
  SystemCoreClock = SYSTEM_CLOCK;
}

// ----------------------------------------------------------------------------
