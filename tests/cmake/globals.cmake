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
# Must be added with `include()` in the `tests` scope before the platform
# globals.

message(VERBOSE "Including global definitions...")

# -----------------------------------------------------------------------------

include_directories(
  # Folders are relative to `tests`.
  "platform-${PLATFORM_NAME}/include-config"
)

# https://cmake.org/cmake/help/v3.20/command/add_compile_definitions.html
add_compile_definitions(
  # NDEBUG is provided by the toolchain definitions on release.

  # TODO: remove DEBUG
  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Debug>:TRACE>

  $<$<CONFIG:Debug>:MICRO_OS_PLUS_DEBUG>
  $<$<CONFIG:Debug>:MICRO_OS_PLUS_TRACE>

  MICRO_OS_PLUS_INCLUDE_CONFIG_H
  OS_USE_OS_APP_CONFIG_H
)

set(global_common_options

  -fmessage-length=0
  -fsigned-char

  # These are used in conjunction with linker `--gc-sections`.
  -ffunction-sections
  -fdata-sections

  -fdiagnostics-color=always

  # $<$<CONFIG:Debug>:-g>
)

add_compile_options(
  ${global_common_options}
)

# When `-flto` is used, the compile options must be passed to the linker too.
add_link_options(
  ${global_common_options}
)

# A list of all imaginable warnings.
# Targets may add options to disable some of them.
xpack_set_all_compiler_warnings(all_warnings)

add_compile_options(
  ${all_warnings}
)

# -----------------------------------------------------------------------------
