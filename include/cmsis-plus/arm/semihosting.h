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

#ifndef CMSIS_PLUS_ARM_SEMIHOSTING_H_
#define CMSIS_PLUS_ARM_SEMIHOSTING_H_

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
extern "C"
{
#endif // defined(__cplusplus)

  // --------------------------------------------------------------------------

  // Semihosting operations.
  enum OperationNumber
  {
    // Regular operations
    SEMIHOSTING_EnterSVC = 0x17,
    SEMIHOSTING_ReportException = 0x18,
    SEMIHOSTING_SYS_CLOSE = 0x02,
    SEMIHOSTING_SYS_CLOCK = 0x10,
    SEMIHOSTING_SYS_ELAPSED = 0x30,
    SEMIHOSTING_SYS_ERRNO = 0x13,
    SEMIHOSTING_SYS_FLEN = 0x0C,
    SEMIHOSTING_SYS_GET_CMDLINE = 0x15,
    SEMIHOSTING_SYS_HEAPINFO = 0x16,
    SEMIHOSTING_SYS_ISERROR = 0x08,
    SEMIHOSTING_SYS_ISTTY = 0x09,
    SEMIHOSTING_SYS_OPEN = 0x01,
    SEMIHOSTING_SYS_READ = 0x06,
    SEMIHOSTING_SYS_READC = 0x07,
    SEMIHOSTING_SYS_REMOVE = 0x0E,
    SEMIHOSTING_SYS_RENAME = 0x0F,
    SEMIHOSTING_SYS_SEEK = 0x0A,
    SEMIHOSTING_SYS_SYSTEM = 0x12,
    SEMIHOSTING_SYS_TICKFREQ = 0x31,
    SEMIHOSTING_SYS_TIME = 0x11,
    SEMIHOSTING_SYS_TMPNAM = 0x0D,
    SEMIHOSTING_SYS_WRITE = 0x05,
    SEMIHOSTING_SYS_WRITEC = 0x03,
    SEMIHOSTING_SYS_WRITE0 = 0x04,

    // Codes returned by SEMIHOSTING_ReportException
    ADP_Stopped_ApplicationExit = ((2 << 16) + 38),
    ADP_Stopped_RunTimeError = ((2 << 16) + 35),

  };

  // ----------------------------------------------------------------------------

#if !defined(__arm__)
// AArch64 (HLT #0xF000) and RISC-V (SLLI/EBREAK/SRAI) use different
// traps and 64-bit parameter blocks; to be added with their ports.
#error "Semihosting is not yet implemented for this architecture."
#endif

// SWI numbers and reason codes for RDI (Angel) monitors.
#define AngelSWI_ARM 0x123456
#ifdef __thumb__
#define AngelSWI 0xAB
#else
#define AngelSWI AngelSWI_ARM
#endif
// For thumb only architectures (Cortex-M) use the BKPT instruction
// instead of SWI.
#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')
#define AngelSWIInsn "bkpt"
#define AngelSWIAsm bkpt
#else
#define AngelSWIInsn "swi"
#define AngelSWIAsm swi
#endif

// The trap instruction is a property of the debugger, not of the CPU:
// for example OpenOCD `cortex_a` targets expect the SVC trap, while
// `aarch64` targets (also used for ARMv8 cores running AArch32 code)
// expect HLT. Define SEMIHOST_TRAP_HLT to select the HLT trap (A32
// HLT #0xF000, T32 HLT #0x3C). HLT is UNDEFINED on ARMv7-A, and does not
// exist on M-profile cores, which always use BKPT #0xAB.
#if defined(SEMIHOST_TRAP_HLT) && defined(__ARM_ARCH_PROFILE) \
    && (__ARM_ARCH_PROFILE == 'M')
#error "SEMIHOST_TRAP_HLT is not available on M-profile cores, which use BKPT."
#endif

#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)
// Testing the local semihosting handler cannot use another BKPT, since this
// configuration cannot trigger HaedFault exceptions while the debugger is
// connected, so we use an illegal op code, that will trigger an
// UsageFault exception.
#define AngelSWITestFault "setend be"
#define AngelSWITestFaultOpCode (0xB658)
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

  static inline int __attribute__ ((always_inline))
  call_host (int reason, void* arg)
  {
    int value;
    __asm__ volatile (

        " mov r0, %[rsn]  \n"
        " mov r1, %[arg]  \n"
#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)
        " " AngelSWITestFault " \n"
#elif defined(SEMIHOST_TRAP_HLT)
  // Use the encoding of the current instruction set; switching to
  // `.arm` here would leave the following instructions in A32.
#if defined(__thumb__)
      " .inst.n 0xBABC \n" // T32 HLT #0x3C
#else
      " .inst 0xE10F0070 \n" // A32 HLT #0xF000
#endif
#else
      " " AngelSWIInsn " %[swi] \n"
#endif
        " mov %[val], r0"

        : [val] "=r"(value) /* Outputs */
        : [rsn] "r"(reason), [arg] "r"(arg), [swi] "i"(AngelSWI) /* Inputs */
        : "r0", "r1", "r2", "r3", "ip", "lr", "memory", "cc"
        // Clobbers r0 and r1, and lr if in supervisor mode
    );

    // Accordingly to page 13-77 of ARM DUI 0040D other registers
    // can also be clobbered. Some memory positions may also be
    // changed by a system call, so they should not be kept in
    // registers. Note: we are assuming the manual is right and
    // Angel is respecting the APCS.
    return value;
  }

#pragma GCC diagnostic pop

  // --------------------------------------------------------------------------

  // Function used in _exit() to return the status code as Angel exception.
  static inline void __attribute__ ((always_inline, noreturn))
  report_exception (int reason)
  {
    call_host (SEMIHOSTING_ReportException, (void*)reason);

    for (;;)
      ;
  }

  // --------------------------------------------------------------------------

#if defined(__cplusplus)
}
#endif // defined(__cplusplus)

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_ARM_SEMIHOSTING_H_ */
