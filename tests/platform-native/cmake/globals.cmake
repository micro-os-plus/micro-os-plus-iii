# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2022 Liviu Ionescu
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/MIT/.
#
# -----------------------------------------------------------------------------

# This file defines the global compiler settings that apply to all targets.
# Must be added with `include()` in the `tests` scope.

message(VERBOSE "Including platform-native global definitions...")

# -----------------------------------------------------------------------------

# Global definitions.
# include_directories()

set(xpack_platform_common_args
  -Werror
  # Apple clang 13 does not support -Wunused-but-set-variable
  # $<$<PLATFORM_ID:Darwin>:-Wno-unknown-warning-option>
  $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-unknown-warning-option>
  $<$<PLATFORM_ID:Darwin>:-Wno-missing-include-dirs>
  # -g
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  list(APPEND xpack_platform_common_args
    # $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-used-but-marked-unused>
  )
endif()

# https://cmake.org/cmake/help/v3.20/variable/CMAKE_LANG_COMPILER_ID.html
# message("${CMAKE_C_COMPILER_ID} ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
# Unfortunatelly in a container it shows aarch64 instead of armv7l.
if("${CMAKE_C_COMPILER_ID}" MATCHES "Clang" AND "${CMAKE_SYSTEM_NAME}" STREQUAL "Linux" AND ("${CMAKE_SYSTEM_PROCESSOR}" MATCHES "armv" OR ("${CMAKE_SYSTEM_PROCESSOR}" STREQUAL "aarch64")))
  # clang-12: error: unable to execute command: Segmentation fault
  # clang-12: error: linker command failed due to signal (use -v to see invocation)
  # Alternate linker was not effective.
  message(STATUS "Clang Linux arm - skip -flto")
else()
  list(APPEND xpack_platform_common_args
    $<$<CONFIG:Release>:-flto>
    $<$<CONFIG:MinSizeRel>:-flto>
  )
endif()

add_compile_definitions(
  # $<$<NOT:$<C_COMPILER_ID:Clang,AppleClang>>:_POSIX_C_SOURCE=200809L>
  _POSIX_C_SOURCE=200809L
)

add_compile_options(
  ${xpack_platform_common_args}
)

# When `-flto` is used, the compile options must be passed to the linker too.
add_link_options(
  ${xpack_platform_common_args}
)

# -----------------------------------------------------------------------------
