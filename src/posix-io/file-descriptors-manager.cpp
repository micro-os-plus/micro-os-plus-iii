/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/file-descriptors-manager.h>
#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix-io/socket.h>

#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <cstddef>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    /**
     * @cond ignore
     */

    std::size_t file_descriptors_manager::size__;

    io** file_descriptors_manager::descriptors_array__;

    /**
     * @endcond
     */

    // ========================================================================
    file_descriptors_manager::file_descriptors_manager (std::size_t size)
    {
      trace::printf ("file_descriptors_manager::%s(%d)=%p\n", __func__, size,
                     this);

      assert(size > 0);

      size__ = size + reserved__; // Add space for standard files.
      descriptors_array__ = new class io*[size__];

      for (std::size_t i = 0; i < file_descriptors_manager::size (); ++i)
        {
          descriptors_array__[i] = nullptr;
        }
    }

    file_descriptors_manager::~file_descriptors_manager ()
    {
      trace::printf ("file_descriptors_manager::%s(%) @%p\n", __func__, this);

      delete[] descriptors_array__;
      size__ = 0;
    }

    // ------------------------------------------------------------------------

    io*
    file_descriptors_manager::io (int fildes)
    {
      // Check if valid descriptor or buffer not yet initialised
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__)
          || (descriptors_array__ == nullptr))
        {
          return nullptr;
        }
      return descriptors_array__[fildes];
    }

    bool
    file_descriptors_manager::valid (int fildes)
    {
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__))
        {
          return false;
        }
      return true;
    }

    int
    file_descriptors_manager::allocate (class io* io)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
      trace::printf ("file_descriptors_manager::%s(%p)\n", __func__, io);
#endif

      if (io->file_descriptor () >= 0)
        {
          // Already allocated
          errno = EBUSY;
          return -1;
        }

      for (std::size_t i = reserved__; i < size__; ++i)
        {
          if (descriptors_array__[i] == nullptr)
            {
              descriptors_array__[i] = io;
              io->file_descriptor (static_cast<int> (i));
#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
              trace::printf ("file_descriptors_manager::%s(%p) fd=%d\n",
                             __func__, io, i);
#endif
              return static_cast<int> (i);
            }
        }

      // Too many files open in system.
      errno = ENFILE;
      return -1;
    }

    int
    file_descriptors_manager::assign (file_descriptor_t fildes, class io* io)
    {
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__))
        {
          errno = EBADF;
          return -1;
        }

      if (io->file_descriptor () >= 0)
        {
          // Already allocated
          errno = EBUSY;
          return -1;
        }

      descriptors_array__[fildes] = io;
      io->file_descriptor (fildes);
      return fildes;
    }

    int
    file_descriptors_manager::deallocate (int fildes)
    {
#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
      trace::printf ("file_descriptors_manager::%s(%d)\n", __func__, fildes);
#endif

      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__))
        {
          errno = EBADF;
          return -1;
        }

      descriptors_array__[fildes]->clear_file_descriptor ();
      descriptors_array__[fildes] = nullptr;
      return 0;
    }

    /* class */ socket*
    file_descriptors_manager::socket (int fildes)
    {
      assert((fildes >= 0) && (static_cast<std::size_t> (fildes) < size__));
      auto* const io = descriptors_array__[fildes];
      if (io->get_type () != static_cast<posix::io::type_t>(io::type::socket))
        {
          return nullptr;
        }
      return reinterpret_cast<class socket*> (io);
    }

    size_t
    file_descriptors_manager::used (void)
    {
      std::size_t count = reserved__;
      for (std::size_t i = reserved__; i < file_descriptors_manager::size ();
          ++i)
        {
          if (descriptors_array__[i] != nullptr)
            {
              ++count;
            }
        }
      return count;
    }

  // ========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------

