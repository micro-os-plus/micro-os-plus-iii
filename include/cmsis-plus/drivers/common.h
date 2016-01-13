/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 * Copyright (c) 2013-2014 ARM Ltd.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
 * The code is inspired by ARM CMSIS Driver_Common.h file, v2.00,
 * and tries to remain functionally close to the CMSIS specifications.
 */

#ifndef CMSIS_PLUS_DRIVERS_COMMON_H_
#define CMSIS_PLUS_DRIVERS_COMMON_H_

// ----------------------------------------------------------------------------
#ifdef __cplusplus

#include <cstdint>

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      using version_t = uint16_t;
      using event_t = uint32_t;
      using status_t = int32_t;
      using power_t = uint32_t;

      // Status & error codes
      constexpr status_t STATUS_OK = 0;

      constexpr status_t ERROR = -1;
      constexpr status_t ERROR_BUSY = -2;
      constexpr status_t ERROR_TIMEOUT = -3;
      constexpr status_t ERROR_UNSUPPORTED = -4;
      constexpr status_t ERROR_PARAMETER = -5;
      constexpr status_t ERROR_SPECIFIC = -6;

      typedef void
      (*signal_event_t) (const void* object, event_t event);

      // ----------------------------------------------------------------------

      enum class Power
        : power_t
          {
            //
        off, //
        low, //
        full
      };

      // ======================================================================

      class Version
      {
      public:

        // --------------------------------------------------------------------

        Version (version_t api, version_t drv) noexcept;

        ~Version () = default;

        // --------------------------------------------------------------------

        version_t
        get_api (void) const noexcept;

        version_t
        get_drv (void) const noexcept;

        // --------------------------------------------------------------------

      private:

        version_t const api_; ///< API version
        version_t const drv_; ///< Driver version
      };

      inline
      Version::Version (version_t api, version_t drv) noexcept :
      api_ (api), //
      drv_ (drv)
        {
          ;
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
        ~Base () noexcept = default;

        // --------------------------------------------------------------------

        /**
         * @brief       Get driver version.
         * @return      @ref ARM_DRIVER_VERSION
         */
        virtual const Version&
        get_version (void) const noexcept = 0;

        /**
         * @brief       Power up/down serial port.
         * @param[in]   state  Power state
         * @return      @ref execution_status
         */
        virtual status_t
        power (Power state) noexcept = 0;

      };

    // ----------------------------------------------------------------------

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_DRIVERS_COMMON_H_ */
