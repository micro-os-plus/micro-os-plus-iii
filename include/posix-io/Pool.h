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

#ifndef POSIX_IO_POOL_H_
#define POSIX_IO_POOL_H_

// ----------------------------------------------------------------------------

#include <cstddef>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class Pool
    {
    public:
      Pool (std::size_t size);
      Pool (const Pool&) = delete;

      virtual
      ~Pool ();

      // ----------------------------------------------------------------------

      void*
      aquire (void);

      bool
      release (void* obj);

      // ----------------------------------------------------------------------

      std::size_t
      getSize (void) const;

      void*
      getObject (std::size_t index) const;

      bool
      getFlag (std::size_t index) const;

      // ----------------------------------------------------------------------

    protected:

      // Referred directly in TPool.

      void** fArray;
      bool* fInUse;
      std::size_t fSize;
    };

    // ------------------------------------------------------------------------

    inline std::size_t
    Pool::getSize (void) const
    {
      return fSize;
    }

    inline void*
    Pool::getObject (std::size_t index) const
    {
      return fArray[index];
    }

    inline bool
    Pool::getFlag (std::size_t index) const
    {
      return fInUse[index];
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_POOL_H_ */
