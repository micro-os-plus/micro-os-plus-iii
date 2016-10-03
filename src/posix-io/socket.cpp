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
#include <cmsis-plus/posix-io/pool.h>
#include <cmsis-plus/posix-io/socket.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    class socket*
    socket (int domain, int type, int protocol)
    {
      errno = 0;

      class socket* sock =
          reinterpret_cast<class socket*> (net_stack::sockets_pool ()->aquire ());
      if (sock == nullptr)
        {
          errno = ENFILE;
          return nullptr;
        }
      int ret = sock->do_socket (domain, type, protocol);
      if (ret < 0)
        {
          sock->close ();
          return nullptr;
        }
      sock->alloc_file_descriptor ();
      return sock;
    }

#if 0
    socket*
    socketpair (int domain, int type, int protocol, int socket_vector[2])
      {
        return nullptr;
      }
#endif

#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    socket::socket () :
        io (type::socket)
    {
      ;
    }

    socket::~socket ()
    {
      ;
    }

    // ------------------------------------------------------------------------

    void
    socket::do_release (void)
    {
      // Files is free, return it to the pool.
      auto pool = net_stack::sockets_pool ();
      if (pool != nullptr)
        {
          pool->release (this);
        }
    }

    // ------------------------------------------------------------------------

    class socket*
    socket::accept (struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      auto pool = net_stack::sockets_pool ();
      if (pool == nullptr)
        {
          errno = EMFILE; // pool is considered the per-process table.
          return nullptr;
        }

      socket* const new_socket = static_cast<socket*> (pool->aquire ());
      if (new_socket == nullptr)
        {
          errno = EMFILE; // pool is considered the per-process table.
          return nullptr;
        }

      // Execute the implementation specific code.
      int ret = do_accept (new_socket, address, address_len);
      if (ret < 0)
        {
          return nullptr;
        }
      return static_cast<socket*> (new_socket->alloc_file_descriptor ());
    }

    int
    socket::bind (const struct sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_bind (address, address_len);
    }

    int
    socket::connect (const struct sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_connect (address, address_len);
    }

    int
    socket::getpeername (struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_getpeername (address, address_len);
    }

    int
    socket::getsockname (struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_getsockname (address, address_len);
    }

    int
    socket::getsockopt (int level, int option_name, void* option_value,
                        socklen_t* option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_getsockopt (level, option_name, option_value, option_len);
    }

    int
    socket::listen (int backlog)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_listen (backlog);
    }

    ssize_t
    socket::recv (void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_recv (buffer, length, flags);
    }

    ssize_t
    socket::recvfrom (void* buffer, size_t length, int flags,
                      struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_recvfrom (buffer, length, flags, address, address_len);
    }

    ssize_t
    socket::recvmsg (struct msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_recvmsg (message, flags);
    }

    ssize_t
    socket::send (const void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_send (buffer, length, flags);
    }

    ssize_t
    socket::sendmsg (const struct msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_sendmsg (message, flags);
    }

    ssize_t
    socket::sendto (const void* message, size_t length, int flags,
                    const struct sockaddr* dest_addr, socklen_t dest_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_sendto (message, length, flags, dest_addr, dest_len);
    }

    int
    socket::setsockopt (int level, int option_name, const void* option_value,
                        socklen_t option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_setsockopt (level, option_name, option_value, option_len);
    }

    int
    socket::shutdown (int how)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_shutdown (how);
    }

    int
    socket::sockatmark (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_sockatmark ();
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    socket::do_accept (socket* sock, struct sockaddr* address,
                       socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_bind (const struct sockaddr* address, socklen_t address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_connect (const struct sockaddr* address, socklen_t address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_getpeername (struct sockaddr* address, socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_getsockname (struct sockaddr* address, socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_getsockopt (int level, int option_name, void* option_value,
                           socklen_t* option_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_listen (int backlog)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    socket::do_recv (void* buffer, size_t length, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    socket::do_recvfrom (void* buffer, size_t length, int flags,
                         struct sockaddr* address, socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    socket::do_recvmsg (struct msghdr* message, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    socket::do_send (const void* buffer, size_t length, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    socket::do_sendmsg (const struct msghdr* message, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    socket::do_sendto (const void* message, size_t length, int flags,
                       const struct sockaddr* dest_addr, socklen_t dest_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_setsockopt (int level, int option_name, const void* option_value,
                           socklen_t option_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_shutdown (int how)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    socket::do_sockatmark (void)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */
