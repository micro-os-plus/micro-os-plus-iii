# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2023 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/mit/.
#
# -----------------------------------------------------------------------------

# This file defines the global compiler settings that apply to all targets.
# Must be included with include() in the `tests` scope.

message(VERBOSE "Including platform-nucleo-f411re global definitions...")

# -----------------------------------------------------------------------------

# Required in devices-qemu-cortexm.
set(xpack_device_compile_definition "MICRO_OS_PLUS_DEVICE_STM32F411XE")

set(xpack_platform_compile_definition "MICRO_OS_PLUS_PLATFORM_NUCLEO_F411RE")

target_compile_options(micro-os-plus-common-interface INTERFACE

  # Turn off many warnings, the Pico SDK throws a lot of them.

  # $<$<COMPILE_LANGUAGE:C>:-Wno-c++-compat>
  # $<$<COMPILE_LANGUAGE:C>:-Wno-old-style-definition>
  # $<$<COMPILE_LANGUAGE:C>:-Wno-missing-prototypes>
  # $<$<COMPILE_LANGUAGE:C>:-Wno-nested-externs>
  $<$<COMPILE_LANGUAGE:C>:-Wno-bad-function-cast>

  # -Wno-undef
  # -Wno-attributes
  -Wno-padded
  # -Wno-format
  # -Wno-format-y2k
  # -Wno-format-nonliteral
  # -Wno-format-security
  # -Wno-aggregate-return
  # -Wno-packed
  # -Wno-missing-declarations
  -Wno-redundant-decls
  -Wno-conversion
  -Wno-duplicated-branches
  -Wno-switch-enum
  -Wno-unused-parameter
  -Wno-switch-default
  -Wno-cast-align
)

# -----------------------------------------------------------------------------
