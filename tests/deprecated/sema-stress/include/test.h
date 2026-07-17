/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2016-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can be
 * obtained from https://opensource.org/licenses/mit.
 */

#ifndef TEST_H_
#define TEST_H_

class Hw_timer
{
public:


  void
  start (uint32_t period);

  void
  stop (void);

  uint32_t
  in_clk_hz(void);

public:

  TIM_HandleTypeDef th;
};

extern Hw_timer tmr;

int
run_tests ();

extern void
(*tim_callback) (void);

#endif /* TEST_H_ */
