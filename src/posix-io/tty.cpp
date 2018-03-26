/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2017 Liviu Ionescu.
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

#include <cmsis-plus/posix-io/tty.h>

#include <cmsis-plus/diag/trace.h>

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
#if defined(OS_TRACE_POSIX_IO_TTY)
      trace::printf ("tty::%s(\"%s\")=@%p\n", __func__, name_, this);
#endif
    }

    tty::~tty ()
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
    tty::tcgetattr (struct termios *ptio)
    {
      return impl ().do_tcgetattr (ptio);
    }

    inline int
    tty::tcsetattr (int options, const struct termios *ptio)
    {
      return impl ().do_tcsetattr (options, ptio);
    }

    inline int
    tty::tcflush (int queue_selector)
    {
      return impl ().do_tcflush (queue_selector);
    }

    // ========================================================================

    tty_impl::tty_impl (tty& self) :
        char_device_impl
          { self }
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
