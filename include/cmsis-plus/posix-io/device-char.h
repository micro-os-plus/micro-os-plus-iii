/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#ifndef CMSIS_PLUS_POSIX_IO_DEVICE_CHAR_H_
#define CMSIS_PLUS_POSIX_IO_DEVICE_CHAR_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/device.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class device_char_impl;

    // ========================================================================

    /**
     * @brief Char device class.
     * @headerfile device-char.h <cmsis-plus/posix-io/device-char.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class device_char : public device
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_char (device_char_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_char (const device_char&) = delete;
      device_char (device_char&&) = delete;
      device_char&
      operator= (const device_char&) = delete;
      device_char&
      operator= (device_char&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~device_char ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      // Support functions.

      device_char_impl&
      impl (void) const;

      /**
       * @}
       */
    };

    // ========================================================================

    class device_char_impl : public device_impl
    {
      // ----------------------------------------------------------------------

      friend class device_block;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_char_impl (device_char& self);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_char_impl (const device_char_impl&) = delete;
      device_char_impl (device_char_impl&&) = delete;
      device_char_impl&
      operator= (const device_char_impl&) = delete;
      device_char_impl&
      operator= (device_char_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~device_char_impl ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      // Implementations

      virtual off_t
      do_lseek (off_t offset, int whence);

      virtual void
      do_sync (void);

      // ----------------------------------------------------------------------
      // Support functions.

      device_char&
      self (void);

      /**
       * @}
       */
    };

    // ========================================================================

    template<typename T>
      class device_char_implementable : public device_char
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
          device_char_implementable (const char* name, Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        device_char_implementable (const device_char_implementable&) = delete;
        device_char_implementable (device_char_implementable&&) = delete;
        device_char_implementable&
        operator= (const device_char_implementable&) = delete;
        device_char_implementable&
        operator= (device_char_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~device_char_implementable ();

        /**
         * @}
         */

        // ----------------------------------------------------------------------
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

        // ----------------------------------------------------------------------
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

    inline device_char_impl&
    device_char::impl (void) const
    {
      return static_cast<device_char_impl&> (impl_);
    }

    // ========================================================================

    inline device_char&
    device_char_impl::self (void)
    {
      return static_cast<device_char&> (self_);
    }

    // ========================================================================

    template<typename T>
      template<typename ... Args>
        device_char_implementable<T>::device_char_implementable (
            const char* name, Args&&... args) :
            device_char
              { impl_instance_, name }, //
            impl_instance_
              { *this, std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_DEVICE_CHAR)
          trace::printf ("device_char_implementable::%s(\"%s\")=@%p\n",
                         __func__, name_, this);
#endif
        }

    template<typename T>
      device_char_implementable<T>::~device_char_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_CHAR)
        trace::printf ("device_char_implementable::%s() @%p %s\n", __func__,
                       this, name_);
#endif
      }

    template<typename T>
      typename device_char_implementable<T>::value_type&
      device_char_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_CHAR_H_ */
