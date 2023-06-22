# Tests

## Overview

The µOS++ testing strategy is to compile the sources with as many
toolchains as possible, and run them on as many platforms as possible.

There is a GitHub Actions CI workflow that runs a selection of the
tests on every push; for details see
[ci.yml](../.github/workflows/ci.yml).

A second workflow is triggered manually, and runs all available tests
on all supported platforms; for details see
[test-all.yml](../.github/workflows/test-all.yml)

## Platforms

The supported platforms are:

- `platform-native` - run the test applications as native process
  on the development machine
- `platform-qemu-cortex-m7f` - run the tests as fully semihosted applications
  on a QEMU mps2-an500 emulated board (an Arm Cortex-M7F development board)
- `platform-qemu-cortex-m0` - run the tests as fully semihosted applications
  on a QEMU mps2-an385 emulated board (an Arm Cortex-M3 development board,
  used to run the M0 code)

The tests are performed on GNU/Linux, macOS and Windows.

Exactly the same source files are used on all platforms, without
changes.

It is planned to add more platforms, like RISC-V, but no dates are set.

## Toolchains

For a better portability, the builds are repeated with multiple toolchains,
even with multiple versions of the same toolchain.

For native tests, the toolchains used are:

- GCC 11, 12
- clang 12, 13, 14, 15

For Cortex-M tests, the toolchain is arm-none-eabi-gcc 11.

## Tests details

### rtos

A simple test to exercise most of the RTOS APIs.

The Cortex-M tests are generally ok, except a HardFault.

On Apple Silicon macoS, gcc segFaults and was disabled.

The native tests occasionally fail:

- on Intel macOS, both gcc and clang hang
- on Apple Silicon macoS, gcc segFaults, all clang seem functional
- on Intel Ubuntu 22, clang & gcc occasionally fail
- on Raspberry Pi OS, both clang & gcc occasionally fail

#### Intel macOS

```console
1: main(argc=1, argv=["/Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc12-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with GCC 12.2.0
1:
join() @0x7fb145804080 th5 joined
1: ~thread() @0x7fb145804080 th5
1: {c  c}{C do_deallocate(0x7fb145804080,536,16) @0x10e4e8640 malloc
1:  C}{c  c}{c  c}{C do_allocate(536,16)=0x7fb145804080 @0x10e4e8640 malloc
1:  C}{c  c}thread() @0x7fb145804080 th6
1: {c  c}{C do_allocate(32768,16)=0x7fb145010200 @0x10e4e8640 malloc
1:  C}{c  c}internal_construct_() @0x7fb145804080 th6 p96 stack{0x7fb145010200,32768}
1: {c  c}{C port::context::create() getcontext 0x7fb145804168
1: port::context::create() makecontext 0x7fb145804168
1: resume() @0x7fb145804080 th6 96
1: {c ready link() front +96 16
1:  c} C}{c  c}yield() from main
1: port::scheduler::reschedule()
1: {c port::scheduler::reschedule() from main 2 1
1: {c  c}ready link() middle 96 +96
1: ready unlink_head() 0x7fb145804080 th6
(hang)
```

```console
1: main(argc=1, argv=["/Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-clang12-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with clang xPack x86_64 Clang 12.0.1
...
1:  C}{c  c}"folder1-with-long-name"
1: lock() @0x7fedf8027ed0 ch-mx by 0x7fedf5704290 main
1: {c  c}{C internal_try_lock_() @0x7fedf8027ed0 ch-mx by 0x7fedf5704290 main LCK
1:  C}{c  c}unlock() @0x7fedf8027ed0 ch-mx by 0x7fedf5704290 main
1: {c  c}{C {c  c}internal_unlock_() @0x7fedf8027ed0 ch-mx ULCK
1:  C}{c  c}lock() @0x7fedf8027ed0 ch-mx by 0x7fedf5704290 main
1: {c  c}{C internal_try_lock_() @0x7fedf8027ed0 ch-mx by 0x7fedf5704290 main LCK
1:  C}{c  c}unlock() @0x7fedf8027ed0 ch-mx by 0x7fedf5704290 main
(hang)
```

With the system ucontext:

```
1: main(argc=1, argv=["/Users/ilg/MyProjects/micro-os-plus.github/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-clang12-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with clang xPack x86_64 Clang 12.0.1
...
1: Test POSIX I/O - Block device locked - C++ API
1: lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2ed0 mx2 ULCK
1:  C}{c  c}lock() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2ed0 mx2 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}lock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C internal_try_lock_() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main LCK
1:  C}{c  c}unlock() @0x10e0f2da8 mx1 by 0x7fa854f05cd0 main
1: {c  c}{C {c  c}internal_unlock_() @0x10e0f2da8 mx1 ULCK
1:  C}{c  c}unlock() @0
1/1 Test #1: rtos-apis-test ...................***Exception: Illegal  7.31 sec

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

clang seems functional.

#### Intel Ubuntu 22

```console
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-clang13-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with clang xPack x86_64 Clang 13.0.1
1:
...
1: port::scheduler::reschedule()
1: {c port::scheduler::reschedule() from idle 2 1
1: {c  c}ready link() empty +16
1: ready unlink_head() 0x1a28fb0 idle
1:  c}port::scheduler::reschedule() same idle
1: yield() to idle
1: wait_for_interrupt()
1: {i {t {c  c}{c  c}{c  c} t} i}yield() from idle
1: port::scheduler::reschedule()
(hang)
```

```console
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-clang14-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with clang xPack x86_64 Clang 14.0.6
1:
...
1: ::operator new(131072)=0x1e6b3f0
1:  C}{c  c}{c  c}{C do_allocate(4100,16)=0x1e5b330 @0x2b3788 malloc
1: ::operator new(4100)=0x1e5b330
1:  C}{c  c}
1: test_diskio(0x1e4ade0, 3, 0x1e5b330, 4100)
1: ---- Test cycle 1 of 3 ----
1:  Number of sectors is 128
(hang)
```

```
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-clang14-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with clang xPack x86_64 Clang 14.0.6
1:
...
1: {c port::scheduler::reschedule() from idle 2 1
1: {c  c}ready link() empty +16
1: ready unlink_head() 0x1db3fb0 idle
1:  c}port::scheduler::reschedule() same idle
1: yield() to idle
1: wait_for_interrupt()
1: {i {t {c  c}{c  c}{c  c} t} i}yield() from idle
(hang)
```

```
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc12-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with GCC 12.2.0
...
1: {c port::scheduler::reschedule() from idle 2 1
1: {c  c}ready link() empty +16
1: ready unlink_head() 0x55ac25e0abc0 idle
1:  c}port::scheduler::reschedule() same idle
1: yield() to idle
1: wait_for_interrupt()
(hang)
```

#### Raspberry Pi OS

```console
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-clang14-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with clang xPack aarch64 Clang 14.0.6
...
1: {c  c}{C {c  c}internal_unlock_() @0x7f8d004ec0 ch-mx ULCK
1:  C}{c  c}lock() @0x7f8d004ec0 ch-mx by 0x37649d10 main
1: {c  c}{C internal_try_lock_() @0x7f8d004ec0 ch-mx by 0x37649d10 main LCK
1:  C}{c  c}unlock() @0x7f8d004ec0 ch-mx by 0x37649d10 main
1: {c  c}{C {c  c}internal_unlock_() @0x7f8d004ec0 ch-mx ULCK
1:  C}{c  c}::operator delete(0x3765ca60)
(hang)
```

```console
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc11-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with GCC 11.3.0
...
1:  c}port::scheduler::reschedule()
1: {c port::scheduler::reschedule() from idle 2 1
1: {c  c}ready link() back 96 +16
1: ready unlink_head() 0x55c2184920 main
1:  c}port::scheduler::reschedule() swapcontext idle -> main
1: join() @0x7f854e6390 - joined
1: join() @0x7f854e76d0 th2
(hang)
```

```console
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-gcc11-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with GCC 11.3.0
...
1:  C}{c  c}{c  c}{C  C}{c  c}resume() @0x55c2184920 main 96
1: {c ready link() empty +96
1:  c}port::scheduler::reschedule()
1: {c port::scheduler::reschedule() from idle 2 1
1: {c  c}ready link() back 96 +16
1: ready unlink_head() 0x55c2184920 main
1:  c}port::scheduler::reschedule() swapcontext idle -> main
1: join() @0x7f854e6390 - joined
1: join() @0x7f854e76d0 th2
(hang)
```

```console
1: main(argc=1, argv=["/home/ilg/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests/build/native-cmake-clang14-debug/platform-bin/rtos-apis-test"]);
1:
1: µOS++ RTOS simple APIs test
1: Built with clang xPack aarch64 Clang 14.0.6
...
1:  c}port::scheduler::reschedule() setcontext main
1: yield() to main
1: {c  c}{C do_allocate(32,16)=0x31f3b3c0 @0x2e0cb0 malloc
1: ::operator new(32)=0x31f3b3c0
1:  C}{c  c}join() @0x31f3b3f0 th6
1: internal_suspend_() @0x31f28d10 main
1: {c  c}port::scheduler::reschedule()
1: {c port::scheduler::reschedule() from main 3 1
(hang)
```
