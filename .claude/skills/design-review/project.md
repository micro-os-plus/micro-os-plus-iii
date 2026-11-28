# Design Review — µOS++ IIIe Rules

The rules of the `design-review` skill that are specific to this
project. They extend `SKILL.md`, and take precedence where they differ.
Read `CLAUDE.md` and `.github/skills/project.md` first.

## Scope

This overrides the default scope of `SKILL.md` (there is no single
umbrella header and no `inlines/` folder). Unless the user names a
part, review, in this order:

1. `include/cmsis-plus/rtos/os.h` (what it includes, and in which
   order), then `os-decls.h`, `os-types.h`, `os-sched.h`,
   `os-thread.h`, `os-clocks.h`, `os-timer.h`, `os-mutex.h`,
   `os-condvar.h`, `os-semaphore.h`, `os-mempool.h`, `os-mqueue.h`,
   `os-evflags.h`, `os-memory.h`, `os-hooks.h`, and `os-inlines.h`, with
   their definitions in `src/rtos/` (the inline definitions are at the
   end of each header);
2. the C API: `rtos/os-c-api.h`, `rtos/os-c-decls.h`, and
   `src/rtos/os-c-wrapper.cpp`;
3. `include/cmsis-plus/memory/` and `src/memory/`;
4. `include/cmsis-plus/estd/` and `src/libcpp/`;
5. `include/cmsis-plus/posix-io/` and `src/posix-io/`;
6. `include/cmsis-plus/diag/` and `src/diag/`;
7. `src/startup/`, `src/libc/`, and `src/rtos/internal/` (the lists),
   last.

Not reviewed: `driver/` and `posix-driver/` (work in progress),
`legacy/cmsis_os.h` (the Arm CMSIS RTOS v1 API; its implementation in
`os-c-wrapper.cpp` is reviewed with the C API), the `posix/` headers
that reproduce missing system headers, and `maintenance/`. Read the
port declarations (`os::rtos::port` in `os-decls.h`, and the
`rtos/port/` files of both ports, see "Architecture Ports" in
`.github/skills/project.md`) as the lower boundary of the design; the
ports themselves are not reviewed.

## Restrictions

Deliberate decisions of the design; do not report them as problems,
and do not propose to change them:

- Errors are reported as `result_t` values (POSIX `errno` codes), not
  as exceptions; the library builds and works with `-fno-exceptions`
  and `-fno-rtti`. `os_assert_throw()` is used where no error code can
  be returned (the constructors); without exceptions it aborts.
- The API follows the CMSIS++ conventions: the `os::rtos` namespaces,
  the POSIX-like semantics and names (`timed_wait()`, `try_lock()`),
  the `_t` member type aliases, and the configuration through
  `attributes` classes (`thread::attributes`, `mutex::attributes`)
  instead of long constructor parameter lists.
- The C API mirrors the C++ API one to one, with opaque storage
  structures of the same size as the C++ objects, so that C code can
  allocate the objects statically.
- Every object that needs memory (stacks, queues, pools) can be
  constructed with storage supplied by the application; the dynamic
  allocations go through the `rtos::memory` allocators and memory
  resources, never directly to `malloc()`. The RTOS core uses no
  standard containers and no `std::function`.
- The `estd` headers reproduce the ISO C++ thread support library
  (names, signatures, and semantics of `std::thread`, `std::mutex`,
  `std::condition_variable`, `std::chrono`) on top of the RTOS; their
  deviations from the standard are findings, their naming is not.
- The `rtos/os-*.h` headers are parts of `rtos/os.h` (their entities
  carry `@headerfile os.h <cmsis-plus/rtos/os.h>`) and are not meant to
  be included alone; their failures in the self-containment check are
  expected. Report the failures of the other public headers.
- The repository is monolithic, with the features selected by
  configuration macros; do not propose splitting it into separate
  libraries (that is the IVe edition, a different project).
- The port contract (`os::rtos::port`) is the only interface to the
  architecture; architecture-specific code outside the ports is limited
  to the guarded blocks allowed by the portability rule in `CLAUDE.md`.

## Header Self-Containment

The headers need the application configuration, the platform, and a
port; take the flags from the `unit` test of a native test build:

```sh
B=tests/build/synthetic-posix-cmake-clang-debug
FLAGS=$(node -e 'const c = require(process.argv[1] +
  "/compile_commands.json").find((e) =>
  e.file.endsWith("unit/src/test-rtos.cpp"));
  console.log(c.command.match(
  / (-[ID]\S+|-isysroot \S+|-stdlib=\S+)/g).join(""))' "$PWD/$B")
CXX=$B/xpacks/.bin/clang++ node \
  .github/skills/design-review/scripts/check-headers.mjs \
  <scratch>/headers cmsis-plus/rtos cmsis-plus/memory \
  cmsis-plus/estd cmsis-plus/posix-io cmsis-plus/diag -- ${=FLAGS}
```

(`${=FLAGS}` splits the words in zsh; in bash, write `$FLAGS`.)

## Probes

Build the probes as described under "Probes" in
`.github/skills/project.md` (natively, and on QEMU for the questions
that depend on the target or on preemption). The cross compile of
`SKILL.md` with `-c` only also works, with the flags of a
`qemu-cortex-m*` test build taken from its `compile_commands.json`
as above.

## Using the API as a Newcomer

The newcomer reads the user manual
(<https://micro-os-plus.github.io/user-manual/>; several chapters are
still to be written), the reference pages
(<https://micro-os-plus.github.io/reference/cmsis-plus/>), and the
public headers. The tasks, each an `os_main()` built with the probe
script:

1. create two threads with statically allocated stacks, with different
   priorities, and join them;
2. a producer and a consumer that share a buffer protected by a mutex
   and a condition variable;
3. pass messages from a timer callback (interrupt-like context) to a
   thread through a message queue;
4. a periodic and a one-shot timer, and a thread that waits with a
   timeout on a semaphore;
5. wait for any or all of several event flags, and for thread flags;
6. a fixed-size block pool (`memory_pool`), and an object allocated
   with a custom memory resource;
7. the same producer and consumer with `estd::thread`, `estd::mutex`,
   and `estd::condition_variable`;
8. task 2 written with the C API (`os-c-api.h`).

The common mistakes to make on purpose:

- call a blocking function (`mutex::lock()`, `semaphore::wait()`,
  `thread::sleep_for()`) from an interrupt handler or a timer callback;
- lock a normal mutex twice from the same thread; unlock a mutex owned
  by another thread;
- destroy a thread object while the thread still runs; let a thread
  function return without `join()`;
- give a stack smaller than the minimum, or a priority out of range;
- send a message larger than the message size of the queue;
- create the RTOS objects or call the scheduler functions before
  `scheduler::start()` (static constructors).

## Embedded Targets

This extends section 7 of `SKILL.md`:

- Measure the size with the release builds of the QEMU platforms
  (`qemu-cortex-m0-cmake-gcc-release` and
  `qemu-cortex-m7f-cmake-gcc-release`), comparing
  `arm-none-eabi-size` of `platform-bin/*-test.elf` before and after
  the change; to show that a refactoring does not change the code, use
  `.github/skills/review-and-fix/scripts/hash-builds.sh`.
- The stacks of the threads are small (the default on Cortex-M is a
  few kilobytes; the idle and the interrupt stacks are smaller); the
  stack used by a library function counts against the stack of every
  thread that calls it.
- The latency matters: report the work done inside critical sections
  that grows with the number of threads or waiters.
