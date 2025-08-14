/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

// ----------------------------------------------------------------------------


#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/cortexm/exception-handlers.h>
#include <cmsis-plus/rtos/os-c-decls.h>

#include <cmsis_device.h>

void __attribute__ ((section(".after_vectors")))
SysTick_Handler (void)
{
  HAL_IncTick();

  // Call the system timer handler to keep track of time.
  os_systick_handler ();
}

// ----------------------------------------------------------------------------
