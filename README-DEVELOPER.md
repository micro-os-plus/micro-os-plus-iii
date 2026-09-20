[![GitHub issues](https://img.shields.io/github/issues/micro-os-plus/micro-os-plus-iii.svg)](https://github.com/micro-os-plus/micro-os-plus-iii/issues/)
[![GitHub pulls](https://img.shields.io/github/issues-pr/micro-os-plus/micro-os-plus-iii.svg)](https://github.com/micro-os-plus/micro-os-plus-iii/pulls)

# Developer info

## Prerequisites

A recent [xpm](https://xpack.github.io/xpm/), which is a portable
[Node.js](https://nodejs.org/) command line application.

## Clone repository

- <https://github.com/micro-os-plus/micro-os-plus-iii>

```sh
rm -rf ~/Work/micro-os-plus-iii/micro-os-plus-iii.git && \
mkdir -p ~/Work/micro-os-plus-iii && \
git clone \
  --branch xpack-development \
  https://github.com/micro-os-plus/micro-os-plus-iii.git \
  ~/Work/micro-os-plus-iii/micro-os-plus-iii.git
```

or, to update an existing folder:

```sh
git -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git pull
```

## Top dependencies

To install top dependencies:

```sh
npm --prefix ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests install
```

## Run a first test

Satisfy dependencies and run a few tests:

```sh
xpm run install -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests

xpm run test -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```

## Run a native test

```sh
xpm run install-native-cmake-sys -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests

xpm run install-native-cmake-sys -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```

## Run all tests

To run the tests with all available toolchains:

```sh
xpm run install-all -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests

xpm run test-all -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```

## Run QEMU Cortex-M tests

To run the QEMU Cortex-M tests with the latest toolchains:

```sh
xpm run install-qemu-cortex-latest -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests

xpm run install-qemu-cortex-latest -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```

## Remove all

To remove all dependencies and build files:

```sh
xpm run deep-clean -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```

After this restart from installing top dependencies.

## Use development writable packages

The previous tests install the dependencies as read only packages.

During development it is necessary to have writable versions of the packages. 
In the npm/xpm ecosystem, this can be achieved by _linking_ local git
repositories.

```sh
xpm run git-clone-deps -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```
