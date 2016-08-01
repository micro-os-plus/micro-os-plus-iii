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

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>
#include <cmsis-plus/rtos/internal/os-flags.h>

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace internal
    {
      // ------------------------------------------------------------------------

      result_t
      event_flags::raise (flags::mask_t mask, flags::mask_t* oflags)
      {
        os_assert_err(mask != 0, EINVAL);

        assert(port::interrupts::is_priority_valid ());

          {
            // ----- Enter critical section -------------------------------------
            interrupts::critical_section ics;

            if (oflags != nullptr)
              {
                *oflags = flags_mask_;
              }

            flags_mask_ |= mask;

            // ----- Exit critical section --------------------------------------
          }
        return result::ok;
      }

      bool
      event_flags::check_raised (flags::mask_t mask, flags::mask_t* oflags,
                                 flags::mode_t mode)
      {
        if (mask == flags::any)
          {
            // Any flag will do it.
            if (flags_mask_ != 0)
              {
                if (oflags != nullptr)
                  {
                    *oflags = flags_mask_;
                  }

                if (mode & flags::mode::clear)
                  {
                    // Clear them all.
                    flags_mask_ = 0;
                  }
                return true;
              }
          }
        else if (((((mode & flags::mode::all) != 0))
            && ((flags_mask_ & mask) == mask))
            || (((mode & flags::mode::any) != 0) && ((flags_mask_ & mask) != 0)))
          {
            if (oflags != nullptr)
              {
                *oflags = (flags_mask_ & mask);
              }

            if (mode & flags::mode::clear)
              {
                // Clear desired flags.
                flags_mask_ &= ~mask;
              }
            return true;
          }

        return false;
      }

      flags::mask_t
      event_flags::get (flags::mask_t mask, flags::mode_t mode)
      {
        flags::mask_t ret;
          {
            // ----- Enter critical section -------------------------------------
            interrupts::critical_section ics;

            if (mask == 0)
              {
                // Return the entire mask.
                ret = flags_mask_;
              }
            else
              {
                ret = flags_mask_ & mask;
                if ((mode & flags::mode::clear) != 0)
                  {
                    // Clear the selected bits; leave the rest untouched.
                    flags_mask_ &= ~mask;
                  }
              }
            // ----- Exit critical section --------------------------------------
          }

        // Return the selected bits.
        return ret;
      }

      result_t
      event_flags::clear (flags::mask_t mask, flags::mask_t* oflags)
      {
        os_assert_err(mask != 0, EINVAL);

          {
            // ----- Enter critical section -------------------------------------
            interrupts::critical_section ics;

            if (oflags != nullptr)
              {
                *oflags = flags_mask_;
              }

            // Clear the selected bits; leave the rest untouched.
            flags_mask_ &= ~mask;

            // ----- Exit critical section --------------------------------------
          }

        return result::ok;
      }

    // --------------------------------------------------------------------------

    } /* namespace internal */
  } /* namespace rtos */
} /* namespace os */
