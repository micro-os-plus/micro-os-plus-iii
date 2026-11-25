/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2023-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#ifndef PLATFORM_QEMU_CORTEX_M33_PLATFORM_H_
#define PLATFORM_QEMU_CORTEX_M33_PLATFORM_H_

// ----------------------------------------------------------------------------

#define ARM_CMSIS_VALIDATOR_IRQHandler WDT_IRQHandler

#define OS_USE_SEMIHOSTING_SYSCALLS

// QEMU raises the SysTick pending flag after the counter reloaded, so the
// high resolution clock may step back by a few cycles (see test-rtos.cpp).
#define OS_TEST_HRCLOCK_BACKWARDS_TOLERANCE_CYCLES 1000

// ----------------------------------------------------------------------------

#endif /* PLATFORM_QEMU_CORTEX_M33_PLATFORM_H_ */
