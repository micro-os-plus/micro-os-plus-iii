/*
 * This file is part of the µOS++ project (https://micro-os-plus.github.io/).
 * Copyright (c) 2016-2025 Liviu Ionescu. All rights reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 *
 * If a copy of the license was not distributed with this file, it can
 * be obtained from https://opensource.org/licenses/mit.
 */

// ----------------------------------------------------------------------------

#include <cmsis-plus/rtos/os.h>

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <sys/time.h>

#include <cmsis-plus/diag/instrumentation.h>

// ----------------------------------------------------------------------------

using namespace os;
using namespace os::rtos;

int
os_main (int argc, char* argv[])
{
  // Align with the next tick.
  sysclock.sleep_for (1);

  // Sleep exactly two ticks.
  sysclock.sleep_for (2);

  {
    int* p = new int;
    delete p;
  }

  {
    mutex* mtx = new mutex ("my-mutex");

    mtx->lock ();
    mtx->unlock ();

    mtx->try_lock ();
    mtx->unlock ();

    delete mtx;
  }

  sysclock.sleep_for (1);

  {
    semaphore* sem = new semaphore_counting ("my-semaphore", 7, 0);

    sem->post ();
    sem->post ();
    sem->post ();

    sem->wait ();
    sem->try_wait ();
    sem->timed_wait (42);

    delete sem;
  }

  sysclock.sleep_for (1);

  {
    message_queue* mqueue = new message_queue ("my-mqueue", 7, sizeof (int));

    int value = 1234;
    mqueue->send (&value, sizeof (value));
    value = 1235;
    mqueue->try_send (&value, sizeof (value));
    value = 1235;
    mqueue->timed_send (&value, sizeof (value), 42);

    value = 0;
    mqueue->receive (&value, sizeof (value));
    value = 0;
    mqueue->try_receive (&value, sizeof (value));
    value = 0;
    mqueue->timed_receive (&value, sizeof (value), 42);

    delete mqueue;
  }

  sysclock.sleep_for (1);

  {
    memory_pool* mpool = new memory_pool ("my-mpool", 7, sizeof (int));

    int* p1 = static_cast<int*> (mpool->alloc ());
    int* p2 = static_cast<int*> (mpool->try_alloc ());
    int* p3 = static_cast<int*> (mpool->timed_alloc (42));
    mpool->free (p1);
    mpool->free (p2);
    mpool->free (p3);

    delete mpool;
  }

  sysclock.sleep_for (1);

  {
    event_flags* evf = new event_flags ("my-evflags");

    evf->raise (0x1248);
    evf->wait (0x1000);
    evf->try_wait (0x0200);
    evf->timed_wait (0x0040, 42);

    delete evf;
  }

    sysclock.sleep_for (1);

  {
    this_thread::thread().flags_raise (0x1248);
    
    this_thread::flags_wait (0x1000);
    this_thread::flags_try_wait (0x0200);
    this_thread::flags_timed_wait (0x0040, 42);
  }

  // Add a delay to allow the trace to be sent.
  sysclock.sleep_for (1);

  return 0;
}

// ----------------------------------------------------------------------------
