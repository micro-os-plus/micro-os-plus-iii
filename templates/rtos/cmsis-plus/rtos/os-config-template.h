/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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
#define OS_INTEGER_RTOS_CRITICAL_SECTION_INTERRUPT_PRIORITY (4)

// ----------------------------------------------------------------------

typedef struct os_thread_user_storage_s
{

} os_thread_user_storage_t;

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_CONFIG_TEMPLATE_H_ */
