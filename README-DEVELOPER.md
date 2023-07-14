[![GitHub issues](https://img.shields.io/github/issues/micro-os-plus/micro-os-plus-iii.svg)](https://github.com/micro-os-plus/micro-os-plus-iii/issues/)
[![GitHub pulls](https://img.shields.io/github/issues-pr/micro-os-plus/micro-os-plus-iii.svg)](https://github.com/micro-os-plus/micro-os-plus-iii/pulls)

# Developer info

## Prerequisites

A recent [xpm](https://xpack.github.io/xpm/), which is a portable
[Node.js](https://nodejs.org/) command line application.

## micro-os-plus-iii

- <https://github.com/micro-os-plus/micro-os-plus-iii>

```sh
rm -rf ~/Work/micro-os-plus-iii/micro-os-plus-iii.git && \
mkdir -p ~/Work/micro-os-plus-iii && \
git clone \
  --branch xpack-develop \
  https://github.com/micro-os-plus/micro-os-plus-iii.git \
  ~/Work/micro-os-plus-iii/micro-os-plus-iii.git
```

or, to update an existing folder:

```sh
git -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git pull

xpm run deep-clean -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```

Satisfy dependencies for all configurations and run all tests:

```sh
xpm run install-all -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests

xpm run test -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```

or, to run the tests with all available toolchains:

```sh
xpm run test-all -C ~/Work/micro-os-plus-iii/micro-os-plus-iii.git/tests
```
