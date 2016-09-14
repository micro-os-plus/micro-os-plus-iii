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

#ifndef CMSIS_PLUS_RTOS_OS_EVFLAGS_H_
#define CMSIS_PLUS_RTOS_OS_EVFLAGS_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>
#include <cmsis-plus/rtos/internal/os-flags.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {

    // ========================================================================

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

    /**
     * @brief Synchronised **event flags**.
     * @headerfile os.h <cmsis-plus/rtos/os.h>
     * @ingroup cmsis-plus-rtos-evflag
     */
    class event_flags : public internal::object_named_system
    {
    public:

      // ======================================================================

      /**
       * @brief Event flags attributes.
       * @headerfile os.h <cmsis-plus/rtos/os.h>
       * @ingroup cmsis-plus-rtos-evflag
       */
      class attributes : public internal::attributes_clocked
      {
      public:

        /**
         * @name Constructors & Destructor
         * @{
         */

        /**
         * @brief Construct an event flags attributes object instance.
         * @par Parameters
         *  None.
         */
        constexpr
        attributes ();

        // The rule of five.
        attributes (const attributes&) = default;
        attributes (attributes&&) = default;
        attributes&
        operator= (const attributes&) = default;
        attributes&
        operator= (attributes&&) = default;

        /**
         * @brief Destruct the event flags attributes object instance.
         */
        ~attributes () = default;

        /**
         * @}
         */

      public:

        /**
         * @name Public Member Variables
         * @{
         */

        // Public members; no accessors and mutators required.
        // Warning: must match the type & order of the C file header.
        // Add more attributes here.
        /**
         * @}
         */

      }; /* class attributes */

      /**
       * @brief Default event flags initialiser.
       * @ingroup cmsis-plus-rtos-evflag
       */
      static const attributes initializer;

      /**
       * @name Constructors & Destructor
       * @{
       */

      /**
       * @brief Construct an event flags object instance.
       * @param [in] attr Reference to attributes.
       */
      event_flags (const attributes& attr = initializer);

      /**
       * @brief Construct a named event flags object instance.
       * @param [in] name Pointer to name.
       * @param [in] attr Reference to attributes.
       */
      event_flags (const char* name, const attributes& attr = initializer);

      /**
       * @cond ignore
       */

      // The rule of five.
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
       * @brief Destruct the event flags object instance.
       */
      ~event_flags ();

      /**
       * @}
       */

      /**
       * @name Operators
       * @{
       */

      /**
       * @brief Compare event flags.
       * @retval true The given event flags object is the same as this
       *  event flags.
       * @retval false The event flags are different.
       */
      bool
      operator== (const event_flags& rhs) const;

      /**
       * @}
       */

    public:

      /**
       * @name Public Member Functions
       * @{
       */

      /**
       * @brief Wait for event flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  if `flags::any`, any flag raised will do it.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  in the mask are expected, and if the flags should be cleared.
       * @retval result::ok All expected flags were raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      wait (flags::mask_t mask, flags::mask_t* oflags,
            flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Try to wait for event flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  if `flags::any`, any flag raised will do it.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  in the mask are expected, and if the flags should be cleared.
       * @retval result::ok All expected flags were raised.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EWOULDBLOCK The expected condition did not occur.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      try_wait (flags::mask_t mask, flags::mask_t* oflags = nullptr,
                flags::mode_t mode = flags::mode::all | flags::mode::clear);

      /**
       * @brief Timed wait for event flags.
       * @param [in] mask The expected flags (OR-ed bit-mask);
       *  if `flags::any`, any flag raised will do it.
       * @param [out] oflags Pointer where to store the current flags;
       *  may be `nullptr`.
       * @param [in] mode Mode bits to select if either all or any flags
       *  in the mask are expected, and if the flags should be cleared.
       * @param [in] timeout Timeout to wait.
       * @retval result::ok All expected flags are raised.
       * @retval EPERM Cannot be invoked from an Interrupt Service Routines.
       * @retval ETIMEDOUT The expected condition did not occur during the
       *  entire timeout duration.
       * @retval EINVAL The mask is outside of the permitted range.
       * @retval EINTR The operation was interrupted.
       * @retval ENOTRECOVERABLE Wait failed.
       */
      result_t
      timed_wait (flags::mask_t mask, clock::duration_t timeout,
                  flags::mask_t* oflags = nullptr,
                  flags::mode_t mode = flags::mode::all | flags::mode::clear);

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
      raise (flags::mask_t mask, flags::mask_t* oflags = nullptr);

      /**
       * @brief Clear event flags.
       * @param [in] mask The OR-ed flags to clear.
       * @param [out] oflags Optional pointer where to store the
       *  previous value of the flags; may be `nullptr`.
       * @retval result::ok The flags were cleared.
       * @retval EINVAL The mask is zero.
       */
      result_t
      clear (flags::mask_t mask, flags::mask_t* oflags = nullptr);

      /**
       * @brief Get (and possibly clear) event flags.
       * @param [in] mask The OR-ed flags to get/clear; can be `flags::any`.
       * @param [in] mode Mode bits to select if the flags should be
       *  cleared (the other bits are ignored).
       * @return The selected bits from the flags mask.
       */
      flags::mask_t
      get (flags::mask_t mask, flags::mode_t mode = flags::mode::clear);

      /**
       * @brief Check if there are threads waiting.
       * @par Parameters
       *  None.
       * @retval true There are threads waiting.
       * @retval false There are no threads waiting.
       */
      bool
      waiting (void);

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

#if !defined(OS_USE_RTOS_PORT_EVENT_FLAGS)
      internal::waiting_threads_list list_;
      clock* clock_;
#endif

#if defined(OS_USE_RTOS_PORT_EVENT_FLAGS)
      friend class port::event_flags;
      os_evflags_port_data_t port_;
#endif

      /**
       * @brief The event flags.
       */
      internal::event_flags event_flags_;

      /**
       * @endcond
       */

      /**
       * @}
       */

    };

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */

// ===== Inline & template implementations ====================================

namespace os
{
  namespace rtos
  {
    constexpr
    event_flags::attributes::attributes ()
    {
      ;
    }

  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_EVFLAGS_H_ */
