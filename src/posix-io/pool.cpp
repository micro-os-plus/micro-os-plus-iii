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

#include <cmsis-plus/posix-io/pool.h>

namespace os
{
  namespace posix
  {

    // ------------------------------------------------------------------------

    pool::pool (std::size_t size)
    {
      size_ = size;
      in_use_ = new bool[size];
      for (std::size_t i = 0; i < size_; ++i)
        {
          in_use_[i] = false;
        }
      // The derived class must alloc and set this pointer.
      array_ = nullptr;
    }

    pool::~pool ()
    {
      delete[] in_use_;
    }

    // ------------------------------------------------------------------------

    void*
    pool::acquire (void)
    {
      for (std::size_t i = 0; i < size_; ++i)
        {
          if (!in_use_[i])
            {
              in_use_[i] = true;
              return array_[i];
            }
        }

      return nullptr;
    }

    bool
    pool::release (void* file)
    {
      for (std::size_t i = 0; i < size_; ++i)
        {
          if (in_use_[i] && (array_[i] == file))
            {
              in_use_[i] = false;
              return true;
            }
        }
      return false;
    }

  } /* namespace posix */
} /* namespace os */
