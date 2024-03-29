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

/*
 * The initial µOS++ RTOS API was inspired by CMSIS RTOS API v1.x,
 * Copyright (c) 2013 ARM LIMITED.
 */

/**
 * @file os.h
 * @brief Single file µOS++ RTOS definitions.
 * @details
 * The code was originally inspired by ARM CMSIS `<cmsis_os.h>` file, v1.02,
 * and tries to remain functionally close to the CMSIS specifications.
 *
 * References are to C++ Standard ISO/IEC 14882:2011(E)
 * Third edition (2011-09-01).
 *
 * Major improvements:
 * - no more macros required to define objects
 * - allow static memory allocations for all objects
 * - very close to POSIX ([IEEE Std 1003.1, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/nframe.html))
 * - specifically designed to facilitate the implementation of
 *   C++ standard thread library (ISO/IEC 14882:2011)
 * - standard POSIX errors definitions used
 * - improved usability, by defining both simple (using defaults)
 *   and complex (using attributes) object constructors (feature
 *   inspired by POSIX threads attributes)
 * - improved readability with explicit three-fold waiting functions
 *   (for example: lock(), try-lock(), timed-lock(), similar to POSIX threads)
 * - POSIX condition variable added
 * - versatile clocks added (Systick_clock, Realtime_clock)
 * - all object can wait using either Systick or Realtime_clock
 *
 * Differences from POSIX:
 * - timeouts are relative, while in POSIX are absolute. Rationale:
 * although internally it may be easier to compare absolute dates,
 * it looked more convenient for the user to enter short durations
 * instead of very large absolute times.
 */

/*
 * TODO:
 * - make Thread virtual, to allow create_hook/delete_hook functionality
 * - make most classes virtual, to allow post_hook functionality
 * - event timestamps: add derived classes that capture the event timestamp
 * - add object type in base class
 * - add Wait_list in base class
 *
 * Notes:
 * - the try_wait(), try_flags_wait() names are probably not very inspired,
 * but at least they are consistent in POSIX.
 */

#ifndef CMSIS_PLUS_RTOS_OS_H_
#define CMSIS_PLUS_RTOS_OS_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#if defined(OS_USE_OS_APP_CONFIG_H)
#include <cmsis-plus/os-app-config.h>
#endif

#include <cmsis-plus/rtos/os-decls.h>

#include <cmsis-plus/rtos/os-sched.h>

// Includes a reference to critical sections.
#include <cmsis-plus/rtos/os-memory.h>

#include <cmsis-plus/rtos/os-thread.h>
#include <cmsis-plus/rtos/os-clocks.h>
#include <cmsis-plus/rtos/os-timer.h>
#include <cmsis-plus/rtos/os-mutex.h>
#include <cmsis-plus/rtos/os-condvar.h>
#include <cmsis-plus/rtos/os-semaphore.h>
#include <cmsis-plus/rtos/os-mempool.h>
#include <cmsis-plus/rtos/os-mqueue.h>
#include <cmsis-plus/rtos/os-evflags.h>

#include <cmsis-plus/rtos/os-hooks.h>

// More or less at the end, when all other definitions are available.
#include <cmsis-plus/rtos/os-inlines.h>
#include <cmsis-plus/rtos/port/os-inlines.h>

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wc++98-compat"
#endif

// ----------------------------------------------------------------------------

namespace os
{
  namespace rtos
  {
    namespace scheduler
    {
      /**
       * @cond ignore
       */

      /**
       * @brief Head of the list with all top level and detached threads.
       */
      extern thread::threads_list top_threads_list_;

      /**
       * @endcond
       */

      /**
       * @brief Get the children threads.
       * @param [in] th Pointer to thread or nullptr.
       * @return Reference to children list.
       */
      thread::threads_list&
      children_threads (thread* th);

    } /* namespace scheduler */

    // ------------------------------------------------------------------------

    namespace interrupts
    {
#if defined(OS_HAS_INTERRUPTS_STACK) || defined(__DOXYGEN__)

      /**
       * @brief Get the interrupts stack
       * @ingroup cmsis-plus-rtos-core
       * @par Parameters
       *  None.
       * @return Pointer to stack object instance.
       */
      class thread::stack*
      stack (void);

#else
#endif /* defined(OS_HAS_INTERRUPTS_STACK) */

    } /* namespace interrupts */

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif

    // ------------------------------------------------------------------------
    /**
     * @brief  Create an object that is owned by a `shared_ptr` and is
     *  allocated using the RTOS system allocator.
     * @ingroup cmsis-plus-rtos-memres
     * @param  args  Arguments for the _T_ object's constructor.
     * @return A shared_ptr that owns the newly created object.
     * @throw * An exception may be thrown from `allocate()` or
     *          from the constructor of _T_.
     */
    template<typename T, typename ... Args>
      inline typename std::enable_if<!std::is_array<T>::value,
          std::shared_ptr<T> >::type
      make_shared (Args&&... args)
      {
        // -Wno-psabi to disble the ABI warning.
        typedef typename std::remove_const<T>::type T_nc;
        return std::allocate_shared<T> (memory::allocator<T_nc> (),
                                        std::forward<Args>(args)...);
      }

#pragma GCC diagnostic pop

  } /* namespace rtos */
} /* namespace os */

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------

#endif /* __cplusplus */

#include <cmsis-plus/rtos/os-c-api.h>

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
