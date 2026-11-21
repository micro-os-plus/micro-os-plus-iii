/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#ifndef CMSIS_PLUS_CORTEXM_EXCEPTION_HANDLERS_H_
#define CMSIS_PLUS_CORTEXM_EXCEPTION_HANDLERS_H_

#if !(defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M'))
#error "This header is specific to Cortex-M."
#endif

#include <stdint.h>

#if defined(DEBUG)
#define __DEBUG_BKPT() __asm__ volatile ("bkpt 0")
#endif

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif

  extern void
  Reset_Handler (void);

  extern void
  NMI_Handler (void);

  extern void
  HardFault_Handler (void);

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)

  extern void
  MemManage_Handler (void);

  extern void
  BusFault_Handler (void);

  extern void
  UsageFault_Handler (void);

  extern void
  DebugMon_Handler (void);

#endif

// Mainline v8: v8-M Main, v8.1-M (M33, M35P, M52, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2) && (__ARM_ARCH >= 8)

  extern void
  SecureFault_Handler (void);

#endif

  extern void
  SVC_Handler (void);

  extern void
  PendSV_Handler (void);

  extern void
  SysTick_Handler (void);

  // Exception Stack Frame for the Cortex-M3 and Cortex-M4 processor.
  typedef struct exception_stack_frame_s
  {
    uint32_t r0;
    uint32_t r1;
    uint32_t r2;
    uint32_t r3;
    uint32_t r12;
    uint32_t lr;
    uint32_t pc;
    uint32_t psr;
// Mainline with DSP or v8: v7E-M, v8-M Main, v8.1-M (M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2) \
    && ((__ARM_ARCH >= 8) || defined(__ARM_FEATURE_DSP))
    uint32_t s[16];
#endif
  } exception_stack_frame_t;

#if defined(TRACE)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  void
  dump_exception_stack (exception_stack_frame_t* frame, uint32_t cfsr,
                        uint32_t mmfar, uint32_t bfar, uint32_t lr);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
// Baseline: v6-M, v8-M Base (M0, M0+, M1, M23).
#if (__ARM_ARCH_ISA_THUMB == 1)
  void
  dump_exception_stack (exception_stack_frame_t* frame, uint32_t lr);
#endif // (__ARM_ARCH_ISA_THUMB == 1)
#endif // defined(TRACE)

  void
  HardFault_Handler_C (exception_stack_frame_t* frame, uint32_t lr);

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)

  void
  UsageFault_Handler_C (exception_stack_frame_t* frame, uint32_t lr);

  void
  BusFault_Handler_C (exception_stack_frame_t* frame, uint32_t lr);

#endif /* (__ARM_ARCH_ISA_THUMB == 2) */

#if defined(__cplusplus)
}
#endif

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_CORTEXM_EXCEPTION_HANDLERS_H_ */
