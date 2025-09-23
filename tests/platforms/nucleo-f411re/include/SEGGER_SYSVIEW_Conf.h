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

#ifndef SEGGER_SYSVIEW_CONF_H
#define SEGGER_SYSVIEW_CONF_H

// ----------------------------------------------------------------------------

// The OS name must be a simple name, since it may be part of a file name.

#define SEGGER_SYSVIEW_APP_NAME "Test Application"
#define SEGGER_SYSVIEW_OS_NAME "micro-os-plus-iii"
#define SEGGER_SYSVIEW_DEVICE_NAME "STM32F411RE"
#define SEGGER_SYSVIEW_CORE_NAME "Cortex-M4"

// In case there are extremely many events and the buffer overflows,
// increase the buffer size (must be a power of 2).
#define SEGGER_SYSVIEW_RTT_BUFFER_SIZE (1024 * 2)

// ----------------------------------------------------------------------------

#endif // SEGGER_SYSVIEW_CONF_H
