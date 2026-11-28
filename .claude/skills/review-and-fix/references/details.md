# Details for the review and fix workflow

Contents:

1. Test patterns (deterministic tests on the RTOS)
2. Negative checks and binary comparisons
3. Pitfalls

The format of the reports, of the GitHub issues, and of the commit
messages is no longer described here: it is in
`.github/skills/review-common.md` and in the `code-review`,
`create-github-issue`, and `github-commit` skills, and the format of
the older `CODE-REVIEW-1.md` is under "Review Documents" in
`.github/skills/project.md`.

## 1. Test patterns (deterministic tests on the RTOS)

Most races can be forced deterministically with priorities and the
scheduler lock; prefer that to timing.

- **A higher priority thread runs at once.** Creating a thread with
  `th_priority = thread::priority::above_normal` (or `high`) from the
  main thread (`normal`) runs it immediately until it blocks.
- **Priority inheritance boosts the owner.** When a high waiter blocks
  on a mutex owned by main, main runs at the waiter priority; a second,
  lower waiter created then does not run until main sleeps - add
  `sysclock.sleep_for (1)` to let it block, or the test silently tests
  nothing.
- **Hold a wake-up back.** Inside `scheduler::critical_section scs;`
  release once (`post()`, `free()`, `try_send()` - `send()` refuses to
  run with the scheduler locked - or `unlock()`), which wakes the head
  waiter but cannot switch to it; then make it leave (busy-wait past its
  deadline with a `__asm__ volatile ("" ::: "memory")` loop, or
  `interrupt()` it); after the section, check the next waiter got the
  resource. On Cortex-M a switch requested while locked waits for the
  next reschedule, so sleep afterwards.
- **Pending interrupt.** `this_thread::thread ().interrupt ()` then a
  timed wait must return `EINTR` in less than half its timeout; clear
  with `interrupt (false)`.
- **Error paths.** An error-checking mutex not owned by the caller makes
  `cv.wait (mx)` fail in `unlock()`; check `this_thread::thread ().state ()`
  before yielding, so a regression fails instead of hanging.
- **Rescue instead of hanging.** When the old code would block forever,
  detect it (a `done` flag after a few ticks) and release the waiter
  (raise the flag again, `interrupt()` it, lock/unlock), then report
  the failure.
- **Stress tests** when a window cannot be forced: vary the moment
  around the tick, 2000 iterations, a watchdog thread that counts lost
  wake-ups and rescues; measure the detection rate without the fix
  (natively it was reliable, on QEMU about two runs in three) and say
  so.
- **Regression-only tests** when no hook reaches the window (for
  example inside a function that already runs under the scheduler
  lock); state that they pass with the old code too.

Helper threads and their argument structs go in the anonymous namespace
at the top of `test-rtos.cpp` (wrap padded structs in
`#pragma GCC diagnostic ignored "-Wpadded"`).

## 2. Negative checks and binary comparisons

Negative check:

```sh
S=<scratchpad>
cp src/rtos/<file>.cpp $S/<file>.cpp.fixed
git show HEAD:src/rtos/<file>.cpp > src/rtos/<file>.cpp   # or the previous fix
xpm run build --config synthetic-posix-cmake-clang-debug -C tests
./tests/build/synthetic-posix-cmake-clang-debug/platform-bin/unit-test | grep <marker>
cp $S/<file>.cpp.fixed src/rtos/<file>.cpp
```

If one early failure returns and hides later cases, temporarily replace
`return 1;` with `(void) test_name;` inside the new block (a bare `;`
trips `-Wextra-semi-stmt`), then restore the test file from the
scratchpad copy.

Binary comparison for no-behaviour-change refactors: run
`scripts/hash-builds.sh <tests-folder> <out-file>` before and after
(build the "before" state by stashing **only your files**, with an
explicit pathspec) and `diff` the two outputs. It hashes the loadable
contents (`arm-none-eabi-objcopy -O binary`, and `otool` sections on
macOS) of every test binary in the release configurations.
`cmsis-os-validator` embeds `__TIME__`; compare its images with
`cmp -l` and expect only 2-4 differing bytes.

## 3. Pitfalls

- zsh does not split unquoted variables: loop over explicit words or
  use `${=VAR}`; `$B:s...` is a zsh modifier, write `${B}`; set
  `setopt null_glob` in scripts with optional globs.
- `git checkout <stash> -- <files>` also stages the files; follow with
  `git reset -q -- <files>`.
- Compiler traps: `-Wundef` (test `defined(X) &&` before comparing),
  `-Wextra-semi-stmt` (empty statements), `-Wold-style-cast`, GCC
  `-Wextra` on `cond ? enum : int` (cast both sides to `result_t`),
  `-Waggregate-return` (prefer out parameters to returned structs), and
  C files where `true` in `#if` is 0 before C23.
- Message queue elements must be at least pointer sized
  (`std::uintptr_t`).
- `synthetic-posix-cmake-gcc` on macOS uses `/usr/bin/g++`, which is Apple
  clang with libc++.
- QEMU test binaries end in `.elf`; native ones have no extension.
- Python scripts that rewrite sources must assert the exact number of
  matches before writing, so a mismatch stops instead of half-applying.
- The user may be editing files (for example `tests/package.json`)
  while you work; never include them in commits or stashes.
- `scripts/build-probe.mjs` links the probe with the objects of the
  test build folder as they are: after changing a source, rebuild that
  folder (`xpm run build --config <name> -C tests`) first, or the probe
  runs the old code.
