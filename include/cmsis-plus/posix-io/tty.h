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

#ifndef CMSIS_PLUS_POSIX_IO_TTY_H_
#define CMSIS_PLUS_POSIX_IO_TTY_H_

// ----------------------------------------------------------------------------

#if defined (__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/posix-io/char-device.h>
#include <cmsis-plus/posix/termios.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class tty_impl;

    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#endif
    class tty : public char_device
    {
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
      ~tty () noexcept override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcgetattr.html
      virtual int
      tcgetattr (/* struct */termios *ptio);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcsetattr.html
      virtual int
      tcsetattr (int options, const /* struct */ termios *ptio);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcflush.html
      virtual int
      tcflush (int queue_selector);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcsendbreak.html
      virtual int
      tcsendbreak (int duration);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcdrain.html
      virtual int
      tcdrain (void);

      // ----------------------------------------------------------------------
      // Support functions.

      tty_impl&
      impl (void) const;
    };
#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

    class tty_impl : public char_device_impl
    {
      // ----------------------------------------------------------------------

      friend class tty;

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      tty_impl (void);

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
      ~tty_impl () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual int
      do_tcgetattr (/* struct */ termios *ptio) = 0;

      virtual int
      do_tcsetattr (int options, const /* struct */ termios *ptio) = 0;

      virtual int
      do_tcflush (int queue_selector) = 0;

      virtual int
      do_tcsendbreak (int duration) = 0;

      virtual int
      do_tcdrain (void) = 0;

      virtual int
      do_isatty (void) final;

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    template<typename T>
      class tty_implementable : public tty
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

        template<typename ... Args>
          tty_implementable (const char* name, Args&&... args);

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

        // --------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        // Support functions.

        value_type&
        impl (void) const;

        /**
         * @}
         */

        // --------------------------------------------------------------------
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

    template<typename T>
      template<typename ... Args>
        tty_implementable<T>::tty_implementable (const char* name,
                                                 Args&&... args) :
            tty
              { impl_instance_, name }, //
            impl_instance_
              { std::forward<Args>(args)... }
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

    template<typename T>
      typename tty_implementable<T>::value_type&
      tty_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_IO_TTY_H_ */
