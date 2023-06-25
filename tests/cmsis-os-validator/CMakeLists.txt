# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2021-2023 Liviu Ionescu. All rights reserved.
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
# `add_subdirectory("tests/cmsis-os-validator")`
#
# The result is an interface library that can be added to the linker with:
#
# `target_link_libraries(your-target PUBLIC test::cmsis-os-validator)`

# -----------------------------------------------------------------------------
## Preamble ##

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required(VERSION 3.20)

# -----------------------------------------------------------------------------
## The test library definitions ##

add_library(test-cmsis-os-validator-interface INTERFACE EXCLUDE_FROM_ALL)

target_include_directories(test-cmsis-os-validator-interface INTERFACE
  "include"
)

target_sources(test-cmsis-os-validator-interface INTERFACE
  # src/main.cpp
)

target_compile_definitions(test-cmsis-os-validator-interface INTERFACE
  # None.
)

target_compile_options(test-cmsis-os-validator-interface INTERFACE
  # None.
)

target_link_libraries(test-cmsis-os-validator-interface INTERFACE
  # None.
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(test-cmsis-os-validator-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library(test::cmsis-os-validator ALIAS test-cmsis-os-validator-interface)
message(VERBOSE "> test::cmsis-os-validator -> test-cmsis-os-validator-interface")

# -----------------------------------------------------------------------------
