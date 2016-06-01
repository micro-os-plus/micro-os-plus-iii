/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
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

#include <cmsis-plus/rtos/os-c-api.h>

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include <test-c-api.h>

// ----------------------------------------------------------------------------

static const char* test_name = "Test C API";

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpadded"

// Message used in message queues.
typedef struct my_msg_s
{
  int i;
  const char* s;
} my_msg_t;

// Block used in memory pools.
typedef struct my_blk_s
{
  int i;
  const char* s;
} my_blk_t;

#pragma GCC diagnostic pop

// Thread function.
void*
func (void* args);

void*
func (void* args __attribute__((unused)))
{
  printf ("%s\n", __func__);

  return NULL;
}

// Timer function.
void
tmfunc (void* args);

void
tmfunc (void* args __attribute__((unused)))
{
  printf ("%s\n", __func__);
}

// ----------------------------------------------------------------------------

int
test_c_api (void)
{
  const char* name;
  bool flag;

  // ==========================================================================

  printf ("\n%s - Scheduler.\n", test_name);

    {
      flag = os_sched_is_started ();
      assert(flag);

      // ...
        {
          // Scheduler critical section
          os_sched_status_t st;
          st = os_sched_lock (true);

          // ...
          flag = os_sched_is_locked ();
          assert(flag);

          // ...
          os_sched_unlock (st);
        }

      // ...
      flag = os_sched_is_locked ();
      assert(!flag);
    }

    {
      // Interrupts critical section
      os_irq_status_t st;
      st = os_irq_critical_enter ();
      // ...
        {
          // Interrupts uncritical section
          os_irq_status_t st2;
          st2 = os_irq_uncritical_enter ();
          // ...
          os_irq_uncritical_exit (st2);
        }
      // ...
      os_irq_critical_exit (st);
    }

  // ==========================================================================

  printf ("\n%s - Clocks.\n", test_name);

    {
      os_sysclock_sleep_for (2);

      os_clock_timestamp_t ts;
      // Return number of ticks since epoch, or, if epoch not set, from startup.
      ts = os_sysclock_now ();

      os_sysclock_sleep_until (ts + 2);

      // Return the number of ticks since startup.
      ts = os_sysclock_steady_now ();

      // An event may resume the thread before the timeout expire.
      os_sysclock_wait_for (2);
    }

  // ==========================================================================

  printf ("\n%s - Threads.\n", test_name);

    {
      // Unnamed default thread; stack dynamically allocated.
      os_thread_t th1;
      os_thread_create (&th1, NULL, func, NULL, NULL);

      name = os_thread_get_name (&th1);

      os_thread_join (&th1, NULL);

      // The destroy() is recommended, but not mandatory when using join().
      // The test checks if join() fully destroys the thread.
      // os_thread_destroy(&th1);
    }

    {
      // Named default threads.
      os_thread_t th2;
      os_thread_create (&th2, "th2", func, NULL, NULL);

      os_thread_join (&th2, NULL);

      // os_thread_destroy(&th2);
    }

    {
      // Custom thread with static stack and lower priority.
      static char stack[2 * OS_INTEGER_RTOS_DEFAULT_STACK_SIZE_BYTES];

      os_thread_attr_t ath3;
      os_thread_attr_init (&ath3);
      ath3.th_priority = os_priority_below_normal;
      ath3.th_stack_address = stack;
      ath3.th_stack_size_bytes = sizeof(stack);

      os_thread_t th3;
      os_thread_create (&th3, "th3", func, NULL, &ath3);

      os_thread_prio_t prio;
      prio = os_thread_get_prio (&th3);
      os_thread_set_prio (os_this_thread (), prio);

      // Lower main thread priority to allow task to run.
      os_thread_set_prio (os_this_thread (), os_priority_below_normal);

      os_thread_join (&th3, NULL);

      // Restore main thread priority.
      os_thread_set_prio (os_this_thread (), os_priority_normal);

      // os_thread_destroy(&th3);
    }

  // ==========================================================================

  printf ("\n%s - Thread stack.\n", test_name);

    {
      size_t n;

      n = os_thread_stack_get_default_size ();
      os_thread_stack_set_default_size (n);

      n = os_thread_stack_get_min_size ();
      os_thread_stack_set_min_size (n);
    }

  // ==========================================================================

  printf ("\n%s - Timers.\n", test_name);

    {
      os_timer_t tm1;
      os_timer_create (&tm1, "tm1", tmfunc, NULL, NULL);

      os_sysclock_sleep_for (1); // Sync
      os_timer_start (&tm1, 1);

      os_sysclock_sleep_for (2);

      os_timer_stop (&tm1);

      name = os_timer_get_name (&tm1);
      assert(strcmp (name, "tm1") == 0);

      os_timer_destroy (&tm1);
    }

  // ==========================================================================

  printf ("\n%s - Mutexes.\n", test_name);

    {
      os_mutex_t mx1;
      os_mutex_create (&mx1, "mx1", NULL);

      os_mutex_lock (&mx1);
      os_mutex_unlock (&mx1);

      os_mutex_try_lock (&mx1);
      os_mutex_unlock (&mx1);

      os_mutex_timed_lock (&mx1, 1);
      os_mutex_unlock (&mx1);

      name = os_mutex_get_name (&mx1);
      assert(strcmp (name, "mx1") == 0);

      os_thread_prio_t prio;
      prio = os_mutex_get_prio_ceiling (&mx1);

      os_mutex_set_prio_ceiling (&mx1, prio, NULL);

      os_thread_t* th;
      th = os_mutex_get_owner (&mx1);
      if (th != NULL)
        {
          os_thread_get_name (th);
        }

      os_mutex_reset (&mx1);

      os_mutex_destroy (&mx1);
    }

    {
      // Custom mutex, with RTC.
      os_mutex_attr_t amx2;
      os_mutex_attr_init (&amx2);

      amx2.mx_priority_ceiling = os_priority_high;
      amx2.mx_protocol = os_mutex_protocol_protect;
      amx2.mx_type = os_mutex_type_recursive;
      amx2.mx_max_count = 7;
      amx2.mx_robustness = os_mutex_robustness_stalled;
      amx2.clock = os_clock_get_rtclock ();

      os_mutex_t mx2;
      os_mutex_create (&mx2, "mx2", &amx2);

      os_mutex_destroy (&mx2);
    }

  // ==========================================================================

  printf ("\n%s - Semaphores.\n", test_name);

    {
      // Default counting semaphore.
      os_semaphore_t sp1;
      os_semaphore_create (&sp1, "sp1", NULL);

      os_semaphore_post (&sp1);
      os_semaphore_wait (&sp1);

      os_semaphore_post (&sp1);
      os_semaphore_try_wait (&sp1);

      os_semaphore_post (&sp1);
      os_semaphore_timed_wait (&sp1, 1);

      os_semaphore_get_value (&sp1);

      os_semaphore_get_initial_value (&sp1);

      os_semaphore_reset (&sp1);

      name = os_semaphore_get_name (&sp1);

      os_semaphore_destroy (&sp1);
    }

    {
      // Custom semaphore.
      os_semaphore_attr_t asp2;
      os_semaphore_attr_init (&asp2);

      asp2.sm_initial_count = 3;
      asp2.sm_max_count = 7;
      asp2.clock = os_clock_get_rtclock ();

      os_semaphore_t sp2;
      os_semaphore_create (&sp2, "sp2", &asp2);

      os_semaphore_destroy (&sp2);
    }

    {
      // Binary semaphore.
      os_semaphore_t sp3;
      os_semaphore_create (&sp3, "sp3", os_semaphore_attr_get_binary ());

      os_semaphore_destroy (&sp3);
    }

    {
      // Custom binary semaphore.
      os_semaphore_attr_t asp4;
      os_semaphore_attr_init_binary (&asp4);
      asp4.clock = os_clock_get_rtclock ();

      os_semaphore_t sp4;
      os_semaphore_create (&sp4, "sp4", &asp4);

      os_semaphore_destroy (&sp4);
    }

  // ==========================================================================

  printf ("\n%s - Memory pools.\n", test_name);

  my_blk_t* blk;

    {
      // Simple pool, dynamically allocated.
      os_mempool_t p1;
      os_mempool_create (&p1, "p1", 3, sizeof(my_blk_t), NULL);

      blk = os_mempool_alloc (&p1);
      os_mempool_free (&p1, blk);

      blk = os_mempool_try_alloc (&p1);
      os_mempool_free (&p1, blk);

      blk = os_mempool_timed_alloc (&p1, 1);
      os_mempool_free (&p1, blk);

      os_mempool_destroy (&p1);
    }

    {
      // Static pool.
      static char pool[1000];

      os_mempool_attr_t ap2;
      os_mempool_attr_init (&ap2);
      ap2.mp_pool_address = pool;
      ap2.mp_pool_size_bytes = sizeof(pool);
      ap2.clock = os_clock_get_rtclock ();

      os_mempool_t p2;
      os_mempool_create (&p2, "p2", 3, sizeof(my_blk_t), &ap2);

      blk = os_mempool_alloc (&p2);

      os_mempool_free (&p2, blk);

      os_mempool_reset (&p2);

      os_mempool_destroy (&p2);
    }

  // ==========================================================================

  printf ("\n%s - Message queues.\n", test_name);

  // Define two messages.

  my_msg_t msg_out =
    { 1, "msg" };

  my_msg_t msg_in =
    { 0, NULL };

  // --------------------------------------------------------------------------

    {
      // Simple queues, dynamically allocated.
      os_mqueue_t q1;
      os_mqueue_create (&q1, "q1", 3, sizeof(my_msg_t), NULL);

      os_mqueue_send (&q1, &msg_out, sizeof(msg_out), 0);
      os_mqueue_try_send (&q1, &msg_out, sizeof(msg_out), 0);
      os_mqueue_timed_send (&q1, &msg_out, 1, sizeof(msg_out), 0);

      msg_in.i = 0;
      os_mqueue_receive (&q1, &msg_in, sizeof(msg_in), NULL);
      assert(msg_in.i = 1);

      msg_in.i = 0;
      os_mqueue_try_receive (&q1, &msg_in, sizeof(msg_in), NULL);
      assert(msg_in.i = 1);

      msg_in.i = 0;
      os_mqueue_timed_receive (&q1, &msg_in, sizeof(msg_in), 1, NULL);
      assert(msg_in.i = 1);

      const char* str = os_mqueue_get_name (&q1);
      assert(strcmp (str, "q1") == 0);

      size_t n;

      n = os_mqueue_get_capacity (&q1);
      assert(n == 3);

      n = os_mqueue_get_length (&q1);
      assert(n == 0);

      n = os_mqueue_get_msg_size (&q1);
      assert(n == sizeof(my_msg_t));

      os_mqueue_is_empty (&q1);

      os_mqueue_is_full (&q1);

      os_mqueue_reset (&q1);

      os_mqueue_destroy (&q1);
    }

    {
      // Static queue.
      // TODO: add macro to compute size.
      static char queue[1000];

      os_mqueue_attr_t aq2;
      os_mqueue_attr_init (&aq2);
      aq2.mq_queue_addr = queue;
      aq2.mq_queue_size_bytes = sizeof(queue);
      aq2.clock = os_clock_get_rtclock ();

      os_mqueue_t q2;
      os_mqueue_create (&q2, "q2", 3, sizeof(my_msg_t), &aq2);

      os_mqueue_send (&q2, &msg_out, sizeof(msg_out), 0);

      msg_in.i = 0;
      os_mqueue_receive (&q2, &msg_in, sizeof(msg_in), NULL);
      assert(msg_in.i = 1);

      os_mqueue_destroy (&q2);
    }

  // ==========================================================================

  printf ("\n%s - Event flags.\n", test_name);

    {
      os_evflags_t ev1;
      os_evflags_create (&ev1, "ev1", NULL);

      // Clear all flags.
      os_evflags_clear (&ev1, 0x0, NULL);

      os_evflags_raise (&ev1, 0x1, NULL);
      os_evflags_wait (&ev1, 0x1, NULL,
                       os_flags_mode_all | os_flags_mode_clear);

      os_evflags_raise (&ev1, 0x1, NULL);
      os_evflags_try_wait (&ev1, 0x1, NULL,
                           os_flags_mode_all | os_flags_mode_clear);

      os_evflags_raise (&ev1, 0x1, NULL);
      os_evflags_timed_wait (&ev1, 0x1, 1, NULL,
                             os_flags_mode_all | os_flags_mode_clear);

      os_evflags_get (&ev1, 0x1, os_flags_mode_clear);

      name = os_evflags_get_name (&ev1);

      os_evflags_destroy (&ev1);
    }

    {
      // Create custom event flags with a specific clock.
      os_evflags_attr_t aev2;
      os_evflags_attr_init (&aev2);

      aev2.clock = os_clock_get_rtclock ();

      os_evflags_t ev2;
      os_evflags_create (&ev2, "ev2", &aev2);

      os_evflags_destroy (&ev2);
    }

  // ==========================================================================

  printf ("\n%s - Condition variables.\n", test_name);

    {
      os_condvar_t cv1;
      os_condvar_create (&cv1, "cv1", NULL);

      os_condvar_signal (&cv1);

      os_condvar_broadcast (&cv1);

      // TODO: test os_condvar_wait()

      name = os_condvar_get_name (&cv1);

      os_condvar_destroy (&cv1);
    }

  // ==========================================================================

  printf ("\n%s - Done.\n", test_name);
  return 0;
}

