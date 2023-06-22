/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

// Empty initialisations. Normally should not be used, but if so,
// they must be used as a pair, and the startup code and linker
// script must be updated.

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wreserved-identifier"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#endif

void
_init (void);

void
_fini(void);

void
__attribute__((weak))
_init (void)
{
}

void
__attribute__((weak))
_fini (void)
{
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
