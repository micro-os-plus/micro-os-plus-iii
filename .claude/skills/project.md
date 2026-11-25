# Project Facts for the Skills — µOS++ IIIe

This file is owned by the project; it was created once from the
template `npm-packages-helper/templates/first-time/.github/skills/project-liquid.md`, and it is never regenerated.

The skills in `.github/skills/` read the facts of the project from
`.github/skills/project-defaults.md` (generated, with the defaults for
the organisation and the project) and from this file. Add below, under
the same heading, only what differs from the default: the text of a
heading extends the default, or overrides it where the two disagree (a
heading that says "None", or "Not applicable", removes the default). A
heading that is absent keeps the default. The headings are: Tests,
Probes, Generated Files, Files Not to Commit, Review
Documents, Breaking Changes, Documentation Updates, and Paths
(see `.github/skills/README.md`).

A skill folder may also have its own `project.md`, for the rules of that
skill only; it takes precedence over this file.

`CLAUDE.md` describes the project (the port contract, the build, the
coding conventions, and the portability rule); read it first. The
headings below give only what the skills need from it, and what
differs from the defaults.

## Architecture Ports

This repository holds only the portable core. It never builds alone:
every application, and every test configuration, also links exactly
one architecture port, from a separate repository, checked out as a
sibling folder:

- `../micro-os-plus-iii-cortexm.git` (`micro-os-plus::iii-cortexm`),
  used by the QEMU and the board platforms;
- `../micro-os-plus-iii-posix-arch.git`
  (`micro-os-plus::iii-posix-arch`), used by the `native` platform.

A change to `namespace port`, to the `rtos/port/*.h` includes, or to
the semantics of the scheduler, thread, or clock internals must be
checked against both ports, and the ports changed with it when needed.
A review that touches these areas reads the port sources (the versions
that the test build actually uses, under
`tests/build/<config>/xpacks/@micro-os-plus/`, which may be links to
the siblings or packages from the xPacks store; say which). The
`smp` branch of this repository holds the experimental SMP scheduler;
compare with it when a change concerns the lists, the clocks, or the
critical sections.

## Tests

This overrides the default: `xpm run test -C tests` runs the native
tests with the system compiler (`test-native-cmake-sys`), and the
default QEMU platform is the Cortex-M33.

- After minor changes, run the native tests with clang:

  ```sh
  xpm run test-native-cmake-clang -C tests
  ```

- After major changes (the scheduler, the synchronisation objects, the
  clocks, the startup, the C API, or the port contract), run
  `xpm run test-selected -C tests`, which runs the native tests with
  clang and the QEMU Cortex-M0 (Armv6-M Baseline), Cortex-M7F
  (Armv7E-M Mainline), and Cortex-M33 (Armv8-M Mainline) tests.

- For wide changes, `test-latest` runs the
  tests on all platforms.

  ```sh
  xpm run test-latest -C tests
  ```

- A change limited to the Cortex-M port, or to code compiled only for
  Cortex-M, needs only the QEMU tests; a change limited to the POSIX
  port needs only the native tests.
- Each `test-<platform>-cmake[-<toolchain>]` action prepares, builds,
  and runs both the debug and the release configurations. A
  configuration that was never installed needs
  `xpm install --config <name> -C tests` first (or the matching
  `install-*` action).
- The physical boards (`test-nucleo-f411re-cmake`,
  `test-nucleo-f767zi-cmake`, `test-nucleo-h743zi-cmake`) 
  are run only after the user confirms
  that the board is connected and powered.
- When the ports are linked to the writable sibling repositories, the
  action is `link-deps` (not `link-dependencies`):
  `xpm run link-deps --config <name> -C tests`, or
  `xpm run link-deps-all -C tests`. When a test fails in CMake before
  compiling, check the links and the branch of the linked siblings
  first.
- The runs take minutes; run them in the background when they may
  exceed the tool timeout.
- The functional tests of a fix or a new behaviour go into the `unit`
  test (`tests/sources/unit/src/test-rtos.cpp`, `test-c-api.c`,
  `test-memory.cpp`); `rtos-apis` only checks that the API calls are
  present and compile.

## Probes

This overrides the default: the sources cannot be compiled alone,
since they need a port, the application configuration
(`<cmsis-plus/os-app-config.h>`), and the platform files. A probe is
compiled with the flags of the `unit` test of an existing test build
folder, linked with the objects of `unit-test` (except those of the
`unit` test itself), and run, natively or on QEMU:

```sh
node .github/skills/review-and-fix/scripts/build-probe.mjs \
  tests/build/native-cmake-clang-debug <scratch>/probe/p1.cpp
```

- The probe defines `int os_main (int argc, char* argv[])`, and reports
  with `trace::printf()`; its exit status is that of `os_main()`.
- Build the test folder first
  (`xpm run build --config <name> -C tests`). Use
  `native-cmake-clang-debug` (or `native-cmake-gcc-debug`)
  natively, `qemu-cortex-m7f-cmake-gcc-debug` for Armv7E-M,
  `qemu-cortex-m33-cmake-gcc-debug` for Armv8-M Mainline, and
  `qemu-cortex-m0-cmake-gcc-debug` for Armv6-M; the QEMU probes run
  exactly as `ctest` runs `unit-test`.
- `--no-run` only compiles and links; extra compiler flags for the
  probe follow `--` (for example `-- -fno-exceptions -DP_CASE_2`). The
  flags apply to the probe only; the library objects are those of the
  test build, so a probe cannot change a configuration macro of the
  library (that needs a test configuration, or a temporary change to
  the `unit` `os-app-config.h`, reverted afterwards).
- On macOS, `native-cmake-gcc` uses `/usr/bin/g++`, which is Apple
  clang; GCC itself is tested by the `native-cmake-gcc<N>`
  configurations (or on the Linux test host).
- The native port is non-preemptive (cooperative, on `libucontext`);
  a race that depends on preemption by the tick must be probed on
  QEMU.

## Generated Files

This overrides the default:

- The top-level `CMakeLists.txt` (the `micro-os-plus-iii-interface`
  INTERFACE library) and `meson.build` are generated from
  `xcdl-package.jsonc` with `xpm run xcdl-export`, as in the default;
  they are never edited by hand, and every new source file is added to
  `xcdl-package.jsonc` (see the `xcdl` skill). The project has no
  `xcdl-test.jsonc` and no `meson.build` in the tests: the
  `CMakeLists.txt` of the test applications in `tests/sources/` are
  written by hand.
- In `tests/`, only `tests/cmake/common-options.cmake`,
  `tests/cmake/tests-main.cmake`, and the
  `tests/platforms/*/cmake/platform-library.cmake` files carry a
  `DO NOT EDIT!` header (from the `build-helper` templates). The
  other platform files, `tests/package.json`, and the device files in
  `tests/device-qemu-cortexm/` are written by hand. There is no
  `tests/scripts/` folder, and no `generate-tests-commons` script.
- The top-level common files (the skills, `.gitignore`, `.npmignore`,
  `.github/workflows/`, `.vscode/settings.json`,
  `.settings/settings.json`, `maintenance/config/.prettierignore`, and
  the scripts in `maintenance/scripts/`) come from the
  `npm-packages-helper` templates, as in the default.
- The reference pages are generated by Doxygen from `doxygen/` into the
  website source repository (outside this one); nothing generated by
  Doxygen is committed here.

## Files Not to Commit

- The `micro-os-plus-micro-os-plus-iii-*.tgz` packages left by
  `npm pack`, the `tests/build/` folders, and `.DS_Store`.
- The files that the user is editing at the same time (often
  `tests/package.json`); never include them in a commit or a stash
  unless the user says so.

## Review Documents

- `maintenance/docs/CODE-REVIEW-1.md` was written before the review
  skills existed, by the `review-and-fix` workflow, in an earlier
  format, which is kept:
  - a summary table, with one row per finding (number, severity, file,
    and title), the title followed by `(**fixed**)` once fixed;
  - one `## #N - <title>` section per finding, with a `**Status:**`
    line: `open`, then `fixed in <short hash> (issue #<n>), with ...`,
    or `decided: <what>`;
  - the sections `Analysis before fixing`, `Fix applied`, and
    `Verification`, added to a finding while it is fixed.

  Mark its items in that format. It counts as an earlier code review,
  so the next one is `CODE-REVIEW-2.md`, in the format of
  `.github/skills/review-common.md`.

- `maintenance/docs/TODO.md` and `maintenance/docs/NOTES.md` are
  maintainer notes, not review reports.

## Breaking Changes

In addition to the default:

- the C API (`os-c-api.h`) and the sizes and the layout of the opaque
  storage structures in `os-c-decls.h`, which must match the C++
  objects;
- the legacy CMSIS RTOS API (`include/cmsis-plus/legacy/cmsis_os.h`);
- the configuration macros (`OS_INTEGER_*`, `OS_INCLUDE_*`, `OS_USE_*`,
  `OS_BOOL_*`, `OS_DISABLE_*`), their names and their defaults, and the
  headers that the application supplies (`os-app-config.h`,
  `platform.h`);
- the port contract (the `os::rtos::port` declarations and the
  `rtos/port/*.h` files that the ports provide); a change there
  requires matching releases of the ports;
- the observable behaviour of the RTOS: which functions may be called
  from interrupt handlers, the `result_t` codes returned, the wake-up
  order, the priority inheritance, and the timer defaults. A behaviour
  change that is not breaking in the API is still listed in
  `CHANGELOG.md` as `**Behaviour change:** ...`.

## Documentation Updates

This overrides the default ("The project has no website"):

- The reference pages (<https://micro-os-plus.github.io/reference/>)
  are generated by Doxygen from the comments in `include/` and `src/`
  and from the pages in `doxygen/pages/`, with the configuration in
  `doxygen/`; they are regenerated by the maintainer at release time.
  Update the comments with every change, as the `doxygen-review` skill
  requires.
- The user manual (<https://micro-os-plus.github.io/user-manual/>)
  lives in the website repository; when a change contradicts it, tell
  the user, and do not edit it from here.
- Update by hand: `CHANGELOG.md` (a dated entry for each user-visible
  change; `**Behaviour change:** ...` for the changes of behaviour),
  `README.md` (the tested compilers and platforms), the READMEs in
  `tests/` and its sub-folders, `CLAUDE.md`, and, at release time,
  `include/cmsis-plus/os-versions.h` and
  `maintenance/docs/README-MAINTAINER.md`.

## Paths

- The paths are relative to the repository root. In the reports, the
  paths under the public include folder may be shortened to the form
  used by `#include` (for example `cmsis-plus/rtos/os-thread.h` for
  `include/cmsis-plus/rtos/os-thread.h`), once the report says so;
  `src/` paths are always written in full. The port files are named
  with the port repository
  (`micro-os-plus-iii-cortexm: src/rtos/os-core.cpp`).
