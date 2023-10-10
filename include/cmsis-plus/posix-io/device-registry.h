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

#ifndef CMSIS_PLUS_POSIX_IO_DEVICE_REGISTRY_H_
#define CMSIS_PLUS_POSIX_IO_DEVICE_REGISTRY_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/device.h>
#include <cmsis-plus/diag/trace.h>

#include <cstddef>
#include <cassert>
#include <cstring>

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
    // ========================================================================

    /**
     * @brief Devices registry static class.
     * @headerfile device-registry.h <cmsis-plus/posix-io/device-registry.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    template<typename T>
      class device_registry
      {
      public:

        using value_type = T;
        using pointer = T*;
        using reference = T&;

        // --------------------------------------------------------------------

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        // Do not allow to create instances of this class.
        device_registry () = delete;

        /**
         * @cond ignore
         */

        // The rule of five.
        device_registry (const value_type&) = delete;
        device_registry (value_type&&) = delete;
        value_type&
        operator= (const value_type&) = delete;
        value_type&
        operator= (value_type&&) = delete;

        /**
         * @endcond
         */

        ~device_registry () = delete;

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Static Member Functions
         * @{
         */

      public:

        static void
        link (value_type* device);

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
        static value_type*
        identify_device (const char* path);
#pragma GCC diagnostic pop

        /**
         * @}
         */

        // --------------------------------------------------------------------
      private:

        /**
         * @cond ignore
         */

        // Since devices may be constructed statically, so may ask
        // to be linked here at any time, this list must be initialised
        // before any static constructor.
        // With the order of static constructors unknown, this means it
        // must be allocated in the BSS and will be initialised to 0 by
        // the startup code.
        using device_list = utils::intrusive_list<device,
        utils::double_list_links, &device::registry_links_, T>;
        static device_list registry_list__;

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

    template<typename T>
      void
      device_registry<T>::link (value_type* device)
      {
#if defined(DEBUG)

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
        for (auto&& d : registry_list__)
          {
            // Validate the device name by checking duplicates.
            if (std::strcmp (device->name (), d.name ()) == 0)
              {
                trace::puts ("Duplicate device name. Abort.");
                std::abort ();
              }
          }
#pragma GCC diagnostic push

#endif // DEBUG

        registry_list__.link (*device);

        trace::printf ("Device '%s%s' linked\n", value_type::device_prefix (),
                       device->name ());
      }

    /**
     * return pointer to device or nullptr if not found.
     */
    template<typename T>
      T*
      device_registry<T>::identify_device (const char* path)
      {
        assert(path != nullptr);

        auto prefix = value_type::device_prefix ();
        if (std::strncmp (prefix, path, std::strlen (prefix)) != 0)
          {
            // The device prefix does not match, not a device.
            return nullptr;
          }

        // The prefix was identified; try to match the rest of the path.
        auto name = path + std::strlen (prefix);

        for (auto&& p : registry_list__)
          {
            if (p.match_name (name))
              {
                return static_cast<value_type*> (&p);
              }
          }

        // Not a known device.
        return nullptr;
      }

    /**
     * @cond ignore
     */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

    // Initialised to 0 by BSS.
    template<typename T>
      typename device_registry<T>::device_list device_registry<T>::registry_list__;

#pragma GCC diagnostic pop

  /**
   * @endcond
   */

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_REGISTRY_H_ */
