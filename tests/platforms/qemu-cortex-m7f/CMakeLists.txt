# -----------------------------------------------------------------------------
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

# Create the platform specific library.

# -----------------------------------------------------------------------------

message (VERBOSE "Processing 'tests/platforms/${PLATFORM_NAME}/'...")

# -----------------------------------------------------------------------------

# Define the platform library.
add_library (micro-os-plus-platform-interface INTERFACE EXCLUDE_FROM_ALL)

target_include_directories (
  micro-os-plus-platform-interface INTERFACE "include"
)

target_sources (micro-os-plus-platform-interface INTERFACE # None.
)

target_compile_definitions (
  micro-os-plus-platform-interface
  INTERFACE
    MICRO_OS_PLUS_PLATFORM_QEMU_CORTEX_M7F
    # The RTOS configuration is in `cmsis-plus/os-app-config.h`, provided
    # by each test.
    OS_USE_OS_APP_CONFIG_H
    $<$<CONFIG:Debug>:TRACE>
    # Full POSIX conformance:
    # https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap02.html#tag_02_01_03
    _POSIX_C_SOURCE=200809L
    # For S_IREAD
    _GNU_SOURCE
)

set (
  xpack_platform_common_options
  -mcpu=cortex-m7
  -mthumb
  # -mfloat-abi=soft
  -mfloat-abi=hard
  # -fno-move-loop-invariants
  # Embedded builds must be warning free.
  -Werror
  # -flto fails to run on QEMU.
  # $<$<CONFIG:Release>:-flto>
  # $<$<CONFIG:MinSizeRel>:-flto>
  $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
  # ... libs-c/src/stdlib/exit.c:132:46
  # $<$<CXX_COMPILER_ID:GNU>:-Wno-missing-attributes>
  # parameter passing for argument of type
  # 'os::rtos::memory::allocator_stateless_default_resource<os::rtos::thread>'
  # changed in GCC 7.1
  $<$<CXX_COMPILER_ID:GNU>:-Wno-psabi>
  # $<$<COMPILE_LANGUAGE:C>:-fxxx>
  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html?highlight=compile_language#genex:COMPILE_LANGUAGE
  # $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
  # $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  # $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
)

target_compile_options (
  micro-os-plus-platform-interface INTERFACE ${xpack_platform_common_options}
)

# When `-flto` is used, the compile options must be passed to the linker too.
target_link_options (
  micro-os-plus-platform-interface
  INTERFACE
  # -v
  ${xpack_platform_common_options}
  -nostartfiles
  # --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon
  # -Wl,--end-group
  # Force the linker to keep the interrupt vectors which otherwise are not
  # referred from anywhere. -u_interrupt_vectors
  # nano has no exceptions. -specs=nano.specs
  -Wl,--gc-sections
)

if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER_EQUAL "12.0.0")
  target_link_options (
    micro-os-plus-platform-interface INTERFACE
    # .elf has a LOAD segment with RWX permissions (GCC 12)
    -Wl,--no-warn-rwx-segment
  )
endif ()

target_link_libraries (
  micro-os-plus-platform-interface INTERFACE micro-os-plus::architecture
                                             micro-os-plus::device
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists (micro-os-plus-platform-interface)
endif ()

# -----------------------------------------------------------------------------

# Aliases.
add_library (micro-os-plus::platform ALIAS micro-os-plus-platform-interface)
message (VERBOSE
         "> micro-os-plus::platform -> micro-os-plus-platform-interface"
)

# -----------------------------------------------------------------------------
