# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# xcdl-package.jsonc
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
# `add_subdirectory("xpacks/@micro-os-plus/micro-os-plus-iii")`
#
# This creates libraries that can be linked to a target with:
#
# ~~~
# target_link_libraries(your-target PUBLIC micro-os-plus::iii)
# ~~~

# -----------------------------------------------------------------------------

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required (VERSION 3.20)
project (
  micro-os-plus-micro-os-plus-iii
  DESCRIPTION "A source code library with the portable part of µOS++ IIIe"
  LANGUAGES C CXX ASM
)

# To access the optional functions, include the
# `cmake/micro-os-plus-build-helper.cmake` file from the
# `@micro-os-plus/build-helper` package.

if (COMMAND xpack_get_package_name_and_version)
  xpack_get_package_name_and_version (
    "${CMAKE_CURRENT_SOURCE_DIR}/package.json"
  )
  message (
    VERBOSE
    "Processing xpm package ${PACKAGE_JSON_NAME}@${PACKAGE_JSON_VERSION}..."
  )
endif ()

# -----------------------------------------------------------------------------

# Interface libraries are the main targets, which consumers (like embedded
# applications) link against. It defines the public API and properties of the
# library; sources are propagated to and compiled by each consumer.

# They are aliased using namespaced syntax (Project::Target). This is the
# canonical pattern for consumption via target_link_libraries(), whether the
# dependency is added via add_subdirectory(), FetchContent, or an installed
# package via find_package(). Using "::" lets CMake detect an invalid or
# misspelled target name at configure time, rather than silently passing it
# through to the linker as a plain string or flag. Note: alias targets are
# read-only; properties must be set on the underlying target, not on the alias
# itself.

# https://cmake.org/cmake/help/v3.20/command/add_library.html?highlight=interface#interface-libraries
# Visibility keywords: PRIVATE applies to the target itself; INTERFACE passes
# properties up to targets that link against it; PUBLIC does both.
add_library (micro-os-plus-iii-interface INTERFACE EXCLUDE_FROM_ALL)

target_include_directories (
  micro-os-plus-iii-interface INTERFACE "include" "include/cmsis-plus/legacy"
)

target_sources (
  micro-os-plus-iii-interface
  INTERFACE "src/diag/trace-itm.cpp"
            "src/diag/trace-segger-rtt.cpp"
            "src/diag/trace-semihosting.cpp"
            "src/diag/trace.cpp"
            "src/driver/common.cpp"
            "src/driver/serial.cpp"
            "src/libc/_sbrk.c"
            "src/libc/newlib/c-newlib-reent.cpp"
            "src/libc/stdlib/assert.c"
            "src/libc/stdlib/atexit.cpp"
            "src/libc/stdlib/atexit.h"
            "src/libc/stdlib/exit.c"
            "src/libc/stdlib/init-fini.c"
            "src/libc/stdlib/malloc.cpp"
            "src/libc/stdlib/timegm.c"
            "src/libcpp/chrono.cpp"
            "src/libcpp/condition-variable.cpp"
            "src/libcpp/cxx.cpp"
            "src/libcpp/memory-resource.cpp"
            "src/libcpp/mutex.cpp"
            "src/libcpp/new.cpp"
            "src/libcpp/system-error.cpp"
            "src/libcpp/thread-cpp.h"
            "src/libcpp/thread.cpp"
            "src/memory/block-pool.cpp"
            "src/memory/first-fit-top.cpp"
            "src/memory/lifo.cpp"
            "src/posix-io/block-device-partition.cpp"
            "src/posix-io/block-device.cpp"
            "src/posix-io/c-syscalls-posix.cpp"
            "src/posix-io/char-device.cpp"
            "src/posix-io/device.cpp"
            "src/posix-io/directory.cpp"
            "src/posix-io/file-descriptors-manager.cpp"
            "src/posix-io/file-system.cpp"
            "src/posix-io/file.cpp"
            "src/posix-io/io.cpp"
            "src/posix-io/net-stack.cpp"
            "src/posix-io/socket.cpp"
            "src/posix-io/tty.cpp"
            "src/rtos/internal/os-flags.cpp"
            "src/rtos/internal/os-lists.cpp"
            "src/rtos/os-c-wrapper.cpp"
            "src/rtos/os-clocks.cpp"
            "src/rtos/os-condvar.cpp"
            "src/rtos/os-core.cpp"
            "src/rtos/os-evflags.cpp"
            "src/rtos/os-idle.cpp"
            "src/rtos/os-main.cpp"
            "src/rtos/os-memory.cpp"
            "src/rtos/os-mempool.cpp"
            "src/rtos/os-mqueue.cpp"
            "src/rtos/os-mutex.cpp"
            "src/rtos/os-semaphore.cpp"
            "src/rtos/os-thread.cpp"
            "src/rtos/os-timer.cpp"
            "src/semihosting/c-syscalls-semihosting.cpp"
            "src/startup/exception-handlers.c"
            "src/startup/initialise-free-store.cpp"
            "src/startup/initialize-hardware.c"
            "src/startup/reset-hardware.c"
            "src/startup/startup.cpp"
            "src/utils/lists.cpp"
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists (micro-os-plus-iii-interface)
endif ()

# -----------------------------------------------------------------------------
# Alias.

# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library (micro-os-plus::iii ALIAS micro-os-plus-iii-interface)
message (VERBOSE "> micro-os-plus::iii -> micro-os-plus-iii-interface")

# -----------------------------------------------------------------------------
