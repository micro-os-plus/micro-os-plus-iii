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

        /**
         * @brief Raise event flags.
         * @param [in] mask The OR-ed flags to raise.
         * @param [out] oflags Optional pointer where to store the
         *  new value of the flags; may be `nullptr`.
         * @retval result::ok The flags were raised.
         * @retval EINVAL The mask is zero.
         * @retval ENOTRECOVERABLE Raise failed.
         */
        result_t
        raise (flags::mask_t mask, flags::mask_t* oflags);

        /**
         * @brief Check if expected flags are raised.
         * @param [in] mask The expected flags (OR-ed bit-mask);
         *  if `flags::any`, any flag raised will do it.
         * @param [out] oflags Pointer where to store the current flags;
         *  may be `nullptr`.
         * @param [in] mode Mode bits to select if either all or any flags
         *  in the mask are expected, and if the flags should be cleared.
         * @retval true The expected flags are raised.
         * @retval false The expected flags are not raised.
         */
        bool
        check_raised (flags::mask_t mask, flags::mask_t* oflags,
                      flags::mode_t mode);

        /**
         * @brief Get (and possibly clear) event flags.
         * @param [in] mask The OR-ed flags to get/clear; can be `flags::any`.
         * @param [in] mode Mode bits to select if the flags should be
         *  cleared (the other bits are ignored).
         * @return The selected bits from the flags mask.
         */
        flags::mask_t
        get (flags::mask_t mask, flags::mode_t mode);

        /**
         * @brief Clear event flags.
         * @param [in] mask The OR-ed flags to clear.
         * @param [out] oflags Optional pointer where to store the
         *  previous value of the flags; may be `nullptr`.
         * @retval result::ok The flags were cleared.
         * @retval EINVAL The mask is zero.
         */
        result_t
        clear (flags::mask_t mask, flags::mask_t* oflags);

        /**
         * @brief Get the flags mask.
         * @return The internal bit-mask.
         */
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
