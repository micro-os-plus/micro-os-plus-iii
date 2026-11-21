# Details for the review and fix workflow

Contents:

1. Review document skeleton
2. Sections added while fixing
3. GitHub issue body template
4. Commit messages
5. Test patterns (deterministic tests on the RTOS)
6. Negative checks and binary comparisons
7. Pitfalls

## 1. Review document skeleton

```markdown
# Code review N - <scope>

<Scope, method, what was not reviewed (maintenance/), port versions used.>

## Summary

| # | Severity | File | Issue |
| --- | --- | --- | --- |
| #1 | important | `src/rtos/os-thread.cpp:684` | <one line> |

Severity values: important (added or made worse by the change),
pre-existing, SMP only, minor, narrow, decision, docs, nit.

## Suggested fix order

Phase 1, small and contained: ...
Phase 2, decide first, then fix: ...
Phase 3, scheduler and synchronisation core, one at a time, with tests
between them: ...
Separately, not tied to this change: ...

---

## #1 - <title>

**File:** `path:line`

**Status:** open

### Problem
### Scenario
### Documentation and tests
### Port behaviour
### Suggested fix
```

## 2. Sections added while fixing

Under each finding, as it progresses:

- **Status:** `fixed (not committed), with ...` → after the commit
  `fixed in <short sha> (issue #<n>), with ...`. Decisions:
  `decided: <what>`.
- `### Analysis before fixing` - what re-reading the code showed (wider,
  narrower, wrong); include it whenever the review was not exactly right.
- `### Fix applied` - files and what changed, what did not change
  (ports, C API, data layout), behaviour changes.
- `### Verification` - the tests, the negative check result (for
  example "with the old file the test returns ETIMEDOUT"), the suites
  run, what could not be verified.
- Problems found while fixing that are elsewhere get their own new
  finding number (#17, #18...) with the same structure, added to the
  summary table and the fix order.

## 3. GitHub issue body template

```markdown
Problem: <what is wrong, where (file, function), since which commit or
"already present in <version>"; a short scenario>.

Change:

- <bullet per change; say what is not changed>
- No port, C API or data layout changes.

Test: `tests/sources/rtos-apis/src/test-cpp-api.cpp`, <what the test does>.

Without the fix <what the test reports>. With the fix,
`test-native-cmake-clang`, `test-qemu-cortex-m7f-cmake` and
`test-qemu-cortex-m0-cmake` pass (debug and release).
```

Title: `fix(<area>): <summary>` (areas used: rtos, arm, startup, estd).
After the commit, append (keep two blank lines before it):

```
Fixed on <YYYY-MM-DD> in <full sha> (`xpack-development`).
```

## 4. Commit messages

```
<file or area>: <short summary> (#<issue>)

- <what changed, one bullet per logical change>
- rtos-apis: <what the new test checks>

<attribution trailer from the system prompt>
```

Examples of subjects used: `os-condvar.cpp: timed_wait() result from what
woke the thread (#146)`, `rtos: no kill() livelock while idle destroys
the thread (#147)`, `portable guards instead of __ARM_EABI__ (#152)`.

## 5. Test patterns (deterministic tests on the RTOS)

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
at the top of `test-cpp-api.cpp` (wrap padded structs in
`#pragma GCC diagnostic ignored "-Wpadded"`).

## 6. Negative checks and binary comparisons

Negative check:

```sh
S=<scratchpad>
cp src/rtos/<file>.cpp $S/<file>.cpp.fixed
git show HEAD:src/rtos/<file>.cpp > src/rtos/<file>.cpp   # or the previous fix
xpm run build --config native-cmake-clang-debug -C tests
./tests/build/native-cmake-clang-debug/platform-bin/rtos-apis-test | grep <marker>
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

## 7. Pitfalls

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
- `native-cmake-gcc` on macOS uses `/usr/bin/g++`, which is Apple
  clang with libc++.
- QEMU test binaries end in `.elf`; native ones have no extension.
- Python scripts that rewrite sources must assert the exact number of
  matches before writing, so a mismatch stops instead of half-applying.
- The user may be editing files (for example `tests/package.json`)
  while you work; never include them in commits or stashes.
