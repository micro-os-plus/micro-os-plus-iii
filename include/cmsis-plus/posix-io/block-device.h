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

#ifndef CMSIS_PLUS_POSIX_IO_BLOCK_DEVICE_H_
#define CMSIS_PLUS_POSIX_IO_BLOCK_DEVICE_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/device.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class block_device_impl;

    // ========================================================================

    /**
     * @brief Block device class.
     * @headerfile block-device.h <cmsis-plus/posix-io/block-device.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class block_device : public device
    {
      // ----------------------------------------------------------------------

    public:

      using blknum_t = std::size_t;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      block_device (block_device_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      block_device (const block_device&) = delete;
      block_device (block_device&&) = delete;
      block_device&
      operator= (const block_device&) = delete;
      block_device&
      operator= (block_device&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~block_device () override;

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
      vioctl (int request, std::va_list args) override;

      virtual ssize_t
      read_block (void* buf, blknum_t blknum, std::size_t nblocks = 1);

      virtual ssize_t
      write_block (const void* buf, blknum_t blknum, std::size_t nblocks = 1);

      // ----------------------------------------------------------------------

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
      block_logical_size_bytes (void);

      std::size_t
      block_physical_size_bytes (void);

      // ----------------------------------------------------------------------
      // Support functions.

      block_device_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    };

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    class block_device_impl : public device_impl
    {
      // ----------------------------------------------------------------------

      friend class block_device;

    public:

      using blknum_t = block_device::blknum_t;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      block_device_impl (block_device& self);

      /**
       * @cond ignore
       */

      // The rule of five.
      block_device_impl (const block_device_impl&) = delete;
      block_device_impl (block_device_impl&&) = delete;
      block_device_impl&
      operator= (const block_device_impl&) = delete;
      block_device_impl&
      operator= (block_device_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~block_device_impl () override;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      virtual ssize_t
      do_read (void* buf, std::size_t nbyte) override;

      virtual ssize_t
      do_write (const void* buf, std::size_t nbyte) override;

      virtual off_t
      do_lseek (off_t offset, int whence) override;

      virtual ssize_t
      do_read_block (void* buf, blknum_t blknum, std::size_t nblocks) = 0;

      virtual ssize_t
      do_write_block (const void* buf, blknum_t blknum,
                      std::size_t nblocks) = 0;

      // ----------------------------------------------------------------------
      // Support functions.

      block_device&
      self (void);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      std::size_t block_logical_size_bytes_ = 0;

      std::size_t block_physical_size_bytes_ = 0;

      blknum_t num_blocks_ = 0;

      /**
       * @endcond
       */
    };

#pragma GCC diagnostic pop

    // ========================================================================

    template<typename T>
      class block_device_implementable : public block_device
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
          block_device_implementable (const char* name, Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        block_device_implementable (const block_device_implementable&) = delete;
        block_device_implementable (block_device_implementable&&) = delete;
        block_device_implementable&
        operator= (const block_device_implementable&) = delete;
        block_device_implementable&
        operator= (block_device_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~block_device_implementable ();

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
      class block_device_lockable : public block_device
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
          block_device_lockable (const char* name, lockable_type& locker,
                                 Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        block_device_lockable (const block_device_lockable&) = delete;
        block_device_lockable (block_device_lockable&&) = delete;
        block_device_lockable&
        operator= (const block_device_lockable&) = delete;
        block_device_lockable&
        operator= (block_device_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~block_device_lockable () override;

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
        close (void) override;

        virtual ssize_t
        read (void* buf, std::size_t nbyte) override;

        virtual ssize_t
        write (const void* buf, std::size_t nbyte) override;

        virtual ssize_t
        writev (const struct iovec* iov, int iovcnt) override;

        virtual int
        vfcntl (int cmd, std::va_list args) override;

        virtual int
        vioctl (int request, std::va_list args) override;

        virtual off_t
        lseek (off_t offset, int whence) override;

        virtual ssize_t
        read_block (void* buf, blknum_t blknum, std::size_t nblocks = 1)
            override;

        virtual ssize_t
        write_block (const void* buf, blknum_t blknum, std::size_t nblocks = 1)
            override;

        virtual void
        sync (void) override;

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

        value_type impl_instance_;

        lockable_type& locker_;

        /**
         * @endcond
         */
      };

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ========================================================================

    inline block_device::blknum_t
    block_device::blocks (void)
    {
      return impl ().num_blocks_;
    }

    inline std::size_t
    block_device::block_logical_size_bytes (void)
    {
      return impl ().block_logical_size_bytes_;
    }

    inline std::size_t
    block_device::block_physical_size_bytes (void)
    {
      return impl ().block_physical_size_bytes_;
    }

    inline block_device_impl&
    block_device::impl (void) const
    {
      return static_cast<block_device_impl&> (impl_);
    }

    // ========================================================================

    inline block_device&
    block_device_impl::self (void)
    {
      return static_cast<block_device&> (self_);
    }

    // ========================================================================

    template<typename T>
      template<typename ... Args>
        block_device_implementable<T>::block_device_implementable (
            const char* name, Args&&... args) :
            block_device
              { impl_instance_, name }, //
            impl_instance_
              { *this, std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
          trace::printf ("block_device_implementable::%s(\"%s\")=@%p\n",
                         __func__, name_, this);
#endif
        }

    template<typename T>
      block_device_implementable<T>::~block_device_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_implementable::%s() @%p %s\n", __func__,
                       this, name_);
#endif
      }

    template<typename T>
      typename block_device_implementable<T>::value_type&
      block_device_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

    // ========================================================================

    template<typename T, typename L>
      template<typename ... Args>
        block_device_lockable<T, L>::block_device_lockable (
            const char* name, lockable_type& locker, Args&&... args) :
            block_device
              { impl_instance_, name }, //
            impl_instance_
              { *this, std::forward<Args>(args)... }, //
            locker_ (locker)
        {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
          trace::printf ("block_device_lockable::%s(\"%s\")=@%p\n", __func__,
                         name_, this);
#endif
        }

    template<typename T, typename L>
      block_device_lockable<T, L>::~block_device_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s() @%p %s\n", __func__, this,
                       name_);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      block_device_lockable<T, L>::close (void)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s() @%p\n", __func__, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::close ();
      }

    template<typename T, typename L>
      ssize_t
      block_device_lockable<T, L>::read (void* buf, std::size_t nbyte)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(0x0%X, %u) @%p\n", __func__,
                       buf, nbyte, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::read (buf, nbyte);
      }

    template<typename T, typename L>
      ssize_t
      block_device_lockable<T, L>::write (const void* buf, std::size_t nbyte)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(0x0%X, %u) @%p\n", __func__,
                       buf, nbyte, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::write (buf, nbyte);
      }

    template<typename T, typename L>
      ssize_t
      block_device_lockable<T, L>::writev (const struct iovec* iov, int iovcnt)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(0x0%X, %d) @%p\n", __func__,
                       iov, iovcnt, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::writev (iov, iovcnt);
      }

    template<typename T, typename L>
      int
      block_device_lockable<T, L>::vfcntl (int cmd, std::va_list args)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(%d) @%p\n", __func__, cmd,
                       this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::vfcntl (cmd, args);
      }

    template<typename T, typename L>
      int
      block_device_lockable<T, L>::vioctl (int request, std::va_list args)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(%d) @%p\n", __func__, request,
                       this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::vioctl (request, args);
      }

    template<typename T, typename L>
      off_t
      block_device_lockable<T, L>::lseek (off_t offset, int whence)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(%d, %d) @%p\n", __func__,
                       offset, whence, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::lseek (offset, whence);
      }

    template<typename T, typename L>
      ssize_t
      block_device_lockable<T, L>::read_block (void* buf, blknum_t blknum,
                                               std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(%p, %u, %u) @%p\n", __func__,
                       buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::read_block (buf, blknum, nblocks);
      }

    template<typename T, typename L>
      ssize_t
      block_device_lockable<T, L>::write_block (const void* buf,
                                                blknum_t blknum,
                                                std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s(%p, %u, %u) @%p\n", __func__,
                       buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::write_block (buf, blknum, nblocks);
      }

    template<typename T, typename L>
      void
      block_device_lockable<T, L>::sync (void)
      {
#if defined(OS_TRACE_POSIX_IO_BLOCK_DEVICE)
        trace::printf ("block_device_lockable::%s() @%p\n", __func__, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return block_device::sync ();
      }

    template<typename T, typename L>
      typename block_device_lockable<T, L>::value_type&
      block_device_lockable<T, L>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_BLOCK_DEVICE_H_ */
