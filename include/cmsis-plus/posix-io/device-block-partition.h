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

#ifndef CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_PARTITION_H_
#define CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_PARTITION_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/device-block.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class device_block_partition_impl;

    // ========================================================================

    /**
     * @brief Block device partition class.
     * @headerfile device-block-partition.h <cmsis-plus/posix-io/device-block-partitions.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class device_block_partition : public device_block
    {
      // ----------------------------------------------------------------------

    public:

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_block_partition (device_block_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_block_partition (const device_block_partition&) = delete;
      device_block_partition (device_block_partition&&) = delete;
      device_block_partition&
      operator= (const device_block_partition&) = delete;
      device_block_partition&
      operator= (device_block_partition&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~device_block_partition ();

      /**
       * @}
       */

      /**
       * @name Public Member Functions
       * @{
       */

    public:

      void
      configure (blknum_t offset, blknum_t nblocks);

      device_block_partition_impl&
      impl (void) const;

      /**
       * @}
       */
    };

    // ========================================================================

    class device_block_partition_impl : public device_block_impl
    {
      // ----------------------------------------------------------------------

      friend device_block_partition;

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_block_partition_impl (device_block_partition& self,
                                   device_block& parent);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_block_partition_impl (const device_block_partition_impl&) = delete;
      device_block_partition_impl (device_block_partition_impl&&) = delete;
      device_block_partition_impl&
      operator= (const device_block_partition_impl&) = delete;
      device_block_partition_impl&
      operator= (device_block_partition_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~device_block_partition_impl ();

      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual int
      do_vioctl (int request, std::va_list args) override;

      virtual int
      do_vopen (const char* path, int oflag, std::va_list args) override;

      virtual ssize_t
      do_read_block (void* buf, blknum_t blknum, std::size_t nblocks) override;

      virtual ssize_t
      do_write_block (const void* buf, blknum_t blknum, std::size_t nblocks)
          override;

      virtual void
      do_sync (void) override;

      virtual int
      do_close (void) override;

      // ----------------------------------------------------------------------

      void
      configure (blknum_t offset, blknum_t nblocks);

      device_block_partition&
      self (void);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      device_block& parent_;

      blknum_t partition_offset_blocks_ = 0;

      /**
       * @endcond
       */
    };

    // ========================================================================

    template<typename T = device_block_partition_impl>
      class device_block_partition_implementable : public device_block_partition
      {
        // --------------------------------------------------------------------

      public:

        using value_type = T;

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        device_block_partition_implementable (const char* name,
                                              device_block& parent);

        /**
         * @cond ignore
         */

        // The rule of five.
        device_block_partition_implementable (
            const device_block_partition_implementable&) = delete;
        device_block_partition_implementable (
            device_block_partition_implementable&&) = delete;
        device_block_partition_implementable&
        operator= (const device_block_partition_implementable&) = delete;
        device_block_partition_implementable&
        operator= (device_block_partition_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~device_block_partition_implementable ();

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        // Include the implementation as a member.
        value_type impl_instance_;

        /**
         * @endcond
         */
      };

    // ========================================================================

    template<typename T, typename L>
      class device_block_partition_lockable : public device_block_partition
      {
        // --------------------------------------------------------------------

      public:

        using value_type = T;
        using lockable_type = L;

        // --------------------------------------------------------------------

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        device_block_partition_lockable (const char* name, device_block& parent,
                                         lockable_type& locker);

        /**
         * @cond ignore
         */

        // The rule of five.
        device_block_partition_lockable (const device_block_partition_lockable&) = delete;
        device_block_partition_lockable (device_block_partition_lockable&&) = delete;
        device_block_partition_lockable&
        operator= (const device_block_partition_lockable&) = delete;
        device_block_partition_lockable&
        operator= (device_block_partition_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~device_block_partition_lockable ();

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

        virtual ssize_t
        read_block (void* buf, blknum_t blknum, std::size_t nblocks = 1)
            override;

        virtual ssize_t
        write_block (const void* buf, blknum_t blknum, std::size_t nblocks = 1)
            override;

        /**
         * @}
         */

        // --------------------------------------------------------------------
      protected:

        /**
         * @cond ignore
         */

        // Include the implementation as a member.
        value_type impl_instance_;

        lockable_type& locker_;

        /**
         * @endcond
         */
      };

    // ========================================================================

    extern template class device_block_partition_implementable<
        device_block_partition_impl> ;

  // ========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ========================================================================

    inline device_block_partition_impl&
    device_block_partition::impl (void) const
    {
      return static_cast<device_block_partition_impl&> (impl_);
    }

    // ========================================================================

    template<typename T>
      device_block_partition_implementable<T>::device_block_partition_implementable (
          const char* name, device_block& parent) :
          device_block_partition
            { impl_instance_, name }, //
          impl_instance_
            { *this, parent }
      {
        trace::printf ("device_block_partition_implementable::%s(\"%s\")=@%p\n",
                       __func__, name_, this);
      }

    template<typename T>
      device_block_partition_implementable<T>::~device_block_partition_implementable ()
      {
        trace::printf ("device_block_partition_implementable::%s() @%p %s\n",
                       __func__, this, name_);
      }

    // ========================================================================

    template<typename T, typename L>
      device_block_partition_lockable<T, L>::device_block_partition_lockable (
          const char* name, device_block& parent, lockable_type& locker) :
          device_block_partition
            { impl_instance_, name }, //
          impl_instance_
            { *this, parent }, //
          locker_ (locker)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK_PARTITION)
        trace::printf ("device_block_partition_lockable::%s(\"%s\")=@%p\n",
                       __func__, name_, this);
#endif

      }

    template<typename T, typename L>
      device_block_partition_lockable<T, L>::~device_block_partition_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK_PARTITION)
        trace::printf ("device_block_partition_lockable::%s() @%p %s\n",
                       __func__, this, name_);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      device_block_partition_lockable<T, L>::vioctl (int request,
                                                     std::va_list args)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK_PARTITION)
        trace::printf ("device_block_partition_lockable::%s(%d) @%p\n",
                       __func__, request, this);
#endif

        std::lock_guard<L> lock (locker_);

        return device_block_partition::vioctl (request, args);
      }

    template<typename T, typename L>
      ssize_t
      device_block_partition_lockable<T, L>::read_block (void* buf,
                                                         blknum_t blknum,
                                                         std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK_PARTITION)
        trace::printf ("device_block_partition_lockable::%s(%p, %u, %u) @%p\n",
                       __func__, buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock (locker_);

        return device_block_partition::read_block (buf, blknum, nblocks);
      }

    template<typename T, typename L>
      ssize_t
      device_block_partition_lockable<T, L>::write_block (const void* buf,
                                                          blknum_t blknum,
                                                          std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK_PARTITION)
        trace::printf ("device_block_partition_lockable::%s(%p, %u, %u) @%p\n",
                       __func__, buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock (locker_);

        return device_block_partition::write_block (buf, blknum, nblocks);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_PARTITION_H_ */
