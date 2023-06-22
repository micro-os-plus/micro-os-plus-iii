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

#ifndef CMSIS_PLUS_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_
#define CMSIS_PLUS_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/types.h>

#include <cstddef>
#include <cassert>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#pragma clang diagnostic ignored "-Wdocumentation-unknown-command"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {

    // ------------------------------------------------------------------------

    class io;
    class socket;

    // ========================================================================

    /**
     * @brief File descriptors manager static class.
     * @headerfile file-descriptors-manager.h <cmsis-plus/posix-io/file-descriptors-manager.h>
     * @ingroup cmsis-plus-posix-io-base
     */
    class file_descriptors_manager
    {
      // ----------------------------------------------------------------------

      /**
       * @name Constructors & Destructor
       * @{
       */

    public:

      file_descriptors_manager (std::size_t size);

      /**
       * @cond ignore
       */

      // The rule of five.
      file_descriptors_manager (const file_descriptors_manager&) = delete;
      file_descriptors_manager (file_descriptors_manager&&) = delete;
      file_descriptors_manager&
      operator= (const file_descriptors_manager&) = delete;
      file_descriptors_manager&
      operator= (file_descriptors_manager&&) = delete;

      /**
       * @endcond
       */

      ~file_descriptors_manager ();

      /**
       * @}
       */

      // ----------------------------------------------------------------------
      /**
       * @name Public Static Member Functions
       * @{
       */

    public:

      static size_t
      size (void);

      static bool
      valid (int fildes);

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wredundant-tags"
#endif

      static class io*
      io (int fildes);

      static class socket*
      socket (int fildes);

#pragma GCC diagnostic pop

      static int
      allocate (class io* io);

      static int
      assign (file_descriptor_t fildes, class io* io);

      static int
      deallocate (file_descriptor_t fildes);

      static size_t
      used (void);

      /**
       * @}
       */

      // ----------------------------------------------------------------------
    private:

      /**
       * @cond ignore
       */

      // Reserve 0, 1, 2 (stdin, stdout, stderr).
      static constexpr std::size_t reserved__ = 3;

      static std::size_t size__;

      static class io** descriptors_array__;

      /**
       * @endcond
       */
    };

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace posix
  {
    // ========================================================================

    inline size_t
    file_descriptors_manager::size (void)
    {
      return size__;
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_ */
