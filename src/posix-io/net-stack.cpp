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

#include <cmsis-plus/posix-io/net-stack.h>
#include <cmsis-plus/posix-io/net-interface.h>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @cond ignore
     */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#pragma clang diagnostic ignored "-Wglobal-constructors"
#endif

    net_stack::net_list net_stack::net_list__;

#pragma GCC diagnostic pop

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-parameter"
#endif
// TODO: remove after fixing implementation.
#pragma GCC diagnostic ignored "-Wnonnull"
#pragma GCC diagnostic ignored "-Wnull-dereference"

    class socket*
    socket (int domain, int type, int protocol)
    {
      errno = 0;

      // TODO: implement a way to identify the net stack.
      net_stack* ns = nullptr;
      class socket* sock = ns->socket (domain, type, protocol);
      if (sock == nullptr)
        {
          return nullptr;
        }
      sock->alloc_file_descriptor ();
      return sock;
    }

#pragma GCC diagnostic pop

    // ========================================================================

    net_stack::net_stack (net_stack_impl& impl, const char* name) :
        name_ (name), //
        impl_ (impl)
    {
#if defined(OS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack::%s(\"%s\")=%p\n", __func__, name_, this);
#endif
      deferred_sockets_list_.clear ();
    }

    net_stack::~net_stack ()
    {
#if defined(OS_TRACE_POSIX_IO_NET_STACK)
      trace::printf ("net_stack::%s(\"%s\") %p\n", __func__, name_, this);
#endif
    }

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif
    class socket*
    net_stack::socket (int domain, int type, int protocol)
    {
      errno = 0;

      return impl ().do_socket (domain, type, protocol);
    }
#pragma GCC diagnostic pop

    // ========================================================================

    net_stack_impl::net_stack_impl (net_interface& interface) :
        interface_ (interface)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("net_stack_impl::%s()=%p\n", __func__, this);
#endif
    }

    net_stack_impl::~net_stack_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
      trace::printf ("net_stack_impl::%s() @%p\n", __func__, this);
#endif
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
