# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# xcdl-package.jsonc
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2022-2026 Liviu Ionescu. All rights reserved.
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
# `subdir('xpacks/@micro-os-plus/micro-os-plus-iii')`
#
# This creates dependencies that can be linked to a target with:
#
# `dependencies: [micro_os_plus_iii_dependency],`

# Note: the Meson configuration is provided only as a proof of concept;
# for production it might need some refinements.

# -----------------------------------------------------------------------------

# A source code library with the portable part of µOS++ IIIe.
message('Processing xpm package @micro-os-plus/micro-os-plus-iii...')

# -----------------------------------------------------------------------------

# Meson's DSL does not provide introspection methods on dependency objects
# to retrieve their properties after creation. The `_local_*` variables
# therefore serve a dual purpose: populating the dependency object and
# iterating over its components for the `message()` calls at the end.

_local_compile_args = [] # Common C/C++ args.
_local_compile_c_args = [] # C only args.
_local_compile_cpp_args = [] # C++ only args.
_local_include_directories = []
_local_sources = []
_local_compile_definitions = []
_local_dependencies = []
_local_link_args = []
_local_link_with = []

_local_include_directories += [
  'include',
  'include/cmsis-plus/legacy',
]

_local_sources += [
  'src/diag/trace-itm.cpp',
  'src/diag/trace-segger-rtt.cpp',
  'src/diag/trace-semihosting.cpp',
  'src/diag/trace.cpp',
  'src/driver/common.cpp',
  'src/driver/serial.cpp',
  'src/libc/_sbrk.c',
  'src/libc/newlib/c-newlib-reent.cpp',
  'src/libc/stdlib/assert.c',
  'src/libc/stdlib/atexit.cpp',
  'src/libc/stdlib/atexit.h',
  'src/libc/stdlib/exit.c',
  'src/libc/stdlib/init-fini.c',
  'src/libc/stdlib/malloc.cpp',
  'src/libc/stdlib/timegm.c',
  'src/libcpp/chrono.cpp',
  'src/libcpp/condition-variable.cpp',
  'src/libcpp/cxx.cpp',
  'src/libcpp/memory-resource.cpp',
  'src/libcpp/mutex.cpp',
  'src/libcpp/new.cpp',
  'src/libcpp/system-error.cpp',
  'src/libcpp/thread-cpp.h',
  'src/libcpp/thread.cpp',
  'src/memory/block-pool.cpp',
  'src/memory/first-fit-top.cpp',
  'src/memory/lifo.cpp',
  'src/posix-io/block-device-partition.cpp',
  'src/posix-io/block-device.cpp',
  'src/posix-io/c-syscalls-posix.cpp',
  'src/posix-io/char-device.cpp',
  'src/posix-io/device.cpp',
  'src/posix-io/directory.cpp',
  'src/posix-io/file-descriptors-manager.cpp',
  'src/posix-io/file-system.cpp',
  'src/posix-io/file.cpp',
  'src/posix-io/io.cpp',
  'src/posix-io/net-stack.cpp',
  'src/posix-io/socket.cpp',
  'src/posix-io/tty.cpp',
  'src/rtos/internal/os-flags.cpp',
  'src/rtos/internal/os-lists.cpp',
  'src/rtos/os-c-wrapper.cpp',
  'src/rtos/os-clocks.cpp',
  'src/rtos/os-condvar.cpp',
  'src/rtos/os-core.cpp',
  'src/rtos/os-evflags.cpp',
  'src/rtos/os-idle.cpp',
  'src/rtos/os-main.cpp',
  'src/rtos/os-memory.cpp',
  'src/rtos/os-mempool.cpp',
  'src/rtos/os-mqueue.cpp',
  'src/rtos/os-mutex.cpp',
  'src/rtos/os-semaphore.cpp',
  'src/rtos/os-thread.cpp',
  'src/rtos/os-timer.cpp',
  'src/semihosting/c-syscalls-semihosting.cpp',
  'src/startup/exception-handlers.c',
  'src/startup/initialise-free-store.cpp',
  'src/startup/initialize-hardware.c',
  'src/startup/reset-hardware.c',
  'src/startup/startup.cpp',
  'src/utils/lists.cpp',
]

# https://mesonbuild.com/Reference-manual_functions.html#declare_dependency
micro_os_plus_iii_dependency = declare_dependency(
  include_directories: include_directories(_local_include_directories),
  compile_args: _local_compile_args,
  sources: files(_local_sources),
  dependencies: _local_dependencies,
  link_args: _local_link_args,
  link_with: _local_link_with,
)

# Meson dependencies cannot differentiate between C and C++ compile args;
# they are therefore exposed as separate variables alongside the dependency.
micro_os_plus_iii_dependency_compile_c_args = _local_compile_c_args
micro_os_plus_iii_dependency_compile_cpp_args = _local_compile_cpp_args

foreach name : _local_include_directories
  message('+ -I ' + name)
endforeach
foreach name : _local_sources + _local_compile_definitions + _local_compile_args + _local_compile_c_args + _local_compile_cpp_args + _local_link_args
  message('+ ' + name)
endforeach
message('> micro_os_plus_iii_dependency')

# -----------------------------------------------------------------------------
