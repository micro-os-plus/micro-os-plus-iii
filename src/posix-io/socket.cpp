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

#include <cerrno>
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
    socket::accept (struct sockaddr* address, socklen_t* address_len)
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
    socket::bind (const struct sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_bind (address, address_len);
    }

    int
    socket::connect (const struct sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_connect (address, address_len);
    }

    int
    socket::getpeername (struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_getpeername (address, address_len);
    }

    int
    socket::getsockname (struct sockaddr* address, socklen_t* address_len)
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
                      struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_recvfrom (buffer, length, flags, address, address_len);
    }

    ssize_t
    socket::recvmsg (struct msghdr* message, int flags)
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
    socket::sendmsg (const struct msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return impl ().do_sendmsg (message, flags);
    }

    ssize_t
    socket::sendto (const void* message, size_t length, int flags,
                    const struct sockaddr* dest_addr, socklen_t dest_len)
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
