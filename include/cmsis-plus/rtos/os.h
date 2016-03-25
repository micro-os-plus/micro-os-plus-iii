/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 * Copyright (c) 2013 ARM LIMITED
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

/**
 * @file os.h
 * @brief Single file CMSIS++ RTOS definitions.
 * @details
 * This file is part of the CMSIS++ proposal, intended as a CMSIS
 * replacement for C++ applications.
 *
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
 * - add libc/newlib errno() function
 * - add a separate Stack object
 *
 * Notes:
 * - the try_wait(), try_sig_wait() names are probably not very inspired,
 * but at least they are consistent in POSIX.
 */

#ifndef CMSIS_PLUS_RTOS_OS_H_
#define CMSIS_PLUS_RTOS_OS_H_

// ----------------------------------------------------------------------------

#if defined(__cplusplus)

#include <cmsis-plus/rtos/os-decls.h>

#include <cmsis-plus/rtos/os-sched.h>
#include <cmsis-plus/rtos/os-thread.h>
#include <cmsis-plus/rtos/os-clocks.h>
#include <cmsis-plus/rtos/os-timer.h>
#include <cmsis-plus/rtos/os-mutex.h>
#include <cmsis-plus/rtos/os-condvar.h>
#include <cmsis-plus/rtos/os-semaphore.h>
#include <cmsis-plus/rtos/os-mempool.h>
#include <cmsis-plus/rtos/os-mqueue.h>
#include <cmsis-plus/rtos/os-evflags.h>

#endif /* __cplusplus */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_RTOS_OS_H_ */
