/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "posix-io/PosixFileSystem.h"
#include "posix-io/PosixFileSystemsManager.h"
#include <cerrno>
#include <cstring>

// ----------------------------------------------------------------------------

namespace os
{

  // --------------------------------------------------------------------------

  std::size_t PosixFileSystemsManager::sfSize;

  PosixFileSystem* PosixFileSystemsManager::sfRoot;
  PosixFileSystem** PosixFileSystemsManager::sfFileSystemsArray;
  const char** PosixFileSystemsManager::sfPathsArray;

  // --------------------------------------------------------------------------

  PosixFileSystemsManager::PosixFileSystemsManager (std::size_t size)
  {
    assert(size > 0);

    sfSize = size;
    sfFileSystemsArray = new PosixFileSystem*[size];
    sfPathsArray = new const char*[size];

    for (std::size_t i = 0; i < size; ++i)
      {
        sfFileSystemsArray[i] = nullptr;
        sfPathsArray[i] = nullptr;
      }
  }

  PosixFileSystemsManager::~PosixFileSystemsManager ()
  {
    delete sfFileSystemsArray;
    delete sfPathsArray;
    sfSize = 0;
  }

  // --------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

  PosixFileSystem*
  PosixFileSystemsManager::identifyFileSystem (const char** path1,
                                               const char** path2)
  {
    assert(path1 != nullptr);
    assert(*path1 != nullptr);

    for (std::size_t i = 0; i < sfSize; ++i)
      {
        size_t len = std::strlen (sfPathsArray[i]);

        // Check if path1 starts with the mounted path.
        if (sfPathsArray[i] != nullptr
            && std::strncmp (sfPathsArray[i], *path1, len) == 0)
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
    if (sfRoot != nullptr){
        return sfRoot;
    }

    // Not found.
    return nullptr;
  }

#pragma GCC diagnostic pop

  int
  PosixFileSystemsManager::setRoot (PosixFileSystem* fs, int flags)
  {
    assert(fs != nullptr);
    errno = 0;

    sfRoot = fs;

    return fs->do_mount (flags);
  }

  int
  PosixFileSystemsManager::mount (PosixFileSystem* fs, const char* path,
                                  int flags)
  {
    assert(fs != nullptr);
    assert(path != nullptr);

    assert(path[std::strlen (path) - 1] == '/');

    errno = 0;

    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfFileSystemsArray[i] == nullptr)
          {
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
  PosixFileSystemsManager::umount (const char* path, int flags)
  {
    assert(path != nullptr);
    errno = 0;

    for (std::size_t i = 0; i < sfSize; ++i)
      {
        if (sfPathsArray[i] != nullptr && strcmp (path, sfPathsArray[i]) == 0)
          {
            sfFileSystemsArray[i]->do_sync ();
            sfFileSystemsArray[i]->do_unmount (flags);

            sfFileSystemsArray[i] = nullptr;
            sfPathsArray[i] = nullptr;

            return 0;
          }
      }

    // The requested directory is not in the mount table.
    errno = EINVAL;
    return -1;
  }

} /* namespace os */

// ----------------------------------------------------------------------------
