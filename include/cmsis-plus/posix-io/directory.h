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

#include <cmsis-plus/posix-io/file-system.h>

#include <cmsis-plus/posix/dirent.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class directory;

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

    // ------------------------------------------------------------------------

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

      directory (void);
      directory (const directory&) = delete;

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

      struct dirent *
      read (void);

      void
      rewind (void);

      int
      close (void);

      // ----------------------------------------------------------------------
      // Support functions.

      struct dirent*
      dir_entry (void);

      const char*
      name (void) const;

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

      // Implementations.

      /**
       * @return object if successful, otherwise nullptr and errno.
       */
      virtual directory*
      do_vopen (const char* dirname) = 0;

      virtual struct dirent*
      do_read (void);

      virtual void
      do_rewind (void);

      virtual int
      do_close (void);

      // Support functions.

      void
      file_system (class file_system* fileSystem);

      /**
       * @}
       */

    private:

      /**
       * @cond ignore
       */

      class file_system* file_system_;

      struct dirent dir_entry_;

      /**
       * @endcond
       */

    };

#pragma GCC diagnostic pop

  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    inline void
    directory::file_system (class file_system* fileSystem)
    {
      file_system_ = fileSystem;
    }

    inline file_system*
    directory::file_system (void) const
    {
      return file_system_;
    }

    inline struct dirent*
    directory::dir_entry (void)
    {
      return &dir_entry_;
    }

    inline const char*
    directory::name (void) const
    {
      return &dir_entry_.d_name[0];
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_DIRECTORY_H_ */
