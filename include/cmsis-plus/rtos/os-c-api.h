/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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
 * replacement for C++ applications, and provides a C API.
 *
 * The code is inspired by ARM CMSIS cmsis_os.h file, v1.02,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_RTOS_OS_C_API_H_
#define CMSIS_PLUS_RTOS_OS_C_API_H_

// ----------------------------------------------------------------------------

// Include the CMSIS++ C API structures definitions.
#include <cmsis-plus/rtos/os-c-defs.h>

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// ----------------------------------------------------------------------------

#ifdef  __cplusplus
extern "C"
{
#endif

// --------------------------------------------------------------------------

//typedef uint32_t os_result_t;

  enum
  {
    os_ok = 0, ///< function completed; no error or event occurred.
  };

  // --------------------------------------------------------------------------

  int
  os_main (int argc, char* argv[]);

  // --------------------------------------------------------------------------

  os_result_t
  os_sched_initialize (void);

  os_result_t
  os_sched_start (void);

  bool
  os_sched_is_started (void);

  // --------------------------------------------------------------------------

  void
  os_thread_create (os_thread_t* thread, const os_thread_attr_t* attr,
                    os_thread_func_t func, const os_thread_func_args_t args);

  void
  os_thread_exit (os_thread_t* thread, void* exit_ptr);

  os_result_t
  os_thread_join (os_thread_t* thread, void** exit_ptr);

  os_thread_prio_t
  os_thread_get_prio (os_thread_t* thread);

  os_result_t
  os_thread_set_prio (os_thread_t* thread, os_thread_prio_t prio);

  void
  os_thread_wakeup (os_thread_t* thread);

  // --------------------------------------------------------------------------

  void
  os_systick_handler(void);

  void
  os_rtc_handler(void);

// --------------------------------------------------------------------------

#ifdef  __cplusplus
}
#endif

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_C_API_H_ */
