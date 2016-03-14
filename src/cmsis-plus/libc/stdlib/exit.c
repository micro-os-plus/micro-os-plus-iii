/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#if defined(__ARM_EABI__)

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
exit(int code)
{
  // Call the cleanup functions enrolled with atexit().
  __call_exitprocs (code, NULL);

  // Run the C++ static destructors.
  __run_fini_array ();

  _Exit (code);

  // Present here in case _exit() was reimplemented poorly and returns.
#if defined(DEBUG)
  trace_dbg_bkpt();
#endif
  for (;;)
    ;
  /* NOTREACHED */
}

#if !defined(OS_USE_SEMIHOSTING)

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
  trace_puts ("%s()", __func__);
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

#endif /* !defined(OS_USE_SEMIHOSTING) */

#endif /* defined(__ARM_EABI__) */
