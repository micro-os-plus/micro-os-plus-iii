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

#ifndef CMSIS_PLUS_POSIX_IO_MOUNT_MANAGER_H_
#define CMSIS_PLUS_POSIX_IO_MOUNT_MANAGER_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class file_system;

    // ------------------------------------------------------------------------

    class mount_manager
    {
    public:

      mount_manager (std::size_t size);
      mount_manager (const mount_manager&) = delete;

      ~mount_manager ();

      // ----------------------------------------------------------------------

      static file_system*
      identifyFileSystem (const char** path1, const char** path2 = nullptr);

      static int
      setRoot (file_system* fs, device_block* blockDevice, unsigned int flags);

      static file_system*
      getRoot (void);

      static int
      mount (file_system* fs, const char* path, device_block* blockDevice,
             unsigned int flags);

      static int
      umount (const char* path, int unsigned flags);

      static std::size_t
      getSize (void);

      static file_system*
      getFileSystem (std::size_t index);

      static const char*
      getPath (std::size_t index);

    private:

      static std::size_t sfSize;

      static file_system* sfRoot;
      static file_system** sfFileSystemsArray;
      static const char** sfPathsArray;

    };

    inline std::size_t
    mount_manager::getSize (void)
    {
      return sfSize;
    }

    inline file_system*
    mount_manager::getFileSystem (std::size_t index)
    {
      assert (index < sfSize);
      return sfFileSystemsArray[index];
    }

    inline const char*
    mount_manager::getPath (std::size_t index)
    {
      assert (index < sfSize);
      return sfPathsArray[index];
    }

    inline file_system*
    mount_manager::getRoot (void)
    {
      return sfRoot;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_MOUNT_MANAGER_H_ */
