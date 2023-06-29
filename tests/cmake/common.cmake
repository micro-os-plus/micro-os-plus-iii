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

# This file defines the global compiler settings that apply to all targets.
# Must be added with `include()` in the `tests` scope before the platform
# globals.

message(VERBOSE "Including top common definitions...")

# -----------------------------------------------------------------------------

add_library(micro-os-plus-common-interface INTERFACE EXCLUDE_FROM_ALL)

# https://cmake.org/cmake/help/v3.20/command/add_compile_definitions.html
target_compile_definitions(micro-os-plus-common-interface INTERFACE
  # NDEBUG is provided by the toolchain definitions on release.

  $<$<CONFIG:Debug>:DEBUG>
  $<$<CONFIG:Debug>:TRACE>

  OS_USE_OS_APP_CONFIG_H
)

set(global_common_options
  -fmessage-length=0
  -fsigned-char

  # These are used in conjunction with linker `--gc-sections`.
  -ffunction-sections
  -fdata-sections

  -fdiagnostics-color=always

  # No need, the toolchain does it.
  # $<$<CONFIG:Debug>:${DEBUG_OPTION}>
)

# A list of all imaginable warnings.
# Targets may add options to disable some of them.
xpack_set_all_compiler_warnings(all_warnings)

target_compile_options(micro-os-plus-common-interface INTERFACE
  ${global_common_options}
  ${all_warnings}
)

# When `-flto` is used, the compile options must be passed to the linker too.
target_link_options(micro-os-plus-common-interface INTERFACE
  ${global_common_options}
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(micro-os-plus-common-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library(micro-os-plus::common ALIAS micro-os-plus-common-interface)
message(VERBOSE "> micro-os-plus::common -> micro-os-plus-common-interface")

# -----------------------------------------------------------------------------
