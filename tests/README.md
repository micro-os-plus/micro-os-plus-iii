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

Question marks:

- the native gcc12 debug occasionally fail on Intel macOS, it may hang or crash
