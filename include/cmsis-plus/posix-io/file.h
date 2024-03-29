/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#ifndef CMSIS_PLUS_POSIX_IO_FILE_H_
#define CMSIS_PLUS_POSIX_IO_FILE_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/utils/lists.h>
#include <cmsis-plus/posix/utime.h>
#include <cmsis-plus/posix/sys/statvfs.h>

#include <mutex>

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

    class file_system;
    class file_impl;

    // ========================================================================

    /**
     * @brief File class.
     * @headerfile file.h <cmsis-plus/posix-io/file.h>
     * @ingroup cmsis-plus-posix-io-base
     */
#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#endif
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

      file (file_impl& impl);

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
      ~file () override;

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

      virtual int
      fstatvfs (struct statvfs *buf);

      // ----------------------------------------------------------------------
      // Support functions.

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif

      class file_system&
      get_file_system (void);

#pragma GCC diagnostic pop

      file_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
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
    };
#pragma GCC diagnostic pop

    // ========================================================================

    class file_impl : public io_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

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

      file_impl (/* class */ file_system& fs);

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
      ~file_impl () override;

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
      // Support functions.

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif

      class file_system&
      get_file_system (void);

#pragma GCC diagnostic pop

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      /* class */ file_system& file_system_;

      /**
       * @endcond
       */
    };

    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#endif
    template<typename T>
      class file_implementable : public file
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

        file_implementable (/* class */ file_system& fs);

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
        ~file_implementable () override;

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

        value_type impl_instance_;

        /**
         * @endcond
         */
      };
#pragma GCC diagnostic pop

    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#pragma GCC diagnostic ignored "-Wpadded"
#endif
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

        file_lockable (/* class */ file_system& fs, lockable_type& locker);

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
        ~file_lockable () override;

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
        writev (const /* struct */ iovec* iov, int iovcnt) override;

        virtual int
        vfcntl (int cmd, std::va_list args) override;

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif
        virtual int
        fstat (struct stat* buf) override;
#pragma GCC diagnostic pop

        virtual off_t
        lseek (off_t offset, int whence) override;

        virtual int
        ftruncate (off_t length) override;

        virtual int
        fsync (void) override;

        // fstatvfs() - must not be locked, since will be locked by the
        // file system. (otherwise non-recursive mutexes will fail).

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

    inline file_system&
    file::get_file_system (void)
    {
      return impl ().get_file_system ();
    }

    inline file_impl&
    file::impl (void) const
    {
      return static_cast<file_impl&> (impl_);
    }

    // ========================================================================

    inline /* class */ file_system&
    file_impl::get_file_system (void)
    {
      return file_system_;
    }

    // ========================================================================

    template<typename T>
      file_implementable<T>::file_implementable (/* class */ file_system& fs) :
          file
            { impl_instance_ }, //
          impl_instance_
            { fs }
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_implementable::%s()=@%p\n", __func__, this);
#endif
      }

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif
    template<typename T>
      file_implementable<T>::~file_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_implementable::%s() @%p\n", __func__, this);
#endif
      }
#pragma GCC diagnostic pop

    template<typename T>
      typename file_implementable<T>::value_type&
      file_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

    // ========================================================================

    template<typename T, typename L>
      file_lockable<T, L>::file_lockable (/* class */ file_system& fs,
                                          lockable_type& locker) :
          file
            { impl_instance_ }, //
          impl_instance_
            { fs }, //
          locker_ (locker)
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_lockable::%s()=@%p\n", __func__, this);
#endif
      }

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif
    template<typename T, typename L>
      file_lockable<T, L>::~file_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_FILE)
        trace::printf ("file_lockable::%s() @%p\n", __func__, this);
#endif
      }
#pragma GCC diagnostic pop

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
      file_lockable<T, L>::writev (const /* struct */ iovec* iov, int iovcnt)
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

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif
    template<typename T, typename L>
      int
      file_lockable<T, L>::fstat (struct stat* buf)
      {
        std::lock_guard<L> lock
          { locker_ };

        return file::fstat (buf);
      }
#pragma GCC diagnostic pop

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

    template<typename T, typename L>
      typename file_lockable<T, L>::value_type&
      file_lockable<T, L>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_IO_FILE_H_ */
