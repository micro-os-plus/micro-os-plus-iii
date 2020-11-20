[![license](https://img.shields.io/github/license/micro-os-plus/micro-os-plus-xpack)](https://github.com/micro-os-plus/micro-os-plus-xpack/blob/xpack/LICENSE)
[![Travis](https://img.shields.io/travis/micro-os-plus/micro-os-plus-xpack.svg)](https://travis-ci.org/micro-os-plus/micro-os-plus-xpack)
[![GitHub issues](https://img.shields.io/github/issues/micro-os-plus/micro-os-plus-xpack.svg)](https://github.com/micro-os-plus/micro-os-plus-xpack/issues)
[![GitHub pulls](https://img.shields.io/github/issues-pr/micro-os-plus/micro-os-plus-xpack.svg)](https://github.com/micro-os-plus/micro-os-plus-xpack/pulls)

# µOS++

A source xPack with **µOS++**, a POSIX inspired open
source framework, written in C++. The project is hosted on
GitHub as
[micro-os-plus/micro-os-plus-xpack](https://github.com/micro-os-plus/micro-os-plus-xpack).

## Purpose

The **µOS++** _(micro oh ɛs plus plus)_ project is
the third iteration of µOS++, a POSIX-like, portable, open source,
royalty-free, multi-tasking real-time framework written in C++,
intended for 32/64-bits embedded applications.

The APIs are documented in the
[µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/).

## Status

The µOS++ project is fully functional for Cortex-M devices and also runs
on synthetic POSIX platforms (like macOS and GNU/Linux).

## Related projects

The current µOS++ code tries to be as portable as possible.

Specific implementations are available from separate projects:

- the Cortex-M port of the scheduler is packed as
[µOS++ Cortex-M xPack](https://github.com/micro-os-plus/cortexm-arch-xpack)
- the synthetic POSIX port of the scheduler is packed as
[µOS++ POSIX arch xPack](https://github.com/micro-os-plus/posix-arch-xpack)

## Build Configuration

To include µOS++ in a project, in addition to one of the port
specific project, consider the following details:

### Include folders

- `include`

### Source folders

- `src`

### Symbols

See [µOS++ Application Config](http://micro-os-plus.github.io/reference/cmsis-plus/group__cmsis-plus-app-config.html).

## Documentation

- [µOS++](http://micro-os-plus.github.io)
- [µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/)

## Demo projects

Examples on how to structure projects using µOS++, including integration
with CubeMX for STM devices, can be found in the separate
[GitHub project](https://github.com/micro-os-plus/eclipse-demo-projects).

## Tests

- tests/rtos - simple test to exercise the µOS++ RTOS C++ API,
the C API and the ISO C++ API
- tests/mutex-stress - a stress test with 10 threads fighting for a mutex
- tests/sema-stress - a stress test posting to a semaphore from a high
frequency interrupt
- tests/gcc - compile test with host GCC compiler

The ARM CMSIS RTOS validator is available from a
[separate project](https://github.com/xpacks/arm-cmsis-rtos-validator).

To run these tests, several Eclipse projects are available in a separate
[GitHub project](https://github.com/micro-os-plus/eclipse-test-projects).

## License

µOS++ is provided free of charge under the terms of the
[MIT License](https://opensource.org/licenses/MIT), with all rights
reserved to [Liviu Ionescu](https://github.com/ilg-ul).
