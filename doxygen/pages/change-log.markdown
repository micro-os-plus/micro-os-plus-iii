# Change log

## RTOS

Records in reverse chronological order.

### v6.3.15 2018-11-19

- [#47] stack::available() comment (size in bytes)
- [#46]: protected mutex, do not keep ownership if error
- [#45]: rtos/os-core.cpp: check locked()

### v6.3.14 2018-07-04

- [#44] os_main.cpp: add os_main_thread cast
- [#35] add missing `_fini()`
- [#17] fix `_LITE_EXIT` redefinition
- rtos/thread: unlock dangling mutexes

- add support for Chan FatFS to the POSIX I/O layer
- add `file_descriptors_manager::used()` to tell how many file 
descriptors are used
- [#41] add nonstandard `timegm()`, as the opposite of `gmtime()`
- implement alignment in memory allocators
- [#40] posix: add `statvfs()`/`fstatvfs()`
- [#36] posix: add `tcdrain()`/`tcflush()`/`tcgetattr()`/`tcsendbreak()`/
`tcgetattr()` to POSIX aliases
- [#29] add thread top try/catch for exceptions; support for C++ exceptions 
was tested and is now available
- [#20] add sys/ioctl.h

### v6.3.13 2017-08-26

- posix-io/c-syscalls-posix.cpp: __posix_gettimeofday() calls rtclock.now ()
- add posix/termios.h
- add posix-io/tty class
- posix/*.h: update guard macros

### v6.3.12 2017-08-24

- [#13] Fix typo (aquire -> acquire)
- [#13] fix typos in documentation
- replace most CMSIS++ references by µOS++

### v6.3.11 2017-01-02

- os-mqueue.cpp: remove wrong assert from *_receive()
- cortexm/os-core.cpp: setMSP() with uint32_t* volatile vectors_addr
- cortexm/os-core.cpp: align thread stack to 8 byte boundary
- cortexm/os-core.cpp: assert both thread stack and MSP aligned

### v6.3.10 2016-11-27

- os-clocks: check timestamps to trigger timer actions after deep sleep
- first-fit-top: fix free list chunk init
- startup/initialize-hardware.c: use `__ARM_FP` instead of `__VFP_FP__`
- micro-os-plus-iii-cortexm.git/os-core: fix PendSV stack frame for FP
- micro-os-plus-iii-cortexm.git/os-core: fix setMSP() optimisation bug

### v6.3.9 2016-10-13

- assert stack size > min_size_bytes_
- diag/trace.cpp: `puts()` accepts empty string
- rtos/os-main.cpp: display interrupts stack size
- rtos/os-clock: add `update_for_slept_time()`
- rename `tests` -> `test`
- reorganise test with subfolders
- rework xpack.json (rename to **µOS++ IIIe**)

### v6.3.8 2016-10-05

- refine thread user storage support, available only when `OS_INCLUDE_RTOS_CUSTOM_THREAD_USER_STORAGE`
- startup: temporarily disable weak `os_startup_initialize_hardware()`
- rename `startup.c` -> `startup.cpp` to be able to call C++ code, like the code to initialise the main stack
- rename `OS_USE_RTOS_PORT_SYSTICK_CLOCK_SLEEP_FOR` -> `OS_USE_RTOS_PORT_CLOCK_SYSTICK_WAIT_FOR`
- rename `OS_USE_RTOS_PORT_REALTIME_CLOCK_SLEEP_FOR` -> `OS_USE_RTOS_PORT_CLOCK_REALTIME_WAIT_FOR`
- os-c-wrapper: fix object creation return values
- idle thread: use separate `os_rtos_idle_actions ()`
- integrate posix-io & posix-driver git subtrees
- rename folder `cmsis-plus/drivers` -> `cmsis-plus/driver` (singular)
- update posix-io & posix-driver to all lower case naming convention
- add separate utils, with generic utility code, like lists
- automate char device registry by using lists; no more need to manually add/remove devices

### v6.3.7 2016-09-14

- always include `assert_failed()` (used by STM HAL)
- for ARMv7-M, in default handlers use BKPT(0) only if C_DEBUGEN is enabled, to prevent crashes when not connected to debugger
- add `os_rtos_idle_enter_power_saving_mode_hook()`
- rename include folder `cmsis-plus/iso` -> `cmsis-plus/estd`
- add memory resource manager `first-fit-top` for variable size allocations
- add memory resource manager `lifo` for one-time allocations
- add memory resource manager `null` to completely disable dynamic allocations
- add `os_startup_initialize_free_store()` to initialise the application free store
- if `OS_INTEGER_RTOS_DYNAMIC_MEMORY_SIZE_BYTES` is defined, optionally initialise a separate RTOS system allocation area
 and class specific pools
- adjust `_sbrk()` to acknowledge all available RAM is used
- refurbish C `malloc` & C++ `new`/`delete` to be thread safe and use the memory resource manager
- for bare bone, if dynamic memory is not used, the default manager is `null_resource`
- for POSIX, the default is `malloc_resource`
- the standard definitions were moved to the C++17-like `estd::pmr` namespace
- `estd::malloc()` is no longer needed and was removed, the standard malloc() is now thread safe
- a C API was added for memory resources
- add statistics to `memory_resource`
- rename C API `_create` -> `_construct`, `_destroy` -> `_destruct` (add compatibility macros for the old names)
- to make use of the system allocator in system classes, add custom `new`/`delete` operators to class `object_named_system`, and use it as base for all system objects
- add `rtos::make_shared<>` to use the system allocator (experimental, to be further refined with thread safe)
- for Cortex-M, manage the interrupts stack via the `thread::stack` class; initialise interrupts stack with pattern and check the bottom marker in the idle thread
- add `os_irq_get_stack()` to the C API 
- add `os_terminate_goodbye()` to display memory usage statistics
- refubish the exit code to avoid crashes when building freestanding apps
- rename `os_initialize_args()` -> `os_startup_initialize_args()`
- rename `os_initialize_hardware_early()` -> `os_startup_initialize_hardware_early()`
- rename `os_initialize_hardware()` -> `os_startup_initialize_hardware()`
- add _out of memory_ hooks to memory managers
- add `os_terminate()` as a portable function to terminate the application (implemented as reset on Cortex-M) 
- add the `block_pool` class to manage pools of memory blocks
- add `block_pool_typed_allocated` and `block_pool_typed_inclusive` templates
- add `first_fit_top_allocated` and `first_fit_top_inclusive` templates
- add `lifo_allocated` and `lifo_inclusive` templates
- add `os_startup_create_thread_idle()` as weak
- add `OS_INTEGER_RTOS_ALLOC_MUTEX_POOL_SIZE` and similar, to create pools of storage for system objects
- rename  `thread_static<>` -> `thread_inclusive<>`
- rename `memory_pool_static<>` -> `memory_pool_inclusive<>`
- rename `message_queue_static<>` -> `message_queue_inclusive<>`

#### Problematic incompatible changes

Experience proved that one of the above changes was trickier than planned: the rename of the `os_startup_initialize_hardware()`, since without the renamed function the linker used the weak version of this function. To help developers, the weak definition was temporarily disabled, so if this function was not yet renamed in the application, the linker will complain. 

### v6.3.6 2016-08-07

- move lists to `internal::`
- rename `named_object` -> `internal::object_named`
- rename `clocked_attributes` -> `internal::attribute_clocked`
- rename `Allocator` -> `allocator_type`
- add `thread::priority_inherited()`, `thread::priority_inherited(int)`
- make `thread::priority()` public
- reimplement POSIX mutex inheritance 
- implement POSIX robust mutexes
- make default mutex inheritable
- add class mutex_recursive
- prefix protected functions with `internal_`

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

### v6.3.3 2016-07-11

- rtos/c api: add `os_clock_get_hrclock()`
- doxy: add details for thread attributes
- rtos/thread state: remove `inactive`, `os_thread_state_inactive`
- rtos/thread: rename `sched_prio()` -> `priority()`
- rename `os_thread_get_priority()`, `os_thread_set_priority()`
- rtos/thread: rename `thread::sched_state()` -> `state()`
- rename `os_thread_get_state()`.

### v6.3.2 2016-07-11

- fix `thread::flags_try_wait()` spelling
- semaphores refurbished (make default as binary, add classes `semaphore_binary` and `semaphore_counting`)
- add `binary_attributes`
- os-semaphore: add explicit `initial_value` to `os_semaphore_attr_binary_init()` and `os_semaphore_binary_create()`
- os-semaphore: rename `attributes_binary`, `attributes_counting`, `initializer_binary`

### v6.3.1 2016-07-10

- validation: waiting function are allowed only if scheduler is not locked
- thread `waiting` renamed `suspended`
- `os_thread_stack_get_bottom()`, `os_thread_stack_get_top()`, `os_thread_stack_check_bottom_magic()`, `os_thread_stack_check_top_magic()` added
- `os_thread_stack_element_t` and `os_thread_stack_allocation_element_t` added to C API
- `flags_clear()`, `flags_get()` moved from `thread` to `this_thread`
- for consistency reasons, thread signals were renamed thread event flags

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

### v0.1.1 2016-03-11 

- initial version of the APIs announced on [ARM Connected Community](https://community.arm.com/groups/tools/blog/2016/03/11/cmsis-a-proposal-for-a-future-cmsis-written-in-c)

---
