# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# build-helper/templates/common/_micro-os-plus/tests/cmake/toolchains/gcc-liquid.cmake
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2021-2026 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

set (CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
  set (extension ".cmd")
else ()
  set (extension "")
endif ()

set (CMAKE_C_COMPILER "gcc${extension}")
set (CMAKE_CXX_COMPILER "g++${extension}")

# Must be explicit, not set by CMake.
set (CMAKE_SIZE "size${extension}")

# https://cmake.org/cmake/help/v3.20/variable/CMAKE_HOST_SYSTEM_NAME.html
if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Darwin")
  # macOS has no separate AR or RANLIB.
else ()
  # Required for -flto.
  set (CMAKE_AR "gcc-ar${extension}")
  set (CMAKE_RANLIB "gcc-ranlib${extension}")
endif ()

# -----------------------------------------------------------------------------

# TODO: understand why CMAKE_<lang>_OUTPUT_EXTENSION is not effective

# set(CMAKE_ASM_FLAGS " -x assembler-with-cpp" CACHE STRING "Flags used by the
# ASM compiler during all build types.")

set (
  CMAKE_ASM_FLAGS_DEBUG
  "-O0 -g3"
  CACHE STRING "Flags used by the ASM compiler during DEBUG builds."
)

set (
  CMAKE_ASM_FLAGS_MINSIZEREL
  "-Os -DNDEBUG"
  CACHE STRING "Flags used by the ASM compiler during MINSIZEREL builds."
)

set (
  CMAKE_ASM_FLAGS_RELEASE
  "-O3 -DNDEBUG"
  CACHE STRING "Flags used by the ASM compiler during RELEASE builds."
)

set (
  CMAKE_ASM_FLAGS_RELWITHDEBINFO
  "-O2 -g3 -DNDEBUG"
  CACHE STRING "Flags used by the ASM compiler during RELWITHDEBINFO builds."
)

# Not effective.
set (CMAKE_ASM_OUTPUT_EXTENSION ".o")

set (CMAKE_C_FLAGS CACHE STRING
                         "Flags used by the C compiler during all build types."
)

set (
  CMAKE_C_FLAGS_DEBUG
  "-O0 -g3"
  CACHE STRING "Flags used by the C compiler during DEBUG builds."
)

set (
  CMAKE_C_FLAGS_MINSIZEREL
  "-Os -DNDEBUG"
  CACHE STRING "Flags used by the C compiler during MINSIZEREL builds."
)

set (
  CMAKE_C_FLAGS_RELEASE
  "-O3 -DNDEBUG"
  CACHE STRING "Flags used by the C compiler during RELEASE builds."
)

set (
  CMAKE_C_FLAGS_RELWITHDEBINFO
  "-O2 -g3 -DNDEBUG"
  CACHE STRING "Flags used by the C compiler during RELWITHDEBINFO builds."
)

# Not effective.
set (CMAKE_C_OUTPUT_EXTENSION ".o")

set (CMAKE_CXX_FLAGS
     CACHE STRING "Flags used by the CXX compiler during all build types."
)

set (
  CMAKE_CXX_FLAGS_DEBUG
  "-O0 -g3"
  CACHE STRING "Flags used by the CXX compiler during DEBUG builds."
)

set (
  CMAKE_CXX_FLAGS_MINSIZEREL
  "-Os -DNDEBUG"
  CACHE STRING "Flags used by the CXX compiler during MINSIZEREL builds."
)

set (
  CMAKE_CXX_FLAGS_RELEASE
  "-O3 -DNDEBUG"
  CACHE STRING "Flags used by the CXX compiler during RELEASE builds."
)

set (
  CMAKE_CXX_FLAGS_RELWITHDEBINFO
  "-O2 -g3 -DNDEBUG"
  CACHE STRING "Flags used by the CXX compiler during RELWITHDEBINFO builds."
)

# Not effective.
set (CMAKE_CXX_OUTPUT_EXTENSION ".o")
# set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)

set (CMAKE_EXE_LINKER_FLAGS
     CACHE STRING "Flags used by the linker during all build types."
)

# Disabled, since apparently CMake already uses the C/CXX flags, and defining
# them here will double them.
if (0)
  set (
    CMAKE_EXE_LINKER_FLAGS_DEBUG
    "-O0 -g3"
    CACHE STRING "Flags used by the linker during DEBUG builds."
  )

  set (
    CMAKE_EXE_LINKER_FLAGS_MINSIZEREL
    "-Os"
    CACHE STRING "Flags used by the linker during MINSIZEREL builds."
  )

  set (
    CMAKE_EXE_LINKER_FLAGS_RELEASE
    "-O3"
    CACHE STRING "Flags used by the linker during RELEASE builds."
  )

  set (
    CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO
    "-O2 -g3"
    CACHE STRING "Flags used by the linker during RELWITHDEBINFO builds."
  )
endif ()

# -----------------------------------------------------------------------------
