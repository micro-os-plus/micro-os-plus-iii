# Change log

## RTOS

### v0.1.1 2016-03-11 

- initial version of the APIs announced on [ARM Connected Community](https://community.arm.com/groups/tools/blog/2016/03/11/cmsis-a-proposal-for-a-future-cmsis-written-in-c)

### v6.2.1 2016-06-21 

First fully functional reference implementation. The C and C++ APIs were completely refurbished and use all lower case letters.

Changes, in reverse chronological order:

- C API updated, with complete reference manual
- rename `OS_USE_SEMIHOSTING_SYSCALLS`
- add `OS_EXCLUDE_RTOS_IDLE_SLEEP`
- rename header `<cmsis-plus/os-app-config.h>`
- support for SEGGER RTT added
- thread statistics (context switches & cpu cycles)
- high resolution clock added
- add iterators to get thread children
- add iterators to C API
- implement intrusive lists for threads
- check stack overflow with each context switch
- update the ISO C++ API & add to tests
- templates for static allocation
- complete refurbish of C++ API to use embedded classes instead of namespaces
- add explicit object names to all objects constructors
- update all to lower case
- add `new_delete_allocator`
- prefix all init functions with `os_`
- many other, too many to mention, inherent for first versions...

### v6.3.1 2016-07-10

- validation: waiting function are allowed only if scheduler is not locked
- thread `waiting` renamed `suspended`
- `os_thread_stack_get_bottom()`, `os_thread_stack_get_top()`, `os_thread_stack_check_bottom_magic()`, `os_thread_stack_check_top_magic()` added
- `os_thread_stack_element_t` and `os_thread_stack_allocation_element_t` added to C API
- `flags_clear()`, `flags_get()` moved from `thread` to `this_thread`
- for consistency reasons, thread signals were renamed thread event flags

### v6.3.2 2016-07-11

- fix `thread::flags_try_wait()` spelling
- semaphores refurbished (make default as binary, add classes `semaphore_binary` and `semaphore_counting`)
- add `binary_attributes`
- os-semaphore: add explicit `initial_value` to `os_semaphore_attr_binary_init()` and `os_semaphore_binary_create()`
- os-semaphore: rename `attributes_binary`, `attributes_counting`, `initializer_binary`

### v6.3.3 2016-07-11

- rtos/c api: add `os_clock_get_hrclock()`
- doxy: add details for thread attributes
- rtos/thread state: remove `inactive`, `os_thread_state_inactive`
- rtos/thread: rename `sched_prio()` -> `priority()`
- rename `os_thread_get_priority()`, `os_thread_set_priority()`
- rtos/thread: rename `thread::sched_state()` -> `state()`
- rename `os_thread_get_state()`.

### v6.3.4 2016-07-21

- rtos/os-semaphore: remove defaults, initial value must be explicitly specified
- doxy: re-enable Operators section
- rtos/os-mutex: add consistent\_ & recoverable\_ flags; not yet fully functional
- rename `recursive_attributes` -> `attributes_recursive`
- rename `recursive_initializer` -> `initializer_recursive`
- rename `normal_initializer` -> `initializer_normal`
- rename `os_mutex_attr_init_recursive()` -> `os_mutex_attr_recursive_init()`
- rtos: add `scheduler::preemptive()` & `scheduler::preemptive(bool)`
- add `OS_BOOL_RTOS_SCHEDULER_PREEMPTIVE`
- add `os_sched_is_preemptive()` & `os_sched_set_preemptive()`.

### v6.3.5 2016-07-31

- addd `OS_TRACE_RTOS_LISTS_CLOCKS`
- refurbish scheduler `lock()`, `unlock()`, `locked(state_t)`
- rename `os_irq_status_t` -> `os_irq_state_t`
- rename `interrupts::status_t` -> `interrupts::state_t`
- flags `clear()` no longer accept 0 mask; use `flags::all`
- add mutex accessors `type()`, `protocol()`, `robustness()`
- add `class double_list_iterator`; use it in `waiting_threads_list`
- rename `timer::periodic_attributes` -> `timer::attributes_periodic`
- add `internal::event_flags`; not public.

---
