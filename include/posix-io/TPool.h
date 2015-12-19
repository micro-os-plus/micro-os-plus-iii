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

#ifndef POSIX_IO_TPOOL_H_
#define POSIX_IO_TPOOL_H_

// ----------------------------------------------------------------------------

#include "posix-io/Pool.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    template<typename T>
      class TPool : public Pool
      {
      public:

        TPool (std::size_t size) :
            Pool (size)
        {
          fArray = reinterpret_cast<void**> (new T*[size]);
          for (std::size_t i = 0; i < size; ++i)
            {
              fArray[i] = new T;
            }
        }

        TPool (const TPool&) = delete;

        virtual
        ~TPool ()
        {
          for (std::size_t i = 0; i < fSize; ++i)
            {
              delete static_cast<T*> (fArray[i]);
            }
          delete[] fArray;
          fSize = 0;
        }

        // ----------------------------------------------------------------------

        inline T*
        __attribute__((always_inline))
        aquire (void)
        {
          return static_cast<T*> (Pool::aquire ());
        }

        inline bool
        __attribute__((always_inline))
        release (T* obj)
        {
          return Pool::release (obj);
        }
      };

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_TPOOL_H_ */
