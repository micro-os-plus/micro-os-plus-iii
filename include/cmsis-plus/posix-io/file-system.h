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

#ifndef CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_
#define CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/file.h>
#include <cmsis-plus/posix-io/directory.h>

#include <cmsis-plus/utils/lists.h>

#include <cmsis-plus/diag/trace.h>

#include <mutex>
#include <cstdarg>
#include <sys/stat.h>
#include <utime.h>

// ----------------------------------------------------------------------------

#define FF_MOUNT_FLAGS_HAS_VOLUME   (1)

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class io;
    class file;
    class directory;
    class block_device;

    class file_system_impl;

    /**
     * @ingroup cmsis-plus-posix-io-func
     * @{
     */

    // ------------------------------------------------------------------------
    // ----- Non-io, global file system functions -----
    int
    mkdir (const char* path, mode_t mode);

    int
    rmdir (const char* path);

    void
    sync (void);

    // ------------------------------------------------------------------------
    // ----- Non-io, file functions -----

    int
    chmod (const char* path, mode_t mode);

    int
    stat (const char* path, struct stat* buf);

    int
    truncate (const char* path, off_t length);

    int
    rename (const char* existing, const char* _new);

    int
    unlink (const char* path);

    int
    utime (const char* path, const struct utimbuf* times);

    int
    statvfs (const char* path, struct statvfs* buf);

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
    /**
     * @brief File system class.
     * @headerfile file-system.h <cmsis-plus/posix-io/file-system.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class file_system
    {
      // ----------------------------------------------------------------------

      /**
       * @cond ignore
       */

      friend int
      mkdir (const char* path, mode_t mode);

      friend int
      rmdir (const char* path);

      friend void
      sync (void);

      friend int
      chmod (const char* path, mode_t mode);

      friend int
      stat (const char* path, struct stat* buf);

      friend int
      truncate (const char* path, off_t length);

      friend int
      rename (const char* existing, const char* _new);

      friend int
      unlink (const char* path);

      friend int
      utime (const char* path, const struct utimbuf* times);

      friend int
      statvfs (struct statvfs* buf);

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      file_system (file_system_impl& impl, const char* name);

      /**
       * @cond ignore
       */

      // The rule of five.
      file_system (const file_system&) = delete;
      file_system (file_system&&) = delete;
      file_system&
      operator= (const file_system&) = delete;
      file_system&
      operator= (file_system&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~file_system ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      int
      mkfs (int options, ...);

      virtual int
      vmkfs (int options, std::va_list args);

      int
      mount (const char* path = nullptr, unsigned int flags = 0, ...);

      /**
       * @brief Mount file system.
       *
       * @param path Path, terminated in `/`. If `/` or nullptr, the
       *   file system is mounted as root, i.e. the default if no other
       *   mount point matches.
       * @param flags File system specific flags.
       * @param args Optional arguments.
       * @retval 0 if successful,
       * @retval -1 otherwise and the variable errno is set to
       *   indicate the error.
       */
      virtual int
      vmount (const char* path, unsigned int flags, std::va_list args);

      /**
       * @brief Unmount file system.
       *
       * @param flags File system specific flags.
       * @retval 0 if successful,
       * @retval -1 otherwise and the variable errno is set to
       *   indicate the error.
       */
      virtual int
      umount (int unsigned flags = 0);

      static file_system*
      identify_mounted (const char** path1, const char** path2 = nullptr);

      // ----------------------------------------------------------------------

      file*
      open (const char* path = nullptr, int oflag = 0, ...);

      virtual file*
      vopen (const char* path, int oflag, std::va_list args);

      // http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html
      virtual directory*
      opendir (const char* dirpath);

      // ----------------------------------------------------------------------
      // Also called from namespace friend functions.

      virtual int
      mkdir (const char* path, mode_t mode);

      virtual int
      rmdir (const char* path);

      virtual void
      sync (void);

      virtual int
      chmod (const char* path, mode_t mode);

      virtual int
      stat (const char* path, struct stat* buf);

      virtual int
      truncate (const char* path, off_t length);

      virtual int
      rename (const char* existing, const char* _new);

      virtual int
      unlink (const char* path);

      virtual int
      utime (const char* path, const struct utimbuf* times);

      virtual int
      statvfs (struct statvfs* buf);

    public:

      // ----------------------------------------------------------------------
      // Support functions.

      const char*
      mounted_path (void);

      const char*
      name (void) const;

      void
      add_deferred_file (file* fil);

      void
      add_deferred_directory (directory* dir);

      using deferred_files_list_t = utils::intrusive_list<file,
      utils::double_list_links, &file::deferred_links_>;

      using deferred_directories_list_t = utils::intrusive_list<directory,
      utils::double_list_links, &directory::deferred_links_>;

      deferred_files_list_t&
      deferred_files_list (void);

      deferred_directories_list_t&
      deferred_directories_list (void);

      // ----------------------------------------------------------------------

      template<typename T>
        T*
        allocate_file (void);

      template<typename T>
        T*
        allocate_directory (void);

      template<typename T, typename L>
        T*
        allocate_file (L& locker);

      template<typename T, typename L>
        T*
        allocate_directory (L& locker);

      template<typename T>
        void
        deallocate_files (void);

      template<typename T>
        void
        deallocate_directories (void);

      // ----------------------------------------------------------------------
      // Support functions.

      block_device&
      device (void) const;

      file_system_impl&
      impl (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      const char* name_ = nullptr;

      file_system_impl& impl_;

      deferred_files_list_t deferred_files_list_;

      deferred_directories_list_t deferred_directories_list_;

      const char* mounted_path_ = nullptr;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    public:

      /**
       * @cond ignore
       */

      // Intrusive node used to link this file system to the
      // mount manager list.
      // Must be public. The constructor clears the pointers.
      utils::double_list_links mount_manager_links_;

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      // Statics.
      using mounted_list = utils::intrusive_list<file_system,
      utils::double_list_links, &file_system::mount_manager_links_>;
      static mounted_list mounted_list__;

      static file_system* mounted_root__;

      /**
       * @endcond
       */
    };

    // ========================================================================

    class file_system_impl
    {
      // ----------------------------------------------------------------------

      friend class file_system;

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      file_system_impl (block_device& device);

      /**
       * @cond ignore
       */

      // The rule of five.
      file_system_impl (const file_system_impl&) = delete;
      file_system_impl (file_system_impl&&) = delete;
      file_system_impl&
      operator= (const file_system_impl&) = delete;
      file_system_impl&
      operator= (file_system_impl&&) = delete;

      /**
       * @endcond
       */

      virtual
      ~file_system_impl ();

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
      do_vmkfs (int options, std::va_list args) = 0;

      virtual int
      do_vmount (unsigned int flags, std::va_list args) = 0;

      virtual int
      do_umount (unsigned int flags) = 0;

      virtual file*
      do_vopen (class file_system& fs, const char* path, int oflag,
                std::va_list args) = 0;

      virtual directory*
      do_opendir (class file_system& fs, const char* dirname) = 0;

      virtual int
      do_mkdir (const char* path, mode_t mode) = 0;

      virtual int
      do_rmdir (const char* path) = 0;

      virtual void
      do_sync (void) = 0;

      virtual int
      do_chmod (const char* path, mode_t mode) = 0;

      virtual int
      do_stat (const char* path, struct stat* buf) = 0;

      virtual int
      do_truncate (const char* path, off_t length) = 0;

      virtual int
      do_rename (const char* existing, const char* _new) = 0;

      virtual int
      do_unlink (const char* path) = 0;

      virtual int
      do_utime (const char* path, const struct utimbuf* times) = 0;

      virtual int
      do_statvfs (struct statvfs* buf) = 0;

      // ----------------------------------------------------------------------
      // Support functions.

      block_device&
      device (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    protected:

      /**
       * @cond ignore
       */

      block_device& device_;

      file_system* fs_ = nullptr;

      /**
       * @endcond
       */
    };

    // ========================================================================

    template<typename T>
      class file_system_implementable : public file_system
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
          file_system_implementable (const char* name, block_device& device,
                                     Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        file_system_implementable (const file_system_implementable&) = delete;
        file_system_implementable (file_system_implementable&&) = delete;
        file_system_implementable&
        operator= (const file_system_implementable&) = delete;
        file_system_implementable&
        operator= (file_system_implementable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~file_system_implementable ();

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

    // ========================================================================

    template<typename T, typename L>
      class file_system_lockable : public file_system
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
          file_system_lockable (const char* name, block_device& device,
                                lockable_type& locker, Args&&... args);

        /**
         * @cond ignore
         */

        // The rule of five.
        file_system_lockable (const file_system_lockable&) = delete;
        file_system_lockable (file_system_lockable&&) = delete;
        file_system_lockable&
        operator= (const file_system_lockable&) = delete;
        file_system_lockable&
        operator= (file_system_lockable&&) = delete;

        /**
         * @endcond
         */

        virtual
        ~file_system_lockable () override;

        /**
         * @}
         */

        // --------------------------------------------------------------------
        /**
         * @name Public Member Functions
         * @{
         */

      public:

        /**
         * @brief Mount file system.
         *
         * @param path Path, terminated in `/`. If `/` or nullptr, the
         *   file system is mounted as root, i.e. the default if no other
         *   mount point matches.
         * @param flags File system specific flags.
         * @param args Optional arguments.
         * @retval 0 if successful,
         * @retval -1 otherwise and the variable errno is set to
         *   indicate the error.
         */
        virtual int
        vmount (const char* path, unsigned int flags, std::va_list args)
            override;

        /**
         * @brief Unmount file system.
         *
         * @param flags File system specific flags.
         * @retval 0 if successful,
         * @retval -1 otherwise and the variable errno is set to
         *   indicate the error.
         */
        virtual int
        umount (int unsigned flags = 0) override;

        // --------------------------------------------------------------------

        virtual file*
        vopen (const char* path, int oflag, std::va_list args) override;

        // http://pubs.opengroup.org/onlinepubs/9699919799/functions/opendir.html
        virtual directory*
        opendir (const char* dirpath) override;

        // --------------------------------------------------------------------

        virtual int
        mkdir (const char* path, mode_t mode) override;

        virtual int
        rmdir (const char* path) override;

        virtual void
        sync (void) override;

        virtual int
        chmod (const char* path, mode_t mode) override;

        virtual int
        stat (const char* path, struct stat* buf) override;

        virtual int
        truncate (const char* path, off_t length) override;

        virtual int
        rename (const char* existing, const char* _new) override;

        virtual int
        unlink (const char* path) override;

        virtual int
        utime (const char* path, const struct utimbuf* times) override;

        virtual int
        statvfs (struct statvfs* buf) override;

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

    inline const char*
    file_system::name (void) const
    {
      return name_;
    }

    inline file_system_impl&
    file_system::impl (void) const
    {
      return static_cast<file_system_impl&> (impl_);
    }

    inline block_device&
    file_system::device (void) const
    {
      return impl ().device ();
    }

    inline void
    file_system::add_deferred_file (file* fil)
    {
      deferred_files_list_.link (*fil);
    }

    inline void
    file_system::add_deferred_directory (directory* dir)
    {
      deferred_directories_list_.link (*dir);
    }

    inline file_system::deferred_files_list_t&
    file_system::deferred_files_list (void)
    {
      return deferred_files_list_;
    }

    inline file_system::deferred_directories_list_t&
    file_system::deferred_directories_list (void)
    {
      return deferred_directories_list_;
    }

    template<typename T>
      T*
      file_system::allocate_file (void)
      {
        using file_type = T;

        file_type* fil;

        if (deferred_files_list_.empty ())
          {
            fil = new file_type (*this);
          }
        else
          {
            fil = static_cast<file_type*> (deferred_files_list_.unlink_head ());

            // Call the constructor before reusing the object,
            fil->~file_type ();

            // Placement new, run only the constructor.
            new (fil) file_type (*this);

            deallocate_files<file_type> ();
          }
        return fil;
      }

    template<typename T, typename L>
      T*
      file_system::allocate_file (L& locker)
      {
        using file_type = T;

        file_type* fil;

        if (deferred_files_list_.empty ())
          {
            fil = new file_type (*this, locker);
          }
        else
          {
            fil = static_cast<file_type*> (deferred_files_list_.unlink_head ());

            // Call the constructor before reusing the object,
            fil->~file_type ();

            // Placement new, run only the constructor.
            new (fil) file_type (*this, locker);

            deallocate_files<file_type> ();
          }
        return fil;
      }

    template<typename T>
      void
      file_system::deallocate_files (void)
      {
        using file_type = T;

        // Deallocate all remaining elements in the list.
        while (!deferred_files_list_.empty ())
          {
            file_type* f =
                static_cast<file_type*> (deferred_files_list_.unlink_head ());

            // Call the destructor and the deallocator.
            delete f;
          }
      }

    template<typename T>
      T*
      file_system::allocate_directory (void)
      {
        using directory_type = T;

        directory_type* dir;

        if (deferred_directories_list_.empty ())
          {
            dir = new directory_type (*this);
          }
        else
          {
            dir =
                static_cast<directory_type*> (deferred_directories_list_.unlink_head ());

            // Call the constructor before reusing the object,
            dir->~directory_type ();

            // Placement new, run only the constructor.
            new (dir) directory_type (*this);

            deallocate_directories<directory_type> ();
          }
        return dir;
      }

    template<typename T, typename L>
      T*
      file_system::allocate_directory (L& locker)
      {
        using directory_type = T;

        directory_type* dir;

        if (deferred_directories_list_.empty ())
          {
            dir = new directory_type (*this, locker);
          }
        else
          {
            dir =
                static_cast<directory_type*> (deferred_directories_list_.unlink_head ());

            // Call the constructor before reusing the object,
            dir->~directory_type ();

            // Placement new, run only the constructor.
            new (dir) directory_type (*this, locker);

            deallocate_directories<directory_type> ();
          }
        return dir;
      }

    template<typename T>
      void
      file_system::deallocate_directories (void)
      {
        using directory_type = T;

        // Deallocate all remaining elements in the list.
        while (!deferred_directories_list_.empty ())
          {
            directory_type* d =
                static_cast<directory_type*> (deferred_directories_list_.unlink_head ());

            // Call the destructor and the deallocator.
            delete d;
          }
      }

    // ========================================================================

    inline block_device&
    file_system_impl::device (void) const
    {
      return device_;
    }

    // ========================================================================

    template<typename T>
      template<typename ... Args>
        file_system_implementable<T>::file_system_implementable (
            const char* name, block_device& device, Args&&... args) :
            file_system
              { impl_instance_, name }, //
            impl_instance_
              { device, std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
          trace::printf ("file_system_implementable::%s(\"%s\")=@%p\n",
                         __func__, name_, this);
#endif
        }

    template<typename T>
      file_system_implementable<T>::~file_system_implementable ()
      {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
        trace::printf ("file_system_implementable::%s() @%p %s\n", __func__,
                       this, name_);
#endif
      }

    template<typename T>
      typename file_system_implementable<T>::value_type&
      file_system_implementable<T>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

    // ========================================================================

    template<typename T, typename L>
      template<typename ... Args>
        file_system_lockable<T, L>::file_system_lockable (const char* name,
                                                          block_device& device,
                                                          lockable_type& locker,
                                                          Args&&... args) :
            file_system
              { impl_instance_, name }, //
            impl_instance_
              { device, locker, std::forward<Args>(args)... }
        {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
          trace::printf ("file_system_lockable::%s()=%p\n", __func__, this);
#endif
        }

    template<typename T, typename L>
      file_system_lockable<T, L>::~file_system_lockable ()
      {
#if defined(OS_TRACE_POSIX_IO_FILE_SYSTEM)
        trace::printf ("file_system_lockable::%s() @%p\n", __func__, this);
#endif
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::vmount (const char* path, unsigned int flags,
                                          std::va_list args)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::vmount (path, flags, args);
      }

    /**
     * @details
     * The root file system must be unmounted last, it cannot be
     * unmounted if other mount points exists.
     */
    template<typename T, typename L>
      int
      file_system_lockable<T, L>::umount (int unsigned flags)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::umount (flags);
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      file*
      file_system_lockable<T, L>::vopen (const char* path, int oflag,
                                         std::va_list args)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::vopen (path, oflag, args);
      }

    template<typename T, typename L>
      directory*
      file_system_lockable<T, L>::opendir (const char* dirpath)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::opendir (dirpath);
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::mkdir (const char* path, mode_t mode)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::mkdir (path, mode);
      }

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::rmdir (const char* path)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::rmdir (path);
      }

    template<typename T, typename L>
      void
      file_system_lockable<T, L>::sync (void)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::sync ();
      }

    // ------------------------------------------------------------------------

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::chmod (const char* path, mode_t mode)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::chmod (path, mode);
      }

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::stat (const char* path, struct stat* buf)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::stat (path, buf);
      }

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::truncate (const char* path, off_t length)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::truncate (path, length);
      }

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::rename (const char* existing,
                                          const char* _new)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::rename (existing, _new);
      }

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::unlink (const char* path)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::unlink (path);
      }

    // http://pubs.opengroup.org/onlinepubs/9699919799/functions/utime.html
    template<typename T, typename L>
      int
      file_system_lockable<T, L>::utime (const char* path,
                                         const struct utimbuf* times)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::utime (path, times);
      }

    template<typename T, typename L>
      int
      file_system_lockable<T, L>::statvfs (struct statvfs* buf)
      {
        std::lock_guard<L> lock
          { impl_instance_.locker () };

        return file_system::statvfs (buf);
      }

    template<typename T, typename L>
      typename file_system_lockable<T, L>::value_type&
      file_system_lockable<T, L>::impl (void) const
      {
        return static_cast<value_type&> (impl_);
      }

// ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_ */
