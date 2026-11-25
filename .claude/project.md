# Project Rules — µOS++ IIIe

This file is owned by the project; it was created from the template
`npm-packages-helper/templates/first-time/.claude/project-liquid.md`
and is never regenerated.

`CLAUDE.md` imports this file. Put here only the rules for this
project; the rules common to all projects are generated into
`CLAUDE.md` and are changed in the `npm-packages-helper` templates.
Where a rule here contradicts `CLAUDE.md`, the rule here applies.

## Project overview

**µOS++ IIIe** (`@micro-os-plus/micro-os-plus-iii`) is a POSIX-like,
portable, real-time framework for 32/64-bit embedded applications,
written in C++20 (C11 for the C sources). It provides the startup code,
a multi-threaded RTOS scheduler and its synchronisation objects, memory
allocators, a POSIX I/O layer (files, file systems, devices, sockets),
ISO C++ threading wrappers, a legacy CMSIS RTOS API, and trace/diagnostics
support.

The project is distributed as an xpm/npm source package and
is consumed by applications as a CMake **interface library**
(`micro-os-plus::iii`). It is a monolithic repository; most components
are compiled conditionally and are enabled by preprocessor definitions
supplied by the application.

Note: the IVe edition (split into separate libraries) is a different
project; this repository is IIIe only.

## Portable core and architecture ports

This repository contains **only the portable part**. It never builds on
its own: every application must also link exactly one architecture port.
The ports are separate repositories, checked out as sibling folders of
this one:

```text
micro-os-plus-iii/
├── micro-os-plus-iii.git               ← this project (portable core)
├── micro-os-plus-iii-cortexm.git       ← Arm Cortex-M port
├── micro-os-plus-iii-posix-arch.git    ← synthetic POSIX port (macOS/Linux)
├── micro-os-plus-iii-aarch32.git       ← other ports (experimental)
├── micro-os-plus-iii-aarch64.git
├── micro-os-plus-iii-riscv.git
├── arm-cmsis.git, arm-cmsis-rtos-validator.git, chan-fatfs.git, ...
```

| Port repository | npm package | CMake target |
| --- | --- | --- |
| `micro-os-plus-iii-cortexm.git` | `@micro-os-plus/micro-os-plus-iii-cortexm` | `micro-os-plus::iii-cortexm` |
| `micro-os-plus-iii-posix-arch.git` | `@micro-os-plus/micro-os-plus-iii-posix-arch` | `micro-os-plus::iii-posix-arch` |

### The port contract

Each port provides the same small set of files, which the core includes
by fixed paths:

- `include/cmsis-plus/rtos/port/os-decls.h` - port types and declarations
  (stack element type, interrupt state, scheduler/thread context types);
  included from `rtos/os-decls.h` and `rtos/os-c-decls.h`
- `include/cmsis-plus/rtos/port/os-c-decls.h` - the C-API equivalents
- `include/cmsis-plus/rtos/port/os-inlines.h` - inline implementations of
  the `os::rtos::port` functions (critical sections, context switch
  requests, clock helpers, etc.); included at the end of `rtos/os.h`
- `src/rtos/os-core.cpp` - non-inline port code (scheduler start,
  context creation/switching, SysTick/timer handling)
- the POSIX port also provides `src/diag/trace-posix.cpp`

The interface the core expects from a port is declared in the
`os::rtos::port` namespace in
[include/cmsis-plus/rtos/os-decls.h](include/cmsis-plus/rtos/os-decls.h)
(sub-namespaces such as `port::clock`, `port::interrupts`,
`port::scheduler`, `port::thread`).

When changing anything in `namespace port`, in the `port/*.h` includes,
or in the semantics of scheduler/thread/clock internals, **check and
update all ports** (at least Cortex-M and POSIX), since they are
compiled together with the core and are not covered by this repository's
own sources.

Some synchronisation objects can be delegated to the port, controlled by
`OS_USE_RTOS_PORT_SCHEDULER`, `OS_USE_RTOS_PORT_TIMER`,
`OS_USE_RTOS_PORT_MUTEX`, `OS_USE_RTOS_PORT_SEMAPHORE`,
`OS_USE_RTOS_PORT_MESSAGE_QUEUE`, `OS_USE_RTOS_PORT_EVENT_FLAGS`,
`OS_USE_RTOS_PORT_CLOCK_SYSTICK_WAIT_FOR`. The portable implementation
must keep working when these are not defined.

### Headers supplied by the application

The core includes headers that are **not** in this repository; the
application (or the test platform) must provide them on the include path:

- `<cmsis-plus/os-app-config.h>` - application configuration
  (`OS_INTEGER_*`, `OS_INCLUDE_*`, `OS_USE_*`, `OS_BOOL_*` definitions);
  see `tests/sources/*/include/cmsis-plus/os-app-config.h` for examples
- `<cmsis-plus/platform.h>` - platform selection
  (see `tests/platforms/*/include/cmsis-plus/platform.h`)
- `<cmsis_device.h>` - on Cortex-M, the vendor CMSIS device header

## Repository layout

- `include/cmsis-plus/` - public headers (the include root is `include`;
  `include/cmsis-plus/legacy` is also added for `cmsis_os.h`)
  - `rtos/` - RTOS C++ API (`os.h` is the umbrella header), C API
    (`os-c-api.h`, `os-c-decls.h`), hooks, inlines, `internal/` lists
    and flags
  - `estd/` - ISO C++ threading headers implemented on top of the RTOS
    (`thread`, `mutex`, `condition_variable`, `chrono`,
    `memory_resource`, ...), in namespace `os::estd`
  - `memory/` - allocators (`first-fit-top`, `lifo`, `block-pool`,
    `null`, `malloc`)
  - `posix-io/` - POSIX I/O class hierarchy (`io`, `file`, `directory`,
    `file-system`, `block-device`, `char-device`, `tty`, `socket`,
    `net-stack`, file descriptors manager) and the syscall aliases
  - `posix/` - POSIX headers missing from newlib (`dirent.h`,
    `sys/socket.h`, `termios.h`, ...)
  - `diag/` - `trace.h` and `instrumentation.h`
  - `driver/`, `posix-driver/` - **work in progress**, mostly not built
  - `arm/semihosting.h`, `cortexm/exception-handlers.h`
  - `os-versions.h` - version macros (update on release)
- `src/` - implementations, mirroring `include/`, plus:
  - `startup/` - reset/startup code, free store initialisation,
    exception handlers
  - `libc/`, `libcpp/` - newlib reentrancy, `_sbrk`, `malloc`,
    `atexit`/`exit`, `new`/`delete`, C++ runtime support
  - `semihosting/` - semihosting syscalls
- `CMakeLists.txt` - defines the `micro-os-plus-iii-interface` INTERFACE
  library (alias `micro-os-plus::iii`); **every new source file must be
  added to `target_sources()` here**, otherwise it is not built
- `tests/` - a separate xpm project with the test suite (see below)
- `doxygen/` - reference documentation (Doxygen) sources
- `maintenance/` - **maintainer-only material; not part of the library.**
  Applications using this library must not use, include or depend on
  anything in this folder, and code reviews must ignore it (do not
  review, flag or suggest changes to its contents). It holds:
  - `config/` - `.clang-format`, `.cmake-format.py`, prettier configs
  - `scripts/` - formatting scripts (generated from templates, do not edit)
  - `docs/` - maintainer notes (`README-DEVELOPER.md`,
    `README-MAINTAINER.md`, `TODO.md`, `HISTORY.md`, POSIX I/O notes)
  - `inspiration/` - third-party reference code (libstdc++, libc++,
    newlib); not compiled, do not modify
  - `templates/` - project templates and work-in-progress headers;
    not compiled

## Build and test

There is no standalone build at the top level; everything is driven
from the `tests` folder with xpm (Node.js >= 20, xpm >= 0.20.8).
Build folders are `tests/build/<configuration>`.

```sh
# Top dependencies and the default tests.
xpm run install -C tests
xpm run test -C tests

# Selected tests.
xpm run install-selected -C tests
xpm run test-selected -C tests

# Tests with latest toolchain.
xpm run install-latest -C tests
xpm run test-latest -C tests

# Everything (all GCC/clang versions, all platforms)
xpm run install-all -C tests
xpm run test-all -C tests

# Same set as the GitHub Actions CI
xpm run install-ci -C tests
xpm run test-ci -C tests

# Clean
xpm run deep-clean -C tests
```

A single configuration can be driven step by step:

```sh
xpm run prepare --config native-cmake-gcc-debug -C tests
xpm run build   --config native-cmake-gcc-debug -C tests
xpm run test    --config native-cmake-gcc-debug -C tests
```

Configuration names follow `<platform>-cmake-<toolchain>-<debug|release>`,
for example `native-cmake-clang17-release` or
`qemu-cortex-m3-cmake-gcc-debug`.

### Test platforms (`tests/platforms/`)

- `native` - runs on the host using the POSIX port and `libucontext`
- `qemu-cortex-m0`, `qemu-cortex-m3`, `qemu-cortex-m4f`,
  `qemu-cortex-m7f`, `qemu-cortex-m33` - Cortex-M port on QEMU (MPS2
  boards, see `tests/device-qemu-cortexm/`); `qemu-cortex-m33` is the
  default platform
- `nucleo-f411re`, `nucleo-f767zi`, `nucleo-h743zi` - physical boards (manual)

(the raspberrypi-pico is not yet functional)

Each platform's `cmake/dependencies-folders.cmake` lists the folders
added to the build, including the port, which is resolved from
`build/<config>/xpacks/@micro-os-plus/micro-os-plus-iii-<port>`.

### Test applications (`tests/sources/`)

- `rtos-apis` - exercises the RTOS C++ API, the C API and the ISO C++ API
  (the presence of the API calls, simple cases)
- `unit` - functional tests of the RTOS (scenarios, races, wake-ups,
  priority inheritance, interrupts, clocks) and the regression tests of
  fixed bugs; new behaviour tests go here, not in `rtos-apis`
- `mutex-stress` - several threads competing for one mutex
- `cmsis-os-validator` - the Arm CMSIS RTOS validator
- `blinky`, `instrumentation` - board/instrumentation demos

`tests/deprecated/` is old material and is not built.

### Working with local (writable) port repositories

By default xpm installs the ports as read-only packages from the
central xPacks store. To develop the core and a port together, link the
sibling repositories:

```sh
# Register the sibling repos as linkable packages (once)
xpm link -C ../micro-os-plus-iii-posix-arch.git
xpm link -C ../micro-os-plus-iii-cortexm.git

# Link them into the per-configuration xpacks folders
xpm run link-deps-all -C tests
# or, for one configuration
xpm run link-deps --config native-cmake-sys-debug -C tests
```

`xpm run git-clone-deps -C tests` does the same, but clones fresh
copies under `~/Work/...`; prefer the existing siblings when they are
already present.

## Formatting

```sh
xpm run clang-format    # src, include, tests/sources|includes|platforms
xpm run cmake-format    # all CMakeLists.txt and *.cmake files
```

The clang-format style is in `maintenance/config/.clang-format` (GNU-like style:
return type on its own line, a space before parentheses, two-space
indentation). Format only the files you touch; avoid reformatting
unrelated code.

## Coding conventions

- C++20. RTOS functions report errors via `result_t` return codes
  (POSIX `errno` values, `result::ok` on success); the code must also
  build with `-fno-exceptions`/`-fno-rtti`. Use `os_assert_err()`
  (asserts in debug, returns the error with `NDEBUG`) and
  `os_assert_throw()` (asserts in debug, throws a system error with
  `NDEBUG`, or aborts if exceptions are disabled).
- Naming: lower-case `snake_case` for types, functions and members; the
  public API lives in `os::rtos`, `os::posix`, `os::memory`, `os::estd`,
  `os::trace`; internal helpers in `internal` namespaces.
- C API wrappers (`os_*` functions in `os-c-api.h`/`os-c-wrapper.cpp`)
  must be kept in sync with the C++ API, including the opaque storage
  structures in `os-c-decls.h` (their sizes must match the C++ objects).
- Functions that may block must check that they are not called from
  interrupt handlers (`interrupts::in_handler_mode()`) and return
  `EPERM`; preserve these checks.
- Keep the trace/instrumentation calls (`trace::printf`,
  `#if defined(OS_TRACE_RTOS_*)` blocks) when adding or changing RTOS
  functions.
- Guard new optional features with `OS_INCLUDE_*`/`OS_USE_*` macros and
  give them sensible defaults in the headers.
- Portability: the code must remain portable to Cortex-M, AArch32,
  AArch64, RISC-V and POSIX native. Guard code by what it needs, never
  by toolchain or ABI macros such as `__ARM_EABI__`, which silently
  exclude it on other architectures:
  - portable bare-metal code: `OS_IS_CROSS_BUILD` (from `os-versions.h`);
  - optional features: their `OS_USE_*`/`OS_INCLUDE_*` macro;
  - Cortex-M only:
    `defined(__ARM_ARCH_PROFILE) && (__ARM_ARCH_PROFILE == 'M')`; tell
    Mainline (v7-M, v7E-M, v8-M Main, v8.1-M) from Baseline (v6-M,
    v8-M Base) with `__ARM_ARCH_ISA_THUMB == 2`/`== 1`, not with lists
    of `__ARM_ARCH_7M__`-style macros (clang names Armv8.1-M
    `__ARM_ARCH_8_1M_MAIN__`), and put a comment listing the matched
    architectures before such tests; use the ACLE version
    `__ARM_ARCH` (6, 7, 8) only to exclude versions not yet supported,
    in one central check (as the Cortex-M port does for Armv8-M);
  - keep architecture-specific lines small and isolated. The AArch32,
    AArch64 and RISC-V ports are not implemented yet; where code needs
    an architecture-specific implementation, use an explicit
    `#error "... not yet implemented for this architecture"` instead of
    untested code.
- Document public APIs with Doxygen comments, consistent with the
  existing ones.
- Each file starts with the standard µOS++ MIT licence header.
- Compiler warnings are treated seriously; the code must stay warning
  free with GCC 11-14, clang 13-19 and arm-none-eabi-gcc.
- Use British English in comments and documentation, and "folder"
  rather than "directory".

## Branches and releases

- `xpack-development` - all development and pull requests
- `xpack` - latest stable release; `xpack-development` is merged into it
  on release
- Releases: bump `package.json`, update `include/cmsis-plus/os-versions.h`,
  `README.md`, `maintenance/docs/README-MAINTAINER.md` and `CHANGELOG.md`, run all tests,
  then `npm version X.Y.Z` (the `postversion` script pushes branches and
  tags). Details in [README-MAINTAINER.md](maintenance/docs/README-MAINTAINER.md).
- The npm package only ships the files allowed by `.npmignore`; check
  with `npm pack` before publishing.
