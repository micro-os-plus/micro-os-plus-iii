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

#ifndef CMSIS_PLUS_POSIX_IO_SOCKET_H_
#define CMSIS_PLUS_POSIX_IO_SOCKET_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix/sys/socket.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class socket;

    // ------------------------------------------------------------------------

    /**
     * @ingroup cmsis-plus-posix-io-func
     * @{
     */

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wshadow"

    class socket*
    socket (int domain, int type, int protocol);

#pragma GCC diagnostic pop

#if 0
    int
    socketpair (int domain, int type, int protocol, socket* socket_vector[2]);
#endif

    /**
     * @}
     */

    // ------------------------------------------------------------------------
    /**
     * @brief Network socket.
     * @headerfile socket.h <cmsis-plus/posix-io/socket.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class socket : public io
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend socket*
      socket (int domain, int type, int protocol);

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      socket ();

      virtual
      ~socket ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      socket*
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

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Private Member Functions
       * @{
       */

    protected:

      /**
       * return 0 if success or -1 & errno
       */
      virtual int
      do_socket (int domain, int type, int protocol) = 0;

      virtual int
      do_accept (socket* sock, struct sockaddr* address,
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
      do_release (void) override;

      /**
       * @}
       */

    };

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_SOCKET_H_ */
