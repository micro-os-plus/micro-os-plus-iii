# Consistency Check — µOS++ IIIe Rules

The rules of the `consistency-check` skill that are specific to this
project. They extend `SKILL.md`, and take precedence where they differ.

## Scope

In addition to the exclusions of `SKILL.md`, do not check
`maintenance/inspiration/` (third-party reference code),
`maintenance/templates/` (templates and work-in-progress headers, not
compiled), `tests/deprecated/` (not built), `doxygen/orig/`, and the
STM32CubeMX code of the board platforms (`tests/platforms/*/*cubemx/`).

## Build and Test Configurations

This replaces the per-platform action names of `SKILL.md`, which do not
match `tests/package.json`:

- The configurations are named
  `<platform>-cmake-<toolchain>-<debug|release>`, and each platform and
  toolchain has both a debug and a release configuration.
- Each platform and toolchain has one `test-<platform>-cmake[-<toolchain>]`
  action, which prepares, builds, and tests both configurations; the
  groups are `test-synthetic-posix-cmake`, `test-cortex-cmake`, and the cleaning
  actions `clean-synthetic-posix-cmake` and `clean-cortex-cmake`.
- Installation and linking are composite actions, with one line per
  configuration: `install`, `install-ci`, `install-all`,
  `install-synthetic-posix-cmake-sys`, `install-qemu-cortex-latest`,
  `link-deps-all`, and `link-deps-synthetic-posix-cmake-sys`. Check that each
  list names every configuration it should, exactly once, and none
  that does not exist (`install-all`, `link-deps-all`, and `clean-all`
  cover all the configurations; `install-ci` matches `test-ci`).
- `.github/workflows/test-ci.yml`, `test-pr.yml`, and `test-all.yml`
  call `install-ci`/`test-ci` and `install-all`/`test-all`; the
  platforms they run on match the `os.platform` conditions of these
  actions.
- Each folder in `tests/platforms/` has a `README.md`, a
  `CMakeLists.txt`, and `cmake/dependencies-folders.cmake`, which adds
  the right port (`micro-os-plus-iii-cortexm` or
  `micro-os-plus-iii-posix-arch`) from the configuration's `xpacks/`
  folder; the QEMU platforms use the devices in
  `tests/device-qemu-cortexm/`.
- Each test application in `tests/sources/` is built by every platform
  that the READMEs say it is, and has its own
  `include/cmsis-plus/os-app-config.h`.

## Build System Integration

This replaces the `xcdl-package.jsonc` and `meson.build` check of
`SKILL.md`: the source files in `target_sources()` of the top-level
`CMakeLists.txt` match the files in `src/` (a file present but not
listed is not built), and the folders of
`target_include_directories()` exist.

## Documentation and Versions

- The version in `package.json`, `include/cmsis-plus/os-versions.h`,
  `README.md`, and the latest entry of `CHANGELOG.md` agree (a
  `-pre` version in `package.json` on `xpack-development` is expected).
- The tested compilers and platforms listed in `README.md` and
  `tests/README.md` match the configurations of `tests/package.json`.
- The `INPUT`, `EXCLUDE`, and `PREDEFINED` settings of
  `doxygen/config-common.doxyfile` name files and macros that exist.
- `CLAUDE.md` and the skills (`.github/skills/project.md` and the
  `project.md` files of the skill folders) describe the current
  actions, folders, and ports.
