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

#ifndef POSIX_IO_SYS_SOCKET_H_
#define POSIX_IO_SYS_SOCKET_H_

#if !defined(__ARM_EABI__)
#include <sys/socket.h>
#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

  typedef uint32_t socklen_t;

  typedef unsigned int sa_family_t;

  struct sockaddr
  {
    sa_family_t sa_family;  //Address family.
    char sa_data[];  // Socket address (variable-length data).
  };

  int
  accept (int socket, struct sockaddr* address, socklen_t* address_len);

  int
  bind (int socket, const struct sockaddr* address, socklen_t address_len);
  int
  connect (int socket, const struct sockaddr* address, socklen_t address_len);
  int
  getpeername (int socket, struct sockaddr* address, socklen_t* address_len);

  int
  getsockname (int socket, struct sockaddr* address, socklen_t* address_len);

  int
  getsockopt (int socket, int level, int option_name, void* option_value,
              socklen_t* option_len);
  int
  listen (int socket, int backlog);

  ssize_t
  recv (int socket, void* buffer, size_t length, int flags);

  ssize_t
  recvfrom (int socket, void* buffer, size_t length, int flags,
            struct sockaddr* address, socklen_t* address_len);

  ssize_t
  recvmsg (int socket, struct msghdr* message, int flags);

  ssize_t
  send (int socket, const void* buffer, size_t length, int flags);

  ssize_t
  sendmsg (int socket, const struct msghdr* message, int flags);

  ssize_t
  sendto (int socket, const void* message, size_t length, int flags,
          const struct sockaddr* dest_addr, socklen_t dest_len);

  int
  setsockopt (int socket, int level, int option_name, const void* option_value,
              socklen_t option_len);

  int
  shutdown (int socket, int how);

  int
  sockatmark (int socket);

  int
  socket (int domain, int type, int protocol);

  int
  socketpair (int domain, int type, int protocol, int socket_vector[2]);

#ifdef __cplusplus
}
#endif

#endif /* __ARM_EABI__ */

#endif /* POSIX_IO_SYS_SOCKET_H_ */
