/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 */

#ifndef CMSIS_PLUS_RTOS_INTERNAL_OS_FLAGS_H_
#define CMSIS_PLUS_RTOS_INTERNAL_OS_FLAGS_H_

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <cmsis-plus/rtos/os-decls.h>

#include <cmsis-plus/diag/trace.h>

namespace os
{
  namespace rtos
  {
    namespace internal
    {

      // ======================================================================

      /**
       * @brief Internal event flags implementation.
       */
      class event_flags
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct an internal event flags object instance.
         */
        event_flags () = default;

        /**
         * @cond ignore
         */

        event_flags (const event_flags&) = delete;
        event_flags (event_flags&&) = delete;
        event_flags&
        operator= (const event_flags&) = delete;
        event_flags&
        operator= (event_flags&&) = delete;

        /**
         * @endcond
         */

        /**
         * @brief Destruct the internal event flags object instance.
         */
        ~event_flags () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Functions
         * @{
         */

        result_t
        raise (flags::mask_t mask, flags::mask_t* oflags);

        bool
        try_wait (flags::mask_t mask, flags::mask_t* oflags,
                  flags::mode_t mode);

#if 0
        result_t
        wait (flags::mask_t mask, flags::mask_t* oflags = nullptr,
            flags::mode_t mode = flags::mode::all | flags::mode::clear);

        result_t
        try_wait (
            flags::mask_t mask, flags::mask_t* oflags = nullptr,
            flags::mode_t mode = flags::mode::all | flags::mode::clear);

        result_t
        timed_wait (
            flags::mask_t mask, clock::duration_t timeout, flags::mask_t* oflags =
            nullptr,
            flags::mode_t mode = flags::mode::all | flags::mode::clear);
#endif

        flags::mask_t
        get (flags::mask_t mask, flags::mode_t mode);

        result_t
        clear (flags::mask_t mask, flags::mask_t* oflags);

        flags::mask_t
        mask (void);

        /**
         * @}
         */

      protected:

        /**
         * @name Private Member Variables
         * @{
         */

        /**
         * @cond ignore
         */

        /**
         * @brief Bit-mask with all flags.
         */
        flags::mask_t volatile flags_mask_ = 0;

        /**
         * @endcond
         */

        /**
         * @}
         */

      };
    /* class event_flags */

    // ------------------------------------------------------------------------
    } /* namespace internal */
  } /* namespace rtos */
} /* namespace os */

namespace os
{
  namespace rtos
  {
    namespace internal
    {

      inline flags::mask_t
      event_flags::mask (void)
      {
        return flags_mask_;
      }

    // ------------------------------------------------------------------------
    } /* namespace internal */
  } /* namespace rtos */
} /* namespace os */

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_INTERNAL_OS_FLAGS_H_ */
