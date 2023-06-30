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

#include <cerrno>

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix/sys/socket.h>
#include <cmsis-plus/posix-io/net-stack.h>

#include <cmsis-plus/posix-io/socket.h>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    socket::socket (socket_impl& impl, class net_stack& ns) :
        io
          { impl, type::socket }, //
        net_stack_ (&ns)
    {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket::%s()=@%p\n", __func__, this);
#endif
    }

    socket::~socket ()
    {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket::%s() @%p\n", __func__, this);
#endif

      net_stack_ = nullptr;
    }

    // ------------------------------------------------------------------------

    class socket*
    socket::accept (/* struct */ sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      class socket* new_socket = impl ().do_accept (address, address_len);
      if (new_socket == nullptr)
        {
          return nullptr;
        }
      new_socket->alloc_file_descriptor ();
      return new_socket;
    }

    int
    socket::bind (const /* struct */ sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_bind (address, address_len);
    }

    int
    socket::connect (const /* struct */ sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_connect (address, address_len);
    }

    int
    socket::getpeername (/* struct */ sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_getpeername (address, address_len);
    }

    int
    socket::getsockname (/* struct */ sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_getsockname (address, address_len);
    }

    int
    socket::getsockopt (int level, int option_name, void* option_value,
                        socklen_t* option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_getsockopt (level, option_name, option_value,
                                    option_len);
    }

    int
    socket::listen (int backlog)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_listen (backlog);
    }

    ssize_t
    socket::recv (void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_recv (buffer, length, flags);
    }

    ssize_t
    socket::recvfrom (void* buffer, size_t length, int flags,
                      /* struct */ sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_recvfrom (buffer, length, flags, address, address_len);
    }

    ssize_t
    socket::recvmsg (/* struct */ msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_recvmsg (message, flags);
    }

    ssize_t
    socket::send (const void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_send (buffer, length, flags);
    }

    ssize_t
    socket::sendmsg (const /* struct */ msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_sendmsg (message, flags);
    }

    ssize_t
    socket::sendto (const void* message, size_t length, int flags,
                    const /* struct */ sockaddr* dest_addr, socklen_t dest_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_sendto (message, length, flags, dest_addr, dest_len);
    }

    int
    socket::setsockopt (int level, int option_name, const void* option_value,
                        socklen_t option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_setsockopt (level, option_name, option_value,
                                    option_len);
    }

    int
    socket::shutdown (int how)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_shutdown (how);
    }

    int
    socket::sockatmark (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_sockatmark ();
    }
    // ========================================================================

    socket_impl::socket_impl (void)
    {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket_impl::%s()=%p\n", __func__, this);
#endif
    }

    socket_impl::~socket_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
      trace::printf ("socket_impl::%s() @%p\n", __func__, this);
#endif
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
