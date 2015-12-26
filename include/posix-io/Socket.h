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

#ifndef POSIX_IO_SOCKET_H_
#define POSIX_IO_SOCKET_H_

// ----------------------------------------------------------------------------

#include "posix-io/IO.h"

#include "posix/sys/socket.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class Socket;

    // ------------------------------------------------------------------------

    Socket*
    socket (int domain, int type, int protocol);

#if 0
    int
    socketpair (int domain, int type, int protocol, Socket* socket_vector[2]);
#endif

    // ------------------------------------------------------------------------

    class Socket : public IO
    {
      // ----------------------------------------------------------------------

      friend Socket*
      socket (int domain, int type, int protocol);

      // ----------------------------------------------------------------------

    public:

      Socket ();

      virtual
      ~Socket ();

      // ----------------------------------------------------------------------

      Socket*
      accept (struct sockaddr* address, socklen_t* address_len);

      int
      bind (const struct sockaddr* address, socklen_t address_len);

      int
      connect (const struct sockaddr* address, socklen_t address_len);

      int
      getpeername (struct sockaddr* address, socklen_t* address_len);

      int
      getsockname (struct sockaddr* address, socklen_t* address_len);

      int
      getsockopt (int level, int option_name, void* option_value,
                  socklen_t* option_len);

      int
      listen (int backlog);

      ssize_t
      recv (void* buffer, size_t length, int flags);

      ssize_t
      recvfrom (void* buffer, size_t length, int flags,
                struct sockaddr* address, socklen_t* address_len);

      ssize_t
      recvmsg (struct msghdr* message, int flags);

      ssize_t
      send (const void* buffer, size_t length, int flags);

      ssize_t
      sendmsg (const struct msghdr* message, int flags);

      ssize_t
      sendto (const void* message, size_t length, int flags,
              const struct sockaddr* dest_addr, socklen_t dest_len);

      int
      setsockopt (int level, int option_name, const void* option_value,
                  socklen_t option_len);

      int
      shutdown (int how);

      int
      sockatmark (void);

      // ----------------------------------------------------------------------
    protected:

      /**
       * return 0 if success or -1 & errno
       */
      virtual int
      do_socket (int domain, int type, int protocol) = 0;

      virtual int
      do_accept (Socket* sock, struct sockaddr* address,
                 socklen_t* address_len);

      virtual int
      do_bind (const struct sockaddr* address, socklen_t address_len);

      virtual int
      do_connect (const struct sockaddr* address, socklen_t address_len);

      virtual int
      do_getpeername (struct sockaddr* address, socklen_t* address_len);

      virtual int
      do_getsockname (struct sockaddr* address, socklen_t* address_len);

      virtual int
      do_getsockopt (int level, int option_name, void* option_value,
                     socklen_t* option_len);

      virtual int
      do_listen (int backlog);

      virtual ssize_t
      do_recv (void* buffer, size_t length, int flags);

      virtual ssize_t
      do_recvfrom (void* buffer, size_t length, int flags,
                   struct sockaddr* address, socklen_t* address_len);

      virtual ssize_t
      do_recvmsg (struct msghdr* message, int flags);

      virtual ssize_t
      do_send (const void* buffer, size_t length, int flags);

      virtual ssize_t
      do_sendmsg (const struct msghdr* message, int flags);

      virtual ssize_t
      do_sendto (const void* message, size_t length, int flags,
                 const struct sockaddr* dest_addr, socklen_t dest_len);

      virtual int
      do_setsockopt (int level, int option_name, const void* option_value,
                     socklen_t option_len);

      virtual int
      do_shutdown (int how);

      virtual int
      do_sockatmark (void);

      virtual void
      doRelease (void) override;

    };

  } /* namespace posix */
} /* namespace os */

#endif /* POSIX_IO_SOCKET_H_ */
