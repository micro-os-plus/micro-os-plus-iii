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

#ifndef CMSIS_PLUS_POSIX_IO_SOCKET_H_
#define CMSIS_PLUS_POSIX_IO_SOCKET_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix/sys/socket.h>
#include <cmsis-plus/utils/lists.h>

#include <mutex>

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

    class socket;
    class socket_impl;
    class net_stack;

    // ------------------------------------------------------------------------
    /**
     * @brief Network socket.
     * @headerfile socket.h <cmsis-plus/posix-io/socket.h>
     * @ingroup cmsis-plus-posix-io-base
     */
#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#endif
    class socket : public io
    {
      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      socket (socket_impl& impl, /* class */ net_stack& ns);

      /**
       * @cond ignore
       */

      // The rule of five.
      socket (const socket&) = delete;
      socket (socket&&) = delete;
      socket&
      operator= (const socket&) = delete;
      socket&
      operator= (socket&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~socket () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual /* class */ socket*
      accept (/* struct */ sockaddr* address, socklen_t* address_len);

      virtual int
      bind (const /* struct */ sockaddr* address, socklen_t address_len);

      virtual int
      connect (const /* struct */ sockaddr* address, socklen_t address_len);

      virtual int
      getpeername (/* struct */ sockaddr* address, socklen_t* address_len);

      virtual int
      getsockname (/* struct */ sockaddr* address, socklen_t* address_len);

      virtual int
      getsockopt (int level, int option_name, void* option_value,
                  socklen_t* option_len);

      virtual int
      listen (int backlog);

      virtual ssize_t
      recv (void* buffer, size_t length, int flags);

      virtual ssize_t
      recvfrom (void* buffer, size_t length, int flags,
                /* struct */ sockaddr* address, socklen_t* address_len);

      virtual ssize_t
      recvmsg (/* struct */ msghdr* message, int flags);

      virtual ssize_t
      send (const void* buffer, size_t length, int flags);

      virtual ssize_t
      sendmsg (const /* struct */ msghdr* message, int flags);

      virtual ssize_t
      sendto (const void* message, size_t length, int flags,
              const /* struct */ sockaddr* dest_addr, socklen_t dest_len);

      virtual int
      setsockopt (int level, int option_name, const void* option_value,
                  socklen_t option_len);

      virtual int
      shutdown (int how);

      virtual int
      sockatmark (void);

      // ----------------------------------------------------------------------
      // Support functions.

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif

      class net_stack*
      net_stack (void);

#pragma GCC diagnostic pop

      socket_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    public:

      /**
       * @cond ignore
       */

      // Intrusive node used to link this socket to the deferred
      // deallocation list. Must be public.
      utils::double_list_links deferred_links_;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      class net_stack* net_stack_;

      /**
       * @endcond
       */

    };
#pragma GCC diagnostic pop

    // ========================================================================

    class socket_impl : public io_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class socket;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      socket_impl (void);

      /**
       * @cond ignore
       */

      // The rule of five.
      socket_impl (const socket_impl&) = delete;
      socket_impl (socket_impl&&) = delete;
      socket_impl&
      operator= (const socket_impl&) = delete;
      socket_impl&
      operator= (socket_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~socket_impl () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual /* class */ socket*
      do_accept (/* struct */ sockaddr* address, socklen_t* address_len) = 0;

      virtual int
      do_bind (const /* struct */ sockaddr* address, socklen_t address_len) = 0;

      virtual int
      do_connect (const /* struct */ sockaddr* address, socklen_t address_len) = 0;

      virtual int
      do_getpeername (/* struct */ sockaddr* address, socklen_t* address_len) = 0;

      virtual int
      do_getsockname (/* struct */ sockaddr* address, socklen_t* address_len) = 0;

      virtual int
      do_getsockopt (int level, int option_name, void* option_value,
                     socklen_t* option_len) = 0;

      virtual int
      do_listen (int backlog) = 0;

      virtual ssize_t
      do_recv (void* buffer, size_t length, int flags) = 0;

      virtual ssize_t
      do_recvfrom (void* buffer, size_t length, int flags,
                   /* struct */ sockaddr* address, socklen_t* address_len) = 0;

      virtual ssize_t
      do_recvmsg (/* struct */ msghdr* message, int flags) = 0;

      virtual ssize_t
      do_send (const void* buffer, size_t length, int flags) = 0;

      virtual ssize_t
      do_sendmsg (const /* struct */ msghdr* message, int flags) = 0;

      virtual ssize_t
      do_sendto (const void* message, size_t length, int flags,
                 const /* struct */ sockaddr* dest_addr, socklen_t dest_len) = 0;

      virtual int
      do_setsockopt (int level, int option_name, const void* option_value,
                     socklen_t option_len) = 0;

      virtual int
      do_shutdown (int how) = 0;

      virtual int
      do_sockatmark (void) = 0;

      /**
       * @}
       */
    };

    // ========================================================================

    template<typename T>
      class socket_implementable : public socket
      {
        // --------------------------------------------------------------------

      public:

        using value_type = T;

        // ---------------------------------------------------------------------

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        socket_implementable (class net_stack& ns);

        /**
         * @cond ignore
         */

        // The rule of five.
        socket_implementable (const socket_implementable&) = delete;
        socket_implementable (socket_implementable&&) = delete;
        socket_implementable&
        operator= (const socket_implementable&) = delete;
        socket_implementable&
        operator= (socket_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~socket_implementable ();

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        // Support functions.

        value_type&
        impl (void) const;

        /**
         * @}
         */

        // --------------------------------------------------------------------
      protected:

        /**
         * @cond ignore
         */

        value_type impl_instance_;

        /**
         * @endcond
         */
      };

    // ========================================================================

    template<typename T, typename L>
      class socket_lockable : public socket
      {
        // --------------------------------------------------------------------

      public:

        using value_type = T;
        using lockable_type = L;

        // --------------------------------------------------------------------

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        socket_lockable (class net_stack& ns, lockable_type& locker);

        /**
         * @cond ignore
         */

        // The rule of five.
        socket_lockable (const socket_lockable&) = delete;
        socket_lockable (socket_lockable&&) = delete;
        socket_lockable&
        operator= (const socket_lockable&) = delete;
        socket_lockable&
        operator= (socket_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~socket_lockable ();

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        virtual int
        close (void) override;

        virtual class socket*
        accept (/* struct */ sockaddr* address, socklen_t* address_len) override;

        virtual int
        bind (const /* struct */ sockaddr* address, socklen_t address_len) override;

        virtual int
        connect (const /* struct */ sockaddr* address, socklen_t address_len)
            override;

        virtual int
        getpeername (/* struct */ sockaddr* address, socklen_t* address_len) override;

        virtual int
        getsockname (/* struct */ sockaddr* address, socklen_t* address_len) override;

        virtual int
        getsockopt (int level, int option_name, void* option_value,
                    socklen_t* option_len) override;

        virtual int
        listen (int backlog) override;

        virtual ssize_t
        recv (void* buffer, size_t length, int flags) override;

        virtual ssize_t
        recvfrom (void* buffer, size_t length, int flags,
                  /* struct */ sockaddr* address, socklen_t* address_len) override;

        virtual ssize_t
        recvmsg (/* struct */ msghdr* message, int flags) override;

        virtual ssize_t
        send (const void* buffer, size_t length, int flags) override;

        virtual ssize_t
        sendmsg (const /* struct */ msghdr* message, int flags) override;

        virtual ssize_t
        sendto (const void* message, size_t length, int flags,
                const /* struct */ sockaddr* dest_addr, socklen_t dest_len) override;

        virtual int
        setsockopt (int level, int option_name, const void* option_value,
                    socklen_t option_len) override;

        virtual int
        shutdown (int how) override;

        virtual int
        sockatmark (void) override;

        // --------------------------------------------------------------------
        // Support functions.

        value_type&
        impl (void) const;

        /**
         * @}
         */

        // --------------------------------------------------------------------
      protected:

        /**
         * @cond ignore
         */

        value_type impl_instance_;

        lockable_type& locker_;

        /**
         * @endcond
         */
      };

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ========================================================================

    inline net_stack*
    socket::net_stack (void)
    {
      return net_stack_;
    }

    inline socket_impl&
    socket::impl (void) const
    {
      return static_cast<socket_impl&> (impl_);
    }

    // ========================================================================

    template<typename T>
      socket_implementable<T>::socket_implementable (class net_stack& ns) :
          socket
            { impl_instance_, ns }
      {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
        trace::printf ("socket_implementable::%s()=@%p\n", __func__, this);
#endif
      }

    template<typename T>
      socket_implementable<T>::~socket_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
        trace::printf ("socket_implementable::%s() @%p\n", __func__, this);
#endif
      }

    template<typename T>
      typename socket_implementable<T>::value_type&
      socket_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

    // ========================================================================

    template<typename T, typename L>
      socket_lockable<T, L>::socket_lockable (class net_stack& ns,
                                              lockable_type& locker) :
          socket
            { impl_instance_, ns }, //
          locker_ (locker)
      {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
        trace::printf ("socket_lockable::%s()=@%p\n", __func__, this);
#endif
      }

    template<typename T, typename L>
      socket_lockable<T, L>::~socket_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_SOCKET)
        trace::printf ("socket_lockable::%s() @%p\n", __func__, this);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      socket_lockable<T, L>::close (void)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::close ();
      }

    template<typename T, typename L>
      /* class */ socket*
      socket_lockable<T, L>::accept (/* struct */ sockaddr* address,
                                     socklen_t* address_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::accept (address, address_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::bind (const /* struct */ sockaddr* address,
                                   socklen_t address_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::bind (address, address_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::connect (const /* struct */ sockaddr* address,
                                      socklen_t address_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::connect (address, address_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::getpeername (/* struct */ sockaddr* address,
                                          socklen_t* address_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::getpeername (address, address_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::getsockname (/* struct */ sockaddr* address,
                                          socklen_t* address_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::getsockname (address, address_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::getsockopt (int level, int option_name,
                                         void* option_value,
                                         socklen_t* option_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::getsockopt (level, option_name, option_value, option_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::listen (int backlog)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::listen (backlog);
      }

    template<typename T, typename L>
      ssize_t
      socket_lockable<T, L>::recv (void* buffer, size_t length, int flags)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::recv (buffer, length, flags);
      }

    template<typename T, typename L>
      ssize_t
      socket_lockable<T, L>::recvfrom (void* buffer, size_t length, int flags,
                                       /* struct */ sockaddr* address,
                                       socklen_t* address_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::recvfrom (buffer, length, flags, address, address_len);
      }

    template<typename T, typename L>
      ssize_t
      socket_lockable<T, L>::recvmsg (/* struct */ msghdr* message, int flags)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::recvmsg (message, flags);
      }

    template<typename T, typename L>
      ssize_t
      socket_lockable<T, L>::send (const void* buffer, size_t length, int flags)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::send (buffer, length, flags);
      }

    template<typename T, typename L>
      ssize_t
      socket_lockable<T, L>::sendmsg (const /* struct */ msghdr* message, int flags)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::sendmsg (message, flags);
      }

    template<typename T, typename L>
      ssize_t
      socket_lockable<T, L>::sendto (const void* message, size_t length,
                                     int flags,
                                     const /* struct */ sockaddr* dest_addr,
                                     socklen_t dest_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::sendto (message, length, flags, dest_addr, dest_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::setsockopt (int level, int option_name,
                                         const void* option_value,
                                         socklen_t option_len)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::setsockopt (level, option_name, option_value, option_len);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::shutdown (int how)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::shutdown (how);
      }

    template<typename T, typename L>
      int
      socket_lockable<T, L>::sockatmark (void)
      {
        std::lock_guard<L> lock
          { locker_ };

        return socket::sockatmark ();
      }

    template<typename T, typename L>
      typename socket_lockable<T, L>::value_type&
      socket_lockable<T, L>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_IO_SOCKET_H_ */
