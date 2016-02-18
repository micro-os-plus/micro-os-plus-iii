/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 */

#ifndef CMSIS_PLUS_RTOS_OS_CONFIG_TEMPLATE_H_
#define CMSIS_PLUS_RTOS_OS_CONFIG_TEMPLATE_H_

// ----------------------------------------------------------------------------

#define OS_INTEGER_SYSTICK_FREQUENCY_HZ (1000)

#define OS_INTEGER_MAX_NUMBER_OF_THREADS (10)


// With 4 bits NVIC, there are 16 levels, 0 = highest, 15 = lowest

// Disable all interrupts from 15 to 4, keep 3-2-1 enabled
#define OS_INTEGER_CRITICAL_SECTION_INTERRUPT_PRIORITY (4)

// ----------------------------------------------------------------------

typedef struct os_thread_user_storage_s
{

} os_thread_user_storage_t;

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_CONFIG_TEMPLATE_H_ */
