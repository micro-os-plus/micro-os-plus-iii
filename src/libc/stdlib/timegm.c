/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2018-2023 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit/.
 */

#include <time.h>
#include <stdlib.h>

// ----------------------------------------------------------------------------

/**
 * As there is no `timegm()` function neither in newlib nor in POSIX,
 * we add one here (based on a simplified newlib `mktime()`).
 *
 * It is used in Chan FatFS to convert dates fields.
 */

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif

#define _SEC_IN_MINUTE 60L
#define _SEC_IN_HOUR 3600L
#define _SEC_IN_DAY 86400L

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
static const int DAYS_IN_MONTH[12] =
  { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
#pragma GCC diagnostic pop

#define _DAYS_IN_MONTH(x) ((x == 1) ? days_in_feb : DAYS_IN_MONTH[x])

#pragma GCC diagnostic push
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wreserved-identifier"
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"
#endif
static const int _DAYS_BEFORE_MONTH[12] =
  { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
#pragma GCC diagnostic pop

#define _ISLEAP(y) (((y) % 4) == 0 && (((y) % 100) != 0 || (((y)+1900) % 400) == 0))
#define _DAYS_IN_YEAR(year) (_ISLEAP(year) ? 366 : 365)

#pragma GCC diagnostic pop

static void
validate_structure (struct tm *tim_p);

time_t
timegm (struct tm* tim_p);

// ----------------------------------------------------------------------------

time_t
timegm (struct tm* tim_p)
{
  time_t tim = 0;
  long days = 0;
  int year;

  /* validate structure */
  validate_structure (tim_p);

  /* compute hours, minutes, seconds */
  tim += tim_p->tm_sec + (tim_p->tm_min * _SEC_IN_MINUTE)
      + (tim_p->tm_hour * _SEC_IN_HOUR);

  /* compute days in year */
  days += tim_p->tm_mday - 1;
  days += _DAYS_BEFORE_MONTH[tim_p->tm_mon];
  if (tim_p->tm_mon > 1 && _DAYS_IN_YEAR (tim_p->tm_year) == 366)
    days++;

  /* compute day of the year */
  tim_p->tm_yday = (int)days;

  if (tim_p->tm_year > 10000 || tim_p->tm_year < -10000)
    return (time_t) -1;

  /* compute days in other years */
  if ((year = tim_p->tm_year) > 70)
    {
      for (year = 70; year < tim_p->tm_year; year++)
        days += _DAYS_IN_YEAR(year);
    }
  else if (year < 70)
    {
      for (year = 69; year > tim_p->tm_year; year--)
        days -= _DAYS_IN_YEAR(year);
      days -= _DAYS_IN_YEAR(year);
    }

  /* compute total seconds */
  tim += (days * _SEC_IN_DAY);

  /* compute day of the week */
  if ((tim_p->tm_wday = (int)((days + 4) % 7)) < 0)
    tim_p->tm_wday += 7;

  return tim;
}

/*
 * This is an unfortunate code duplication, but the newlib functions
 * are also static and cannot be used here.
 */
static void
validate_structure (struct tm *tim_p)
{
  div_t res;
  int days_in_feb = 28;

#pragma GCC diagnostic push
#if defined(__clang__)
#elif defined(__GNUC__)
// For div()
#pragma GCC diagnostic ignored "-Waggregate-return"
#endif
  /* calculate time & date to account for out of range values */
  if (tim_p->tm_sec < 0 || tim_p->tm_sec > 59)
    {
      res = div (tim_p->tm_sec, 60);
      tim_p->tm_min += res.quot;
      if ((tim_p->tm_sec = res.rem) < 0)
        {
          tim_p->tm_sec += 60;
          --tim_p->tm_min;
        }
    }

  if (tim_p->tm_min < 0 || tim_p->tm_min > 59)
    {
      res = div (tim_p->tm_min, 60);
      tim_p->tm_hour += res.quot;
      if ((tim_p->tm_min = res.rem) < 0)
        {
          tim_p->tm_min += 60;
          --tim_p->tm_hour;
        }
    }

  if (tim_p->tm_hour < 0 || tim_p->tm_hour > 23)
    {
      res = div (tim_p->tm_hour, 24);
      tim_p->tm_mday += res.quot;
      if ((tim_p->tm_hour = res.rem) < 0)
        {
          tim_p->tm_hour += 24;
          --tim_p->tm_mday;
        }
    }

  if (tim_p->tm_mon < 0 || tim_p->tm_mon > 11)
    {
      res = div (tim_p->tm_mon, 12);
      tim_p->tm_year += res.quot;
      if ((tim_p->tm_mon = res.rem) < 0)
        {
          tim_p->tm_mon += 12;
          --tim_p->tm_year;
        }
    }
#pragma GCC diagnostic pop

  if (_DAYS_IN_YEAR (tim_p->tm_year) == 366)
    days_in_feb = 29;

  if (tim_p->tm_mday <= 0)
    {
      while (tim_p->tm_mday <= 0)
        {
          if (--tim_p->tm_mon == -1)
            {
              tim_p->tm_year--;
              tim_p->tm_mon = 11;
              days_in_feb = ((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ? 29 : 28);
            }
          tim_p->tm_mday += _DAYS_IN_MONTH(tim_p->tm_mon);
        }
    }
  else
    {
      while (tim_p->tm_mday > _DAYS_IN_MONTH(tim_p->tm_mon))
        {
          tim_p->tm_mday -= _DAYS_IN_MONTH(tim_p->tm_mon);
          if (++tim_p->tm_mon == 12)
            {
              tim_p->tm_year++;
              tim_p->tm_mon = 0;
              days_in_feb = ((_DAYS_IN_YEAR (tim_p->tm_year) == 366) ? 29 : 28);
            }
        }
    }
}

// ----------------------------------------------------------------------------
