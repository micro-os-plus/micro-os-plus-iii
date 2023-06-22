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

#if defined(__ARM_EABI__)

// ----------------------------------------------------------------------------

#if !defined(_LITE_EXIT)
#define _LITE_EXIT
#endif

/*
 *  Common definitions for atexit-like routines
 */

#if defined(__cplusplus)
extern "C"
{
#endif

  enum __atexit_types
  {
    __et_atexit, //
    __et_onexit, //
    __et_cxa
  };

  extern void
  __call_exitprocs (int, void*);

  typedef void
  (*exit_func_t) (void);

  extern int
  __register_exitproc (int, exit_func_t fn, void*, void*);

  extern void
  os_run_fini_array (void);

#if defined(__cplusplus)
}
#endif

// ----------------------------------------------------------------------------

#endif /* defined(__ARM_EABI__) */
