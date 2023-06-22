/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_RTOS_OS_CONFIG_TEMPLATE_H_
#define CMSIS_PLUS_RTOS_OS_CONFIG_TEMPLATE_H_

// ----------------------------------------------------------------------------

#define OS_INTEGER_SYSTICK_FREQUENCY_HZ (1000)

#define OS_INTEGER_MAX_NUMBER_OF_THREADS (10)


// With 4 bits NVIC, there are 16 levels, 0 = highest, 15 = lowest

// Disable all interrupts from 15 to 4, keep 3-2-1 enabled
#define OS_INTEGER_RTOS_CRITICAL_SECTION_INTERRUPT_PRIORITY (4)

// ----------------------------------------------------------------------

typedef struct os_thread_user_storage_s
{

} os_thread_user_storage_t;

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_CONFIG_TEMPLATE_H_ */
