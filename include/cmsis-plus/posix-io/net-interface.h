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

#ifndef CMSIS_PLUS_POSIX_IO_NET_INTERFACE_H_
#define CMSIS_PLUS_POSIX_IO_NET_INTERFACE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

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

    class net_interface_impl;

    // ========================================================================

    /**
     * @brief Network interface class.
     * @headerfile net-interface.h <cmsis-plus/posix-io/net-interface.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class net_interface
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      net_interface (net_interface_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      net_interface (const net_interface&) = delete;
      net_interface (net_interface&&) = delete;
      net_interface&
      operator= (const net_interface&) = delete;
      net_interface&
      operator= (net_interface&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~net_interface ();

      /**
       * @}
       */

      // TODO: add content
    };

  } /* namespace posix */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_IO_NET_INTERFACE_H_ */
