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

/*
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
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

        std::size_t
        get_length ();

        // TODO add iterator begin(), end()

#endif
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

      inline std::size_t
      Prioritised_list::get_length ()
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
