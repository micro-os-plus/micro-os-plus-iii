/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2023-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit.
 */

#ifndef PLATFORM_NUCLEO_F411RE_BLINK_LED_H_
#define PLATFORM_NUCLEO_F411RE_BLINK_LED_H_

#pragma GCC diagnostic ignored "-Wold-style-cast"

#include "stm32f4xx_hal.h"

class blink_led
{
public:
  blink_led ()
  {
    // No hardware changes on constructors.
  }

  ~blink_led ()
  {
    // No hardware changes on destructors.
  }

  void
  power_up ()
  {
    // Enable GPIOA clock.
    __HAL_RCC_GPIOA_CLK_ENABLE ();

    // Configure PA5 as output, push-pull, no pull-up/down, high speed.
    GPIO_InitTypeDef GPIO_InitStruct = { .Pin = GPIO_PIN_5,
                                         .Mode = GPIO_MODE_OUTPUT_PP,
                                         .Pull = GPIO_NOPULL,
                                         .Speed = GPIO_SPEED_FREQ_HIGH,
                                         .Alternate = 0 };
    HAL_GPIO_Init (GPIOA, &GPIO_InitStruct);

    // Turn LED on (active high).
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
  }

  void
  blink ()
  {
    // Toggle LED.
    HAL_GPIO_TogglePin (GPIOA, GPIO_PIN_5);
  }

  void
  power_down ()
  {
    // Turn LED off (active high).
    HAL_GPIO_WritePin (GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);

    // Optionally de-initialize PA5.
    HAL_GPIO_DeInit (GPIOA, GPIO_PIN_5);
  }
};

#endif // PLATFORM_NUCLEO_F411RE_BLINK_LED_H_
