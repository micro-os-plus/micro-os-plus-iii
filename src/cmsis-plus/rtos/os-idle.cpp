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

#include <cassert>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// Better be the last, to undef putchar()
#include <cmsis-plus/diag/trace.h>

// ----------------------------------------------------------------------------

#if !defined(OS_INCLUDE_RTOS_PORT_THREAD)

namespace os
{
  namespace rtos
  {
    namespace scheduler
    {

      // ======================================================================

      void*
      _idle_func (thread::func_args_t args);

      void
      _create_idle (void)
      {
        static stack::element_t idle_stack[OS_INTEGER_RTOS_IDLE_STACK_SIZE_BYTES
            / sizeof(stack::element_t)];

        static thread::Attributes attr
          { "idle" };
        attr.th_stack_address = idle_stack;
        attr.th_stack_size_bytes = sizeof(idle_stack);

        // The CMSIS RTOS validator creates threads with `priority::idle`,
        // so, to be sure that the system idle thread has the lowest priority,
        // go below the idle priority.
        attr.th_priority = thread::priority::idle - 1;

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wexit-time-destructors"
#endif
        static Thread idle_thread
          { attr, _idle_func, nullptr };
#pragma GCC diagnostic pop
      }

      void*
      _idle_func (thread::func_args_t args __attribute__((unused)))
      {
        while (true)
          {
            port::scheduler::_wait_for_interrupt ();
            // TODO: cleanup destroyed threads.
            this_thread::yield ();
          }
      }

    // ------------------------------------------------------------------------
    } /* namespace scheduler */
  } /* namespace rtos */
} /* namespace os */

#endif