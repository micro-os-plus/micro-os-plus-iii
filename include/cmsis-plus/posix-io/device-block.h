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

#ifndef CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_H_
#define CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/posix-io/device.h>
#include <cmsis-plus/utils/lists.h>

// ----------------------------------------------------------------------------

#if ! defined(OS_STRING_POSIX_DEVICE_BLOCK_PREFIX)
#define OS_STRING_POSIX_DEVICE_BLOCK_PREFIX "/bdev/"
#endif

namespace os
{
  namespace rtos
  {
    class mutex;
  }
}
// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    /**
     * @brief Block device class.
     * @headerfile device-block.h <cmsis-plus/posix-io/device-block.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class device_block : public device
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend io*
      vopen (const char* path, int oflag, std::va_list args);

      /**
       * @endcond
       */

    public:

      // TODO: define a way to change this to uint64_t;
      using blknum_t = uint32_t;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_block (const char* name, os::rtos::mutex* mutex = nullptr);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_block (const device_block&) = delete;
      device_block (device_block&&) = delete;
      device_block&
      operator= (const device_block&) = delete;
      device_block&
      operator= (device_block&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~device_block ();

      /**
       * @}
       */

      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual int
      vioctl (int request, std::va_list args) override;

      ssize_t
      read_block (void* buf, blknum_t blknum, std::size_t nblocks = 1);

      ssize_t
      write_block (const void* buf, blknum_t blknum, std::size_t nblocks = 1);

      /**
       *
       * @return The number of blocks.
       */
      blknum_t
      blocks (void);

      /**
       *
       * @return The number of bytes in a block.
       */
      std::size_t
      block_size_bytes (void);

      // ----------------------------------------------------------------------

      static const char*
      device_prefix (void);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Private Member Functions
       * @{
       */

    protected:

      // Derived classes **must** set `block_size_` and `num_blocks_`.
      virtual int
      do_vopen (const char* path, int oflag, std::va_list args) = 0;

      virtual int
      do_vioctl (int request, std::va_list args) override;

      virtual ssize_t
      do_read (void* buf, std::size_t nbyte) override;

      virtual ssize_t
      do_write (const void* buf, std::size_t nbyte) override;

      virtual ssize_t
      do_read_block (void* buf, blknum_t blknum, std::size_t nblocks) = 0;

      virtual ssize_t
      do_write_block (const void* buf, blknum_t blknum,
                      std::size_t nblocks) = 0;

      virtual int
      do_isatty (void) final;

      virtual off_t
      do_lseek (off_t offset, int whence) override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      os::rtos::mutex* mutex_ = nullptr;
      std::size_t block_size_bytes_ = 0;
      blknum_t num_blocks_ = 0;

      /**
       * @endcond
       */
    };

  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    inline device_block::blknum_t
    device_block::blocks (void)
    {
      return num_blocks_;
    }

    inline std::size_t
    device_block::block_size_bytes (void)
    {
      return block_size_bytes_;
    }

    inline const char*
    device_block::device_prefix (void)
    {
      return OS_STRING_POSIX_DEVICE_BLOCK_PREFIX;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_H_ */
