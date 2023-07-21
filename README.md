[![GitHub package.json version](https://img.shields.io/github/package-json/v/micro-os-plus/micro-os-plus-iii)](https://github.com/micro-os-plus/micro-os-plus-iii/blob/xpack/package.json)
[![GitHub tag (latest by date)](https://img.shields.io/github/v/tag/micro-os-plus/micro-os-plus-iii)](https://github.com/micro-os-plus/micro-os-plus-iii/tags/)
[![license](https://img.shields.io/github/license/micro-os-plus/micro-os-plus-iii)](https://github.com/micro-os-plus/micro-os-plus-iii/blob/xpack/LICENSE)
[![CI on Push](https://github.com/micro-os-plus/micro-os-plus-iii/actions/workflows/ci.yml/badge.svg)](https://github.com/micro-os-plus/micro-os-plus-iii/actions/workflows/ci.yml)
[![Website](https://img.shields.io/website?url=https%3A%2F%2Fmicro-os-plus.github.io%2F)](https://micro-os-plus.github.io/micro-os-plus-iii/)

# A source code library with the portable part of µOS++ IIIe (an xpm/npm package)

**µOS++** is a POSIX-like open-source framework for writing embedded
applications, written in C++. It includes multiple components, like startup,
memory allocators, a multi-threaded scheduler, device drivers, files,
file systems and more.

The open-source project is hosted on GitHub as
[micro-os-plus/micro-os-plus-iii](https://github.com/micro-os-plus/micro-os-plus-iii.git).

## Maintainer info

This page is addressed to developers who plan to include this source code
library into their own projects.

For maintainer info, please see the
[README-MAINTAINER](README-MAINTAINER.md) file.

## Purpose

The **µOS++** _(micro oh ɛs plus plus)_ project is
the third iteration of µOS++, a POSIX-like, portable, open-source,
royalty-free, multi-tasking real-time framework written in C++,
intended for 32/64-bits embedded applications.

The APIs are documented in the
[µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/).

## Install

As a source code library, this project can be integrated into another project
in the traditional way,
by either copying the relevant files into the target project, or by linking
the entire project as a Git submodule.

However, the workflow can be further automated and the most convenient way is
to **add it as a dependency** to the project via **xpm**.

### Install with xpm/npm

Along with the source files, this project also includes a
`package.json` file with the metadata that allows it to be identified as an
**xpm/npm** package so that it can be directly installed from GitHub.

#### Prerequisites

A recent [xpm](https://xpack.github.io/xpm/),
which is a portable [Node.js](https://nodejs.org/) command line application
that complements [npm](https://docs.npmjs.com)
with several extra features specific to
**C/C++ projects**.

It is recommended to install/update to the latest version with:

```sh
npm install --global xpm@latest
```

For details please follow the instructions in the
[xPack install](https://xpack.github.io/install/) page.

Warning: Be sure **xpm** is not installed with administrative/root rights.

#### xpm

This source code library can be installed as a package from GitHub with:

```sh
cd my-project
xpm init # Add a package.json if not already present

xpm install github:micro-os-plus/micro-os-plus-iii#v7.0.0 --save-dev --copy

ls -l xpacks/@micro-os-plus/micro-os-plus-iii
```

Note: without `--copy`, the default is to create a link
to a read-only instance of the package in the **xpm** central store.

#### npm

The package can also be installed with [npm](https://docs.npmjs.com)
or related, but
the features specific to C/C++ projects will not be available;
therefore, at least for consistency reasons, it is recommended
to use **xpm**.

### Add as a Git submodule

If, for any reason, xpm/npm are not available, it is always possible
to manually copy the relevant files into the target
project. However, this will need extra maintenance to keep the
project up to date.

A more convenient
solution is to link the entire project as a **Git submodule**,
for example below an `xpacks` folder:

```sh
cd my-project
git init # Unless already a Git project
mkdir -p xpacks

git submodule add https://github.com/micro-os-plus/micro-os-plus-iii.git \
  xpacks/@micro-os-plus/micro-os-plus-iii
```

## Related projects

The current µOS++ code tries to be as portable as possible.

The following related projects complement µOS++ with specific implementations for different platforms:

- the Cortex-M port of the scheduler is in the separate project
[µOS++ Cortex-M](https://github.com/micro-os-plus/micro-os-plus-iii-cortexm)
- the synthetic POSIX port of the scheduler is in the separate project
[µOS++ POSIX arch](https://github.com/micro-os-plus/micro-os-plus-iii-posix-arch)

Third party source code libraries are available from

- <https://github.com/xpacks> - deprecated since mid 2023
- <https://github.com/xpack-3rd-party>

## Branches

Apart from the unused `master` branch, there are two active branches:

- `xpack`, with the latest stable version (default)
- `xpack-develop`, with the current development version

All development is done in the `xpack-develop` branch, and contributions via
Pull Requests should be directed to this branch.

When new releases are published, the `xpack-develop` branch is merged
into `xpack`.

## Developer info

### Overview

The µOS++ IIIe source code is split between a portable part (this project)
and platform specific code (like Cortex-M). Applications must include both.

The code is relatively complex, and includes multiple components in a
monolithic repository. Most components use conditional compilation
and require preprocessor definitions to enable them, otherwise they are
not included in the build.

### Status

µOS++ IIIe is fully functional for Cortex-M devices and also runs
on synthetic POSIX platforms (like macOS and GNU/Linux).

Note: The next edition of the project (IVe) is currently work in progress,
with the monolithic
repository to be split into multiple separate source code libraries.

### Build & integration info

The project is written in C/C++ and it is expected
to be used in C and C++ projects.

The source code was compiled with GCC 11/12, clang 12/13/14/15,
arm-none-eabi-gcc 12, and should be warning free.

To ease the integration of this package into user projects, there
are already made CMake configuration files (see below).

For other build systems, consider the following details:

#### Include folders

The following folders should be passed to the compiler during the build:

- `include`

#### Source folders

The source files to be added to the build are in the following folder:

- `src`

#### Preprocessor definitions

There are multiple definitions, grouped by various criteria.
For details, see
[µOS++ Application Config](http://micro-os-plus.github.io/reference/cmsis-plus/group__cmsis-plus-app-config.html).

#### Compiler options

- `-std=c++20` or higher for C++ sources
- `-std=c11` for C sources

#### Dependencies

The portable part of µOS++ is monolithic and does not have direct
dependencies; however applications must include the platform specific
implementation, like for Cortex-M.

#### CMake

To integrate the **micro-os-plus-iii** source code library
into a CMake application,
add this folder to the build:

```cmake
add_subdirectory("xpacks/@micro-os-plus/micro-os-plus-iii")
```

The result is an interface library that can be added as an application
dependency with:

```cmake
target_link_libraries(your-target PRIVATE

  micro-os-plus::iii
)
```

### Examples

- none

### Known problems

- none

### Limitations

- none

### Tests

- `tests/rtos-apis` - simple test to exercise the µOS++ RTOS C++ API,
the C API and the ISO C++ API
- `tests/mutex-stress` - a stress test with 10 threads fighting for a mutex
- `tests/cmsis-os-validator` - the Arm CMSIS OS validator

The ARM CMSIS RTOS validator is available from a
[separate project](https://github.com/xpacks/arm-cmsis-rtos-validator).

## Documentation

- [µOS++](http://micro-os-plus.github.io)
- [µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/)

## Demo projects

For guidance on how to structure projects using µOS++,
including integration with CubeMX for STM devices,
please see the `tests` folder and the Eclipse projects in the separate
[GitHub eclipse-demo-projects](https://github.com/micro-os-plus/eclipse-demo-projects).

## Change log - incompatible changes

According to [semver](https://semver.org) rules:

> Major version X (X.y.z | X > 0) MUST be incremented if any
backwards incompatible changes are introduced to the public API.

The incompatible changes, in reverse chronological order, are:

- v7.x: two minor changes spotted by GCC 12
  - a name clash between the `file_system()` method and the class;
  the method was renamed to `get_file_system()`
  - a name clash between the unscoped enum `socket` definition and
  the top definition; the definition was changed to a scoped `enum class`

## License

Unless otherwise stated, the content is released under the terms of the
[MIT License](https://opensource.org/licenses/mit/),
with all rights reserved to
[Liviu Ionescu](https://github.com/ilg-ul).
