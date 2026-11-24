# xcdl — µOS++ IIIe Rules

Not applicable: this project has no `xcdl-package.jsonc` and no
`xcdl-test.jsonc`, and generates no `CMakeLists.txt` or `meson.build`
(there is no Meson build). The `xcdl-export` action of `package.json`
and `maintenance/scripts/xcdl-export.mjs` come from the common
template and have nothing to export.

Instead:

- A new, renamed, or removed source file is changed by hand in
  `target_sources()` of the top-level `CMakeLists.txt` (the
  `micro-os-plus-iii-interface` INTERFACE library, alias
  `micro-os-plus::iii`); the public include folders are in its
  `target_include_directories()`.
- The test applications have hand-written `CMakeLists.txt` files in
  `tests/sources/<name>/`.
- The configuration macros are not listed in any `xcdl` file; the
  application defines them in its `<cmsis-plus/os-app-config.h>`, and
  they are documented in `doxygen/pages/os-app-config.h` (see the
  examples in `tests/sources/*/include/cmsis-plus/os-app-config.h`).
