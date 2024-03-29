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

#ifndef CMSIS_PLUS_DIAG_TRACE_H_
#define CMSIS_PLUS_DIAG_TRACE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)
#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <cstdlib>
#else
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#endif

#include <sys/types.h>

#if defined(__cplusplus)

// To be effective, <stdio.h> must be included *before* this patch.
#include <stdio.h>

// This is a very annoying issue, some very old libraries still
// define putchar() as a macro. This is abusive, since it prevents
// the use of putchar() in other name spaces.
#if defined(putchar)
#undef putchar
#endif

#endif

// ----------------------------------------------------------------------------

/**
 * @brief Insert a BKPT0 for debugger usage.
 */
inline void
__attribute__((always_inline))
trace_dbg_bkpt (void)
{
#if defined(__ARM_EABI__)
  __asm__ volatile ("bkpt 0");
#endif
}

#if defined(TRACE)

#if defined(__cplusplus)

namespace os
{
  /**
   * @brief Tracing support namespace.
   * @ingroup cmsis-plus-diag
   * @details
   * The trace device is an independent output channel, intended
   * for diagnostic purposes.
   *
   * The API is simple, and mimics the standard C output calls:
   * - `os::trace::printf()` / `trace_printf()`
   * - `os::trace::puts()` / `trace_puts()`
   * - `os::trace::putchar()` / `trace_putchar()`
   *
   * The implementation is done in:
   * - os::trace::initialize()
   * - os::trace::write()
   *
   * If these functions are not defined in another place, there are
   * weak definitions that simply discard the trace output.
   *
   * Trace support is enabled by adding the `TRACE` macro definition.
   *
   * When `TRACE` is not defined, all functions are inlined to empty bodies.
   * This has the advantage that the trace calls do not need to be
   * conditionally compiled with
   * <tt> \#ifdef TRACE </tt> / <tt> \#endif </tt>
   */
  namespace trace
  {
    // ----------------------------------------------------------------------

    void
    initialize (void);

    ssize_t
    write (const void* buf, std::size_t nbyte);

    // ----------------------------------------------------------------------

    /**
     * @brief Write a formatted string to the trace device.
     * @param [in] format A null terminate string with the format.
     * @return A nonnegative number for success.
     *
     * @ingroup cmsis-plus-diag
     */
    int
    printf (const char* format, ...);

    /**
     * @brief Write a formatted variable arguments list to the trace device.
     * @param [in] format A null terminate string with the format.
     * @param [in] args A variable arguments list.
     * @return A nonnegative number for success.
     *
     * @ingroup cmsis-plus-diag
     */
    int
    vprintf (const char* format, std::va_list args);

    /**
     * @brief Write the string and a line terminator to the trace device.
     * @param s A null terminated string.
     * @return A nonnegative number for success.
     *
     * @ingroup cmsis-plus-diag
     */
    int
    puts (const char* s);

    /**
     * @brief Write the single character to the trace device.
     * @param c A single byte character.
     * @return The written character.
     *
     * @ingroup cmsis-plus-diag
     */
    int
    putchar (int c);

    /**
     * @brief Write the argv[] array to the trace device.
     * @param argc The number of argv[] strings.
     * @param argv An array of pointer to args.
     *
     * @ingroup cmsis-plus-diag
     */
    void
    dump_args (int argc, char* argv[]);

    /**
     * @brief Flush the output.
     * @par Parameters
     *  None.
     * @par Returns
     *  Nothing.
     */
    void
    flush(void);

    // ------------------------------------------------------------------------

    /**
     * @brief Insert a BKPT0 for debugger usage.
     */
    inline void
    __attribute__((always_inline))
    dbg_bkpt (void)
    {
      trace_dbg_bkpt ();
    }

  } /* namespace trace */
} /* namespace os */

#endif /* defined(__cplusplus) */

#if defined(__cplusplus)
extern "C"
{
#endif

  // ----- Implementation -----
  // These functions must be implemented for a specific trace channel.

  /**
   * Called from startup.
   */
  void
  trace_initialize (void);

  ssize_t
  trace_write (const void* buf, size_t nbyte);

  void
  trace_flush (void);

  // ----- Portable -----

  int
  trace_printf (const char* format, ...);

  int
  trace_vprintf (const char* format, va_list args);

  int
  trace_puts (const char* s);

  int
  trace_putchar (int c);

  void
  trace_dump_args (int argc, char* argv[]);

#if defined(__cplusplus)
}
#endif

#else /* !defined(TRACE) */

// Empty definitions when trace is not defined

#if defined(__cplusplus)

namespace os
  {
    namespace trace
      {
        // ----------------------------------------------------------------

        inline void
        initialize (void);

        inline ssize_t
        write (const void* buf, std::size_t nbyte);

        // ----------------------------------------------------------------

        inline int
        printf (const char* format, ...);

        inline int
        vprintf (const char* format, std::va_list args);

        inline int
        puts (const char* s);

        inline int
        putchar (int c);

        inline void
        dump_args (int argc, char* argv[]);

        // ----------------------------------------------------------------

        inline void __attribute__((always_inline))
        initialize (void)
          {
          }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

        inline ssize_t __attribute__((always_inline))
        write (const void* buf, std::size_t nbyte)
          {
            return static_cast<ssize_t> (nbyte);
          }

        inline void __attribute__((always_inline))
        flush (void)
          {
          }

        inline int __attribute__((always_inline))
        printf (const char* format, ...)
          {
            return 0;
          }

        inline int __attribute__((always_inline))
        vprintf (const char* format, std::va_list args)
          {
            return 0;
          }

        inline int __attribute__((always_inline))
        puts (const char* s)
          {
            return 0;
          }

        inline int __attribute__((always_inline))
        putchar (int c)
          {
            return c;
          }

        inline void __attribute__((always_inline))
        dump_args (int argc, char* argv[])
          {
          }

#pragma GCC diagnostic pop

      } /* namespace trace */
  } /* namespace os */

#endif /* defined(__cplusplus) */

#if defined(__cplusplus)
extern "C"
  {
#endif

    inline void
    trace_initialize (void);

    // Implementation dependent
    inline ssize_t
    trace_write (const void* buf, size_t nbyte);

    inline void
    trace_flush (void);

    inline int
    trace_printf (const char* format, ...);

    inline int
    trace_vprintf (const char* format, va_list args);

    inline int
    trace_puts (const char* s);

    inline int
    trace_putchar (int c);

    inline void
    trace_dump_args (int argc, char* argv[]);

#if defined(__cplusplus)
  }
#endif

inline void
__attribute__((always_inline))
trace_initialize (void)
  {
  }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#if defined(__cplusplus)
#pragma GCC diagnostic ignored "-Wold-style-cast"
#endif
#endif

inline ssize_t
__attribute__((always_inline))
trace_write (const void* buf, size_t nbyte)
  {
    return (ssize_t) (nbyte);
  }

inline void
__attribute__((always_inline))
trace_flush (void)
  {
  }

#pragma GCC diagnostic pop

inline int
__attribute__((always_inline))
trace_printf (const char* format, ...)
  {
    return 0;
  }

inline int
__attribute__((always_inline))
trace_vprintf (const char* format, va_list args)
  {
    return 0;
  }

inline int
__attribute__((always_inline))
trace_puts (const char* s)
  {
    return 0;
  }

inline int
__attribute__((always_inline))
trace_putchar (int c)
  {
    return c;
  }

inline void
__attribute__((always_inline))
trace_dump_args (int argc, char* argv[])
  {
  }

#pragma GCC diagnostic pop

#endif /* defined(TRACE) */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DIAG_TRACE_H_ */
