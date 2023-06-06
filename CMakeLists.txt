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
# be obtained from https://opensource.org/licenses/MIT/.
#
# -----------------------------------------------------------------------------

# This file is intended to be consumed by applications with:
#
# `add_subdirectory("xpacks/@micro-os-plus/micro-os-plus-iii")`
#
# The result is an interface library that can be added to the linker with:
#
# `target_link_libraries(your-target PUBLIC micro-os-plus::micro-os-plus-iii)`

# -----------------------------------------------------------------------------
## Preamble ##

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required(VERSION 3.20)

project(micro-os-plus-iii
  DESCRIPTION "µOS++ IIIe"
)

if(COMMAND xpack_get_package_name_and_version)
  xpack_get_package_name_and_version("${CMAKE_CURRENT_SOURCE_DIR}/package.json")
  message(VERBOSE "Processing xPack ${PACKAGE_JSON_NAME}@${PACKAGE_JSON_VERSION}...")
endif()

# -----------------------------------------------------------------------------
## The project library definitions ##

add_library(micro-os-plus-iii-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------
# Target settings.

target_include_directories(micro-os-plus-iii-interface INTERFACE
  "include"
)

target_sources(micro-os-plus-iii-interface INTERFACE
  src/diag/trace-itm.cpp
  src/diag/trace-segger-rtt.cpp
  src/diag/trace-semihosting.cpp
  src/diag/trace.cpp
  src/driver/common.cpp
  src/driver/serial.cpp
  # src/driver/usart-wrapper.cpp
  # src/driver/usb-device.cpp
  # src/driver/usb-host.cpp
  # src/driver/usbd-wrapper.cpp
  # src/driver/usbh-wrapper.cpp
  src/libc/_sbrk.c
  src/libc/newlib/c-newlib-reent.cpp
  src/libc/stdlib/assert.c
  src/libc/stdlib/atexit.cpp
  src/libc/stdlib/atexit.h
  src/libc/stdlib/exit.c
  src/libc/stdlib/init-fini.c
  src/libc/stdlib/malloc.cpp
  src/libc/stdlib/timegm.c
  src/libcpp/chrono.cpp
  src/libcpp/condition-variable.cpp
  src/libcpp/cxx.cpp
  src/libcpp/memory-resource.cpp
  src/libcpp/mutex.cpp
  src/libcpp/new.cpp
  src/libcpp/system-error.cpp
  src/libcpp/thread-cpp.h
  src/libcpp/thread.cpp
  src/memory/block-pool.cpp
  src/memory/first-fit-top.cpp
  src/memory/lifo.cpp
  src/posix-io/block-device-partition.cpp
  src/posix-io/block-device.cpp
  src/posix-io/c-syscalls-posix.cpp
  src/posix-io/char-device.cpp
  src/posix-io/device.cpp
  src/posix-io/directory.cpp
  src/posix-io/file-descriptors-manager.cpp
  src/posix-io/file-system.cpp
  src/posix-io/file.cpp
  src/posix-io/io.cpp
  src/posix-io/net-stack.cpp
  src/posix-io/socket.cpp
  src/posix-io/tty.cpp
  src/rtos/internal/os-flags.cpp
  src/rtos/internal/os-lists.cpp
  src/rtos/os-c-wrapper.cpp
  src/rtos/os-clocks.cpp
  src/rtos/os-condvar.cpp
  src/rtos/os-core.cpp
  src/rtos/os-evflags.cpp
  src/rtos/os-idle.cpp
  src/rtos/os-main.cpp
  src/rtos/os-memory.cpp
  src/rtos/os-mempool.cpp
  src/rtos/os-mqueue.cpp
  src/rtos/os-mutex.cpp
  src/rtos/os-semaphore.cpp
  src/rtos/os-thread.cpp
  src/rtos/os-timer.cpp
  src/semihosting/c-syscalls-semihosting.cpp
  src/startup/exception-handlers.c
  src/startup/initialise-free-store.cpp
  src/startup/initialize-hardware.c
  src/startup/reset-hardware.c
  src/startup/startup.cpp
  src/utils/lists.cpp
)

target_compile_definitions(micro-os-plus-iii-interface INTERFACE
  # None.
)

target_compile_options(micro-os-plus-iii-interface INTERFACE
  # None.
)

target_link_libraries(micro-os-plus-iii-interface INTERFACE
  # None.
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(micro-os-plus-iii-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library(micro-os-plus::iii ALIAS micro-os-plus-iii-interface)
message(VERBOSE "> micro-os-plus::iii -> micro-os-plus-iii-interface")

# -----------------------------------------------------------------------------
