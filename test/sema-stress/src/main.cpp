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

#include <stm32f4xx_hal.h>

#include <cmsis-plus/rtos/os.h>
#include <cmsis-plus/diag/trace.h>

#include <test.h>

using namespace os;

extern RNG_HandleTypeDef hrng;

int
os_main (int argc __attribute__((unused)), char* argv[] __attribute__((unused)))
{
  printf ("\nSemaphore stress test.\n");
#if defined(__clang__)
  printf ("Built with clang " __VERSION__ ".\n");
#else
  printf ("Built with GCC " __VERSION__ ".\n");
#endif

  HAL_NVIC_SetPriority (TIM2_IRQn, 10, 0);
  HAL_NVIC_EnableIRQ (TIM2_IRQn);

  __HAL_RCC_RNG_CLK_ENABLE ();

  hrng.Instance = RNG;
  HAL_RNG_Init (&hrng);

  uint32_t seed;

  int status;
  for (int i = 0;; ++i)
    {
      HAL_RNG_GenerateRandomNumber (&hrng, &seed);

      printf ("\nIteration %d\n", i);
      printf ("Seed %lu\n", seed);

      srand (seed);

      status = run_tests ();
      if (status)
        {
          return status;
        }
    }
}

Hw_timer tmr;

void
Hw_timer::start (uint32_t period)
{

  __TIM2_CLK_ENABLE ();

  th.Instance = TIM2;
  th.Init.Prescaler = 1;
  th.Init.CounterMode = TIM_COUNTERMODE_DOWN;
  th.Init.Period = period;
  th.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

  HAL_TIM_Base_Init (&th);
  HAL_TIM_Base_Start_IT (&th);
}

void
Hw_timer::stop ()
{
  HAL_TIM_Base_Stop_IT (&th);
  HAL_TIM_Base_DeInit (&th);
}

uint32_t
Hw_timer::in_clk_hz (void)
{
  return SystemCoreClock / 4;
}

void
(*tim_callback) (void);

extern "C"
{
  void
  HAL_TIMEx_BreakCallback (TIM_HandleTypeDef *htim);
  void
  HAL_TIMEx_CommutationCallback (TIM_HandleTypeDef *htim);
  void
  HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim);
}

void
HAL_TIMEx_BreakCallback (TIM_HandleTypeDef *htim __attribute__((unused)))
{
}

void
HAL_TIMEx_CommutationCallback (TIM_HandleTypeDef *htim __attribute__((unused)))
{
}

void
HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef *htim __attribute__((unused)))
{
  if (tim_callback != nullptr)
    {
      tim_callback ();
    }
}

extern "C" void
TIM2_IRQHandler ();

void
TIM2_IRQHandler ()
{
  HAL_TIM_IRQHandler (&tmr.th);
}

//------------------
