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

    class char_device_impl;

    // ========================================================================

    /**
     * @brief Char device class.
     * @headerfile device-char.h <cmsis-plus/posix-io/device-char.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class char_device : public device
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      char_device (char_device_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      char_device (const char_device&) = delete;
      char_device (char_device&&) = delete;
      char_device&
      operator= (const char_device&) = delete;
      char_device&
      operator= (char_device&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~char_device ();

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

      char_device_impl&
      impl (void) const;

      /**
       * @}
       */
    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class char_device_impl : public device_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      char_device_impl (char_device& self);

      /**
       * @cond ignore
       */

      // The rule of five.
      char_device_impl (const char_device_impl&) = delete;
      char_device_impl (char_device_impl&&) = delete;
      char_device_impl&
      operator= (const char_device_impl&) = delete;
      char_device_impl&
      operator= (char_device_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~char_device_impl ();

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

      char_device&
      self (void);

      /**
       * @}
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    template<typename T>
      class char_device_implementable : public char_device
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
          char_device_implementable (const char* name, Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        char_device_implementable (const char_device_implementable&) = delete;
        char_device_implementable (char_device_implementable&&) = delete;
        char_device_implementable&
        operator= (const char_device_implementable&) = delete;
        char_device_implementable&
        operator= (char_device_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~char_device_implementable ();

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

    inline char_device_impl&
    char_device::impl (void) const
    {
      return static_cast<char_device_impl&> (impl_);
    }

    // ========================================================================

    inline char_device&
    char_device_impl::self (void)
    {
      return static_cast<char_device&> (self_);
    }

    // ========================================================================

    template<typename T>
      template<typename ... Args>
        char_device_implementable<T>::char_device_implementable (
            const char* name, Args&&... args) :
            char_device
              { impl_instance_, name }, //
            impl_instance_
              { *this, std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_DEVICE_CHAR)
          trace::printf ("char_device_implementable::%s(\"%s\")=@%p\n",
                         __func__, name_, this);
#endif
        }

    template<typename T>
      char_device_implementable<T>::~char_device_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_CHAR)
        trace::printf ("char_device_implementable::%s() @%p %s\n", __func__,
                       this, name_);
#endif
      }

    template<typename T>
      typename char_device_implementable<T>::value_type&
      char_device_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_CHAR_H_ */
