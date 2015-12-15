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

#ifndef POSIX_FILE_SYSTEMS_MANAGER_H_
#define POSIX_FILE_SYSTEMS_MANAGER_H_

#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

namespace os
{
  // --------------------------------------------------------------------------

  class PosixFileSystem;

  // --------------------------------------------------------------------------

  class PosixFileSystemsManager
  {
  public:

    PosixFileSystemsManager (std::size_t size);

    ~PosixFileSystemsManager ();

    // ------------------------------------------------------------------------

    static PosixFileSystem*
    identifyFileSystem (const char** path1, const char** path2 = nullptr);

    static int
    setRoot (PosixFileSystem* fs, int flags);

    static PosixFileSystem*
    getRoot (void);

    static int
    mount (PosixFileSystem* fs, const char* path, int flags);

    static int
    umount (const char* path, int flags);

    static std::size_t
    getSize (void);

    static PosixFileSystem*
    getFileSystem (std::size_t index);

    static const char*
    getPath (std::size_t index);

  protected:

    static std::size_t sfSize;

    static PosixFileSystem* sfRoot;
    static PosixFileSystem** sfFileSystemsArray;
    static const char** sfPathsArray;

  };

  inline std::size_t
  PosixFileSystemsManager::getSize (void)
  {
    return sfSize;
  }

  inline PosixFileSystem*
  PosixFileSystemsManager::getFileSystem (std::size_t index)
  {
    assert(index < sfSize);
    return sfFileSystemsArray[index];
  }

  inline const char*
  PosixFileSystemsManager::getPath (std::size_t index)
  {
    assert(index < sfSize);
    return sfPathsArray[index];
  }

  inline PosixFileSystem*
  PosixFileSystemsManager::getRoot (void)
  {
    return sfRoot;
  }

} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* POSIX_FILE_SYSTEMS_MANAGER_H_ */
