# Change log

## RTOS

### v0.1.1 2016-03-11 

- initial version of the APIs announced on [ARM Connected Community](https://community.arm.com/groups/tools/blog/2016/03/11/cmsis-a-proposal-for-a-future-cmsis-written-in-c)

### v0.2.1 2016-06-21 

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

### v0.3.1 2016-07-10

Changes, in reverse chronological order:

- for consistency reasons, thread signals were renamed thread event flags
- `flags_clear()`, `flags_get()` moved from `thread` to `this_thread`
- `os_thread_stack_element_t` and `os_thread_stack_allocation_element_t` added to C API
- `os_thread_stack_get_bottom()`, `os_thread_stack_get_top()`, `os_thread_stack_check_bottom_magic()`, `os_thread_stack_check_top_magic()` added
- thread `waiting` renamed `suspended`
- validation: waiting function are allowed only if scheduler is not locked






