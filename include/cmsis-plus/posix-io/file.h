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

#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix/utime.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class file_system;
    class pool;

    // ------------------------------------------------------------------------

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

      file ();
      file (const file&) = delete;

      ~file ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Static Member Functions
       * @{
       */

    public:

      static file*
      open (const char* path, int oflag, ...);

      static file*
      vopen (const char* path, int oflag, std::va_list args);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Member Functions
       * @{
       */

    public:

      off_t
      lseek (off_t offset, int whence);

      int
      ftruncate (off_t length);

      int
      fsync (void);

      // ----------------------------------------------------------------------
      // Support functions.

      class file_system*
      file_system (void) const;

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Private Member Functions
       * @{
       */

    protected:

      // Implementations

      /**
       * return 0 if success or -1 & errno
       */
      virtual int
      do_vopen (const char* path, int oflag, std::va_list args) = 0;

      virtual off_t
      do_lseek (off_t offset, int whence);

      virtual int
      do_ftruncate (off_t length);

      virtual int
      do_fsync (void);

      virtual void
      do_release (void) override;

      // ----------------------------------------------------------------------
      // Support functions.

      void
      file_system (class file_system* file_system);

      /**
       * @}
       */

    private:

      /**
       * @cond ignore
       */

      class file_system* file_system_;

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

    inline file*
    file::vopen (const char* path, int oflag, std::va_list args)
    {
      return static_cast<file*> (os::posix::vopen (path, oflag, args));
    }

    inline void
    file::file_system (class file_system* file_system)
    {
      file_system_ = file_system;
    }

    inline file_system*
    file::file_system (void) const
    {
      return file_system_;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_FILE_H_ */
