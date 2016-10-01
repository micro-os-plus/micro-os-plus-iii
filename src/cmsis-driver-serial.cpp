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

#include "posix-drivers/cmsis-driver-serial.h"
#include "Driver_USART.h"

// ----------------------------------------------------------------------------

namespace os
{
  namespace cmsis
  {
    namespace driver
    {
      // ----------------------------------------------------------------------

      Serial::Serial ()
      {
        cb_event_ = nullptr;
        cb_object_ = nullptr;
      }

      int32_t
      Serial::initialize (signal_event_t cb_event, const void* cb_object)
      {
        cb_event_ = cb_event;
        cb_object_ = cb_object;

        return do_initialize ();
      }

      // ----------------------------------------------------------------------

      void
      Serial::signal_event (uint32_t event)
      {
        if (cb_event_ != nullptr)
          {
            // Forward event to registered callback.
            cb_event_ (cb_object_, event);
          }
      }

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

// Forward C calls to C++ implementation.

void
cmsis_driver_serial_signal_event (void* object, uint32_t event)
{
  ((os::cmsis::driver::Serial*) object)->signal_event (event);
}
