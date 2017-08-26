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

#ifndef CMSIS_PLUS_POSIX_IO_TTY_H_
#define CMSIS_PLUS_POSIX_IO_TTY_H_

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/posix-io/device-char.h>
#include <cmsis-plus/posix/termios.h>

#if defined (__cplusplus)

namespace os
{
  namespace posix
  {

    class tty : public os::posix::device_char
    {
    public:

      // --------------------------------------------------------------------

      tty (const char* name);

      tty (const tty&) = delete;

      tty (tty&&) = delete;

      tty&
      operator= (const tty&) = delete;

      tty&
      operator= (tty&&) = delete;

      virtual
      ~tty () noexcept;

      int
      tcgetattr (struct termios *ptio);

      int
      tcsetattr (int options, const struct termios *ptio);

      int
      tcflush (int queue_selector);

      int
      tcsendbreak (int duration);

      // --------------------------------------------------------------------

    protected:

      virtual int
      do_tcgetattr (struct termios *ptio) = 0;

      virtual int
      do_tcsetattr (int options, const struct termios *ptio) = 0;

      virtual int
      do_tcflush (int queue_selector) = 0;

      virtual int
      do_tcsendbreak (int duration) = 0;

      // --------------------------------------------------------------------

    private:

    };

  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {

    inline int
    tty::tcsendbreak (int duration)
    {
      return do_tcsendbreak (duration);
    }

    inline int
    tty::tcgetattr (struct termios *ptio)
    {
      return do_tcgetattr (ptio);
    }

    inline int
    tty::tcsetattr (int options, const struct termios *ptio)
    {
      return do_tcsetattr (options, ptio);
    }

    inline int
    tty::tcflush (int queue_selector)
    {
      return do_tcflush (queue_selector);
    }

  } /* namespace posix */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_TTY_H_ */
