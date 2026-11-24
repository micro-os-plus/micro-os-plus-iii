# Defaults of the Project Facts — µOS++ IIIe

<!--
DO NOT EDIT! Automatically generated from template file:
npm-packages-helper/templates/common/.github/skills/project-defaults-liquid.md;
put the project rules in .github/skills/project.md; see
.github/skills/README.md.
-->

The default values of the project facts that the skills read. The
headings are those of `.github/skills/project.md`; a heading that is
absent from `project.md` keeps the default below, and a heading that is
present extends it, or overrides it where the two disagree (a heading
that says "None", or "Not applicable", removes the default). Read this
file first, then `project.md`.

## Tests

- After major changes, run a minimal selection of tests:
  `xpm run test-selected -C tests`.
- After minor changes, run only the tests built with the system
  compiler: `xpm run test -C tests`.
- For a change that affects the embedded targets, also run the
  platform-specific actions of `tests/package.json` (for example
  `xpm run test-qemu-cortex-m33-cmake-gcc -C tests`); on physical
  boards, only after the user confirms that the board is powered up
  (for example `xpm run test-raspberry-pi-pico-2-arm-cmake-gcc -C tests`).
- When the dependencies are linked to writable projects, run
  `xpm run link-dependencies --config <name> -C tests` first.

## Probes

Build the probe programs in the scratchpad folder, against the sources
of the project, with no other dependency:

```sh
P=<scratch>/probe
W="-Wall -Wextra -Wpedantic"
for f in src/*.cpp
do
  c++ -std=c++20 $W -Iinclude -c "$f" -o "$P/$(basename "$f" .cpp).o"
done
c++ -std=c++20 $W -Iinclude "$P/p1.cpp" "$P"/*.o -o "$P/p1"
"$P/p1"
```

When the code of the project is specific to the embedded targets, so
that a probe cannot run on the host, the probe only checks what
compiles, with the cross compiler of the target (from the `xpacks/.bin/`
folder of a project that uses it, for example a test build folder),
with `-c`; the compiler and the flags are given under "Probes" in
`project.md`. The behaviour is checked by the tests of the projects
that use this one.

## Generated Files

- The top-level `CMakeLists.txt` and `meson.build` are generated from
  `xcdl-package.jsonc` with `xpm run xcdl-export`; they are never
  edited by hand (see the `xcdl` skill).
- The `CMakeLists.txt` and `meson.build` of the test programs in
  `tests/sources/` are generated from `xcdl-test.jsonc` with
  `xpm run xcdl-export`.
- The platform files in `tests/platforms/` (their `CMakeLists.txt`,
  `meson.build`, and project definitions) and the generated scripts in
  `tests/scripts/` come from the `build-helper` templates, and are
  regenerated with `npm run generate-tests-commons` in `tests/`; the
  `cmake/definitions.cmake` and `cmake/artefacts.cmake` files of the
  platforms are local.
- The top-level common files, including the skills in
  `.github/skills/` (except `project.md`, the `project.md` files of the
  skill folders, and the skills that belong to this project only), come
  from the `npm-packages-helper` templates, and are regenerated with
  `npm run generate-top-commons`.
- The files with a `DO NOT EDIT!` header are generated from templates
  (named in the header) and are fixed in their templates.

## Files Not to Commit

- The `node_modules/` and `xpacks/` folders, the build folders, and
  the `.tgz` packages.

## Review Documents

- None yet; the review reports are written to `maintenance/docs/`.

## Breaking Changes

- The public API (the
  public headers in `include/`), the behaviour that the documentation describes,
  and the minimum supported toolchains.

## Documentation Updates

- The project has no website.

## Paths

- The paths in the reports and in the issues are relative to the
  repository root.
