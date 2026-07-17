# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2021-2025 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

# This file is intended to be consumed by applications with:
#
# `add_subdirectory("tests/blinky")`
#
# The result is an interface library that can be added to the linker with:
#
# `target_link_libraries(your-target PUBLIC test::blinky)`

# -----------------------------------------------------------------------------
# Preamble.

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required (VERSION 3.20)

# -----------------------------------------------------------------------------
# The test library definitions.
add_library (test-blinky-interface INTERFACE EXCLUDE_FROM_ALL)

target_include_directories (test-blinky-interface INTERFACE "include")

target_sources (test-blinky-interface INTERFACE "src/main.cpp")

target_compile_definitions (test-blinky-interface INTERFACE # None.
)

target_compile_options (test-blinky-interface INTERFACE # None.
)

target_link_options (test-blinky-interface INTERFACE # None.
)

target_link_libraries (test-blinky-interface INTERFACE # None.
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists (test-blinky-interface)
endif ()

# -----------------------------------------------------------------------------
# Aliases.

# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library (test::blinky ALIAS test-blinky-interface)
message (VERBOSE "> test::blinky -> test-blinky-interface")

# -----------------------------------------------------------------------------
