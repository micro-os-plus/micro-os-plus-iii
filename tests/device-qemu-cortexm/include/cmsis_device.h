/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus/)
 * Copyright (c) 2022-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef MICRO_OS_PLUS_DEVICE_H_
#define MICRO_OS_PLUS_DEVICE_H_

// ----------------------------------------------------------------------------

// #include <micro-os-plus/architecture.h>

// ----------------------------------------------------------------------------

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

  // ----------------------------------------------------------------------------

  typedef enum
  {
    /******  Cortex-M4 Processor Exceptions Numbers
     ****************************************************************/
    NonMaskableInt_IRQn = -14, /*!< 2 Non Maskable Interrupt */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    MemoryManagement_IRQn = -12, /*!< 4 Memory Management Interrupt */
    BusFault_IRQn = -11, /*!< 5 Bus Fault Interrupt */
    UsageFault_IRQn = -10, /*!< 6 Usage Fault Interrupt */
#endif
    SVCall_IRQn = -5, /*!< 11 SV Call Interrupt */
#if defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7EM__)
    DebugMonitor_IRQn = -4, /*!< 12 Debug Monitor Interrupt */
#endif
    PendSV_IRQn = -2, /*!< 14 Pend SV Interrupt */
    SysTick_IRQn = -1, /*!< 15 System Tick Interrupt */
  } IRQn_Type;

#pragma GCC diagnostic push
#ifdef __cplusplus
#pragma GCC diagnostic ignored "-Wuseless-cast"
#pragma GCC diagnostic ignored "-Wvolatile"
#pragma GCC diagnostic ignored "-Wregister"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
#pragma GCC diagnostic ignored "-Wconversion"
#pragma GCC diagnostic ignored "-Wundef"

#if defined(MICRO_OS_PLUS_DEVICE_QEMU_CORTEX_M0)

#define __CM0_REV 0x0000U /*!< Core revision r0p0 */
#define __MPU_PRESENT 0U /*!< MPU not present */
#define __NVIC_PRIO_BITS 4U /*!< Uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig \
  0U /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT 0U /*!< FPU not present */
#define __CORTEX_SC 0U

#include "core_cm0.h"

#elif defined(MICRO_OS_PLUS_DEVICE_QEMU_CORTEX_M0PLUS)

#define __CM0PLUS_REV 0x0000U /*!< Core revision r0p0 */
#define __MPU_PRESENT 0U /*!< MPU not present */
#define __NVIC_PRIO_BITS 4U /*!< Uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig \
  0U /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT 0U /*!< FPU not present */

#include "core_cm0plus.h"

#elif defined(MICRO_OS_PLUS_DEVICE_QEMU_CORTEX_M3)

#define __CM3_REV 0x0000U /*!< Core revision r0p0 */
#define __MPU_PRESENT 1U /*!< MPU present */
#define __NVIC_PRIO_BITS 4U /*!< Uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig \
  0U /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT 0U /*!< FPU not present */

#include "core_cm3.h"

#elif defined(MICRO_OS_PLUS_DEVICE_QEMU_CORTEX_M4)

#define __CM4_REV 0x0000U /*!< Core revision r0p0 */
#define __MPU_PRESENT 1U /*!< MPU present */
#define __NVIC_PRIO_BITS 4U /*!< Uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig \
  0U /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT 1U /*!< FPU present */

#include "core_cm4.h"

#elif defined(MICRO_OS_PLUS_DEVICE_QEMU_CORTEX_M7)

#define __CM7_REV 0x0000U /*!< Core revision r0p0 */
#define __MPU_PRESENT 1U /*!< MPU present */
#define __NVIC_PRIO_BITS 4U /*!< Uses 4 Bits for the Priority Levels */
#define __Vendor_SysTickConfig \
  0U /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT 1U /*!< FPU present */

#define __ICACHE_PRESENT 1U
#define __DCACHE_PRESENT 1U

#include "core_cm7.h"

#else
#error "Device not supported"
#endif

#pragma GCC diagnostic pop

  extern uint32_t SystemCoreClock; /*!< System Clock Frequency (Core Clock) */

  extern void
  SystemInit (void);
  extern void
  SystemCoreClockUpdate (void);

  // ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif // MICRO_OS_PLUS_DEVICE_H_

// ----------------------------------------------------------------------------
