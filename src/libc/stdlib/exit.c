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

#include <cmsis-plus/diag/trace.h>

#include <stdlib.h>
#include "atexit.h"

// ----------------------------------------------------------------------------

/**
 * @details
 * `exit()` does two kinds of cleanup before ending execution of your
 * program.
 *
 * First, it calls all application-defined cleanup functions
 * you have enrolled with atexit().
 *
 * Second, files and streams are
 * cleaned up: any pending output is delivered to the host system, each
 * open file or stream is closed, and files created by <<tmpfile>> are
 * deleted (wishful thinking, not implemented).
 */
void
__attribute__ ((noreturn))
exit(int status)
{
  trace_printf("%s(%d)\n", __func__, status);

  // Call the cleanup functions enrolled with atexit().
  __call_exitprocs (status, NULL);

  // Run the C++ static destructors.
  os_run_fini_array ();

  _Exit (status);

  // Present here in case _exit() was reimplemented poorly and returns.
#if defined(DEBUG)
  trace_dbg_bkpt();
#endif
  while (1)
    ;
  /* NOTREACHED */
}

#if !defined(OS_USE_SEMIHOSTING_SYSCALLS)

#if !defined(DEBUG)
extern void __attribute__((noreturn))
__reset_hardware(void);
#endif

// ----------------------------------------------------------------------------

// On Release, call the hardware reset procedure.
// On Debug, use a breakpoint to notify the debugger.
//
// It can be redefined by the application, if more functionality
// is required. For example, when semihosting is used, this
// function sends the return code to the host.

void __attribute__((weak, noreturn))
_Exit (int code __attribute__((unused)))
{
  trace_printf ("%s()", __func__);

  trace_flush();

#if !defined(DEBUG)
  __reset_hardware();
#else
  trace_dbg_bkpt();
#endif

  // For just in case.
  while (1)
    ;
  /* NOTREACHED */
}

void __attribute__((weak, alias ("_Exit")))
_exit (int status);

#endif /* !defined(OS_USE_SEMIHOSTING_SYSCALLS) */

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
