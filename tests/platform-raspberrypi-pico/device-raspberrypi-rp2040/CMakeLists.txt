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

message(VERBOSE "Processing 'tests/platform-raspberrypi-pico/device-raspberrypi-rp2040'...")

# -----------------------------------------------------------------------------

if(NOT DEFINED xpack_device_compile_definition)
  message(FATAL_ERROR "Define xpack_device_compile_definition in platform*/cmake/globals.cmake")
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
  # None.
)

target_link_libraries(device-raspberrypi-rp2040-interface INTERFACE
  cmsis_core
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(device-raspberrypi-rp2040-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

add_library(micro-os-plus::device ALIAS device-raspberrypi-rp2040-interface)
message(VERBOSE "> micro-os-plus::device -> device-raspberrypi-rp2040-interface")

# -----------------------------------------------------------------------------
