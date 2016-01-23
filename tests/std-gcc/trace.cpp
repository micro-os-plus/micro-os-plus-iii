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

#if defined(TRACE)

#include "diag/trace.h"

#include <cstdarg>
#include <cstdio>
#include "string.h"

#ifndef OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE
#define OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE (200)
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace trace
  {
    // ------------------------------------------------------------------------

    void __attribute__((weak))
    initialize (void)
    {
      ;
    }

    ssize_t __attribute__((weak))
    write (const void* buf __attribute__((unused)), std::size_t nbyte)
    {
      return (ssize_t) nbyte;
    }

    // ------------------------------------------------------------------------

    int __attribute__((weak))
    printf (const char* format, ...)
    {
      std::va_list args;
      va_start(args, format);

      int ret = os::trace::vprintf (format, args);

      va_end(args);
      return ret;
    }

    int __attribute__((weak))
    vprintf (const char* format, std::va_list args)
    {
      // Caution: allocated on the stack!
      char buf[OS_INTEGER_TRACE_PRINTF_TMP_ARRAY_SIZE];

      // TODO: possibly rewrite it to no longer use newlib,
      // (although the nano version is no longer very heavy).

      // Print to the local buffer
      int ret = ::vsnprintf (buf, sizeof(buf), format, args);
      if (ret > 0)
        {
          // Transfer the buffer to the device.
          ret = (int)os::trace::write (buf, (size_t) ret);
        }
      return ret;
    }

    int __attribute__((weak))
    puts (const char* s)
    {
      int ret = (int)os::trace::write (s, strlen (s));
      if (ret > 0)
        {
          ret = (int)os::trace::write ("\n", 1); // Add a line terminator
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
      int ret = (int)os::trace::write ((const char*) &c, 1);
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
    dumpArgs (int argc, char* argv[])
    {
      os::trace::printf ("main(argc=%d, argv=[", argc);
      for (int i = 0; i < argc; ++i)
        {
          if (i != 0)
            {
              os::trace::printf (", ");
            }
          os::trace::printf ("\"%s\"", argv[i]);
        }
      os::trace::printf ("]);\n");
    }

  } /* namespace trace */
} /* namespace os */

// ----------------------------------------------------------------------------

// These two cannot be aliased, since they might be defined
// in a different translation units (and usually they are).

void __attribute__((weak))
trace_initialize (void)
{
  os::trace::initialize ();
}

ssize_t __attribute__((weak))
trace_write (const void* buf, std::size_t nbyte)
{
  return os::trace::write (buf, nbyte);
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
trace_vprintf (const char* format, ...);

int __attribute__((weak, alias("_ZN2os5trace4putsEPKc")))
trace_puts (const char *s);

int __attribute__((weak, alias("_ZN2os5trace7putcharEi")))
trace_putchar (int c);

void __attribute__((weak, alias("_ZN2os5trace8dumpArgsEiPPc")))
trace_dump_args (int argc, char* argv[]);

#else

// For non-embedded platforms, to remain compatible with OS X which does
// not support aliases, redefine the C functions to call the C++ versions.

int
trace_printf (const char* format, ...)
{
  std::va_list args;
  va_start(args, format);

  int ret = os::trace::vprintf (format, args);

  va_end(args);
  return ret;
}

int
trace_vprintf (const char* format, va_list args)
{
  return os::trace::vprintf (format, args);
}

int
trace_puts (const char* s)
{
  return os::trace::puts (s);
}

int
trace_putchar (int c)
{
  return os::trace::putchar (c);
}

void
trace_dump_args (int argc, char* argv[])
{
  os::trace::dumpArgs (argc, argv);
}

#endif

// ----------------------------------------------------------------------------

#endif // defined(TRACE)
