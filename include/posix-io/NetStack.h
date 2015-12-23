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

#ifndef POSIX_IO_NET_STACK_H_
#define POSIX_IO_NET_STACK_H_

// ----------------------------------------------------------------------------

#include "posix-io/types.h"
#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {

    // ------------------------------------------------------------------------

    class IO;
    class Socket;
    class Pool;

    // ------------------------------------------------------------------------

    class NetStack
    {
    public:

      NetStack (Pool* socketsPool);
      NetStack (const NetStack&) = delete;

      ~NetStack ();

      // ----------------------------------------------------------------------

      static Pool*
      getSocketsPool (void);

      // ----------------------------------------------------------------------
    private:

      static Pool* sfSocketsPool;

    };

    // ------------------------------------------------------------------------

    inline Pool*
    NetStack::getSocketsPool (void)
    {
      return sfSocketsPool;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_NET_STACK_H_ */
