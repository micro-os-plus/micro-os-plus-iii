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

#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"

#include <cerrno>
#include <cstring>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    std::size_t MountManager::sfSize;

    FileSystem* MountManager::sfRoot;
    FileSystem** MountManager::sfFileSystemsArray;
    const char** MountManager::sfPathsArray;

    // ------------------------------------------------------------------------

    MountManager::MountManager (std::size_t size)
    {
      assert(size > 0);

      sfSize = size;
      sfFileSystemsArray = new FileSystem*[size];
      sfPathsArray = new const char*[size];

      for (std::size_t i = 0; i < size; ++i)
        {
          sfFileSystemsArray[i] = nullptr;
          sfPathsArray[i] = nullptr;
        }
    }

    MountManager::~MountManager ()
    {
      delete[] sfFileSystemsArray;
      delete[] sfPathsArray;
      sfSize = 0;
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    FileSystem*
    MountManager::identifyFileSystem (const char** path1, const char** path2)
    {
      assert(path1 != nullptr);
      assert(*path1 != nullptr);

      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfPathsArray[i] == nullptr)
            {
              continue;
            }

          auto len = std::strlen (sfPathsArray[i]);

          // Check if path1 starts with the mounted path.
          if (std::strncmp (sfPathsArray[i], *path1, len) == 0)
            {
              // If so, adjust paths to skip over prefix, but keep '/'.
              *path1 = (*path1 + len - 1);
              if ((path2 != nullptr) && (*path2 != nullptr))
                {
                  *path2 = (*path2 + len - 1);
                }

              return sfFileSystemsArray[i];
            }
        }

      // If root file system defined, return it.
      if (sfRoot != nullptr)
        {
          return sfRoot;
        }

      // Not found.
      return nullptr;
    }

#pragma GCC diagnostic pop

    int
    MountManager::setRoot (FileSystem* fs, BlockDevice* blockDevice,
                           unsigned int flags)
    {
      assert(fs != nullptr);
      errno = 0;

      sfRoot = fs;

      fs->setBlockDevice (blockDevice);
      return fs->do_mount (flags);
    }

    int
    MountManager::mount (FileSystem* fs, const char* path,
                         BlockDevice* blockDevice, unsigned int flags)
    {
      assert(fs != nullptr);
      assert(path != nullptr);

      assert(path[std::strlen (path) - 1] == '/');

      errno = 0;

      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfPathsArray[i] != nullptr)
            {
              if (std::strcmp (sfPathsArray[i], path) == 0)
                {
                  // Folder already mounted.
                  errno = EBUSY;
                  return -1;
                }
            }
        }

      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfFileSystemsArray[i] == nullptr)
            {
              fs->setBlockDevice (blockDevice);
              fs->do_mount (flags);

              sfFileSystemsArray[i] = fs;
              sfPathsArray[i] = path;

              return 0;
            }
        }

      // The meaning is actually 'array size exceeded', but could
      // not find a better match.
      errno = ENOENT;
      return -1;
    }

    int
    MountManager::umount (const char* path, unsigned int flags)
    {
      assert(path != nullptr);
      errno = 0;

      for (std::size_t i = 0; i < sfSize; ++i)
        {
          if (sfPathsArray[i] != nullptr && strcmp (path, sfPathsArray[i]) == 0)
            {
              sfFileSystemsArray[i]->do_sync ();
              sfFileSystemsArray[i]->do_unmount (flags);
              sfFileSystemsArray[i]->setBlockDevice (nullptr);

              sfFileSystemsArray[i] = nullptr;
              sfPathsArray[i] = nullptr;

              return 0;
            }
        }

      // The requested directory is not in the mount table.
      errno = EINVAL;
      return -1;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
