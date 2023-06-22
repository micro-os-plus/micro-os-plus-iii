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

#ifndef CMSIS_PLUS_POSIX_IO_CHAR_DEVICE_H_
#define CMSIS_PLUS_POSIX_IO_CHAR_DEVICE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/device.h>

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

    class char_device_impl;

    // ========================================================================

    /**
     * @brief Char device class.
     * @headerfile char-device.h <cmsis-plus/posix-io/char-device.h>
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
      ~char_device () override;

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
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

    class char_device_impl : public device_impl
    {
      // ----------------------------------------------------------------------

      friend class char_device;

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      char_device_impl (void);

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
      ~char_device_impl () override;

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
      do_lseek (off_t offset, int whence) override;

      virtual void
      do_sync (void) override;

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
        ~char_device_implementable () override;

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

    template<typename T>
      template<typename ... Args>
        char_device_implementable<T>::char_device_implementable (
            const char* name, Args&&... args) :
            char_device
              { impl_instance_, name }, //
            impl_instance_
              { std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_CHAR_DEVICE)
          trace::printf ("char_device_implementable::%s(\"%s\")=@%p\n",
                         __func__, name_, this);
#endif
        }

    template<typename T>
      char_device_implementable<T>::~char_device_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_CHAR_DEVICE)
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

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_IO_CHAR_DEVICE_H_ */
