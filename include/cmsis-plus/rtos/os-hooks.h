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

#ifndef CMSIS_PLUS_RTOS_OS_HOOKS_H_
#define CMSIS_PLUS_RTOS_OS_HOOKS_H_

#include <stdbool.h>

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif /* __cplusplus */

  /**
   * @addtogroup cmsis-plus-app-hooks
   * @{
   */

  /**
   * @name Startup Routines
   * @{
   */

  /**
   * @brief The standard C application entry point.
   * @par Parameters
   *  None.
   * @par Returns
   *  Does not return.
   */
  void
  __attribute__ ((noreturn))
  _start (void);

  /**
   * @brief Early hardware initialisations.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  os_startup_initialize_hardware_early (void);

  /**
   * @brief Hardware initialisations.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  os_startup_initialize_hardware (void);

  /**
   * @brief Free store initialisations.
   * @param heap_begin The first unallocated RAM address (after the BSS).
   * @param heap_end The address after the free store.
   * @par Returns
   *  Nothing.
   */
  void
  os_startup_initialize_free_store (void* heap_begin, void* heap_end);

  /**
   * @brief Arguments initialisations.
   * @param [out] p_argc Pointer to argc.
   * @param [out] p_argv Pointer to argv.
   */
  void
  os_startup_initialize_args (int* p_argc, char*** p_argv);

  /**
   * @}
   */

  /**
   * @name Hooks
   * @{
   */

  /**
   * @brief Hook to enter a power saving mode.
   * @par Parameters
   *  None.
   * @retval true The hook entered a power saving mode.
   * @retval false The hook did not enter a power saving mode.
   */
  bool
  os_rtos_idle_enter_power_saving_mode_hook (void);

  /**
   * @brief Hook to handle out of memory in the application free store.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  os_rtos_application_out_of_memory_hook (void);

  /**
   * @brief Hook to handle out of memory in the RTOS dynamic memory.
   * @par Parameters
   *  None.
   * @par Returns
   *  Nothing.
   */
  void
  os_rtos_system_out_of_memory_hook (void);

/**
 * @}
 */

/**
 * @name Compatibility Macros
 * @{
 */

#define os_initialize_hardware_early os_startup_initialize_hardware_early
#define os_initialize_hardware os_startup_initialize_hardware
#define os_initialize_args os_startup_initialize_args

/**
 * @}
 */

/**
 * @}
 */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_HOOKS_H_ */
