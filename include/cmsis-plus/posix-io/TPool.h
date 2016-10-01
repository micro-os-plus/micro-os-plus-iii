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
