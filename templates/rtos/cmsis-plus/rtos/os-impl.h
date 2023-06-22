/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
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
