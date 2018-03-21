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

#ifndef CMSIS_PLUS_POSIX_IO_DIRECTORY_H_
#define CMSIS_PLUS_POSIX_IO_DIRECTORY_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cmsis-plus/utils/lists.h>
#include <cmsis-plus/estd/mutex>

#include <cmsis-plus/posix/dirent.h>

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

    /**
     * @ingroup cmsis-plus-posix-io-func
     * @{
     */

    /**
     * @brief Open directory.
     * @param dirname [in] Directory name.
     * @return Pointer to `directory` object.
     */
    directory*
    opendir (const char* dirname);

    /**
     * @}
     */

    // ========================================================================
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Directory class.
     * @headerfile directory.h <cmsis-plus/posix-io/directory.h>
     * @ingroup cmsis-plus-posix-io-base
     */
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

      directory (directory_impl& impl, class file_system& fs);

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
      virtual struct dirent *
      read (void);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html
      virtual void
      rewind (void);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html
      virtual int
      close (void);

      // ----------------------------------------------------------------------
      // Support functions.

      struct dirent*
      dir_entry (void);

      class file_system*
      file_system (void) const;

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

    protected:

      /**
       * @cond ignore
       */

      directory_impl& impl_;

      class file_system* file_system_;

      /**
       * @endcond
       */
    };

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

      directory_impl (directory& self);

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
      virtual struct dirent*
      do_read (void) = 0;

      virtual void
      do_rewind (void) = 0;

      virtual int
      do_close (void) = 0;

      // ----------------------------------------------------------------------

      directory&
      self (void);

      /**
       * @}
       */

    protected:

      /**
       * @cond ignore
       */

      directory& self_;

      // This also solves the readdir() re-entrancy issue.
      struct dirent dir_entry_;

      /**
       * @endcond
       */
    };

    // ========================================================================

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

        directory_implementable (class file_system& fs);

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
        ~directory_implementable ();

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

        directory_lockable (class file_system& fs, lockable_type& locker);

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
        ~directory_lockable ();

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
        virtual struct dirent *
        read (void) override;

        // http://pubs.opengroup.org/onlinepubs/9699919799/functions/rewinddir.html
        virtual void
        rewind (void) override;

        // http://pubs.opengroup.org/onlinepubs/9699919799/functions/closedir.html
        virtual int
        close (void) override;

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

    inline file_system*
    directory::file_system (void) const
    {
      return file_system_;
    }

    inline struct dirent*
    directory::dir_entry (void)
    {
      return &(impl ().dir_entry_);
    }

    inline directory_impl&
    directory::impl (void) const
    {
      return static_cast<directory_impl&> (impl_);
    }

    // ========================================================================

    inline directory&
    directory_impl::self (void)
    {
      return static_cast<directory&> (self_);
    }

    // ========================================================================

    template<typename T>
      directory_implementable<T>::directory_implementable (
          class file_system& fs) :
          directory
            { impl_instance_, fs }, //
          impl_instance_
            { *this }
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_implementable::%s()=@%p\n", __func__, this);
#endif
      }

    template<typename T>
      directory_implementable<T>::~directory_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_implementable::%s() @%p\n", __func__, this);
#endif
      }

    // ========================================================================

    template<typename T, typename L>
      directory_lockable<T, L>::directory_lockable (class file_system& fs,
                                                    lockable_type& locker) :
          directory
            { impl_instance_, fs }, //
          impl_instance_
            { *this }, //
          locker_ (locker)
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s()=@%p\n", __func__, this);
#endif
      }

    template<typename T, typename L>
      directory_lockable<T, L>::~directory_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      struct dirent *
      directory_lockable<T, L>::read (void)
      {
#if defined(OS_TRACE_POSIX_IO_DIRECTORY)
        trace::printf ("directory_lockable::%s() @%p\n", __func__, this);
#endif

        estd::lock_guard<L> lock
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

        estd::lock_guard<L> lock
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

        estd::lock_guard<L> lock
          { locker_ };

        return directory::close ();
      }

  // ========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DIRECTORY_H_ */
