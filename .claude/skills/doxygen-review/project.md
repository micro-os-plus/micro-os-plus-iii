# Doxygen Review — µOS++ IIIe Rules

The rules of the `doxygen-review` skill that are specific to this
project. They extend `SKILL.md`, and take precedence where they differ.

## Scope

- The default scope is `include/` and `src/`, without
  `include/cmsis-plus/driver/`, `include/cmsis-plus/posix-driver/`,
  `src/driver/`, `src/posix-driver/` (work in progress), the `posix/`
  headers that reproduce system headers, and `maintenance/`.
- The documentation of the whole tree is far from complete (on
  2026-11-24, at `82819dc3`: about 1700 Doxygen warnings, 2800
  entities without a brief or a details paragraph, and 1050 style
  problems). Unless the user asks for the whole tree, work on the
  files named by the user, or changed by the current work, and filter
  the outputs of the checks to them; give the counts for those files,
  not for the tree.
- `doxygen/pages/` holds the module pages (`*.dox`, which define the
  `@defgroup`s used by the `@ingroup` commands) and
  `doxygen/pages/os-app-config.h`, which documents the configuration
  macros; a new or changed macro is documented there.

## Step 1 — Automatic Checks

This overrides "Step 1" of `SKILL.md`: the project has a Doxygen
configuration, in `doxygen/` (not `website/doxygen/`), so all three
checks can be run.

- Include `config-common.doxyfile`, not `config.doxyfile`, whose
  `OUTPUT_DIRECTORY` is a local path of the maintainer:

  ```text
  @INCLUDE = ./config-common.doxyfile
  FILE_PATTERNS += *.dox
  ```

  followed by the other lines of the overlay in `SKILL.md`. The
  common configuration sets `WARN_AS_ERROR = YES`; the overlay must
  keep `WARN_AS_ERROR = NO`. Without `FILE_PATTERNS += *.dox`, the
  module pages are not read, and every `@ingroup` reports a
  non-existing group.

- Run Doxygen in `doxygen/`:
  `cd doxygen && doxygen <scratch>/check.doxyfile` (a few seconds).
- Run `check-doxygen-merged.mjs` and `check-doxygen-style.mjs` from the
  repository root, as in `SKILL.md`.

## Conventions of This Project

- There is no `inlines/` folder: the inline definitions are at the end
  of the header that declares them (and in `rtos/os-inlines.h`), and
  their comments carry the `@details` (often only a `@warning`, for
  example `Cannot be invoked from Interrupt Service Routines.`); the
  style check, which looks for `inlines/`, therefore reports these
  comments as "declaration comment without @brief"; ignore those
  reports for the definitions placed after the class.
- The public entities carry `@ingroup cmsis-plus-rtos-<module>` (the
  groups are defined in `doxygen/pages/module-*.dox`), and the parts of
  `rtos/os.h` carry `@headerfile os.h <cmsis-plus/rtos/os.h>`; keep both
  on new entities.
- Every function of the RTOS states whether it may be called from
  interrupt handlers, with
  `@warning Cannot be invoked from Interrupt Service Routines.` or
  `@note Can be invoked from Interrupt Service Routines.`. Check it
  against the `interrupts::in_handler_mode()` test in the code.
- The `@retval` lists of the functions that return `result_t` give
  every POSIX code that the code can return (including `EPERM` from
  interrupt handlers, `ETIMEDOUT`, and `EINTR`); check them against the
  code paths, not against the comment of a sibling function.
- The comments of the C API functions in `os-c-api.h` repeat those of
  their C++ equivalents (the `@brief`, the parameters, the `@retval`
  lists, and the interrupt notes); a change to one is made to the
  other.
- The `PREDEFINED` list of `config-common.doxyfile` defines
  `__ARM_EABI__`, `TRACE`, and some `OS_INCLUDE_*` macros, so the
  documented API includes the code guarded by them.
