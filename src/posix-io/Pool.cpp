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

#include "posix-io/Pool.h"

namespace os
{
  namespace posix
  {

    // ------------------------------------------------------------------------

    Pool::Pool (std::size_t size)
    {
      fSize = size;
      fInUse = new bool[size];
      for (std::size_t i = 0; i < fSize; ++i)
        {
          fInUse[i] = false;
        }
      // The derived class must alloc and set this pointer.
      fArray = nullptr;
    }

    Pool::~Pool ()
    {
      delete[] fInUse;
    }

    // ------------------------------------------------------------------------

    void*
    Pool::aquire (void)
    {
      for (std::size_t i = 0; i < fSize; ++i)
        {
          if (!fInUse[i])
            {
              fInUse[i] = true;
              return fArray[i];
            }
        }

      return nullptr;
    }

    bool
    Pool::release (void* file)
    {
      for (std::size_t i = 0; i < fSize; ++i)
        {
          if (fInUse[i] && (fArray[i] == file))
            {
              fInUse[i] = false;
              return true;
            }
        }
      return false;
    }

  } /* namespace posix */
} /* namespace os */
