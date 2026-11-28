# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# sources/mutex-stress/xcdl-test.jsonc
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
# `add_subdirectory("sources/mutex-stress")`
#
# This creates interface libraries that can be linked to a target with:
#
# ~~~
# target_link_libraries(your-target PUBLIC test::mutex-stress)
# ~~~

# -----------------------------------------------------------------------------

message (VERBOSE "Processing 'tests/sources/mutex-stress/'...")

# -----------------------------------------------------------------------------

# The source code for a mutex stress test.

# https://cmake.org/cmake/help/v3.20/command/add_library.html?highlight=interface#interface-libraries
# Visibility keywords: INTERFACE passes properties up to targets that link
# against it.
add_library (test-mutex-stress-interface INTERFACE EXCLUDE_FROM_ALL)

target_include_directories (test-mutex-stress-interface INTERFACE "include")

target_sources (
  test-mutex-stress-interface INTERFACE "src/main.cpp" "src/test.cpp"
)

target_link_libraries (
  test-mutex-stress-interface INTERFACE "micro-os-plus::iii"
)

xpack_display_target_lists (test-mutex-stress-interface)

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
add_library (test::mutex-stress ALIAS test-mutex-stress-interface)
message (VERBOSE "> test::mutex-stress -> test-mutex-stress-interface")

# -----------------------------------------------------------------------------
