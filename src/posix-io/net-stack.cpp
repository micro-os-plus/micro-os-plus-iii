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

#include <cmsis-plus/posix-io/net-stack.h>
#include <cmsis-plus/posix-io/net-interface.h>

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
#pragma GCC diagnostic ignored "-Wunused-parameter"

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

    class socket*
    net_stack::socket (int domain, int type, int protocol)
    {
      errno = 0;

      return impl ().do_socket (domain, type, protocol);
    }

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
