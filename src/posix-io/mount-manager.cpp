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

#include <cmsis-plus/posix-io/file-system.h>
#include <cmsis-plus/posix-io/mount-manager.h>
#include <cerrno>
#include <cstring>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @cond ignore
     */

    std::size_t mount_manager::size__;

    file_system* mount_manager::root__;
    file_system** mount_manager::file_systems_array__;
    const char** mount_manager::paths_array__;

    /**
     * @endcond
     */

    // ------------------------------------------------------------------------
    mount_manager::mount_manager (std::size_t size)
    {
      assert(size > 0);

      size__ = size;
      file_systems_array__ = new file_system*[size];
      paths_array__ = new const char*[size];

      for (std::size_t i = 0; i < size; ++i)
        {
          file_systems_array__[i] = nullptr;
          paths_array__[i] = nullptr;
        }
    }

    mount_manager::~mount_manager ()
    {
      delete[] file_systems_array__;
      delete[] paths_array__;
      size__ = 0;
    }

    // ------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    file_system*
    mount_manager::identify_file_system (const char** path1, const char** path2)
    {
      assert(path1 != nullptr);
      assert(*path1 != nullptr);

      for (std::size_t i = 0; i < size__; ++i)
        {
          if (paths_array__[i] == nullptr)
            {
              continue;
            }

          auto len = std::strlen (paths_array__[i]);

          // Check if path1 starts with the mounted path.
          if (std::strncmp (paths_array__[i], *path1, len) == 0)
            {
              // If so, adjust paths to skip over prefix, but keep '/'.
              *path1 = (*path1 + len - 1);
              if ((path2 != nullptr) && (*path2 != nullptr))
                {
                  *path2 = (*path2 + len - 1);
                }

              return file_systems_array__[i];
            }
        }

      // If root file system defined, return it.
      if (root__ != nullptr)
        {
          return root__;
        }

      // Not found.
      return nullptr;
    }

#pragma GCC diagnostic pop

    int
    mount_manager::root (file_system* fs, device_block* blockDevice,
                         unsigned int flags)
    {
      assert(fs != nullptr);
      errno = 0;

      root__ = fs;

      fs->device (blockDevice);
      return fs->do_mount (flags);
    }

    int
    mount_manager::mount (file_system* fs, const char* path,
                          device_block* blockDevice, unsigned int flags)
    {
      assert(fs != nullptr);
      assert(path != nullptr);

      assert(path[std::strlen (path) - 1] == '/');

      errno = 0;

      for (std::size_t i = 0; i < size__; ++i)
        {
          if (paths_array__[i] != nullptr)
            {
              if (std::strcmp (paths_array__[i], path) == 0)
                {
                  // Folder already mounted.
                  errno = EBUSY;
                  return -1;
                }
            }
        }

      for (std::size_t i = 0; i < size__; ++i)
        {
          if (file_systems_array__[i] == nullptr)
            {
              fs->device (blockDevice);
              fs->do_mount (flags);

              file_systems_array__[i] = fs;
              paths_array__[i] = path;

              return 0;
            }
        }

      // The meaning is actually 'array size exceeded', but could
      // not find a better match.
      errno = ENOENT;
      return -1;
    }

    int
    mount_manager::umount (const char* path, unsigned int flags)
    {
      assert(path != nullptr);
      errno = 0;

      for (std::size_t i = 0; i < size__; ++i)
        {
          if (paths_array__[i] != nullptr
              && strcmp (path, paths_array__[i]) == 0)
            {
              file_systems_array__[i]->do_sync ();
              file_systems_array__[i]->do_unmount (flags);
              file_systems_array__[i]->device (nullptr);

              file_systems_array__[i] = nullptr;
              paths_array__[i] = nullptr;

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
