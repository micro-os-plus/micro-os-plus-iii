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

void
f0 (void)
{
  trace_printf ("f0()\n");
}

void
f1 (const void* args)
{
  trace_printf ("f1(0x%p)\n", args);
}

void
f2 (void* args)
{
  trace_printf ("f2(0x%p)\n", args);
}

// Templates only
void
f3 (int n)
{
  trace_printf ("f3(%d)\n", n);
}

void
f4 (int n, const char* s)
{
  trace_printf ("f4(%d, %s)\n", n, s);
}

class Func1
{
public:
  Func1 (int n) :
      n_
        { n }
  {
  }

  void
  operator() (const char* s)
  {
    trace_printf ("Func1(%d, %s)\n", n_, s);
  }

  int n_;
};

// ----------------------------------------------------------------------------

int
main (int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

  using namespace os::cmsis::rtos;

  char* ch1;
  char* ch2;

  Thread t01
    { "t01", nullptr, 0, Priority::normal, (Thread_func_vp)f0 };
  t01.__run_function ();

  Thread t12
    { "t12", nullptr, 0, Priority::normal, (Thread_func_vp)f1, nullptr };
  t12.__run_function ();

  Thread t13
    { "t13", nullptr, 0, Priority::normal, (Thread_func_vp)f1, &ch1 };
  t13.__run_function ();

  Thread t15
    { "t15", 0, Priority::normal, f1, nullptr };
  t15.__run_function ();

  Thread t16
    { "t16", 0, Priority::normal, f1, &ch2 };
  t16.__run_function ();

  Thread t21
    { "t21", nullptr, 0, Priority::normal, f2, nullptr };
  t21.__run_function ();

  Thread t22
    { "t22", nullptr, 0, Priority::normal, f2, &ch1 };
  t22.__run_function ();

  Thread t23
    { "t23", 0, Priority::normal, f2, nullptr };
  t23.__run_function ();

  Thread t24
    { "t24", 0, Priority::normal, f2, &ch2 };
  t24.__run_function ();

  Thread t31
    { "t31", 0, Priority::normal, f3, 7 };
  t31.__run_function ();

  Thread t32
    { "t32", 0, Priority::normal, f3, 8 };
  t32.__run_function ();

  int n = 9;
  Thread t33
    { "t33", 0, Priority::normal, f3, n };
  t33.__run_function ();

  Thread t41
    { "t41", 0, Priority::normal, f4, 7, "abc" };
  t41.__run_function ();

  Thread t42
    { "t42", 0, Priority::normal, f4, 8, "cde" };
  t42.__run_function ();

  char* str = (char*) "fgh";
  Thread t43
    { "t43", 0, Priority::normal, f4, n, str };
  t43.__run_function ();

  auto l5 = [](void)
    { trace_printf ("l5()\n");};

  Thread t51
    { "t51", 0, Priority::normal, l5 };
  t51.__run_function ();

  auto l6 = [](const void* arg)
    { trace_printf ("l6(%p)\n", arg);};

  Thread t61
    { "t61", 0, Priority::normal, l6, nullptr };
  t61.__run_function ();

  Thread t62
    { "t62", 0, Priority::normal, l6, &ch2 };
  t62.__run_function ();

  // Capture by value
  int ln = 77;
  auto l7 = [ln](void* arg)
    { trace_printf ("l7(%p) %d\n", arg, ln);};

  Thread t71
    { "t71", 0, Priority::normal, l7, nullptr };
  t71.__run_function ();

  ln = 78; // Should print 77
  Thread t72
    { "t72", 0, Priority::normal, l7, &ch2 };
  t72.__run_function ();

  // Capture by ref
  auto l8 = [&ln](void* arg)
    { trace_printf ("l8(%p) %d\n", arg, ln);};

  Thread t81
    { "t81", 0, Priority::normal, l8, nullptr };
  t81.__run_function ();

  ln = 79; // Should print 79
  Thread t82
    { "t82", 0, Priority::normal, l8, &ch2 };
  t82.__run_function ();

  Func1 fn1
    { 55 };
  Thread t91
    { "t91", 0, Priority::normal, fn1, "ert" };
  t91.__run_function ();

  Thread t92
    { "t92", 0, Priority::normal, fn1, "asd" };
  t92.__run_function ();

  return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
