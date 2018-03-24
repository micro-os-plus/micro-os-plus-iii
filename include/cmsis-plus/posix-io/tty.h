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

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/posix-io/device-char.h>
#include <cmsis-plus/posix/termios.h>

#if defined (__cplusplus)

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class tty_impl;

    // ========================================================================

    class tty : public device_char
    {
    public:

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      tty (tty_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      tty (const tty&) = delete;
      tty (tty&&) = delete;
      tty&
      operator= (const tty&) = delete;
      tty&
      operator= (tty&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~tty () noexcept;

      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual int
      tcgetattr (struct termios *ptio);

      virtual int
      tcsetattr (int options, const struct termios *ptio);

      virtual int
      tcflush (int queue_selector);

      virtual int
      tcsendbreak (int duration);

      // ----------------------------------------------------------------------

      tty_impl&
      impl (void) const;
    };

    // ========================================================================

    class tty_impl : public device_char_impl
    {
      // ----------------------------------------------------------------------

      friend class device_block;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      tty_impl (tty& self);

      /**
       * @cond ignore
       */

      // The rule of five.
      tty_impl (const tty_impl&) = delete;
      tty_impl (tty_impl&&) = delete;
      tty_impl&
      operator= (const tty_impl&) = delete;
      tty_impl&
      operator= (tty_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~tty_impl ();

      /**
       * @}
       */

      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual int
      do_tcgetattr (struct termios *ptio) = 0;

      virtual int
      do_tcsetattr (int options, const struct termios *ptio) = 0;

      virtual int
      do_tcflush (int queue_selector) = 0;

      virtual int
      do_tcsendbreak (int duration) = 0;

      virtual int
      do_isatty (void) final;

      // ----------------------------------------------------------------------

      tty&
      self (void);

      /**
       * @}
       */
    };

    // ========================================================================

    template<typename T>
      class tty_implementable : public device_char
      {
        // --------------------------------------------------------------------

      public:

        using value_type = T;

        // --------------------------------------------------------------------
        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        tty_implementable (const char* name);

        /**
         * @cond ignore
         */

        // The rule of five.
        tty_implementable (const tty_implementable&) = delete;
        tty_implementable (tty_implementable&&) = delete;
        tty_implementable&
        operator= (const tty_implementable&) = delete;
        tty_implementable&
        operator= (tty_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~tty_implementable ();

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        value_type impl_instance_;

        /**
         * @endcond
         */
      };

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ========================================================================

    inline tty_impl&
    tty::impl (void) const
    {
      return static_cast<tty_impl&> (impl_);
    }

    // ========================================================================

    inline tty&
    tty_impl::self (void)
    {
      return static_cast<tty&> (self_);
    }

    // ========================================================================

    template<typename T>
      tty_implementable<T>::tty_implementable (const char* name) :
          device_char
            { impl_instance_, name }, //
          impl_instance_
            { *this }
      {
#if defined(OS_TRACE_POSIX_IO_TTY)
        trace::printf ("tty_implementable::%s(\"%s\")=@%p\n", __func__, name_,
                       this);
#endif
      }

    template<typename T>
      tty_implementable<T>::~tty_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_TTY)
        trace::printf ("tty_implementable::%s() @%p %s\n", __func__, this,
                       name_);
#endif
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_TTY_H_ */
