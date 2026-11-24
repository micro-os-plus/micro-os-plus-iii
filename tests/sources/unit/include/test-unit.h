/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2026 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#ifndef TEST_UNIT_H_
#define TEST_UNIT_H_

#if defined(__cplusplus)
extern "C"
{
#endif

  int
  test_unit_memory (void);

  int
  test_unit_rtos (void);

  int
  test_unit_c_api (void);

#if defined(__cplusplus)
}
#endif

#endif /* TEST_UNIT_H_ */
