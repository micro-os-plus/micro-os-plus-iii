[![license](https://img.shields.io/github/license/micro-os-plus/micro-os-plus)](https://github.com/micro-os-plus/micro-os-plus/blob/xpack/LICENSE)
[![Travis](https://img.shields.io/travis/micro-os-plus/micro-os-plus.svg)](https://travis-ci.org/micro-os-plus/micro-os-plus)
[![GitHub issues](https://img.shields.io/github/issues/micro-os-plus/micro-os-plus.svg)](https://github.com/micro-os-plus/micro-os-plus/issues)
[![GitHub pulls](https://img.shields.io/github/issues-pr/micro-os-plus/micro-os-plus.svg)](https://github.com/micro-os-plus/micro-os-plus/pulls)

# An xpm/npm package with the portable part of µOS++ IIIe

**µOS++** is a POSIX inspired open
source framework, written in C++.

The open-source project is hosted on GitHub as
[micro-os-plus/micro-os-plus-iii](https://github.com/micro-os-plus/micro-os-plus-iii.git).

## Maintainer info

This page is addressed to developers who plan to include this source
library into their own projects.

For maintainer info, please see the
[README-MAINTAINER](README-MAINTAINER.md) file.

## Purpose

The **µOS++** _(micro oh ɛs plus plus)_ project is
the third iteration of µOS++, a POSIX-like, portable, open source,
royalty-free, multi-tasking real-time framework written in C++,
intended for 32/64-bits embedded applications.

The APIs are documented in the
[µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/).

## Status

µOS++ IIIe is fully functional for Cortex-M devices and also runs
on synthetic POSIX platforms (like macOS and GNU/Linux).

Note: The next edition of the project (IVe) is currently work in progress,
with the monolithic
repository to be split into multiple separate source libraries.

## Install

As a source code library, this project can be integrated into another project
in the traditional way,
by either copying the relevant files into the target project, or by linking
the entire project as a Git submodule.

However, things can be further automated and the most convenient way is
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

Warning: Be sure **xpm** is not installed with administrative rights.

#### xpm

This project can be installed as a package from GitHub with:

```sh
cd my-project
xpm init # Unless a package.json is already present

xpm install github:micro-os-plus/micro-os-plus-iii#v7.0.0 --save-dev --copy

ls -l xpacks/@micro-os-plus/micro-os-plus-iii
```

#### npm

The package can also be installed with [npm](https://docs.npmjs.com)
or related, but
the features specific to C/C++ projects will not be available;
therefore, at least for consistency reasons, it is recommended
to use **xpm**.

### Add as Git submodule

Besides manually copying the relevant files to the target
project, which will later require extra maintenance efforts to keep the
project up to date, a more convenient
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

Specific implementations are available from separate projects:

- the Cortex-M port of the scheduler is in the separate project
[µOS++ Cortex-M](https://github.com/micro-os-plus/micro-os-plus-iii-cortexm)
- the synthetic POSIX port of the scheduler is in the separate project
[µOS++ POSIX arch](https://github.com/micro-os-plus/micro-os-plus-iii-posix-arch)

Third party libraries are available from

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

## Build Configuration

To include µOS++ in a project, in addition to one of the port
specific project, consider the following details:

### Include folders

- `include`

### Source folders

- `src`

### Symbols

See [µOS++ Application Config](http://micro-os-plus.github.io/reference/cmsis-plus/group__cmsis-plus-app-config.html).

## Documentation

- [µOS++](http://micro-os-plus.github.io)
- [µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/)

## Demo projects

Examples on how to structure projects using µOS++, including integration
with CubeMX for STM devices, can be found in the separate
[GitHub project](https://github.com/micro-os-plus/eclipse-demo-projects).

## Tests

- tests/rtos-apis - simple test to exercise the µOS++ RTOS C++ API,
the C API and the ISO C++ API
- tests/mutex-stress - a stress test with 10 threads fighting for a mutex
- tests/cmsis-os-validator - the Arm CMSIS OS validator

The ARM CMSIS RTOS validator is available from a
[separate project](https://github.com/xpacks/arm-cmsis-rtos-validator).

## License

The original content is released under the
[MIT License](https://opensource.org/licenses/mit),
with all rights reserved to
[Liviu Ionescu](https://github.com/ilg-ul).
