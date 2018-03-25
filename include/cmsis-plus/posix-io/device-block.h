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

    class device_block_impl;

    // ========================================================================

    /**
     * @brief Block device class.
     * @headerfile device-block.h <cmsis-plus/posix-io/device-block.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class device_block : public device
    {
      // ----------------------------------------------------------------------

    public:

      // TODO: define a way to change this to uint64_t;
      using blknum_t = uint32_t;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_block (device_block_impl& impl, const char* name);

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

      device_block_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    };

    // ========================================================================

    class device_block_impl : public device_impl
    {
      // ----------------------------------------------------------------------

      friend class device_block;

    public:

      using blknum_t = device_block::blknum_t;

      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      device_block_impl (device_block& self);

      /**
       * @cond ignore
       */

      // The rule of five.
      device_block_impl (const device_block_impl&) = delete;
      device_block_impl (device_block_impl&&) = delete;
      device_block_impl&
      operator= (const device_block_impl&) = delete;
      device_block_impl&
      operator= (device_block_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~device_block_impl ();

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

      device_block&
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

    // ========================================================================

    template<typename T>
      class device_block_implementable : public device_block
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
          device_block_implementable (const char* name, Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        device_block_implementable (const device_block_implementable&) = delete;
        device_block_implementable (device_block_implementable&&) = delete;
        device_block_implementable&
        operator= (const device_block_implementable&) = delete;
        device_block_implementable&
        operator= (device_block_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~device_block_implementable ();

        /**
         * @}
         */

        // ----------------------------------------------------------------------
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

        // ----------------------------------------------------------------------
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
      class device_block_lockable : public device_block
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
          device_block_lockable (const char* name, lockable_type& locker,
                                 Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        device_block_lockable (const device_block_lockable&) = delete;
        device_block_lockable (device_block_lockable&&) = delete;
        device_block_lockable&
        operator= (const device_block_lockable&) = delete;
        device_block_lockable&
        operator= (device_block_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~device_block_lockable ();

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

    inline device_block::blknum_t
    device_block::blocks (void)
    {
      return impl ().num_blocks_;
    }

    inline std::size_t
    device_block::block_logical_size_bytes (void)
    {
      return impl ().block_logical_size_bytes_;
    }

    inline std::size_t
    device_block::block_physical_size_bytes (void)
    {
      return impl ().block_physical_size_bytes_;
    }

    inline device_block_impl&
    device_block::impl (void) const
    {
      return static_cast<device_block_impl&> (impl_);
    }

    // ========================================================================

    inline device_block&
    device_block_impl::self (void)
    {
      return static_cast<device_block&> (self_);
    }

    // ========================================================================

    template<typename T>
      template<typename ... Args>
        device_block_implementable<T>::device_block_implementable (
            const char* name, Args&&... args) :
            device_block
              { impl_instance_, name }, //
            impl_instance_
              { *this, std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
          trace::printf ("device_block_implementable::%s(\"%s\")=@%p\n",
                         __func__, name_, this);
#endif
        }

    template<typename T>
      device_block_implementable<T>::~device_block_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_implementable::%s() @%p %s\n", __func__,
                       this, name_);
#endif
      }

    template<typename T>
      typename device_block_implementable<T>::value_type&
      device_block_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

    // ========================================================================

    template<typename T, typename L>
      template<typename ... Args>
        device_block_lockable<T, L>::device_block_lockable (
            const char* name, lockable_type& locker, Args&&... args) :
            device_block
              { impl_instance_, name }, //
            impl_instance_
              { *this, std::forward<Args>(args)... }, //
            locker_ (locker)
        {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
          trace::printf ("device_block_lockable::%s(\"%s\")=@%p\n", __func__,
                         name_, this);
#endif
        }

    template<typename T, typename L>
      device_block_lockable<T, L>::~device_block_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s() @%p %s\n", __func__, this,
                       name_);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      device_block_lockable<T, L>::close (void)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s() @%p\n", __func__, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::close ();
      }

    template<typename T, typename L>
      ssize_t
      device_block_lockable<T, L>::read (void* buf, std::size_t nbyte)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(0x0%X, %u) @%p\n", __func__,
                       buf, nbyte, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::read (buf, nbyte);
      }

    template<typename T, typename L>
      ssize_t
      device_block_lockable<T, L>::write (const void* buf, std::size_t nbyte)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(0x0%X, %u) @%p\n", __func__,
                       buf, nbyte, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::write (buf, nbyte);
      }

    template<typename T, typename L>
      ssize_t
      device_block_lockable<T, L>::writev (const struct iovec* iov, int iovcnt)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(0x0%X, %d) @%p\n", __func__,
                       iov, iovcnt, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::writev (iov, iovcnt);
      }

    template<typename T, typename L>
      int
      device_block_lockable<T, L>::vfcntl (int cmd, std::va_list args)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(%d) @%p\n", __func__, cmd,
                       this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::vfcntl (cmd, args);
      }

    template<typename T, typename L>
      int
      device_block_lockable<T, L>::vioctl (int request, std::va_list args)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(%d) @%p\n", __func__, request,
                       this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::vioctl (request, args);
      }

    template<typename T, typename L>
      off_t
      device_block_lockable<T, L>::lseek (off_t offset, int whence)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(%d, %d) @%p\n", __func__,
                       offset, whence, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::lseek (offset, whence);
      }

    template<typename T, typename L>
      ssize_t
      device_block_lockable<T, L>::read_block (void* buf, blknum_t blknum,
                                               std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(%p, %u, %u) @%p\n", __func__,
                       buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::read_block (buf, blknum, nblocks);
      }

    template<typename T, typename L>
      ssize_t
      device_block_lockable<T, L>::write_block (const void* buf,
                                                blknum_t blknum,
                                                std::size_t nblocks)
      {
#if defined(OS_TRACE_POSIX_IO_DEVICE_BLOCK)
        trace::printf ("device_block_lockable::%s(%p, %u, %u) @%p\n", __func__,
                       buf, blknum, nblocks, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return device_block::write_block (buf, blknum, nblocks);
      }

    template<typename T, typename L>
      typename device_block_lockable<T, L>::value_type&
      device_block_lockable<T, L>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DEVICE_BLOCK_H_ */
