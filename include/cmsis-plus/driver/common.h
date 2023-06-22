/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

/*
 * The code is inspired by ARM CMSIS Driver_Common.h file, v2.00,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_DRIVER_COMMON_H_
#define CMSIS_PLUS_DRIVER_COMMON_H_

// ----------------------------------------------------------------------------

#ifdef __cplusplus

// ----------------------------------------------------------------------------

#include <cstdint>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace driver
  {
    // ----------------------------------------------------------------------

    using version_t = uint16_t;
    using event_t = uint32_t;
    using return_t = int32_t;
    using power_t = uint32_t;

    // ----- Return & error codes -----

    ///< Operation succeeded
    constexpr return_t RETURN_OK = 0;

    ///< Unspecified error
    constexpr return_t ERROR = -1;
    ///< Driver is busy
    constexpr return_t ERROR_BUSY = -2;
    ///< Timeout occurred
    constexpr return_t ERROR_TIMEOUT = -3;
    ///< Operation not supported
    constexpr return_t ERROR_UNSUPPORTED = -4;
    ///< Parameter error
    constexpr return_t ERROR_PARAMETER = -5;
    ///< Start of driver specific errors
    constexpr return_t ERROR_SPECIFIC = -6;

    typedef void
    (*signal_event_t) (const void* object, event_t event);

    // ----------------------------------------------------------------------

    enum class Power
      : power_t
        {
          //

      // Completely power off the device.
      off,
      // Low power mode.
      low,
      // Fully power on the
      full
    };

    // ======================================================================

    class Version
    {
    public:

      // --------------------------------------------------------------------

      constexpr
      Version () noexcept;

      constexpr
      Version (version_t api, version_t drv) noexcept;

      Version (const Version&) = default;

      Version&
      operator= (const Version&) = default;

      ~Version () noexcept = default;

      // --------------------------------------------------------------------

      version_t
      get_api (void) const noexcept;

      version_t
      get_drv (void) const noexcept;

      // --------------------------------------------------------------------

    private:

      version_t api_; ///< API version
      version_t drv_; ///< Driver version
    };

    inline constexpr
    Version::Version () noexcept :
    api_ (0), //
    drv_ (0)
      {
      }

    inline constexpr
    Version::Version (version_t api, version_t drv) noexcept :
    api_ (api), //
    drv_ (drv)
      {
      }

    inline version_t
    Version::get_api (void) const noexcept
    {
      return api_;
    }

    inline version_t
    Version::get_drv (void) const noexcept
    {
      return drv_;
    }

    // ========================================================================

    class Base
    {

    public:

      // --------------------------------------------------------------------

      Base () noexcept = default;

      virtual
      ~Base () noexcept;

      // --------------------------------------------------------------------

      /**
       * @brief       Get driver version.
       * @return      Version.
       */
      const Version&
      get_version (void) noexcept;

      /**
       * @brief       Power up/down device.
       * @param[in]   state  Power state
       * @return      Execution status
       */
      return_t
      power (Power state) noexcept;

      // --------------------------------------------------------------------

    protected:

      virtual const Version&
      do_get_version (void) noexcept = 0;

      virtual return_t
      do_power (Power state) noexcept = 0;

    };

    // ----------------------------------------------------------------------

    inline const Version&
    Base::get_version (void) noexcept
    {
      return do_get_version ();
    }

    inline return_t
    Base::power (Power state) noexcept
    {
      return do_power (state);
    }

  } /* namespace driver */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_DRIVER_COMMON_H_ */
