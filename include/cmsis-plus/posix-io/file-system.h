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

#include <cstdarg>
#include <sys/stat.h>
#include <utime.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class io;
    class file;
    class directory;
    class device_block;
    class pool;

    /**
     * @ingroup cmsis-plus-posix-io-func
     * @{
     */

    // ----------------------------------------------------------------------
    // ----- Non-io, global file system functions -----
    int
    mkdir (const char* path, mode_t mode);

    int
    rmdir (const char* path);

    void
    sync (void);

    // ----------------------------------------------------------------------
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

    /**
     * @}
     */

    // ------------------------------------------------------------------------
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

      friend class file;
      friend class directory;
      friend class mount_manager;
      friend class io;

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

      /**
       * @endcond
       */

      // ----------------------------------------------------------------------
      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      file_system (device_block& device, pool* filesPool, pool* dirs_pool);

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

      /**
       * @brief Mount file system.
       *
       * @param path Path, terminated in `/`. If `/` or nullptr, the
       *   file system is mounted as root, i.e. the default if no other
       *   mount point matches.
       * @param flags File system specific flags.
       * @retval 0 if successful,
       * @retval -1 otherwise and the variable errno is set to
       *   indicate the error.
       */
      int
      mount (const char* path = nullptr, unsigned int flags = 0);

      /**
       * @brief Unmount file system.
       *
       * @param flags File system specific flags.
       * @retval 0 if successful,
       * @retval -1 otherwise and the variable errno is set to
       *   indicate the error.
       */
      int
      umount (int unsigned flags = 0);

      // ----------------------------------------------------------------------

      io*
      vopen (const char* path, int oflag, std::va_list args);

      directory*
      opendir (const char* dirpath);

      // ----------------------------------------------------------------------
      // Support functions.

      device_block&
      device (void) const;

      pool*
      files_pool (void) const;

      pool*
      dirs_pool (void) const;

      /**
       * @}
       */

    protected:

      // ----------------------------------------------------------------------
      // Called from namespace friend functions.

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

      void
      sync (void);

      // ----------------------------------------------------------------------
      // Implementations.

      virtual int
      do_chmod (const char* path, mode_t mode);

      virtual int
      do_stat (const char* path, struct stat* buf);

      virtual int
      do_truncate (const char* path, off_t length);

      virtual int
      do_rename (const char* existing, const char* _new);

      virtual int
      do_unlink (const char* path);

      virtual int
      do_utime (const char* path, const struct utimbuf* times);

      virtual int
      do_mkdir (const char* path, mode_t mode);

      virtual int
      do_rmdir (const char* path);

      virtual void
      do_sync (void);

      virtual int
      do_mount (unsigned int flags);

      virtual int
      do_umount (unsigned int flags);

      // ----------------------------------------------------------------------
      // Support functions.

    protected:

      void
      device (device_block* block_device);

      /**
       * @name Public Member Functions
       * @{
       */

    public:

      const char*
      adjust_path (const char* path);

      /**
       * @}
       */

    private:

      /**
       * @cond ignore
       */

      pool* files_pool_;
      pool* dirs_pool_;

      device_block& block_device_;

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

    inline pool*
    file_system::files_pool (void) const
    {
      return files_pool_;
    }

    inline pool*
    file_system::dirs_pool (void) const
    {
      return dirs_pool_;
    }

    inline device_block&
    file_system::device (void) const
    {
      return block_device_;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_FILE_SYSTEM_H_ */
