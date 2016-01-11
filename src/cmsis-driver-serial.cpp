/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
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
            cb_event_ (cb_object_, event);
          }
      }

    } /* namespace driver */
  } /* namespace cmsis */
} /* namespace os */

// Forward C calls to C++ implementation.

void
cmsis_driver_serial_signal_event(void* object, uint32_t event)
{
  ((os::cmsis::driver::Serial*)object)->signal_event(event);
}
