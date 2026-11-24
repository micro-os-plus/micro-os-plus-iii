# Code Review — µOS++ IIIe Rules

The rules of the `code-review` skill that are specific to this project.
They extend `SKILL.md`, and take precedence where they differ. Read
`CLAUDE.md` (the port contract and the coding conventions) and
`.github/skills/project.md` first.

## Scope

- The default scope is `include/`, `src/`, and `tests/sources/`, as in
  `SKILL.md`, with these exclusions:
  - `maintenance/` is never reviewed (no findings, no suggestions),
    except that the report itself is written to `maintenance/docs/`;
  - `include/cmsis-plus/driver/`, `include/cmsis-plus/posix-driver/`,
    `src/driver/`, and `src/posix-driver/` are work in progress and
    mostly not built; review them only when the user names them;
  - `tests/deprecated/` and the STM32CubeMX code of the board
    platforms (`tests/platforms/*/*cubemx/`) are not reviewed;
  - `include/cmsis-plus/legacy/cmsis_os.h` follows the Arm CMSIS RTOS
    v1 API; review its implementation in `src/rtos/os-c-wrapper.cpp`
    and its use, not the names that the Arm API fixes.
- When the change touches the scheduler, the threads, the clocks, the
  critical sections, or anything in `namespace port`, read how both
  ports drive the core (see "Architecture Ports" in
  `.github/skills/project.md`), and state in the report which port
  versions were read. The ports are not reviewed, only used as
  context; a problem found in a port is listed under "Not reported",
  for the port repository.

## Folder Structure

This overrides "Folder structure" in `SKILL.md`:

- There is no `inlines/` folder. The inline definitions of a class are
  at the end of its own header, after the declarations (for example
  `mutex::owner()` at the end of `include/cmsis-plus/rtos/os-mutex.h`),
  and the inline definitions shared by the RTOS are in
  `include/cmsis-plus/rtos/os-inlines.h`; the port inlines are in the
  port, in `rtos/port/os-inlines.h`.
- There are several public entry points, not one:
  `cmsis-plus/rtos/os.h` (the C++ RTOS API, whose parts are the
  `rtos/os-*.h` headers), `cmsis-plus/rtos/os-c-api.h` (the C API),
  `cmsis-plus/legacy/cmsis_os.h`, the `cmsis-plus/estd/*` headers, the
  `cmsis-plus/memory/*.h` allocators, the `cmsis-plus/posix-io/*.h`
  classes, and `cmsis-plus/diag/trace.h`. Internal helpers are in the
  `internal/` sub-folders and in `internal` namespaces.
- The `estd/` headers have no extension and use the names of the ISO
  C++ headers (`chrono`, `mutex`, `thread`); this is deliberate.
- New source files are added to `target_sources()` in the top-level
  `CMakeLists.txt` (there is no `xcdl-package.jsonc`; see "Generated
  Files" in `.github/skills/project.md`); a file left out is silently
  not built.
- The C sources (`.c`, in `src/startup/`, `src/libc/`, and
  `tests/sources/unit/src/test-c-api.c`) are C11; the C++ rules do not
  apply to them.

## Naming and Style

- The header guards are the path relative to `include/`, in upper
  case, with the non-alphanumeric characters replaced by underscores,
  and a trailing underscore (`CMSIS_PLUS_RTOS_OS_MUTEX_H_`).
- The licence header is the µOS++ MIT block
  (`This file is part of the µOS++ project ...`), with the years of
  the file; new files use the current year.
- The type aliases of the API end in `_t` (`result_t`, `priority_t`,
  `state_t`, `flags::mask_t`); this is the CMSIS++ convention, not a
  violation of the `snake_case` rule.
- Older code still has bare `#ifdef` and non-guard `#ifndef` (about 40
  occurrences; the `posix/` headers that reproduce system headers use
  them deliberately, like the system headers do). Report them in the
  changed lines; in a review of the whole tree, report them as a
  single item.

## Project Checks

Check these in every change (they come from `CLAUDE.md`):

- **Errors**: the RTOS functions return `result_t` (`result::ok`, or a
  POSIX `errno` value); errors are checked with `os_assert_err()` and
  `os_assert_throw()`; the code builds with `-fno-exceptions` and
  `-fno-rtti`.
- **Interrupt context**: every function that may block checks
  `interrupts::in_handler_mode()` and returns `EPERM`; the
  documentation of each function states whether it may be called from
  interrupt handlers, and the check and the documentation agree.
- **Critical sections**: shared state is changed inside a
  `interrupts::critical_section` or a `scheduler::critical_section`,
  as the surrounding code does; look for lost wake-ups (a state tested
  in one critical section and acted on in another), and for
  `scheduler::critical_section` held across a call that may block.
- **Instrumentation**: the `trace::printf()` calls and the
  `#if defined(OS_TRACE_RTOS_*)` blocks are kept, and new RTOS functions
  have them.
- **C API**: every change to the C++ API is mirrored in `os-c-api.h`,
  `src/rtos/os-c-wrapper.cpp`, and the legacy `cmsis_os.h` wrapper
  where applicable; the opaque structures in `os-c-decls.h` keep the
  sizes of the C++ objects (the `static_assert`s in
  `os-c-wrapper.cpp`).
- **Port delegation**: the portable implementation still compiles and
  works when the `OS_USE_RTOS_PORT_*` macros are not defined, and the
  delegated branches still compile when they are. Only the
  `USE_FREERTOS` configuration of the `cmsis-os-validator` test defines
  them, and no test configuration builds it, so the delegated branches
  are not compiled by the tests; check them by reading, and say so.
- **Configuration macros**: new optional features are guarded by
  `OS_INCLUDE_*`/`OS_USE_*` macros with defaults in the headers, and
  are documented in `doxygen/pages/os-app-config.h` and in the
  `os-app-config.h` of the tests that use them.
- **Portability**: no guard by toolchain or ABI macros
  (`__ARM_EABI__`, `__ARM_ARCH_7M__`-style lists); apply the rule in
  `CLAUDE.md` (`OS_IS_CROSS_BUILD`, `__ARM_ARCH_PROFILE == 'M'`,
  `__ARM_ARCH_ISA_THUMB`, `#error` for the architectures not yet
  implemented).
- **Language**: British English and "folder" in the comments and the
  messages.
- **Warnings**: the test builds use `-Weverything -Werror` with clang
  and `-Wall -Wextra` plus many more with GCC (11 to 14) and
  arm-none-eabi-gcc; a new warning in any of them breaks the build.

## Tests

- A behaviour change or a fix needs a test in the `unit` test
  (`tests/sources/unit/src/`); `rtos-apis` only checks that the API
  calls exist and compile, so a functional check there is a finding.
- A test that passes also without the fix tests nothing; ask how the
  test was shown to fail without the fix (the negative check of the
  `review-and-fix` skill).
