# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# sources/rtos-apis/xcdl-test.jsonc
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

# This file is intended to be used by applications via:
#
# `add_subdirectory("sources/rtos-apis")`
#
# This creates interface libraries that can be linked to a target with:
#
# ~~~
# target_link_libraries(your-target PUBLIC test::rtos-apis)
# ~~~

# -----------------------------------------------------------------------------

message (VERBOSE "Processing 'tests/sources/rtos-apis/'...")

# -----------------------------------------------------------------------------

# The source code for the RTOS APIs test.

# https://cmake.org/cmake/help/v3.20/command/add_library.html?highlight=interface#interface-libraries
# Visibility keywords: INTERFACE passes properties up to targets that link
# against it.
add_library (test-rtos-apis-interface INTERFACE EXCLUDE_FROM_ALL)

target_include_directories (test-rtos-apis-interface INTERFACE "include")

target_sources (
  test-rtos-apis-interface
  INTERFACE "src/main.cpp"
            "src/test-c-api.c"
            "src/test-chan-fatfs.cpp"
            "src/test-cmsis-os1.cpp"
            "src/test-cpp-api.cpp"
            "src/test-cpp-mem.cpp"
            "src/test-iso-api.cpp"
            "src/test-posix-io-api.cpp"
            "src/instrumentation-config.cpp"
)

target_link_libraries (test-rtos-apis-interface INTERFACE "micro-os-plus::iii")

xpack_display_target_lists (test-rtos-apis-interface)

# -----------------------------------------------------------------------------
# Alias.

# Export an alias target using namespaced syntax (Project::Target). This is the
# canonical pattern for consumption via target_link_libraries(), whether the
# dependency is added via add_subdirectory(), FetchContent, or an installed
# package via find_package(). Using "::" lets CMake detect an invalid or
# misspelled target name at configure time, rather than silently passing it
# through to the linker as a plain string or flag. Note: alias targets are
# read-only; properties must be set on the underlying target, not on the alias
# itself.
# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library (test::rtos-apis ALIAS test-rtos-apis-interface)
message (VERBOSE "> test::rtos-apis -> test-rtos-apis-interface")

# -----------------------------------------------------------------------------
