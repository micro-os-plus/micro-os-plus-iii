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

#include <cmsis-plus/posix-io/block-device.h>
#include <cmsis-plus/posix-io/device-registry.h>

#include <cmsis-plus/posix/sys/ioctl.h>

#include <cstring>
#include <cassert>
#include <cerrno>
#include <cstdarg>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    block_device::block_device (block_device_impl& impl, const char* name) :
        device
          { impl, type::block_device, name, }
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device::%s(\"%s\")=@%p\n", __func__, name_, this);
#endif

      device_registry<device>::link (this);
    }

    block_device::~block_device ()
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device::%s() @%p %s\n", __func__, this, name_);
#endif
    }

    // ------------------------------------------------------------------------

    ssize_t
    block_device::read_block (void* buf, blknum_t blknum, std::size_t nblocks)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device::%s(%p, %u, %u) @%p\n", __func__, buf,
                     blknum, nblocks, this);
#endif

      if (blknum + nblocks > impl ().num_blocks_)
        {
          errno = EINVAL;
          return -1;
        }

      return impl ().do_read_block (buf, blknum, nblocks);
    }

    ssize_t
    block_device::write_block (const void* buf, blknum_t blknum,
                               std::size_t nblocks)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device::%s(%p, %u, %u) @%p\n", __func__, buf,
                     blknum, nblocks, this);
#endif

      if (blknum + nblocks > impl ().num_blocks_)
        {
          errno = EINVAL;
          return -1;
        }

      return impl ().do_write_block (buf, blknum, nblocks);
    }

    int
    block_device::vioctl (int request, std::va_list args)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device::%s(%d) @%p\n", __func__, request, this);
#endif

      errno = 0;

      switch (request)
        {
        case BLKSSZGET:
          // Get logical device sector size (to be used for read/writes).
          {
            std::size_t* sz = va_arg(args, std::size_t*);
            if (sz == nullptr || impl ().block_logical_size_bytes_ != 0)
              {
                errno = EINVAL;
                return -1;
              }

            *sz = impl ().block_logical_size_bytes_;
            return 0;
          }

        case BLKPBSZGET:
          // Get physical device sector size (internally used for erase).
          {
            std::size_t* sz = va_arg(args, std::size_t*);
            if (sz == nullptr || impl ().block_physical_size_bytes_ != 0)
              {
                errno = EINVAL;
                return -1;
              }

            *sz = impl ().block_physical_size_bytes_;
            return 0;
          }

        case BLKGETSIZE64:
          // Get device size in bytes.
          {
            uint64_t* sz = va_arg(args, uint64_t*);
            if (sz == nullptr || impl ().num_blocks_ != 0)
              {
                errno = EINVAL;
                return -1;
              }

            *sz = ((uint64_t) impl ().num_blocks_
                * impl ().block_logical_size_bytes_);
            return 0;
          }

        default:

          // Execute the implementation specific code.
          return impl ().do_vioctl (request, args);
        }
    }

    // ========================================================================

    block_device_impl::block_device_impl (block_device& self) :
        device_impl
          { self }
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device_impl::%s()=@%p\n", __func__, this);
#endif
    }

    block_device_impl::~block_device_impl ()
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device_impl::%s() @%p\n", __func__, this);
#endif

      block_logical_size_bytes_ = 0;
      num_blocks_ = 0;
    }

    // ------------------------------------------------------------------------

    off_t
    block_device_impl::do_lseek (off_t offset, int whence)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device_impl::%s(%d, %d) @%p\n", __func__, offset,
                     whence, this);
#endif

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
    block_device_impl::do_read (void* buf, std::size_t nbyte)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device_impl::%s(%p, %u) @%p\n", __func__, buf,
                     nbyte, this);
#endif

      if ((block_logical_size_bytes_ == 0)
          || ((nbyte % block_logical_size_bytes_) != 0)
          || ((offset_ % block_logical_size_bytes_) != 0))
        {
          errno = EINVAL;
          return -1;
        }

      std::size_t nblocks = nbyte / block_logical_size_bytes_;
      blknum_t blknum = offset_ / block_logical_size_bytes_;

      ssize_t ret = self ().read_block (buf, blknum, nblocks);
      if (ret >= 0)
        {
          ret *= block_logical_size_bytes_;
        }
      return ret;
    }

    ssize_t
    block_device_impl::do_write (const void* buf, std::size_t nbyte)
    {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
      trace::printf ("block_device_impl::%s(%p, %u) @%p\n", __func__, buf,
                     nbyte, this);
#endif

      if ((block_logical_size_bytes_ == 0)
          || ((nbyte % block_logical_size_bytes_) != 0)
          || ((offset_ % block_logical_size_bytes_) != 0))
        {
          errno = EINVAL;
          return -1;
        }

      std::size_t nblocks = nbyte / block_logical_size_bytes_;
      blknum_t blknum = offset_ / block_logical_size_bytes_;

      ssize_t ret = self ().write_block (buf, blknum, nblocks);
      if (ret >= 0)
        {
          ret *= block_logical_size_bytes_;
        }
      return ret;
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
