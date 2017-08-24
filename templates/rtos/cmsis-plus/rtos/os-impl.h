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

#ifndef CMSIS_PLUS_RTOS_OS_IMPL_H_
#define CMSIS_PLUS_RTOS_OS_IMPL_H_

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os-config.h>

// ----------------------------------------------------------------------------

#ifdef  __cplusplus

#include <cstdint>
#include <cstddef>

namespace os
{
  namespace rtos
  {
    class Thread;

    namespace impl
    {
      // ----------------------------------------------------------------------

      // Simple collection of threads, ordered by priorities.
      class Prioritised_list
      {
      public:

        Prioritised_list ();
        ~Prioritised_list ();
#if 1
        void
        add (Thread* thread);
        void
        remove (Thread* thread);

        Thread*
        get_top (void);

        bool
        empty(void);

        std::size_t
        length ();

        // TODO add iterator begin(), end()

#endif
      protected:

        void
        remove (std::size_t pos);

      protected:

        Thread* array_[OS_INTEGER_MAX_NUMBER_OF_THREADS];
        std::size_t count_;
      };

    // ----------------------------------------------------------------------

    } /* namespace impl */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace impl
    {
      // ----------------------------------------------------------------------

      inline bool
      Prioritised_list::empty(void)
      {
        return count_ == 0;
      }

      inline std::size_t
      Prioritised_list::length ()
      {
        return count_;
      }

    // ----------------------------------------------------------------------

    } /* namespace impl */
  } /* namespace rtos */
} /* namespace os */

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#endif /* CMSIS_PLUS_RTOS_OS_IMPL_H_ */
