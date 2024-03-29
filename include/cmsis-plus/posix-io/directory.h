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

#ifndef CMSIS_PLUS_POSIX_IO_DIRECTORY_H_
#define CMSIS_PLUS_POSIX_IO_DIRECTORY_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/utils/lists.h>
#include <cmsis-plus/posix/dirent.h>

#include <cmsis-plus/diag/trace.h>

#include <mutex>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class directory;
    class directory_impl;
    class file_system;

    // ------------------------------------------------------------------------

    // ========================================================================

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wpadded"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wpadded"
#endif

    /**
     * @brief Directory class.
     * @headerfile directory.h <cmsis-plus/posix-io/directory.h>
     * @ingroup cmsis-plus-posix-io-base
     */
#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#pragma GCC diagnostic ignored "-Wsuggest-final-types"
#endif
    class directory
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class file_system;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      directory (directory_impl& impl);

      /**
       * @cond ignore
       */

      // The rule of five.
      directory (const directory&) = delete;
      directory (directory&&) = delete;
      directory&
      operator= (const directory&) = delete;
      directory&
      operator= (directory&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~directory ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html
      virtual /* struct */ dirent *
      read (void);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html
      virtual void
      rewind (void);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html
      virtual int
      close (void);

      // ----------------------------------------------------------------------
      // Support functions.

      /* struct */ dirent*
      dir_entry (void);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif

      class file_system&
      get_file_system (void) const;

#pragma GCC diagnostic pop

      directory_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    public:

      /**
       * @cond ignore
       */

      // Intrusive node used to link this device to the deferred
      // deallocation list. Must be public.
      utils::double_list_links deferred_links_;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      directory_impl& impl_;

      /**
       * @endcond
       */
    };
#pragma GCC diagnostic pop

    // ========================================================================

    class directory_impl
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend class directory;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      directory_impl (/* class */ file_system& fs);

      /**
       * @cond ignore
       */

      // The rule of five.
      directory_impl (const directory_impl&) = delete;
      directory_impl (directory_impl&&) = delete;
      directory_impl&
      operator= (const directory_impl&) = delete;
      directory_impl&
      operator= (directory_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~directory_impl ();

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

      /**
       * @return object if successful, otherwise nullptr and errno.
       */
      virtual /* struct */ dirent*
      do_read (void) = 0;

      virtual void
      do_rewind (void) = 0;

      virtual int
      do_close (void) = 0;

      // ----------------------------------------------------------------------
      // Support functions.

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif

      class file_system&
      get_file_system (void) const;

#pragma GCC diagnostic pop

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      // This also solves the readdir() re-entrancy issue.
      /* struct */ dirent dir_entry_;

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
      class directory_implementable : public directory
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

        directory_implementable (/* class */ file_system& fs);

        /**
         * @cond ignore
         */

        // The rule of five.
        directory_implementable (const directory_implementable&) = delete;
        directory_implementable (directory_implementable&&) = delete;
        directory_implementable&
        operator= (const directory_implementable&) = delete;
        directory_implementable&
        operator= (directory_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~directory_implementable () override;

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
#endif
    template<typename T, typename L>
      class directory_lockable : public directory
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

        directory_lockable (/* class */ file_system& fs, lockable_type& locker);

        /**
         * @cond ignore
         */

        // The rule of five.
        directory_lockable (const directory_lockable&) = delete;
        directory_lockable (directory_lockable&&) = delete;
        directory_lockable&
        operator= (const directory_lockable&) = delete;
        directory_lockable&
        operator= (directory_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~directory_lockable () override;

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        // opendir() uses the file system lock.

        // http://pubs.opengroup.org/onlinepubs/9699919799/functions/readdir.html
        virtual /* struct */ dirent *
        read (void) override;

        // http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html
        virtual void
        rewind (void) override;

        // http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html
        virtual int
        close (void) override;

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
    directory::get_file_system (void) const
    {
      return impl ().get_file_system ();
    }

    inline /* struct */ dirent*
    directory::dir_entry (void)
    {
      return &(impl ().dir_entry_);
    }

    inline directory_impl&
    directory::impl (void) const
    {
      return /* static_cast<directory_impl&> */ (impl_);
    }

    // ========================================================================

    inline file_system&
    directory_impl::get_file_system (void) const
    {
      return file_system_;
    }

    // ========================================================================

    template<typename T>
      directory_implementable<T>::directory_implementable (
          /* class */ file_system& fs) :
          directory
            { impl_instance_ }, //
          impl_instance_
            { fs }
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_implementable::%s()=@%p\n", __func__, this);
#endif
      }

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif
    template<typename T>
      directory_implementable<T>::~directory_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_implementable::%s() @%p\n", __func__, this);
#endif
      }
#pragma GCC diagnostic pop

    template<typename T>
      typename directory_implementable<T>::value_type&
      directory_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

    // ========================================================================

    template<typename T, typename L>
      directory_lockable<T, L>::directory_lockable (/* class */ file_system& fs,
                                                    lockable_type& locker) :
          directory
            { impl_instance_ }, //
          impl_instance_
            { fs }, //
          locker_ (locker)
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s()=@%p\n", __func__, this);
#endif
      }

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsuggest-final-methods"
#endif
    template<typename T, typename L>
      directory_lockable<T, L>::~directory_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif
      }
#pragma GCC diagnostic pop

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      /* struct */ dirent *
      directory_lockable<T, L>::read (void)
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return directory::read ();
      }

    template<typename T, typename L>
      void
      directory_lockable<T, L>::rewind (void)
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return directory::rewind ();
      }

    template<typename T, typename L>
      int
      directory_lockable<T, L>::close (void)
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif

        std::lock_guard<L> lock
          { locker_ };

        return directory::close ();
      }

    template<typename T, typename L>
      typename directory_lockable<T, L>::value_type&
      directory_lockable<T, L>::impl (void) const
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

#endif /* CMSIS_PLUS_POSIX_IO_DIRECTORY_H_ */
