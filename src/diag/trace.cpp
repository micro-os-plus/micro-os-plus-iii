/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(TRACE)

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif
#include <cmsis-plus/diag/trace.h>

#include <cstdarg>
#include <cstdio>
#include <cstring>

#ifndef OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE
#define OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE (200)
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace trace
  {
    // ----------------------------------------------------------------------

    void __attribute__((weak))
    initialize (void)
    {
    }

    /**
     * @brief Write the given number of bytes to the trace output channel.
     * @return  The number of characters actually written, or -1 if error.
     */
    ssize_t __attribute__((weak))
    write (const void* buf __attribute__((unused)), std::size_t nbyte)
    {
      return static_cast<ssize_t> (nbyte);
    }

    void __attribute__((weak))
    flush (void)
    {
    }

    // ----------------------------------------------------------------------

    int __attribute__((weak))
    printf (const char* format, ...)
    {
      std::va_list args;
      va_start(args, format);

      int ret = vprintf (format, args);

      va_end(args);
      return ret;
    }

#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wuseless-cast"
#endif

    int __attribute__((weak))
    vprintf (const char* format, std::va_list args)
    {
      // Caution: allocated on the stack!
      char buf[OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE];

      // TODO: possibly rewrite it to no longer use newlib,
      // (although the nano version is no longer very heavy).

      // Print to the local buffer
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wformat-nonliteral"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
#endif
      int ret = ::vsnprintf (buf, sizeof(buf), format, args);
#pragma GCC diagnostic pop
      if (ret > 0)
        {
          // Transfer the buffer to the device.
          ret = static_cast<int> (write (buf, static_cast<size_t> (ret)));
        }
      return ret;
    }

    int __attribute__((weak))
    puts (const char* s)
    {
      int ret = static_cast<int> (write (s, strlen (s)));
      if (ret >= 0)
        {
          ret = static_cast<int> (write ("\n", 1)); // Add a line terminator
        }
      if (ret > 0)
        {
          return ret;
        }
      else
        {
          return EOF;
        }
    }

    int __attribute__((weak))
    putchar (int c)
    {
      int ret = static_cast<int> (write (reinterpret_cast<const char*> (&c), 1));
      if (ret > 0)
        {
          return c;
        }
      else
        {
          return EOF;
        }
    }

    void __attribute__((weak))
    dump_args (int argc, char* argv[])
    {
      printf ("main(argc=%d, argv=[", argc);
      for (int i = 0; i < argc; ++i)
        {
          if (i != 0)
            {
              printf (", ");
            }
          printf ("\"%s\"", argv[i]);
        }
      printf ("]);\n");
    }

  } /* namespace trace */
} /* namespace os */

// ----------------------------------------------------------------------------

using namespace os;

// These cannot be aliased, since they might be defined
// in a different translation units (and usually they are).

void __attribute__((weak))
trace_initialize (void)
{
  trace::initialize ();
}

ssize_t __attribute__((weak))
trace_write (const void* buf, std::size_t nbyte)
{
  return trace::write (buf, nbyte);
}

void __attribute__((weak))
trace_flush (void)
{
  return trace::flush ();
}

// ----------------------------------------------------------------------------

#if defined(__ARM_EABI__)

// For embedded platforms, optimise with aliases.
//
// Aliases can only refer symbols defined in the same translation unit
// and C++ de-mangling must be done manually.

int __attribute__((weak, alias ("_ZN2os5trace6printfEPKcz")))
trace_printf (const char* format, ...);

int __attribute__((weak, alias ("_ZN2os5trace7vprintfEPKcSt9__va_list")))
trace_vprintf (const char* format, va_list args);

int __attribute__((weak, alias("_ZN2os5trace4putsEPKc")))
trace_puts (const char *s);

int __attribute__((weak, alias("_ZN2os5trace7putcharEi")))
trace_putchar (int c);

void __attribute__((weak, alias("_ZN2os5trace9dump_argsEiPPc")))
trace_dump_args (int argc, char* argv[]);

#else

// For non-embedded platforms, to remain compatible with OS X which does
// not support aliases, redefine the C functions to call the C++ versions.

int
trace_printf (const char* format, ...)
{
  std::va_list args;
  va_start(args, format);

  int ret = trace::vprintf (format, args);

  va_end(args);
  return ret;
}

int
trace_vprintf (const char* format, va_list args)
{
  return trace::vprintf (format, args);
}

int
trace_puts (const char* s)
{
  return trace::puts (s);
}

int
trace_putchar (int c)
{
  return trace::putchar (c);
}

void
trace_dump_args (int argc, char* argv[])
{
  trace::dump_args (argc, argv);
}

#endif

// ----------------------------------------------------------------------------

#endif // defined(TRACE)
