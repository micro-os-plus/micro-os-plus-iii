/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include <cmsis-plus/rtos/os.h>

#if defined(__EXCEPTIONS)
#include <string>
#include <system_error>
#endif

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wc++98-compat-bind-to-temporary-copy"
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
        name () const noexcept override;

        virtual std::string
        message (int i) const override;

      };

    const char*
    system_error_category::name () const noexcept
      {
        return "system";
      }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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
        name () const noexcept override;

        virtual std::string
        message (int i) const override;
      };

    const char*
    cmsis_error_category::name () const noexcept
      {
        return "cmsis";
      }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunused-parameter"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif

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
      // error: copying parameter of type 'os::estd::system_error_category' when binding a reference to a temporary would invoke a deleted constructor in C++98 [-Werror,-Wc++98-compat-bind-to-temporary-copy]
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
      // error: copying parameter of type 'os::estd::cmsis_error_category' when binding a reference to a temporary would invoke a deleted constructor in C++98 [-Werror,-Wc++98-compat-bind-to-temporary-copy]
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
