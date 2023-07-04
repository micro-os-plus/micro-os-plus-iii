# Tests

## Overview

The µOS++ testing strategy is to compile the sources with as many
toolchains as possible, and run them on as many platforms as possible.

There is a GitHub Actions CI workflow that runs a selection of the
tests on every push; for details see
[ci.yml](../.github/workflows/ci.yml) (to be activated soon for automated
tests).

A second workflow is triggered manually, and runs all available tests
on all supported platforms; for details see
[test-all.yml](../.github/workflows/test-all.yml) (to be activated soon).

## Platforms

The supported platforms for running the µOS++ IIIe tests are:

- `platform-native` - run the test applications as native process
  on the development machine, compiled with gcc or clang
- `platform-qemu-cortex-m7f` - run the tests as fully semihosted applications
  on a QEMU mps2-an500 emulated board (an Arm Cortex-M7F development board)
- `platform-qemu-cortex-m3` - run the tests as fully semihosted applications
  on a QEMU mps2-an385 emulated board (an Arm Cortex-M3 development board)
- `platform-qemu-cortex-m0` - run the tests as fully semihosted applications
  on a QEMU mps2-an385 emulated board (an Arm Cortex-M3 development board,
  used to run the M0 code)
- `platform-raspberrypi-pico` - run the tests on a physical Raspberry Pi
  Pico (Cortex-M0+) via OpenOCD semihosting
- `platform-nucleo-f767zi` - run the tests on a physical NUCLEO-Z767ZI
  board (Cortex-M7) via OpenOCD semihosting

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

For Cortex-M tests, the toolchain is arm-none-eabi-gcc 12.

## Tests details

To run the tests in a forever loop:

```sh
set -e
while (true); do xpm run test-cortex-cmake -C "${HOME}/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests"; done
```

```sh
set -e
while (true); do xpm run test-all -C "${HOME}/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests"; done
```

The tests ran many hours in loops without problems.

Note: there should be no trace messages in the scheduler interrupt, otherwise
the tests occasionally fail, as shown below.

### rtos-apis

A simple test to exercise most of the RTOS APIs, both C and C++.

### mutex-stress

This test exercises the mutex logic, by using random locks from multiple
threads and checking the distribution.

### cmsis-os-validator

This tests uses the Arm CMSIS Validator.

### deprecated

The old tests are kept for historical reasons. Some of them might be
revived in the future.
