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

#ifndef CMSIS_PLUS_POSIX_IO_FILE_H_
#define CMSIS_PLUS_POSIX_IO_FILE_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/utils/lists.h>
#include <cmsis-plus/posix/utime.h>

#include <mutex>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class file_system;
    class file_impl;

    // ========================================================================

    /**
     * @brief File class.
     * @headerfile file.h <cmsis-plus/posix-io/file.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class file : public io
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class file_system;
      friend class io;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      file (file_impl& impl, class file_system& fs);

      /**
       * @cond ignore
       */

      // The rule of five.
      file (const file&) = delete;
      file (file&&) = delete;
      file&
      operator= (const file&) = delete;
      file&
      operator= (file&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~file ();

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

      virtual int
      ftruncate (off_t length);

      virtual int
      fsync (void);

      // ----------------------------------------------------------------------
      // Support functions.

      class file_system*
      file_system (void);

      file_impl&
      impl (void) const;

      /**
       * @}
       */

    public:

      /**
       * @cond ignore
       */

      // Intrusive node used to link this file to the deferred
      // deallocation list. Must be public.
      utils::double_list_links deferred_links_;

      /**
       * @endcond
       */

    protected:

      /**
       * @cond ignore
       */

      class file_system* file_system_;

      /**
       * @endcond
       */
    };

    // ========================================================================

    class file_impl : public io_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      // friend class file_system;
      // friend class io;
      friend class file;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      file_impl (file& self);

      /**
       * @cond ignore
       */

      // The rule of five.
      file_impl (const file_impl&) = delete;
      file_impl (file_impl&&) = delete;
      file_impl&
      operator= (const file_impl&) = delete;
      file_impl&
      operator= (file_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~file_impl ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      // Implementations

      virtual int
      do_ftruncate (off_t length) = 0;

      virtual int
      do_fsync (void) = 0;

      // ----------------------------------------------------------------------

      file&
      self (void);

      /**
       * @}
       */
    };

    // ========================================================================

    template<typename T>
      class file_implementable : public file
      {
        // --------------------------------------------------------------------

      public:

        using value_type = T;

        // ---------------------------------------------------------------------

        /**
         * @name Constructors & Destructor
         * @{
         */

      public:

        file_implementable (class file_system& fs);

        /**
         * @cond ignore
         */

        // The rule of five.
        file_implementable (const file_implementable&) = delete;
        file_implementable (file_implementable&&) = delete;
        file_implementable&
        operator= (const file_implementable&) = delete;
        file_implementable&
        operator= (file_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~file_implementable ();

        /**
         * @}
         */

      protected:

        /**
         * @cond ignore
         */

        value_type impl_instance_;

        /**
         * @endcond
         */
      };

    // ========================================================================

    template<typename T, typename L>
      class file_lockable : public file
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

        file_lockable (class file_system& fs, lockable_type& locker);

        /**
         * @cond ignore
         */

        // The rule of five.
        file_lockable (const file_lockable&) = delete;
        file_lockable (file_lockable&&) = delete;
        file_lockable&
        operator= (const file_lockable&) = delete;
        file_lockable&
        operator= (file_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~file_lockable ();

        /**
         * @}
         */

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
        fstat (struct stat* buf);

        virtual off_t
        lseek (off_t offset, int whence) override;

        virtual int
        ftruncate (off_t length);

        virtual int
        fsync (void);

        /**
         * @}
         */

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

    inline file_system*
    file::file_system (void)
    {
      return file_system_;
    }

    inline file_impl&
    file::impl (void) const
    {
      return static_cast<file_impl&> (impl_);
    }

    // ========================================================================

    inline file&
    file_impl::self (void)
    {
      return static_cast<file&> (self_);
    }

    // ========================================================================

    template<typename T>
      file_implementable<T>::file_implementable (class file_system& fs) :
          file
            { impl_instance_, fs }, //
          impl_instance_
            { *this }
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_implementable::%s()=@%p\n", __func__, this);
#endif
      }

    template<typename T>
      file_implementable<T>::~file_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_implementable::%s() @%p\n", __func__, this);
#endif
      }

    // ========================================================================

    template<typename T, typename L>
      file_lockable<T, L>::file_lockable (class file_system& fs,
                                          lockable_type& locker) :
          file
            { impl_instance_, fs }, //
          impl_instance_
            { *this }, //
          locker_ (locker)
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_lockable::%s()=@%p\n", __func__, this);
#endif
      }

    template<typename T, typename L>
      file_lockable<T, L>::~file_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_lockable::%s() @%p\n", __func__, this);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      file_lockable<T, L>::close (void)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::close ();
      }

    template<typename T, typename L>
      ssize_t
      file_lockable<T, L>::read (void* buf, std::size_t nbyte)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::read (buf, nbyte);
      }

    template<typename T, typename L>
      ssize_t
      file_lockable<T, L>::write (const void* buf, std::size_t nbyte)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::write (buf, nbyte);
      }

    template<typename T, typename L>
      ssize_t
      file_lockable<T, L>::writev (const struct iovec* iov, int iovcnt)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::writev (iov, iovcnt);
      }

    template<typename T, typename L>
      int
      file_lockable<T, L>::vfcntl (int cmd, std::va_list args)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::vfcntl (cmd, args);
      }

    template<typename T, typename L>
      int
      file_lockable<T, L>::fstat (struct stat* buf)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::fstat (buf);
      }

    template<typename T, typename L>
      off_t
      file_lockable<T, L>::lseek (off_t offset, int whence)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::lseek (offset, whence);
      }

    template<typename T, typename L>
      int
      file_lockable<T, L>::ftruncate (off_t length)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::ftruncate (length);
      }

    template<typename T, typename L>
      int
      file_lockable<T, L>::fsync (void)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::fsync ();
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_FILE_H_ */
