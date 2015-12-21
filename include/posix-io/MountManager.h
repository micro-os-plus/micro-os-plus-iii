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
