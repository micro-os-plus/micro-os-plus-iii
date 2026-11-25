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

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wempty-translation-unit"
#endif

// ----------------------------------------------------------------------------

// Cortex-M specific; Mainline (v7-M, v7E-M, v8-M Main, v8.1-M) is
// identified by `__ARM_ARCH_ISA_THUMB == 2`, Baseline (v6-M,
// v8-M Base) by `__ARM_ARCH_ISA_THUMB == 1`.
#if defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/port/os-c-decls.h>

#include <cmsis_device.h>
#include <cmsis-plus/arm/semihosting.h>
#include <cmsis-plus/diag/trace.h>
#include <cmsis-plus/cortexm/exception-handlers.h>

#include <string.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wredundant-decls"

extern void __attribute__ ((noreturn, weak))
_start (void);

#pragma GCC diagnostic pop

extern unsigned int _Heap_Limit;
extern unsigned int __stack;

typedef void (*handler_ptr_t) (void);

extern handler_ptr_t _interrupt_vectors[];

// ----------------------------------------------------------------------------

// Default exception handlers.
// Weak definitions, override them with similar
// handler routines defined in the application code.
//
// The ARCH_7M exception handlers are:
// 0x00 stack
// 0x04 Reset
// 0x08 NMI
// 0x0C HardFault
// 0x10 MemManage
// 0x14 BusFault
// 0x18 UsageFault
// 0x1C 0
// 0x20 0
// 0x24 0
// 0x28 0
// 0x2C SVC
// 0x30 DebugMon
// 0x34 0
// 0x38 PendSV
// 0x3C SysTick

// ----------------------------------------------------------------------------

// This function is not naked, and has a proper stack frame,
// to allow setting breakpoints at Reset_Handler.
void __attribute__ ((section (".after_vectors"), noreturn, weak))
Reset_Handler (void)
{
  // For just in case, when started via QEMU.
  __asm__ (" MSR msp, %0 " : : "r"(&__stack) :);

  // A hardware reset leaves all interrupts disabled, but a soft start
  // (a debugger loading and running without a reset, or a jump from a
  // bootloader) may leave SysTick or NVIC interrupts enabled. Their
  // handlers must not run before the DATA and BSS sections are
  // initialised; `_start()` enables the interrupts after that.
  __disable_irq ();

  // SCB
  // https://developer.arm.com/documentation/dui0552/a/cortex-m3-peripherals/system-control-block

  // SCB->VTOR
  // https://developer.arm.com/documentation/dui0552/a/cortex-m3-peripherals/system-control-block/vector-table-offset-register
  // Mandatory when running from RAM. Not available on Cortex-M0.
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  *((uint32_t*)0xE000ED08)
      = ((uint32_t)_interrupt_vectors & (uint32_t)(~0x3F));
#endif

#if defined(__ARM_FP)
  // Enable CP10 and CP11 coprocessor.
  // SCB->CPACR |= (0xF << 20);
  *((uint32_t*)0xE000ED88) |= (uint32_t)(0xF << 20);

  // Lazy save.
  // FPU->FPCCR |= FPU_FPCCR_ASPEN_Msk | FPU_FPCCR_LSPEN_Msk;
  *((uint32_t*)0xE000EF34) |= (uint32_t)(0x3 << 29);
#endif // defined(__ARM_FP)

  // Fill the main stack with a pattern, to detect usage and underflow.
  for (unsigned int* p = &_Heap_Limit; p < &__stack;)
    {
      *p++ = OS_INTEGER_RTOS_STACK_FILL_MAGIC; // DEADBEEF
    }

  _start ();
}

void __attribute__ ((section (".after_vectors"), weak))
NMI_Handler (void)
{
#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

// ----------------------------------------------------------------------------

#if defined(TRACE)

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)

// The values of BFAR and MMFAR remain unchanged if the BFARVALID or
// MMARVALID is set. However, if a new fault occurs during the
// execution of this fault handler, the value of the BFAR and MMFAR
// could potentially be erased. In order to ensure the fault addresses
// accessed are valid, the following procedure should be used:
// 1. Read BFAR/MMFAR.
// 2. Read CFSR to get BFARVALID or MMARVALID. If the value is 0, the
//    value of BFAR or MMFAR accessed can be invalid and can be discarded.
// 3. Optionally clear BFARVALID or MMARVALID.
// (See Joseph Yiu's book).

void
dump_exception_stack (exception_stack_frame_t* frame, uint32_t cfsr,
                      uint32_t mmfar, uint32_t bfar, uint32_t lr)
{
  trace_printf ("Stack frame:\n");
  trace_printf (" R0   = %08X\n", frame->r0);
  trace_printf (" R1   = %08X\n", frame->r1);
  trace_printf (" R2   = %08X\n", frame->r2);
  trace_printf (" R3   = %08X\n", frame->r3);
  trace_printf (" R12  = %08X\n", frame->r12);
  trace_printf (" LR   = %08X\n", frame->lr);
  trace_printf (" PC   = %08X\n", frame->pc);
  trace_printf (" PSR  = %08X\n", frame->psr);
  trace_printf ("FSR/FAR:\n");
  trace_printf (" CFSR = %08X\n", cfsr);
  trace_printf (" HFSR = %08X\n", SCB->HFSR);
  trace_printf (" DFSR = %08X\n", SCB->DFSR);
  trace_printf (" AFSR = %08X\n", SCB->AFSR);

#if (__ARM_ARCH >= 8)
  // UFSR.STKOF; the stack frame above may be invalid, since the
  // hardware does not write it below the stack limit.
  if (cfsr & (1UL << 20))
    {
      trace_printf (" STKOF: stack overflow (the frame may be invalid)\n");
    }
#endif

  if (cfsr & (1UL << 7))
    {
      trace_printf (" MMFAR= %08X\n", mmfar);
    }
  if (cfsr & (1UL << 15))
    {
      trace_printf (" BFAR = %08X\n", bfar);
    }
  trace_printf ("Misc\n");
  trace_printf (" LR/EXC_RETURN = %08X\n", lr);
}

#endif /* (__ARM_ARCH_ISA_THUMB == 2) */

// Baseline: v6-M, v8-M Base (M0, M0+, M1, M23).
#if (__ARM_ARCH_ISA_THUMB == 1)

void
dump_exception_stack (exception_stack_frame_t* frame, uint32_t lr)
{
  trace_printf ("Stack frame:\n");
  trace_printf (" R0  = %08X\n", frame->r0);
  trace_printf (" R1  = %08X\n", frame->r1);
  trace_printf (" R2  = %08X\n", frame->r2);
  trace_printf (" R3  = %08X\n", frame->r3);
  trace_printf (" R12 = %08X\n", frame->r12);
  trace_printf (" LR  = %08X\n", frame->lr);
  trace_printf (" PC  = %08X\n", frame->pc);
  trace_printf (" PSR = %08X\n", frame->psr);
  trace_printf ("Misc\n");
  trace_printf (" LR/EXC_RETURN = %08X\n", lr);
}

#endif /* (__ARM_ARCH_ISA_THUMB == 1) */

#endif /* defined(TRACE) */

// ----------------------------------------------------------------------------

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)

#if defined(OS_USE_SEMIHOSTING_SYSCALLS) \
    || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT) \
    || defined(OS_USE_TRACE_SEMIHOSTING_DEBUG)

int
is_semihosting (exception_stack_frame_t* frame, uint16_t opCode);

/**
 * This function provides the minimum functionality to make a semihosting
 * program execute even without the debugger present.
 * @param frame pointer to an exception stack frame.
 * @param opCode the 16-bin word of the BKPT instruction.
 * @return 1 if the instruction was a valid semihosting call; 0 otherwise.
 */
int
is_semihosting (exception_stack_frame_t* frame, uint16_t opCode)
{
  uint16_t* pw = (uint16_t*)frame->pc;
  if (*pw == opCode)
    {
      uint32_t r0 = frame->r0;
#if defined(OS_DEBUG_SEMIHOSTING_FAULTS) \
    || defined(OS_USE_SEMIHOSTING_SYSCALLS) \
    || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT)
      uint32_t r1 = frame->r1;
#endif
#if defined(OS_USE_SEMIHOSTING_SYSCALLS) \
    || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT)
      uint32_t* blk = (uint32_t*)r1;
#endif

#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)
      // trace_printf ("sh r0=%d\n", r0);
#endif

      switch (r0)
        {

#if defined(OS_USE_SEMIHOSTING_SYSCALLS)

        case SEMIHOSTING_SYS_CLOCK:
        case SEMIHOSTING_SYS_ELAPSED:
        case SEMIHOSTING_SYS_FLEN:
        case SEMIHOSTING_SYS_GET_CMDLINE:
        case SEMIHOSTING_SYS_REMOVE:
        case SEMIHOSTING_SYS_RENAME:
        case SEMIHOSTING_SYS_SEEK:
        case SEMIHOSTING_SYS_SYSTEM:
        case SEMIHOSTING_SYS_TICKFREQ:
        case SEMIHOSTING_SYS_TMPNAM:
        case SEMIHOSTING_SYS_ISTTY:
          // The call is not successful or not supported.
          frame->r0 = (uint32_t)-1;
          break;

        case SEMIHOSTING_SYS_CLOSE:
          // The call is successful.
          frame->r0 = 0;
          break;

        case SEMIHOSTING_SYS_ERRNO:
          // Should be the value of the C library errno variable.
          frame->r0 = 0;
          break;

        case SEMIHOSTING_SYS_HEAPINFO:
          blk[0] = 0; // heap_base
          blk[1] = 0; // heap_limit
          blk[2] = 0; // stack_base
          blk[3] = 0; // stack_limit
          break;

        case SEMIHOSTING_SYS_ISERROR:
          // 0 if the status word is not an error indication.
          frame->r0 = 0;
          break;

        case SEMIHOSTING_SYS_READ:
          // If R0 contains the same value as word 3, the call has
          // failed and EOF is assumed.
          frame->r0 = blk[2];
          break;

        case SEMIHOSTING_SYS_READC:
          // The byte read from the console.
          frame->r0 = '\0';
          break;

        case SEMIHOSTING_SYS_TIME:
          // The number of seconds since 00:00 January 1, 1970.
          frame->r0 = 0;
          break;

        case SEMIHOSTING_ReportException:

          NVIC_SystemReset ();
          // Should not reach here
          return 0;

#endif /* defined(OS_USE_SEMIHOSTING_SYSCALLS) */

#if defined(OS_USE_SEMIHOSTING_SYSCALLS) \
    || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT)

#define HANDLER_STDIN (1)
#define HANDLER_STDOUT (2)
#define HANDLER_STDERR (3)

        case SEMIHOSTING_SYS_OPEN:
          // Process only standard io/out/err and return 1/2/3
          if (strcmp ((char*)blk[0], ":tt") == 0)
            {
              if ((blk[1] == 0))
                {
                  frame->r0 = HANDLER_STDIN;
                  break;
                }
              else if (blk[1] == 4)
                {
                  frame->r0 = HANDLER_STDOUT;
                  break;
                }
              else if (blk[1] == 8)
                {
                  frame->r0 = HANDLER_STDERR;
                  break;
                }
            }
          // The call is not successful or not supported.
          frame->r0 = (uint32_t)-1;
          break;

        case SEMIHOSTING_SYS_WRITE:
          // Silently ignore writes to stdout/stderr, fail on all other
          // handler.
          if ((blk[0] == HANDLER_STDOUT) || (blk[0] == HANDLER_STDERR))
            {
#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)
              frame->r0
                  = (uint32_t)blk[2] - trace_write ((char*)blk[1], blk[2]);
#else
              frame->r0 = 0; // all sent, no more.
#endif /* defined(OS_DEBUG_SEMIHOSTING_FAULTS) */
            }
          else
            {
              // If other handler, return the total number of bytes
              // as the number of bytes that are not written.
              frame->r0 = blk[2];
            }
          break;

#endif /* defined(OS_USE_SEMIHOSTING_SYSCALLS) || \
          defined(OS_USE_TRACE_SEMIHOSTING_STDOUT) */

#if defined(OS_USE_SEMIHOSTING_SYSCALLS) \
    || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT) \
    || defined(OS_USE_TRACE_SEMIHOSTING_DEBUG)

        case SEMIHOSTING_SYS_WRITEC:
#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)
          {
            char ch = *((char*)r1);
            trace_write (&ch, 1);
          }
#endif
          // Register R0 is corrupted.
          break;

        case SEMIHOSTING_SYS_WRITE0:
#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)
          {
            char* p = ((char*)r1);
            trace_write (p, strlen (p));
          }
#endif
          // Register R0 is corrupted.
          break;

#endif /* semihosting */

        default:
          return 0;
        }

      // Alter the PC to make the exception returns to
      // the instruction after the faulty BKPT.
      frame->pc += 2;
      return 1;
    }
  return 0;
}

#endif

// Armv8-M and Armv8.1-M Mainline (M33, M55, M85): after a stack overflow
// (UFSR.STKOF, bit 20 of CFSR) SP is at MSPLIM, and the first push of the
// handler would fault again, locking up the processor. The handlers
// that report faults start by clearing MSPLIM in this case; they never
// return to the interrupted code, so the check is not needed later.
// Clobbers r3, which is saved in the exception stack frame.
#if (__ARM_ARCH >= 8)
#define OS_EXCEPTION_CLEAR_MSPLIM_ON_STKOF \
  " ldr r3,=0xE000ED28 \n" \
  " ldr r3,[r3]        \n" \
  " tst r3,#0x100000   \n" \
  " beq 1f             \n" \
  " movs r3,#0         \n" \
  " msr msplim,r3      \n" \
  "1:                  \n"
#else
#define OS_EXCEPTION_CLEAR_MSPLIM_ON_STKOF ""
#endif

// Hard Fault handler wrapper in assembly.
// Extract the location of the stack frame and pass it to the C
// handler as a pointer. Also pass the LR value as second
// parameter.
// (Based on Joseph Yiu's, The Definitive Guide to ARM Cortex-M3 and
// Cortex-M4 Processors, Third Edition, Chap. 12.8, page 402).

void __attribute__ ((section (".after_vectors"), weak, naked))
HardFault_Handler (void)
{
  __asm__ volatile (OS_EXCEPTION_CLEAR_MSPLIM_ON_STKOF
                    " tst lr,#4       \n"
                    " ite eq          \n"
                    " mrseq r0,msp    \n"
                    " mrsne r0,psp    \n"
                    " mov r1,lr       \n"
                    " ldr r2,=HardFault_Handler_C \n"
                    " bx r2"

                    : /* Outputs */
                    : /* Inputs */
                    : /* Clobbers */
  );
}

void __attribute__ ((section (".after_vectors"), weak, used))
HardFault_Handler_C (exception_stack_frame_t* frame __attribute__ ((unused)),
                     uint32_t lr __attribute__ ((unused)))
{
#if defined(TRACE)
  uint32_t mmfar = SCB->MMFAR; // MemManage Fault Address
  uint32_t bfar = SCB->BFAR; // Bus Fault Address
  uint32_t cfsr = SCB->CFSR; // Configurable Fault Status Registers
#endif /* defined(TRACE) */

#if defined(OS_USE_SEMIHOSTING_SYSCALLS) \
    || defined(OS_USE_TRACE_SEMIHOSTING_STDOUT) \
    || defined(OS_USE_TRACE_SEMIHOSTING_DEBUG)

  // If the BKPT instruction is executed with C_DEBUGEN == 0 and MON_EN == 0,
  // it will cause the processor to enter a HardFault exception, with DEBUGEVT
  // in the Hard Fault Status register (HFSR) set to 1, and BKPT in the
  // Debug Fault Status register (DFSR) also set to 1.

  if (((SCB->DFSR & SCB_DFSR_BKPT_Msk) != 0)
      && ((SCB->HFSR & SCB_HFSR_DEBUGEVT_Msk) != 0))
    {
      if (is_semihosting (frame, 0xBE00 + (AngelSWI & 0xFF)))
        {
          // Clear the exception cause in exception status.
          SCB->HFSR = SCB_HFSR_DEBUGEVT_Msk;

          // Continue after the BKPT
          return;
        }
    }

#endif /* semihosting */

#if defined(TRACE)
  trace_printf ("[HardFault]\n");
  dump_exception_stack (frame, cfsr, mmfar, bfar, lr);
#endif /* defined(TRACE) */

#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

#endif /* (__ARM_ARCH_ISA_THUMB == 2) */

// Baseline: v6-M, v8-M Base (M0, M0+, M1, M23).
#if (__ARM_ARCH_ISA_THUMB == 1)

// Hard Fault handler wrapper in assembly.
// It extracts the location of stack frame and passes it to handler
// in C as a pointer. We also pass the LR value as second
// parameter.
// (Based on Joseph Yiu's, The Definitive Guide to ARM Cortex-M0
// First Edition, Chap. 12.8, page 402).

void __attribute__ ((section (".after_vectors"), weak, naked))
HardFault_Handler (void)
{
  __asm__ volatile (" movs r0,#4      \n"
                    " mov r1,lr       \n"
                    " tst r0,r1       \n"
                    " beq 1f          \n"
                    " mrs r0,psp      \n"
                    " b   2f          \n"
                    "1:               \n"
                    " mrs r0,msp      \n"
                    "2:"
                    " mov r1,lr       \n"
                    " ldr r2,=HardFault_Handler_C \n"
                    " bx r2"

                    : /* Outputs */
                    : /* Inputs */
                    : /* Clobbers */
  );
}

void __attribute__ ((section (".after_vectors"), weak, used))
HardFault_Handler_C (exception_stack_frame_t* frame __attribute__ ((unused)),
                     uint32_t lr __attribute__ ((unused)))
{
  // There is no semihosting support for Cortex-M0, since on ARMv6-M
  // faults are fatal and it is not possible to return from the handler.

#if defined(TRACE)
  trace_printf ("[HardFault]\n");
  dump_exception_stack (frame, lr);
#endif /* defined(TRACE) */

#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

#endif /* (__ARM_ARCH_ISA_THUMB == 1) */

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)

void __attribute__ ((section (".after_vectors"), weak))
MemManage_Handler (void)
{
#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

void __attribute__ ((section (".after_vectors"), weak, naked))
BusFault_Handler (void)
{
  __asm__ volatile (" tst lr,#4       \n"
                    " ite eq          \n"
                    " mrseq r0,msp    \n"
                    " mrsne r0,psp    \n"
                    " mov r1,lr       \n"
                    " ldr r2,=BusFault_Handler_C \n"
                    " bx r2"

                    : /* Outputs */
                    : /* Inputs */
                    : /* Clobbers */
  );
}

void __attribute__ ((section (".after_vectors"), weak, used))
BusFault_Handler_C (exception_stack_frame_t* frame __attribute__ ((unused)),
                    uint32_t lr __attribute__ ((unused)))
{
#if defined(TRACE)
  uint32_t mmfar = SCB->MMFAR; // MemManage Fault Address
  uint32_t bfar = SCB->BFAR; // Bus Fault Address
  uint32_t cfsr = SCB->CFSR; // Configurable Fault Status Registers

  trace_printf ("[BusFault]\n");
  dump_exception_stack (frame, cfsr, mmfar, bfar, lr);
#endif /* defined(TRACE) */

#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

void __attribute__ ((section (".after_vectors"), weak, naked))
UsageFault_Handler (void)
{
  __asm__ volatile (OS_EXCEPTION_CLEAR_MSPLIM_ON_STKOF
                    " tst lr,#4       \n"
                    " ite eq          \n"
                    " mrseq r0,msp    \n"
                    " mrsne r0,psp    \n"
                    " mov r1,lr       \n"
                    " ldr r2,=UsageFault_Handler_C \n"
                    " bx r2"

                    : /* Outputs */
                    : /* Inputs */
                    : /* Clobbers */
  );
}

void __attribute__ ((section (".after_vectors"), weak, used))
UsageFault_Handler_C (exception_stack_frame_t* frame __attribute__ ((unused)),
                      uint32_t lr __attribute__ ((unused)))
{
#if defined(TRACE)
  uint32_t mmfar = SCB->MMFAR; // MemManage Fault Address
  uint32_t bfar = SCB->BFAR; // Bus Fault Address
  uint32_t cfsr = SCB->CFSR; // Configurable Fault Status Registers
#endif /* defined(TRACE) */

#if defined(OS_DEBUG_SEMIHOSTING_FAULTS)

  if ((cfsr & (1UL << 16)) != 0) // UNDEFINSTR
    {
      // For testing purposes, instead of BKPT use 'setend be'.
      if (is_semihosting (frame, AngelSWITestFaultOpCode))
        {
          return;
        }
    }

#endif /* defined(OS_DEBUG_SEMIHOSTING_FAULTS) */

#if defined(TRACE)
  trace_printf ("[UsageFault]\n");
  dump_exception_stack (frame, cfsr, mmfar, bfar, lr);
#endif /* defined(TRACE) */

#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

#endif

void __attribute__ ((section (".after_vectors"), weak))
SVC_Handler (void)
{
#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)

void __attribute__ ((section (".after_vectors"), weak))
DebugMon_Handler (void)
{
#if defined(DEBUG)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

#endif /* (__ARM_ARCH_ISA_THUMB == 2) */

void __attribute__ ((section (".after_vectors"), weak))
PendSV_Handler (void)
{
#if defined(DEBUG)
// Mainline: v7-M, v7E-M, v8-M Main, v8.1-M (M3, M4, M7, M33, M55, M85).
#if (__ARM_ARCH_ISA_THUMB == 2)
  if ((CoreDebug->DHCSR & CoreDebug_DHCSR_C_DEBUGEN_Msk) != 0)
    {
      __BKPT (0);
    }
#else
  __BKPT (0);
#endif /* (__ARM_ARCH_ISA_THUMB == 2) */
#endif /* defined(DEBUG) */

  while (true)
    {
      __NOP ();
    }
}

void __attribute__ ((section (".after_vectors"), weak))
SysTick_Handler (void)
{
  // DO NOT loop, just return.
  // Useful in case someone (like STM HAL) always enables SysTick.
}

// ----------------------------------------------------------------------------

#endif /* Cortex-M */
