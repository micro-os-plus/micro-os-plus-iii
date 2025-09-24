/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2016-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit.
 */

#if defined(OS_INCLUDE_INSTRUMENTATION)

#include <cmsis_device.h>

#include "SEGGER_SYSVIEW.h"

// ----------------------------------------------------------------------------

// The address of SYSVIEW_X_OS_TraceAPI is passed to SEGGER_SYSVIEW_Init().
extern const SEGGER_SYSVIEW_OS_API SYSVIEW_X_OS_TraceAPI;

// Frequency of the timestamp, in Hz.
// Must match SEGGER_SYSVIEW_GET_TIMESTAMP in SEGGER_SYSVIEW_Conf.h
#define SYSVIEW_TIMESTAMP_FREQ (SystemCoreClock)

// System Frequency in Hz.
// SystemcoreClock is used in most CMSIS compatible projects.
#define SYSVIEW_CPU_FREQ (SystemCoreClock)

extern unsigned int __data_start__;

// The lowest RAM address used for IDs (pointers)
#define SYSVIEW_RAM_BASE reinterpret_cast<unsigned int> (&__data_start__)

#define DWT_CTRL \
  (*reinterpret_cast<volatile U32*> (0xE0001000uL)) // DWT Control Register
#define NOCYCCNT_BIT (1uL << 25)
// Cycle counter support bit
#define CYCCNTENA_BIT (1uL << 0)
// Cycle counter enable bit

// Sends SystemView description strings.
static void
_cbSendSystemDesc (void)
{
  SEGGER_SYSVIEW_SendSysDesc (
      "N=" SEGGER_SYSVIEW_APP_NAME); // Application name
  SEGGER_SYSVIEW_SendSysDesc ("O=" SEGGER_SYSVIEW_OS_NAME); // Operating system
  SEGGER_SYSVIEW_SendSysDesc (
      "D=" SEGGER_SYSVIEW_DEVICE_NAME); // Target device
  SEGGER_SYSVIEW_SendSysDesc ("C=" SEGGER_SYSVIEW_CORE_NAME); // Target core
  SEGGER_SYSVIEW_SendSysDesc ("I#15=SysTick"); // Interrupt #<InterruptID>
}

// Global functions
void
SEGGER_SYSVIEW_Conf (void)
{
  // The cycle counter must be activated in order
  // to use time related functions.
  if ((DWT_CTRL & NOCYCCNT_BIT) == 0)
    {
      if ((DWT_CTRL & CYCCNTENA_BIT) == 0)
        {
          DWT_CTRL |= CYCCNTENA_BIT; // Cycle counter supported?
          // Cycle counter not enabled?
          // Enable Cycle counter
        }
    }

  SEGGER_SYSVIEW_Init (
      SYSVIEW_TIMESTAMP_FREQ, // Frequency of the timestamp
      SYSVIEW_CPU_FREQ, // Frequency of the system
      &SYSVIEW_X_OS_TraceAPI, // OS-specific SEGGER_SYSVIEW_OS_API
      _cbSendSystemDesc); // Callback for application-specific description

  // Sets the RAM base address, which is subtracted from IDs in order to save
  // bandwidth.
  SEGGER_SYSVIEW_SetRAMBase (SYSVIEW_RAM_BASE);
}

#endif // defined(OS_INCLUDE_INSTRUMENTATION)

// ----------------------------------------------------------------------------
