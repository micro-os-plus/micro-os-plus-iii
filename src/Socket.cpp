/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#include "posix-io/Socket.h"
#include "posix-io/NetStack.h"
#include "posix-io/Pool.h"
#include <cerrno>
#include "posix/sys/socket.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    Socket*
    socket (int domain, int type, int protocol)
    {
      errno = 0;

      Socket* sock =
          reinterpret_cast<Socket*> (NetStack::getSocketsPool ()->aquire ());
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
      sock->allocFileDescriptor ();
      return sock;
    }

#if 0
    Socket*
    socketpair (int domain, int type, int protocol, int socket_vector[2])
      {
        return nullptr;
      }
#endif

#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    Socket::Socket ()
    {
      fType = Type::SOCKET;
    }

    Socket::~Socket ()
    {
      ;
    }

    // ------------------------------------------------------------------------

    void
    Socket::doRelease (void)
    {
      // Files is free, return it to the pool.
      auto pool = NetStack::getSocketsPool ();
      if (pool != nullptr)
        {
          pool->release (this);
        }
    }

    // ------------------------------------------------------------------------

    Socket*
    Socket::accept (struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      auto pool = NetStack::getSocketsPool ();
      if (pool == nullptr)
        {
          errno = EMFILE; // Pool is considered the per-process table.
          return nullptr;
        }

      Socket* const new_socket = static_cast<Socket*> (pool->aquire ());
      if (new_socket == nullptr)
        {
          errno = EMFILE; // Pool is considered the per-process table.
          return nullptr;
        }

      // Execute the implementation specific code.
      int ret = do_accept (new_socket, address, address_len);
      if (ret < 0)
        {
          return nullptr;
        }
      return static_cast<Socket*> (new_socket->allocFileDescriptor ());
    }

    int
    Socket::bind (const struct sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_bind (address, address_len);
    }

    int
    Socket::connect (const struct sockaddr* address, socklen_t address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_connect (address, address_len);
    }

    int
    Socket::getpeername (struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_getpeername (address, address_len);
    }

    int
    Socket::getsockname (struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_getsockname (address, address_len);
    }

    int
    Socket::getsockopt (int level, int option_name, void* option_value,
                        socklen_t* option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_getsockopt (level, option_name, option_value, option_len);
    }

    int
    Socket::listen (int backlog)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_listen (backlog);
    }

    ssize_t
    Socket::recv (void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_recv (buffer, length, flags);
    }

    ssize_t
    Socket::recvfrom (void* buffer, size_t length, int flags,
                      struct sockaddr* address, socklen_t* address_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_recvfrom (buffer, length, flags, address, address_len);
    }

    ssize_t
    Socket::recvmsg (struct msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_recvmsg (message, flags);
    }

    ssize_t
    Socket::send (const void* buffer, size_t length, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_send (buffer, length, flags);
    }

    ssize_t
    Socket::sendmsg (const struct msghdr* message, int flags)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_sendmsg (message, flags);
    }

    ssize_t
    Socket::sendto (const void* message, size_t length, int flags,
                    const struct sockaddr* dest_addr, socklen_t dest_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_sendto (message, length, flags, dest_addr, dest_len);
    }

    int
    Socket::setsockopt (int level, int option_name, const void* option_value,
                        socklen_t option_len)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_setsockopt (level, option_name, option_value, option_len);
    }

    int
    Socket::shutdown (int how)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_shutdown (how);
    }

    int
    Socket::sockatmark (void)
    {
      errno = 0;

      // Execute the implementation specific code.
      return do_sockatmark ();
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    Socket::do_accept (Socket* sock, struct sockaddr* address,
                       socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_bind (const struct sockaddr* address, socklen_t address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_connect (const struct sockaddr* address, socklen_t address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_getpeername (struct sockaddr* address, socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_getsockname (struct sockaddr* address, socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_getsockopt (int level, int option_name, void* option_value,
                           socklen_t* option_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_listen (int backlog)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    Socket::do_recv (void* buffer, size_t length, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    Socket::do_recvfrom (void* buffer, size_t length, int flags,
                         struct sockaddr* address, socklen_t* address_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    Socket::do_recvmsg (struct msghdr* message, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    Socket::do_send (const void* buffer, size_t length, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    Socket::do_sendmsg (const struct msghdr* message, int flags)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    ssize_t
    Socket::do_sendto (const void* message, size_t length, int flags,
                       const struct sockaddr* dest_addr, socklen_t dest_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_setsockopt (int level, int option_name, const void* option_value,
                           socklen_t option_len)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_shutdown (int how)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

    int
    Socket::do_sockatmark (void)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */
