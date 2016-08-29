/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

#include <cmsis-plus/rtos/os.h>

#if defined(__EXCEPTIONS)
#include <string>
#include <system_error>
#endif

// ----------------------------------------------------------------------------

using namespace os;

// ----------------------------------------------------------------------------

namespace os
{
  namespace estd
  {
    // ========================================================================

#if defined(__EXCEPTIONS)

    struct system_error_category : public std::error_category
    {
      virtual const char*
      name () const noexcept;

      virtual std::string
      message (int i) const;

    };

    const char*
    system_error_category::name () const noexcept
    {
      return "system";
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    std::string
    system_error_category::message (int i) const
    {
#if defined(DEBUG)
      return std::string (strerror (i));
#else
      return std::string ("");
#endif
    }

    struct cmsis_error_category : public std::error_category
    {
      virtual const char*
      name () const noexcept;

      virtual std::string
      message (int i) const;
    };

    const char*
    cmsis_error_category::name () const noexcept
    {
      return "cmsis";
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    std::string
    cmsis_error_category::message (int i) const
    {
#if defined(DEBUG)
      return std::string (strerror (i));
#else
      return std::string ("");
#endif
    }

#pragma GCC diagnostic pop

#endif

    void
    __throw_system_error (int ev, const char* what_arg)
    {
#if defined(__EXCEPTIONS)
      throw std::system_error (std::error_code (ev, system_error_category ()),
                               what_arg);
#else
      trace_printf ("system_error(%d, %s)\n", ev, what_arg);
      std::abort ();
#endif
    }

    void
    __throw_cmsis_error (int ev, const char* what_arg)
    {
#if defined(__EXCEPTIONS)
      throw std::system_error (std::error_code (ev, cmsis_error_category ()),
                               what_arg);
#else
      trace_printf ("system_error(%d, %s)\n", ev, what_arg);
      std::abort ();
#endif
    }

  // --------------------------------------------------------------------------

  } /* namespace estd */
} /* namespace os */

// ----------------------------------------------------------------------------
