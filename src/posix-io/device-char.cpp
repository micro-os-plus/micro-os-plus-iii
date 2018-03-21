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

#include <cmsis-plus/posix-io/device-char.h>
#include <cmsis-plus/posix-io/device-registry.h>

#include <cstring>
#include <cassert>
#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ========================================================================

    device_char::device_char (device_char_impl& impl, const char* name) :
        device
          { impl, type::device, name }
    {
      trace::printf ("device_char::%s(\"%s\")=@%p\n", __func__, name_, this);

      device_registry<device>::link (this);
    }

    device_char::~device_char ()
    {
      trace::printf ("device_char::%s() @%p %s\n", __func__, this, name_);

      registry_links_.unlink ();

      name_ = nullptr;
    }

    // ========================================================================

    device_char_impl::device_char_impl (device_char& self) :
        device_impl
          { self }
    {
      trace::printf ("device_char_impl::%s()=@%p\n", __func__, this);
    }

    device_char_impl::~device_char_impl ()
    {
      trace::printf ("device_char_impl::%s() @%p\n", __func__, this);
    }

  // ==========================================================================
  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
