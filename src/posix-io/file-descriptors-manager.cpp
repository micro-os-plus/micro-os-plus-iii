/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2015-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/file-descriptors-manager.h>
#include <cmsis-plus/posix-io/io.h>
#include <cmsis-plus/posix-io/socket.h>
#include <cmsis-plus/rtos/os.h>

#include <cmsis-plus/diag/trace.h>

#include <cerrno>
#include <cassert>
#include <cstddef>

// ----------------------------------------------------------------------------

#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
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

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
    io** file_descriptors_manager::descriptors_array__;
#pragma GCC diagnostic pop

    /**
     * @endcond
     */

    // ========================================================================
    file_descriptors_manager::file_descriptors_manager (std::size_t size)
    {
      trace::printf ("file_descriptors_manager::%s(%d)=%p\n", __func__, size,
                     this);

      assert (size > 0);

      size__ = size + reserved__; // Add space for standard files.
      descriptors_array__ = new class io*[size__];

      for (std::size_t i = 0; i < file_descriptors_manager::size (); ++i)
        {
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
          descriptors_array__[i] = nullptr;
#pragma GCC diagnostic pop
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
      assert (!rtos::interrupts::in_handler_mode ());

      rtos::scheduler::critical_section scs;

      // Check if valid descriptor or buffer not yet initialised
      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__)
          || (descriptors_array__ == nullptr))
        {
          return nullptr;
        }
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      return descriptors_array__[fildes];
#pragma GCC diagnostic pop
    }

    bool
    file_descriptors_manager::valid (int fildes)
    {
      assert (!rtos::interrupts::in_handler_mode ());

      rtos::scheduler::critical_section scs;

      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__)
          || (descriptors_array__ == nullptr)
          || (descriptors_array__[fildes] == nullptr))
        {
          return false;
        }
      return true;
    }

    int
    file_descriptors_manager::allocate (class io* io)
    {
      assert (!rtos::interrupts::in_handler_mode ());

#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
      trace::printf ("file_descriptors_manager::%s(%p)\n", __func__, io);
#endif

      if (io->file_descriptor () >= 0)
        {
          // Already allocated
          errno = EBUSY;
          return -1;
        }

      rtos::scheduler::critical_section scs;

      for (std::size_t i = reserved__; i < size__; ++i)
        {
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
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
#pragma GCC diagnostic pop
        }

      // Too many files open in system.
      errno = ENFILE;
      return -1;
    }

    int
    file_descriptors_manager::assign (file_descriptor_t fildes, class io* io)
    {
      assert (!rtos::interrupts::in_handler_mode ());

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

      rtos::scheduler::critical_section scs;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      descriptors_array__[fildes] = io;
#pragma GCC diagnostic pop
      io->file_descriptor (fildes);
      return fildes;
    }

    int
    file_descriptors_manager::deallocate (int fildes)
    {  
      assert (!rtos::interrupts::in_handler_mode ());

#if defined(OS_TRACE_POSIX_IO_FILE_DESCRIPTORS_MANAGER)
      trace::printf ("file_descriptors_manager::%s(%d)\n", __func__, fildes);
#endif

      rtos::scheduler::critical_section scs;

      if ((fildes < 0) || (static_cast<std::size_t> (fildes) >= size__)
          || (descriptors_array__ == nullptr)
          || (descriptors_array__[fildes] == nullptr))
        {
          errno = EBADF;
          return -1;
        }

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      descriptors_array__[fildes]->clear_file_descriptor ();
      descriptors_array__[fildes] = nullptr;
#pragma GCC diagnostic pop
      return 0;
    }

    /* class */ socket*
    file_descriptors_manager::socket (int fildes)
    {
      assert (!rtos::interrupts::in_handler_mode ());

      assert ((fildes >= 0) && (static_cast<std::size_t> (fildes) < size__));

      rtos::scheduler::critical_section scs;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
      auto* const io = (descriptors_array__ != nullptr)
                           ? descriptors_array__[fildes]
                           : nullptr;
#pragma GCC diagnostic pop
      if (io == nullptr
          || io->get_type () != static_cast<posix::io::type_t> (io::type::socket))
        {
          return nullptr;
        }
      return reinterpret_cast<class socket*> (io);
    }

    size_t
    file_descriptors_manager::used (void)
    {
      assert (!rtos::interrupts::in_handler_mode ());

      rtos::scheduler::critical_section scs;

      std::size_t count = reserved__;
      for (std::size_t i = reserved__; i < file_descriptors_manager::size ();
           ++i)
        {
#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
          if (descriptors_array__ != nullptr && descriptors_array__[i] != nullptr)
            {
              ++count;
            }
#pragma GCC diagnostic pop
        }
      return count;
    }

    // ========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
