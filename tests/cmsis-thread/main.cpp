//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------
//
// Semihosting STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// ----------------------------------------------------------------------------

void*
f1 (void* args)
{
  trace_printf ("f1(0x%p)\n", args);

  return nullptr;
}

// ----------------------------------------------------------------------------

int
main (int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

  using namespace os::rtos;

    {
      Thread t01
        { (thread::func_t) f1, (thread::func_args_t) nullptr };

      static char stack[2000];
      thread::Attributes attr
        { "t02" };
      attr.th_priority = thread::priority::high;
      attr.th_stack_size_bytes = sizeof(stack);
      attr.th_stack_address = stack;

      char c;
      Thread t02
        { attr, f1, &c };
    }

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
