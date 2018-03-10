/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018 Liviu Ionescu.
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

#include <cmsis-plus/posix-io/device-block.h>
#include <cmsis-plus/posix-io/device-registry.h>

#include <cmsis-plus/posix/sys/ioctl.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <cstring>
#include <cassert>
#include <cerrno>
#include <cstdarg>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    device_block::device_block (const char* name, os::rtos::mutex* mutex) :
        device (type::block, name), //
        mutex_ (mutex)
    {
      trace::printf ("device_block::%s(\"%s\")=@%p\n", __func__, name_, this);

      device_registry<device>::link (this);
    }

    device_block::~device_block ()
    {
      trace::printf ("device_block::%s() @%p %s\n", __func__, this, name_);

      mutex_ = nullptr;
      block_logical_size_bytes_ = 0;
      num_blocks_ = 0;
    }

    // ------------------------------------------------------------------------

    ssize_t
    device_block::read_block (void* buf, blknum_t blknum, std::size_t nblocks)
    {
      trace::printf ("device_block::%s(%p, %u, %u) @%p\n", __func__, buf,
                     blknum, nblocks, this);

      if (blknum + nblocks > num_blocks_)
        {
          errno = EINVAL;
          return -1;
        }

      ssize_t ret;
      if (mutex_ != nullptr)
        {
          mutex_->lock ();
        }
      ret = do_read_block (buf, blknum, nblocks);
      if (mutex_ != nullptr)
        {
          mutex_->unlock ();
        }
      return ret;
    }

    ssize_t
    device_block::write_block (const void* buf, blknum_t blknum,
                               std::size_t nblocks)
    {
      trace::printf ("device_block::%s(%p, %u, %u) @%p\n", __func__, buf,
                     blknum, nblocks, this);

      if (blknum + nblocks > num_blocks_)
        {
          errno = EINVAL;
          return -1;
        }

      ssize_t ret;
      if (mutex_ != nullptr)
        {
          mutex_->lock ();
        }
      ret = do_write_block (buf, blknum, nblocks);
      if (mutex_ != nullptr)
        {
          mutex_->unlock ();
        }
      return ret;
    }

    int
    device_block::vioctl (int request, std::va_list args)
    {
      trace::printf ("device_block::%s(%d) @%p\n", __func__, request, this);

      errno = 0;

      switch (request)
        {
        case BLKSSZGET:
          // Get logical device sector size (to be used for read/writes).
          {
            std::size_t* sz = va_arg(args, std::size_t*);
            if (sz == nullptr || block_logical_size_bytes_ != 0)
              {
                errno = EINVAL;
                return -1;
              }

            *sz = block_logical_size_bytes_;
            return 0;
          }

        case BLKPBSZGET:
          // Get physical device sector size (internally used for erase).
          {
            std::size_t* sz = va_arg(args, std::size_t*);
            if (sz == nullptr || block_physical_size_bytes_ != 0)
              {
                errno = EINVAL;
                return -1;
              }

            *sz = block_physical_size_bytes_;
            return 0;
          }

        case BLKGETSIZE64:
          // Get device size in bytes.
          {
            uint64_t* sz = va_arg(args, uint64_t*);
            if (sz == nullptr || num_blocks_ != 0)
              {
                errno = EINVAL;
                return -1;
              }

            *sz = ((uint64_t) num_blocks_ * block_logical_size_bytes_);
            return 0;
          }

        default:

          // Execute the implementation specific code.
          int ret;
          if (mutex_ != nullptr)
            {
              mutex_->lock ();
            }
          ret = do_vioctl (request, args);
          if (mutex_ != nullptr)
            {
              mutex_->unlock ();
            }
          return ret;
        }
    }

    off_t
    device_block::do_lseek (off_t offset, int whence)
    {
      trace::printf ("device_block::%s(%d, %d) @%p\n", __func__, offset, whence,
                     this);

      errno = 0;
      off_t tmp = offset_;

      switch (whence)
        {
        case SEEK_SET:
          tmp = offset;
          break;

        case SEEK_CUR:
          tmp += offset;
          break;

        case SEEK_END:
          errno = EINVAL;
          return -1;
        }

      if (tmp < 0)
        {
          errno = EINVAL;
          return -1;
        }
      offset_ = tmp;
      return tmp;
    }

    // ------------------------------------------------------------------------

    ssize_t
    device_block::do_read (void* buf, std::size_t nbyte)
    {
      trace::printf ("device_block::%s(%p, %u) @%p\n", __func__, buf, nbyte,
                     this);

      if ((block_logical_size_bytes_ == 0)
          || ((nbyte % block_logical_size_bytes_) != 0)
          || ((offset_ % block_logical_size_bytes_) != 0))
        {
          errno = EINVAL;
          return -1;
        }

      std::size_t nblocks = nbyte / block_logical_size_bytes_;
      blknum_t blknum = offset_ / block_logical_size_bytes_;

      ssize_t ret = read_block (buf, blknum, nblocks);
      if (ret >= 0)
        {
          ret *= block_logical_size_bytes_;
        }
      return ret;
    }

    ssize_t
    device_block::do_write (const void* buf, std::size_t nbyte)
    {
      trace::printf ("device_block::%s(%p, %u) @%p\n", __func__, buf, nbyte,
                     this);

      if ((block_logical_size_bytes_ == 0)
          || ((nbyte % block_logical_size_bytes_) != 0)
          || ((offset_ % block_logical_size_bytes_) != 0))
        {
          errno = EINVAL;
          return -1;
        }

      std::size_t nblocks = nbyte / block_logical_size_bytes_;
      blknum_t blknum = offset_ / block_logical_size_bytes_;

      ssize_t ret = write_block (buf, blknum, nblocks);
      if (ret >= 0)
        {
          ret *= block_logical_size_bytes_;
        }
      return ret;
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    int
    device_block::do_vioctl (int request, std::va_list args)
    {
      errno = ENOSYS; // Not implemented
      return -1;
    }

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
