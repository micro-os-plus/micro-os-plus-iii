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
   * @brief Hook to enter a power saving mode.
   * @par Parameters
   *  None
   * @retval true The hook entered a power saving mode.
   * @retval false The hook did not enter a power saving mode.
   */
  bool
  os_rtos_idle_enter_power_saving_mode_hook (void);

/**
 * @}
 */

#if defined(__cplusplus)
}
#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_HOOKS_H_ */
