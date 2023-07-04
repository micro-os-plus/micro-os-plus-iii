# TO CHECK

The tests generally pass, as long as there are no trace prints in the
scheduler interrupt handler; otherwise they fail in several ways,
less when emulated and almost always on physical boards.

Below are some examples.

Failures of release versions were not recorded; even so,
the recorded failures should be remembered and possibly investigated at
a later time.

#### Intel macOS

```console
1: Test command: /Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc11-debug/platform-bin/rtos-apis-test

1: µOS++ IIIe version 7.0.0-beta
1: Copyright (c) 2007-2023 Liviu Ionescu
1: POSIX synthetic, running on x86_64 Darwin 21.6.0; non-preemptive
1: Scheduler frequency: 1000 ticks/sec
1: Default stack size: 32768 bytes
1: Built with GCC 11.3.0, with exceptions
...

1:  C}{c  c}allocator_stateless_polymorphic_synchronized() @0x7f914803faf0 0x1096db660
1: allocator_stateless_polymorphic_synchronized() @0x7f914803fe3e 0x1096db660
1: allocate(1) @0x7f914803f5e6
1: {c  c}do_allocate(72,8)=0x600000808000 @0x1096db660 malloc
1: {c  c}mutex() @0x600000808000 mx7
1: {c  c}lock() @0x600000808000 mx7 by 0x7f9147704680 main
1: {c  c}{C internal_try_lock_() @0x600000808000 mx7 by 0x7f9147704680 main LCK
1:  C}{c  c}unlock() @0x600000808000 mx7 by 0x7f9147704680 main
1: {c  c}{C {c  c}internal_unlock_() @0x600000808000 mx7 ULCK
1:  C}{c  c}allocator_stateless_polymorphic_synchronized() @0x7f914803fe3f 0x1096db660
1: allocate(1) @0x7f914803f5e6
1: {c  c}do_allocate(72,8)=0x600000808050 @0x1096db660 malloc
1: {c  c}mutex() @0x600000808050 mx8
1/1 Test #1: rtos-apis-test ...................***Exception: Illegal  0.02 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   0.02 sec

The following tests FAILED:
	  1 - rtos-apis-test (ILLEGAL)
Errors while running CTest
Output from these tests are in: /Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc11-debug/Testing/Temporary/LastTest.log
```

```
1: Test command: /Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc11-debug/platform-bin/rtos-apis-test
...
1: µOS++ IIIe version 7.0.0-beta
1: Copyright (c) 2007-2023 Liviu Ionescu
1: POSIX synthetic, running on x86_64 Darwin 21.6.0; non-preemptive
1: Scheduler frequency: 1000 ticks/sec
1: Default stack size: 32768 bytes
1: Built with GCC 11.3.0, with exceptions
...
1: {c  c}{C {c  c}internal_unlock_() @0x7f8af803fd80 ch-mx ULCK
1:  C}{c  c}lock() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main
1: {c  c}{C internal_try_lock_() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main LCK
1:  C}{c  c}unlock() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main
1: {c  c}{C {c  c}internal_unlock_() @0x7f8af803fd80 ch-mx ULCK
1:  C}{c  c}lock() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main
1: {c  c}{C internal_try_lock_() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main LCK
1:  C}{c  c}unlock() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main
1: {c  c}{C {c  c}internal_unlock_() @0x7f8af803fd80 ch-mx ULCK
1:  C}{c  c}lock() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main
1: {c  c}{C internal_try_lock_() @0x7f8af803fd80 ch-mx by 0x7f8af1704680 main LCK
1:  C}{c  c}
1/1 Test #1: rtos-apis-test ...................***Exception: Illegal  7.08 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   7.09 sec

The following tests FAILED:
	  1 - rtos-apis-test (ILLEGAL)
Errors while running CTest
Output from these tests are in: /Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc11-debug/Testing/Temporary/LastTest.log
```

#### Apple Silicon macOS

```console
1: Test command: /Users/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc11-release/platform-bin/rtos-apis-test
1: Test timeout computed to be: 10000000
1/1 Test #1: rtos-apis-test ...................***Exception: SegFault  7.16 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   7.17 sec

The following tests FAILED:
	  1 - rtos-apis-test (SEGFAULT)
Errors while running CTest
(repetitive)
```

```console
1: Test command: /Users/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc12-release/platform-bin/rtos-apis-test
1: Test timeout computed to be: 10000000
1/1 Test #1: rtos-apis-test ...................***Exception: SegFault  7.17 sec

0% tests passed, 1 tests failed out of 1

Total Test time (real) =   7.17 sec

The following tests FAILED:
	  1 - rtos-apis-test (SEGFAULT)
Errors while running CTest
(repetitive)
```

#### Intel Ubuntu 22

...

#### Raspberry Pi OS

...

#### Cortex-M

```console
1: Test command: /home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/qemu-cortex-m7f-cmake-debug/xpacks/.bin/qemu-system-arm "--machine" "mps2-an500" "--cpu" "cortex-m7" "--kernel" "rtos-apis-test.elf" "--nographic" "-d" "unimp,guest_errors" "--semihosting-config" "enable=on,target=native,arg=rtos-apis-test"
...
1: main(argc=1, argv=["rtos-apis-test"]);
1: internal_invoke_with_exit_() @0x207fe340 idle
1: priority(16) @0x207fe340 idle
1: {C first_fit_top::do_allocate(1024,8)=0x207f9c80,1032 @0x20003068 app
1: ::malloc(1024)=0x207f9c80
1:  C}
1: µOS++ RTOS simple APIs test
1: Built with GCC 12.2.1 20221205
...
1: {C  C}internal_invoke_with_exit_() @0x207fc978 -
1: static void os::estd::thread::run_function_object(const void*) [with F_T = std::_Bind<void (*(int, const char*))(int, const char*)>]()
1: task4(7,xyz)
1: internal_exit_() @0x207fc978 -
1: {C  C}terminated link() 0x207fc98c -
1: join() @0x207feeac
1: ~thread() @0x207fdaa8 -
1: kill() @0x207fdaa8 -
1: {C internal_destroy_() @0x207fdaa8 -
1: internal_check_stack_() @0x207fdaa8 - stack: 968/2044 bytes used
1: {C first_fit_top::do_deallocate(0x207fd2a0,2048,8) @0x207fbbc0 sys
1:  C}{C  C} C}{C first_fit_top::do_deallocate(0x207fdaa8,136,8) @0x207fbbc0 sys
1:  C}join() @0x207feeac joined
1: join() @0x207feea4
1: static void os::estd::thread::delete_function_object(const void*) [with F_T = std::_Bind<void (*(char*))(void*)>]()
1: ::operator delete(0x207f9c60,8)
1: {C first_fit_top::do_deallocate(0x207f9c60,8,8) @0x20003068 app
1:  C}internal_invoke_with_exit_() @0x207fd210 -
1: static void os::estd::thread::run_function_object(const void*) [with F_T = std::_Bind<void (*(char*))(void*)>]()
1: [HardFault]
1: Stack frame:
1:  R0   = 207FEEB7
1:  R1   = 207F9C60
1:  R2   = 207F9C64
1:  R3   = 207FEEB7
1:  R12  = 0003FF3D
1:  LR   = 00016787
1:  PC   = 00000000
1:  PSR  = 00030000
1: FSR/FAR:
1:  CFSR = 00020000
1:  HFSR = 40000000
1:  DFSR = 00000000
1: Aux Fault status registers unimplemented
1:  AFSR = 00000000
1: Misc
1:  LR/EXC_RETURN = FFFFFFED
1: NVIC: Bad read offset 0xdf0
```

```console
1: Test command: /home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/qemu-cortex-m7f-cmake-debug/xpacks/.bin/qemu-system-arm "--machine" "mps2-an500" "--cpu" "cortex-m7" "--kernel" "rtos-apis-test.elf" "--nographic" "-d" "unimp,guest_errors" "--semihosting-config" "enable=on,target=native,arg=rtos-apis-test"

1: µOS++ IIIe version 7.0.0-beta
1: Copyright (c) 2007-2023 Liviu Ionescu
1: Scheduler: µOS++ Cortex-M7 FP, preemptive, BASEPRI(4), WFI
1: System clock: 16000000 Hz
1: Scheduler frequency: 1000 ticks/sec
1: Default stack size: 2048 bytes
1: Interrupts stack size: 3072 bytes
1: Built with GCC 12.2.1 20221205, with exceptions
...
1: static void os::estd::thread::run_function_object(const void*) [with F_T = std::_Bind<void (*(int, const char*))(int, const char*)>]()
1: task4(7,xyz)
1: internal_exit_() @0x207fc978 -
1: {C  C}terminated link() 0x207fc98c -
1: join() @0x207feeac
1: ~thread() @0x207fdaa8 -
1: kill() @0x207fdaa8 -
1: {C internal_destroy_() @0x207fdaa8 -
1: internal_check_stack_() @0x207fdaa8 - stack: 968/2044 bytes used
1: {C first_fit_top::do_deallocate(0x207fd2a0,2048,8) @0x207fbbc0 sys
1:  C}{C  C} C}{C first_fit_top::do_deallocate(0x207fdaa8,136,8) @0x207fbbc0 sys
1:  C}join() @0x207feeac joined
1: join() @0x207feea4
1: static void os::estd::thread::delete_function_object(const void*) [with F_T = std::_Bind<void (*(char*))(void*)>]()
1: ::operator delete(0x207f9c50,8)
1: {C first_fit_top::do_deallocate(0x207f9c50,8,8) @0x20003068 app
1:  C}[HardFault]
1: Stack frame:
1:  R0   = 207FEEB7
1:  R1   = 207F9C50
1:  R2   = 207F9C54
1:  R3   = 207FEEB7
1:  R12  = 0003FF3D
1:  LR   = 00016787
1:  PC   = 00000000
1:  PSR  = 00030000
1: FSR/FAR:
1:  CFSR = 00020000
1:  HFSR = 40000000
1:  DFSR = 00000000
1: Aux Fault status registers unimplemented
1:  AFSR = 00000000
1: Misc
1:  LR/EXC_RETURN = FFFFFFED
1: NVIC: Bad read offset 0xdf0
```
