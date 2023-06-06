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

#ifndef CMSIS_PLUS_POSIX_IO_BLOCK_DEVICE_PARTITION_H_
#define CMSIS_PLUS_POSIX_IO_BLOCK_DEVICE_PARTITION_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/block-device.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class block_device_partition_impl;

    // ========================================================================

    /**
     * @brief Block device partition class.
     * @headerfile block-device-partition.h <cmsis-plus/posix-io/block-device-partitions.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class block_device_partition : public block_device
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      block_device_partition (block_device_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      block_device_partition (const block_device_partition&) = delete;
      block_device_partition (block_device_partition&&) = delete;
      block_device_partition&
      operator= (const block_device_partition&) = delete;
      block_device_partition&
      operator= (block_device_partition&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~block_device_partition () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      void
      configure (blknum_t offset, blknum_t nblocks);

      // ----------------------------------------------------------------------
      // Support functions.

      block_device_partition_impl&
      impl (void) const;

      /**
       * @}
       */
    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class block_device_partition_impl : public block_device_impl
    {
      // ----------------------------------------------------------------------

      friend block_device_partition;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      block_device_partition_impl (block_device& parent);

      /**
       * @cond ignore
       */

      // The rule of five.
      block_device_partition_impl (const block_device_partition_impl&) = delete;
      block_device_partition_impl (block_device_partition_impl&&) = delete;
      block_device_partition_impl&
      operator= (const block_device_partition_impl&) = delete;
      block_device_partition_impl&
      operator= (block_device_partition_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~block_device_partition_impl () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
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

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      block_device& parent_;

      blknum_t partition_offset_blocks_ = 0;

      /**
       * @endcond
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    template<typename T = block_device_partition_impl>
      class block_device_partition_implementable : public block_device_partition
      {
        // --------------------------------------------------------------------

      public:

        using value_type = T;

        // --------------------------------------------------------------------

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        template<typename ... Args>
          block_device_partition_implementable (const char* name,
                                                block_device& parent,
                                                Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        block_device_partition_implementable (
            const block_device_partition_implementable&) = delete;
        block_device_partition_implementable (
            block_device_partition_implementable&&) = delete;
        block_device_partition_implementable&
        operator= (const block_device_partition_implementable&) = delete;
        block_device_partition_implementable&
        operator= (block_device_partition_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~block_device_partition_implementable () override;

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        // Support functions.

        value_type&
        impl (void) const;

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

        /**
         * @endcond
         */
      };

    // ========================================================================

    template<typename T, typename L>
      class block_device_partition_lockable : public block_device_partition
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

        template<typename ... Args>
          block_device_partition_lockable (const char* name,
                                           block_device& parent,
                                           lockable_type& locker,
                                           Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        block_device_partition_lockable (const block_device_partition_lockable&) = delete;
        block_device_partition_lockable (block_device_partition_lockable&&) = delete;
        block_device_partition_lockable&
        operator= (const block_device_partition_lockable&) = delete;
        block_device_partition_lockable&
        operator= (block_device_partition_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~block_device_partition_lockable () override;

        /**
         * @}
         */

        // --------------------------------------------------------------------
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

        // --------------------------------------------------------------------
        // Support functions.

        value_type&
        impl (void) const;

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

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wweak-template-vtables"
// error: extern templates are incompatible with C++98 [-Werror,-Wc++98-compat-pedantic]
#pragma clang diagnostic ignored "-Wc++98-compat-pedantic"
#endif

    extern template class block_device_partition_implementable<
        block_device_partition_impl> ;

#pragma GCC diagnostic pop

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ========================================================================

    inline block_device_partition_impl&
    block_device_partition::impl (void) const
    {
      return static_cast<block_device_partition_impl&> (impl_);
    }

    // ========================================================================

    template<typename T>
      template<typename ... Args>
        block_device_partition_implementable<T>::block_device_partition_implementable (
            const char* name, block_device& parent, Args&&... args) :
            block_device_partition
              { impl_instance_, name }, //
            impl_instance_
              { parent, std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
          trace::printf (
              "block_device_partition_implementable::%s(\"%s\")=@%p\n",
              __func__, name_, this);
#endif
        }

    template<typename T>
      block_device_partition_implementable<T>::~block_device_partition_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
        trace::printf ("block_device_partition_implementable::%s() @%p %s\n",
                       __func__, this, name_);
#endif
      }

    template<typename T>
      typename block_device_partition_implementable<T>::value_type&
      block_device_partition_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

    // ========================================================================

    template<typename T, typename L>
      template<typename ... Args>
        block_device_partition_lockable<T, L>::block_device_partition_lockable (
            const char* name, block_device& parent, lockable_type& locker,
            Args&&... args) :
            block_device_partition
              { impl_instance_, name }, //
            impl_instance_
              { parent, std::forward<Args>(args)... }, //
            locker_ (locker)
        {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
          trace::printf ("block_device_partition_lockable::%s(\"%s\")=@%p\n",
                         __func__, name_, this);
#endif

        }

    template<typename T, typename L>
      block_device_partition_lockable<T, L>::~block_device_partition_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
        trace::printf ("block_device_partition_lockable::%s() @%p %s\n",
                       __func__, this, name_);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      block_device_partition_lockable<T, L>::vioctl (int request,
                                                     std::va_list args)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
        trace::printf ("block_device_partition_lockable::%s(%d) @%p\n",
                       __func__, request, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device_partition::vioctl (request, args);
      }

    template<typename T, typename L>
      ssize_t
      block_device_partition_lockable<T, L>::read_block (void* buf,
                                                         blknum_t blknum,
                                                         std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
        trace::printf ("block_device_partition_lockable::%s(%p, %u, %u) @%p\n",
                       __func__, buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device_partition::read_block (buf, blknum, nblocks);
      }

    template<typename T, typename L>
      ssize_t
      block_device_partition_lockable<T, L>::write_block (const void* buf,
                                                          blknum_t blknum,
                                                          std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE_PARTITION)
        trace::printf ("block_device_partition_lockable::%s(%p, %u, %u) @%p\n",
                       __func__, buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device_partition::write_block (buf, blknum, nblocks);
      }

    template<typename T, typename L>
      typename block_device_partition_lockable<T, L>::value_type&
      block_device_partition_lockable<T, L>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#pragma GCC diagnostic pop

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_BLOCK_DEVICE_PARTITION_H_ */
