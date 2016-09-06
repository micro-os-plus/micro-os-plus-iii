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

void
iterate_threads (os_thread_t* th, unsigned int depth);

static const char* thread_state[] =
  { "undf", "inac", "rdy", "run", "wait", "term", "dead" };

/*
 * To compute thread percentages, use totals provided by:
 * - os_sched_stat_get_context_switches();
 * - os_sched_stat_get_cpu_cycles();
 */
void
iterate_threads (os_thread_t* th, unsigned int depth)
{
  os_iterator_t it = os_children_threads_iter_begin (th);
  os_iterator_t end = os_children_threads_iter_end (th);

  while (it != end)
    {
      // Get the pointer to the thread from the iterator.
      os_thread_t* p = os_children_threads_iter_get (it);

      // Get the pointer to the stack object instance.
      os_thread_stack_t* pst = os_thread_get_stack (p);

      // Get stack size & used, in bytes.
      unsigned int used = (unsigned int) (os_thread_stack_get_size (pst)
          - os_thread_stack_get_available (pst));
      unsigned int used_proc = (unsigned int) (used * 100
          / os_thread_stack_get_size (pst));
      unsigned int st = (unsigned int) (os_thread_get_state (p));

      os_statistics_counter_t thread_switches =
          os_thread_stat_get_context_switches (p);

      os_statistics_duration_t thread_cpu_cycles =
          os_thread_stat_get_cpu_cycles (p);

      printf ("%s, %u%% (%u/%u), %s, %u, %u \n", os_thread_get_name (p),
              used_proc, used, (unsigned int) (os_thread_stack_get_size (pst)),
              thread_state[st], (unsigned int) thread_switches,
              (unsigned int) thread_cpu_cycles);

      // Go down one level.
      iterate_threads (p, depth + 1);

      // Increment the iterator to the next element in the list.
      it = os_children_threads_iter_next (it);
    }
}

// ----------------------------------------------------------------------------

int
test_c_api (void)
{
#pragma GCC diagnostic push

#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

  const char* name;
  bool flag;

#pragma GCC diagnostic pop

  // ==========================================================================

#if 1
  printf ("\nThreads:\n");

  iterate_threads (NULL, 0);
#endif

  // ==========================================================================

  printf ("\n%s - Scheduler.\n", test_name);

    {
      flag = os_sched_is_started ();
      assert(flag);

      // ...
        {
          // Scheduler critical section
          os_sched_state_t st;
          st = os_sched_lock ();

          // ...
          flag = os_sched_is_locked ();
          assert(flag);

          // ...
          os_sched_set_locked (st);
        }

      // ...
      flag = os_sched_is_locked ();
      assert(!flag);
    }

    {
      // Interrupts critical section
      os_irq_state_t st;
      st = os_irq_critical_enter ();
      // ...
        {
          // Interrupts uncritical section
          os_irq_state_t st2;
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
      // Return number of ticks since startup.
      ts = os_sysclock_now ();

      os_sysclock_sleep_until (ts + 2);

      // Return the number of ticks since startup.
      ts = os_clock_steady_now (os_clock_get_sysclock ());

      // An event may resume the thread before the timeout expire.
      os_sysclock_wait_for (2);
    }

  // ==========================================================================

  printf ("\n%s - Threads.\n", test_name);

    {
      // Unnamed static thread; stack dynamically allocated.
      os_thread_t th1;
      os_thread_construct (&th1, NULL, func, NULL, NULL);

      name = os_thread_get_name (&th1);

      os_thread_join (&th1, NULL);

      // The destroy() is recommended, but not mandatory when using join().
      // The test checks if join() fully destroys the thread.
      os_thread_destruct (&th1);
    }

    {
      // Named static threads; stack dynamically allocated.
      os_thread_t th2;
      os_thread_construct (&th2, "th2", func, NULL, NULL);

      os_thread_join (&th2, NULL);

      os_thread_destruct (&th2);
    }

    {
      // Named dynamically allocated thread; dynamically allocated stack.
      os_thread_t* th3;
      th3 = os_thread_new ("th3", func, NULL, NULL);

      os_thread_join (th3, NULL);

      os_thread_delete (th3);
    }

    {
      // Custom static thread with static stack and lower priority.
      static char stack[2 * OS_INTEGER_RTOS_DEFAULT_STACK_SIZE_BYTES];

      os_thread_attr_t ath3;
      os_thread_attr_init (&ath3);
      ath3.th_priority = os_thread_priority_below_normal;
      ath3.th_stack_address = stack;
      ath3.th_stack_size_bytes = sizeof(stack);

      os_thread_t th3;
      os_thread_construct (&th3, "th3", func, NULL, &ath3);

      os_thread_prio_t prio;
      prio = os_thread_get_priority (&th3);
      os_thread_set_priority (os_this_thread (), prio);

      // Lower main thread priority to allow task to run.
      os_thread_set_priority (os_this_thread (),
                              os_thread_priority_below_normal);

      os_thread_join (&th3, NULL);

      // Restore main thread priority.
      os_thread_set_priority (os_this_thread (), os_thread_priority_normal);

      os_thread_destruct (&th3);
    }

  // ==========================================================================

  printf ("\n%s - Thread stack.\n", test_name);

    {
      size_t n;

      n = os_thread_stack_get_default_size ();
      os_thread_stack_set_default_size (n);

      n = os_thread_stack_get_min_size ();
      os_thread_stack_set_min_size (n);

      os_thread_stack_t* stack;
      stack = os_thread_get_stack (os_this_thread ());

      os_thread_stack_get_bottom (stack);

      os_thread_stack_get_top (stack);

      os_thread_stack_check_bottom_magic (stack);
      os_thread_stack_check_top_magic (stack);
    }

  // ==========================================================================

  printf ("\n%s - Thread event flags.\n", test_name);

    {
      os_this_thread_flags_clear (os_flags_all, NULL);

      os_thread_flags_raise (os_this_thread (), 0x3, NULL);
      os_this_thread_flags_wait (0x3, NULL, os_flags_mode_all);

      os_thread_flags_raise (os_this_thread (), 0x3, NULL);
      os_this_thread_flags_try_wait (0x3, NULL, os_flags_mode_all);

      os_thread_flags_raise (os_this_thread (), 0x3, NULL);
      os_this_thread_flags_timed_wait (0x3, 10, NULL, os_flags_mode_all);
    }

  // ==========================================================================

  printf ("\n%s - Timers.\n", test_name);

    {
      os_timer_t tm1;
      os_timer_construct (&tm1, "tm1", tmfunc, NULL, NULL);

      os_sysclock_sleep_for (1); // Sync
      os_timer_start (&tm1, 1);

      os_sysclock_sleep_for (2);

      os_timer_stop (&tm1);

      name = os_timer_get_name (&tm1);
      assert(strcmp (name, "tm1") == 0);

      os_timer_destruct (&tm1);
    }

    {
      // Periodic timer
      os_timer_t tm2;
      os_timer_construct (&tm2, "tm2", tmfunc, NULL,
                          os_timer_attr_get_periodic ());

      os_sysclock_sleep_for (1); // Sync
      os_timer_start (&tm2, 1);

      os_sysclock_sleep_for (2);

      os_timer_stop (&tm2);

      name = os_timer_get_name (&tm2);
      assert(strcmp (name, "tm2") == 0);

      os_timer_destruct (&tm2);
    }

    {
      os_timer_t* tm3;
      tm3 = os_timer_new ("tm3", tmfunc, NULL, NULL);

      os_sysclock_sleep_for (1); // Sync
      os_timer_start (tm3, 1);

      os_sysclock_sleep_for (2);

      os_timer_stop (tm3);

      name = os_timer_get_name (tm3);
      assert(strcmp (name, "tm3") == 0);

      os_timer_delete (tm3);
    }

  // ==========================================================================

  printf ("\n%s - Mutexes.\n", test_name);

    {
      os_mutex_t mx1;
      os_mutex_construct (&mx1, "mx1", NULL);

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

      os_mutex_get_type (&mx1);

      os_mutex_get_protocol (&mx1);

      os_mutex_get_robustness (&mx1);

      os_mutex_reset (&mx1);

      os_mutex_destruct (&mx1);
    }

    {
      // Custom mutex, with RTC.
      os_mutex_attr_t amx2;
      os_mutex_attr_init (&amx2);

      amx2.mx_priority_ceiling = os_thread_priority_high;
      amx2.mx_protocol = os_mutex_protocol_protect;
      amx2.mx_type = os_mutex_type_recursive;
      amx2.mx_max_count = 7;
      amx2.mx_robustness = os_mutex_robustness_stalled;
      amx2.clock = os_clock_get_rtclock ();

      os_mutex_t mx2;
      os_mutex_construct (&mx2, "mx2", &amx2);

      os_mutex_destruct (&mx2);
    }

    {
      // Recursive mutex.
      os_mutex_t mx3;
      os_mutex_construct (&mx3, "mx3", os_mutex_attr_get_recursive ());

      os_mutex_destruct (&mx3);
    }

    {
      // Custom recursive mutex, with RTC.
      os_mutex_attr_t amx4;
      os_mutex_attr_recursive_init (&amx4);

      amx4.clock = os_clock_get_rtclock ();

      os_mutex_t mx4;
      os_mutex_construct (&mx4, "mx4", &amx4);

      os_mutex_destruct (&mx4);
    }

    {
      os_mutex_t* mx5;
      mx5 = os_mutex_new ("mx5", NULL);

      os_mutex_lock (mx5);
      os_mutex_unlock (mx5);

      os_mutex_delete (mx5);
    }

  // ==========================================================================

  printf ("\n%s - Semaphores.\n", test_name);

    {
      // Binary semaphore, start at 0.
      os_semaphore_t sp1;
      os_semaphore_binary_construct (&sp1, "sp1", 0);

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

      os_semaphore_destruct (&sp1);
    }

    {
      // Custom semaphore.
      os_semaphore_attr_t asp2;
      os_semaphore_attr_init (&asp2);

      asp2.sm_initial_value = 3;
      asp2.sm_max_value = 7;
      asp2.clock = os_clock_get_rtclock ();

      os_semaphore_t sp2;
      os_semaphore_construct (&sp2, "sp2", &asp2);

      os_semaphore_destruct (&sp2);
    }

    {
      // Counting semaphore, 7 resources, start at 7.
      os_semaphore_t sp3;
      os_semaphore_counting_construct (&sp3, "sp3", 7, 7);

      os_semaphore_destruct (&sp3);
    }

    {
      // Custom binary semaphore.
      os_semaphore_attr_t asp4;
      os_semaphore_attr_binary_init (&asp4, 0);
      asp4.clock = os_clock_get_rtclock ();

      os_semaphore_t sp4;
      os_semaphore_construct (&sp4, "sp4", &asp4);

      os_semaphore_destruct (&sp4);
    }

    {
      // Binary semaphore, start at 0.
      os_semaphore_t* sp5;
      sp5 = os_semaphore_binary_new ("sp5", 0);

      os_semaphore_post (sp5);
      os_semaphore_wait (sp5);

      os_semaphore_delete (sp5);
    }

  // ==========================================================================

  printf ("\n%s - Memory pools.\n", test_name);

  my_blk_t* blk;

    {
      // Simple pool, dynamically allocated.
      os_mempool_t p1;
      os_mempool_construct (&p1, "p1", 3, sizeof(my_blk_t), NULL);

      blk = os_mempool_alloc (&p1);
      os_mempool_free (&p1, blk);

      blk = os_mempool_try_alloc (&p1);
      os_mempool_free (&p1, blk);

      blk = os_mempool_timed_alloc (&p1, 1);
      os_mempool_free (&p1, blk);

      os_mempool_destruct (&p1);
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
      os_mempool_construct (&p2, "p2", 3, sizeof(my_blk_t), &ap2);

      blk = os_mempool_alloc (&p2);

      os_mempool_free (&p2, blk);

      os_mempool_reset (&p2);

      os_mempool_destruct (&p2);
    }

    {
      // Simple pool, dynamically allocated.
      os_mempool_t* p3;
      p3 = os_mempool_new ("p3", 3, sizeof(my_blk_t), NULL);

      blk = os_mempool_alloc (p3);
      os_mempool_free (p3, blk);

      os_mempool_delete (p3);
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
      os_mqueue_construct (&q1, "q1", 3, sizeof(my_msg_t), NULL);

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

#pragma GCC diagnostic push

#if defined(__clang__)
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif

      const char* str;
      size_t n;

#pragma GCC diagnostic pop

      str = os_mqueue_get_name (&q1);
      assert(strcmp (str, "q1") == 0);

      n = os_mqueue_get_capacity (&q1);
      assert(n == 3);

      n = os_mqueue_get_length (&q1);
      assert(n == 0);

      n = os_mqueue_get_msg_size (&q1);
      assert(n == sizeof(my_msg_t));

      os_mqueue_is_empty (&q1);

      os_mqueue_is_full (&q1);

      os_mqueue_reset (&q1);

      os_mqueue_destruct (&q1);
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
      os_mqueue_construct (&q2, "q2", 3, sizeof(my_msg_t), &aq2);

      os_mqueue_send (&q2, &msg_out, sizeof(msg_out), 0);

      msg_in.i = 0;
      os_mqueue_receive (&q2, &msg_in, sizeof(msg_in), NULL);
      assert(msg_in.i = 1);

      os_mqueue_destruct (&q2);
    }

    {
      // Simple queues, dynamically allocated.
      os_mqueue_t* q3;
      q3 = os_mqueue_new ("q3", 3, sizeof(my_msg_t), NULL);

      os_mqueue_send (q3, &msg_out, sizeof(msg_out), 0);

      os_mqueue_receive (q3, &msg_in, sizeof(msg_in), NULL);

      os_mqueue_delete (q3);
    }

  // ==========================================================================

  printf ("\n%s - Event flags.\n", test_name);

    {
      os_evflags_t ev1;
      os_evflags_construct (&ev1, "ev1", NULL);

      // Clear all flags.
      os_evflags_clear (&ev1, os_flags_all, NULL);

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

      os_evflags_destruct (&ev1);
    }

    {
      // Initialise custom event flags with a specific clock.
      os_evflags_attr_t aev2;
      os_evflags_attr_init (&aev2);

      aev2.clock = os_clock_get_rtclock ();

      os_evflags_t ev2;
      os_evflags_construct (&ev2, "ev2", &aev2);

      os_evflags_destruct (&ev2);
    }

    {
      os_evflags_t* ev3;
      ev3 = os_evflags_new ("ev3", NULL);

      // Clear all flags.
      os_evflags_clear (ev3, os_flags_all, NULL);

      os_evflags_raise (ev3, 0x1, NULL);
      os_evflags_wait (ev3, 0x1, NULL, os_flags_mode_all | os_flags_mode_clear);

      os_evflags_delete (ev3);
    }

  // ==========================================================================

  printf ("\n%s - Condition variables.\n", test_name);

    {
      os_condvar_t cv1;
      os_condvar_construct (&cv1, "cv1", NULL);

      os_condvar_signal (&cv1);

      os_condvar_broadcast (&cv1);

      // TODO: test os_condvar_wait()

      name = os_condvar_get_name (&cv1);

      os_condvar_destruct (&cv1);
    }

    {
      os_condvar_t* cv2;
      cv2 = os_condvar_new ("cv2", NULL);

      os_condvar_signal (cv2);

      os_condvar_delete (cv2);
    }

  // ==========================================================================

  printf ("\n%s - Done.\n", test_name);
  return 0;
}

