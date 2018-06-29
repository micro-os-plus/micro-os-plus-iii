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

#ifndef CMSIS_PLUS_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_
#define CMSIS_PLUS_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_

#if defined(__cplusplus)

// ----------------------------------------------------------------------------

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/posix-io/types.h>

#include <cstddef>
#include <cassert>

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

      static class io*
      io (int fildes);

      static class socket*
      socket (int fildes);

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

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_POSIX_IO_FILE_DESCRIPTORS_MANAGER_H_ */
