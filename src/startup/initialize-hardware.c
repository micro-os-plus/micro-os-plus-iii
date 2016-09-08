/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-hooks.h>
#include <cmsis_device.h>

// ----------------------------------------------------------------------------

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
extern unsigned int __vectors_start;
#endif

// ----------------------------------------------------------------------------

/**
 * @details
 * This is the default early hardware initialisation routine.
 *
 * It is called right at the beginning of `_start()`, to switch clocks
 * to higher frequencies and have the rest of the initialisations run faster.
 *
 * The application can redefine it
 * for more complex cases that requires inits before DATA and BSS init.
 *
 * It is mandatory on platforms like Kinetis, which start with the
 * watch dog enabled and require an early sequence to disable it.
 *
 * Also useful on platform with external RAM, that need to be
 * initialised before filling the BSS section.
 */
void __attribute__((weak))
os_startup_initialize_hardware_early (void)
{
  // Call the CSMSIS system initialisation routine.
  SystemInit ();

#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)

  // Set VTOR to the actual address, provided by the linker script.
  // Override the manual, possibly wrong, SystemInit() setting.
  SCB->VTOR = (uint32_t) (&__vectors_start);
  // Ensure all subsequence instructions use the new configuration.
  __DSB ();

#endif

  // The current version of SystemInit() leaves the value of the clock
  // in a RAM variable (SystemCoreClock), which will be cleared shortly,
  // so it needs to be recomputed after the RAM initialisations
  // are completed.

#if defined(OS_INCLUDE_STARTUP_INIT_FP) || (defined (__VFP_FP__) && !defined (__SOFTFP__))

  // Normally FP init is done by SystemInit(). In case this is not done
  // there, it is possible to force its inclusion by defining
  // OS_INCLUDE_STARTUP_INIT_FP.

  // Enable the Cortex-M4 FPU only when -mfloat-abi=hard.
  // Code taken from Section 7.1, Cortex-M4 TRM (DDI0439C)

  // Set bits 20-23 to enable CP10 and CP11 coprocessor.
  SCB->CPACR |= (0xF << 20);

  // Lazy save.
  FPU->FPCCR |= FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;

#endif // (__VFP_FP__) && !(__SOFTFP__)

#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)

  SCB->SHCSR |= SCB_SHCSR_USGFAULTENA_Msk;

#endif
}

/**
 * @details
 * This is the default implementation for the second hardware
 * initialisation routine.
 *
 * It is called from `_start()`, right after DATA & BSS init, before
 * the static constructors.
 *
 * The application can
 * redefine it for more complex cases that
 * require custom inits (before constructors), otherwise these inits can
 * be done in main().
 */
void __attribute__((weak))
os_startup_initialize_hardware (void)
{
  // Call the CSMSIS system clock routine to store the clock frequency
  // in the SystemCoreClock global RAM location.
  SystemCoreClockUpdate ();
}

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */

