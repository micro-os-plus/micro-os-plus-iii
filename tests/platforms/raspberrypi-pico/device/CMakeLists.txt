# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2022-2025 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

message(VERBOSE "Processing 'tests/platforms/${PLATFORM_NAME}/device'...")

# -----------------------------------------------------------------------------
if(NOT DEFINED xpack_device_compile_definition)
  message(FATAL_ERROR "Define xpack_device_compile_definition in platform/${PLATFORM_NAME}/cmake/definitions.cmake")
endif()

# -----------------------------------------------------------------------------
# Define the device library.
add_library(device-raspberrypi-rp2040-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------
target_include_directories(device-raspberrypi-rp2040-interface INTERFACE
  "include"
)

target_sources(device-raspberrypi-rp2040-interface INTERFACE
  "src/initialize-hardware.c"
  "src/exception-handlers.cpp"
  "src/vectors-rp2040.c"
)

target_compile_definitions(device-raspberrypi-rp2040-interface INTERFACE
  "${xpack_device_compile_definition}"
)

target_compile_options(device-raspberrypi-rp2040-interface INTERFACE

  # Turn off many warnings, the Pico SDK throws a lot of them.
  $<$<COMPILE_LANGUAGE:C>:-Wno-c++-compat>
  $<$<COMPILE_LANGUAGE:C>:-Wno-old-style-definition>
  $<$<COMPILE_LANGUAGE:C>:-Wno-missing-prototypes>
  $<$<COMPILE_LANGUAGE:C>:-Wno-nested-externs>
  $<$<COMPILE_LANGUAGE:C>:-Wno-bad-function-cast>

  -Wno-undef
  -Wno-attributes
  -Wno-padded
  -Wno-format
  -Wno-format-y2k
  -Wno-format-nonliteral
  -Wno-format-security
  -Wno-aggregate-return
  -Wno-packed
  -Wno-missing-declarations
)

target_link_options(device-raspberrypi-rp2040-interface INTERFACE
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/mem-pico.ld
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/sections-flash.ld
)

target_link_libraries(device-raspberrypi-rp2040-interface INTERFACE
  cmsis_core
)

if(COMMAND xpack_display_target_lists)
  xpack_display_target_lists(device-raspberrypi-rp2040-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.
add_library(micro-os-plus::device ALIAS device-raspberrypi-rp2040-interface)
message(VERBOSE "> micro-os-plus::device -> device-raspberrypi-rp2040-interface")

# -----------------------------------------------------------------------------
