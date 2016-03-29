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

#ifndef POSIX_SCHED_H_
#define POSIX_SCHED_H_

#if !defined(__ARM_EABI__)
#include <sched.h>
#else

#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

// ----------------------------------------------------------------------------
// Standard definitions from:
// http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sched.h.html
// ----------------------------------------------------------------------------

  struct sched_param
  {
    int sched_priority; // process execution scheduling priority
    int sched_ss_low_priority; // Low scheduling priority for sporadic server.
    struct timespec sched_ss_repl_period; // Replenishment period for  sporadic server.
    struct timespec sched_ss_init_budget; // Initial budget for sporadic server.
    int sched_ss_max_repl; // Maximum pending replenishments for sporadic server.
  };

// First in-first out (FIFO) scheduling policy.
#define SCHED_FIFO  4
// Round robin scheduling policy.
#define SCHED_RR  2
// Another scheduling policy.
#define  SCHED_OTHER 1
// Sporadic server scheduling policy.
#define SCHED_SPORADIC  8

  int
  sched_get_priority_max (int policy);

  int
  sched_get_priority_min (int policy);

  int
  sched_getparam (pid_t, struct sched_param *);

  int
  sched_getscheduler (pid_t);

  int
  sched_rr_get_interval (pid_t, struct timespec *);

  int
  sched_setparam (pid_t, const struct sched_param *);

  int
  sched_setscheduler (pid_t, int, const struct sched_param *);

  int
  sched_yield (void);

// ----------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* __ARM_EABI__ */

#endif /* POSIX_SCHED_H_ */
