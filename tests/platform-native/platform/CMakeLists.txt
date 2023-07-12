# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2022-2023 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/mit/.
#
# -----------------------------------------------------------------------------

# This file creates the platform specific test executables.

message(VERBOSE "Processing 'tests/platform-native/platform'...")

# -----------------------------------------------------------------------------

if(NOT DEFINED xpack_platform_compile_definition)
  message(FATAL_ERROR "Define xpack_platform_compile_definition in platform*/cmake/globals.cmake")
endif()

# -----------------------------------------------------------------------------
# Define the platform library.

add_library(platform-native-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(platform-native-interface INTERFACE
  "include"
)

target_sources(platform-native-interface INTERFACE
  # None.
)

target_compile_definitions(platform-native-interface INTERFACE
  "${xpack_platform_compile_definition}"
  # $<$<NOT:$<C_COMPILER_ID:Clang,AppleClang>>:_POSIX_C_SOURCE=200809L>
  _POSIX_C_SOURCE=200809L
)

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

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  # https://libcxx.llvm.org/UsingLibcxx.html
  list(APPEND xpack_platform_common_args
    $<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>
  )
endif()

# https://cmake.org/cmake/help/v3.20/variable/CMAKE_LANG_COMPILER_ID.html
# message("${CMAKE_C_COMPILER_ID} ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
# Unfortunatelly in a container it shows aarch64 instead of armv7l.

# -flto seems ok now with clang too.
list(APPEND xpack_platform_common_args
  $<$<CONFIG:Release,MinSizeRel>:-flto>
)

target_compile_options(platform-native-interface INTERFACE
  ${xpack_platform_common_args}
)

# When `-flto` is used, the compile options must be passed to the linker too.
target_link_options(platform-native-interface INTERFACE
  -v
  ${xpack_platform_common_args}

  $<$<PLATFORM_ID:Windows>:-static>

  $<$<AND:$<C_COMPILER_ID:GNU>,$<PLATFORM_ID:Darwin>>:-static-libgcc>
  $<$<AND:$<C_COMPILER_ID:GNU>,$<PLATFORM_ID:Darwin>>:-static-libstdc++>

  $<$<PLATFORM_ID:Darwin>:-Wl,-dead_strip>
  $<$<PLATFORM_ID:Linux,Windows>:-Wl,--gc-sections>
)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" )
  # https://clang.llvm.org/docs/Toolchain.html#compiler-runtime
  target_link_options (platform-native-interface INTERFACE
    # Fails on Raspberry Pi (it hangs, even in Debug).
    # -rtlib=compiler-rt
    # -lunwind
    $<$<PLATFORM_ID:Linux>:-fuse-ld=lld>
  )
endif()

target_link_libraries(platform-native-interface INTERFACE
  micro-os-plus::common
  micro-os-plus::iii-posix-arch
  xpack-3rd-party::libucontext
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(platform-native-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

add_library(micro-os-plus::platform ALIAS platform-native-interface)
message(VERBOSE "> micro-os-plus::platform -> platform-native-interface")

# -----------------------------------------------------------------------------
