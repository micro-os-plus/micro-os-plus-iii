# CMake

> DO NOT EDIT! Automatically generated from template file:
> build-helper/templates/common/_micro-os-plus/tests/cmake/README-liquid.md

These files are intended as helpers during CMake builds.

## `toolchains/arm-none-eabi-gcc.cmake`

This file can be used when invoking cmake to build projects using
the `arm-none-eabi-gcc` toolchains (bare-metal Cortex-M and AArch32):

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/arm-none-eabi-gcc.cmake
```

## `toolchains/aarch64-none-elf-gcc.cmake`

This file can be used when invoking cmake to build projects using
the `aarch64-none-elf-gcc` toolchains (bare-metal AArch64):

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/aarch64-none-elf-gcc.cmake
```

## `toolchains/riscv-none-elf-gcc.cmake`

This file can be used when invoking cmake to build projects using
the `riscv-none-elf-gcc` toolchains (bare-metal RISC-V):

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/riscv-none-elf-gcc.cmake
```

## `toolchains/gcc.cmake`

This file can be used when invoking cmake to build projects using
a native `gcc` toolchain, instead of the system default compiler:

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/gcc.cmake
```

## `toolchains/clang.cmake`

This file can be used when invoking cmake to build projects using
a native `clang` toolchain, instead of the system default compiler:

```sh
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/toolchains/clang.cmake
```

## Other resources

- <https://cmake.org/cmake/help/v3.20/manual/cmake-variables.7.html>
- <https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html>
- <https://cmake.org/cmake/help/v3.20/variable/CMAKE_LANG_COMPILER_ID.html>
