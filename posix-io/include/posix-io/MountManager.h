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

#ifndef POSIX_IO_MOUNT_MANAGER_H_
#define POSIX_IO_MOUNT_MANAGER_H_

#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    class FileSystem;

    // ------------------------------------------------------------------------

    class MountManager
    {
    public:

      MountManager (std::size_t size);
      MountManager (const MountManager&) = delete;

      ~MountManager ();

      // ----------------------------------------------------------------------

      static FileSystem*
      identifyFileSystem (const char** path1, const char** path2 = nullptr);

      static int
      setRoot (FileSystem* fs, BlockDevice* blockDevice, unsigned int flags);

      static FileSystem*
      getRoot (void);

      static int
      mount (FileSystem* fs, const char* path, BlockDevice* blockDevice,
             unsigned int flags);

      static int
      umount (const char* path, int unsigned flags);

      static std::size_t
      getSize (void);

      static FileSystem*
      getFileSystem (std::size_t index);

      static const char*
      getPath (std::size_t index);

    private:

      static std::size_t sfSize;

      static FileSystem* sfRoot;
      static FileSystem** sfFileSystemsArray;
      static const char** sfPathsArray;

    };

    inline std::size_t
    MountManager::getSize (void)
    {
      return sfSize;
    }

    inline FileSystem*
    MountManager::getFileSystem (std::size_t index)
    {
      assert(index < sfSize);
      return sfFileSystemsArray[index];
    }

    inline const char*
    MountManager::getPath (std::size_t index)
    {
      assert(index < sfSize);
      return sfPathsArray[index];
    }

    inline FileSystem*
    MountManager::getRoot (void)
    {
      return sfRoot;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_IO_MOUNT_MANAGER_H_ */
