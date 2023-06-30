/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2017-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/tty.h>

#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    tty::tty (tty_impl& impl, const char* name) :
        char_device
          { impl, name }
    {
      type_ |= static_cast<type_t>(type::tty);
#if defined(OS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty::%s(\"%s\")=@%p\n", __func__, name_, this);
#endif
    }

    tty::~tty () noexcept
    {
#if defined(OS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty::%s() @%p %s\n", __func__, this, name_);
#endif
    }

    // ------------------------------------------------------------------------

    inline int
    tty::tcsendbreak (int duration)
    {
      return impl ().do_tcsendbreak (duration);
    }

    inline int
    tty::tcgetattr (/* struct */ termios *ptio)
    {
      return impl ().do_tcgetattr (ptio);
    }

    inline int
    tty::tcsetattr (int options, const /* struct */ termios *ptio)
    {
      return impl ().do_tcsetattr (options, ptio);
    }

    inline int
    tty::tcflush (int queue_selector)
    {
      return impl ().do_tcflush (queue_selector);
    }

    inline int
    tty::tcdrain (void)
    {
      return impl ().do_tcdrain ();
    }

    // ========================================================================

    tty_impl::tty_impl (void)
    {
#if defined(OS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty_impl::%s()=@%p\n", __func__, this);
#endif
    }

    tty_impl::~tty_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty_impl::%s() @%p\n", __func__, this);
#endif
    }

    int
    tty_impl::do_isatty (void)
    {
      return 1; // Yes!
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
