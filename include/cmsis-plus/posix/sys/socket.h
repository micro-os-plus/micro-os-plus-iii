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

#ifndef POSIX_IO_SYS_SOCKET_H_
#define POSIX_IO_SYS_SOCKET_H_

// ----------------------------------------------------------------------------

#include <unistd.h>

#if defined(_POSIX_VERSION)

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wgnu-include-next"
#endif
#include_next <sys/socket.h>
#pragma GCC diagnostic pop

#else

#include <sys/types.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------

  typedef uint32_t socklen_t;

#if !defined (OS_EXCLUDE_SOCKET_SA_FAMILY_T)
  typedef unsigned int sa_family_t;
#endif

#if !defined (OS_EXCLUDE_SOCKET_STRUCT_SOCKADDR)
  struct sockaddr
  {
    sa_family_t sa_family;  //Address family.
    char sa_data[];  // Socket address (variable-length data).
  };
#endif

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

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* defined(_POSIX_VERSION) */

#endif /* POSIX_IO_SYS_SOCKET_H_ */
