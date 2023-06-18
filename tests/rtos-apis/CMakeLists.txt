# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2021 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/mit/.
#
# -----------------------------------------------------------------------------

# This file is intended to be consumed by applications with:
#
# `add_subdirectory("tests/rtos-apis")`
#
# The result is an interface library that can be added to the linker with:
#
# `target_link_libraries(your-target PUBLIC test::rtos-apis)`

# -----------------------------------------------------------------------------
## Preamble ##

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required(VERSION 3.20)

# -----------------------------------------------------------------------------
## The test library definitions ##

add_library(test-rtos-apis-interface INTERFACE EXCLUDE_FROM_ALL)

target_include_directories(test-rtos-apis-interface INTERFACE
  "include"
)

target_sources(test-rtos-apis-interface INTERFACE
  src/main.cpp
  src/test-c-api.c
  src/test-chan-fatfs.cpp
  src/test-cmsis-os1.cpp
  src/test-cpp-api.cpp
  src/test-cpp-mem.cpp
  src/test-iso-api.cpp
  src/test-posix-io-api.cpp
)

target_compile_definitions(test-rtos-apis-interface INTERFACE
  # None.
)

target_compile_options(test-rtos-apis-interface INTERFACE
  # None.
)

target_link_libraries(test-rtos-apis-interface INTERFACE
  # None.
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(test-rtos-apis-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library(test::rtos-apis ALIAS test-rtos-apis-interface)
message(VERBOSE "> test::rtos-apis -> test-rtos-apis-interface")

# -----------------------------------------------------------------------------
