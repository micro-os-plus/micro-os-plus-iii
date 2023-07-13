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

message(VERBOSE "Processing 'tests/platform-nucleo-h743zi/device'...")

# -----------------------------------------------------------------------------

if(NOT DEFINED xpack_device_compile_definition)
  message(FATAL_ERROR "Define xpack_device_compile_definition in platform*/cmake/globals.cmake")
endif()

# -----------------------------------------------------------------------------
# Define the device library.

add_library(device-stm32h743zi-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(device-stm32h743zi-interface INTERFACE
  "include"
)

target_sources(device-stm32h743zi-interface INTERFACE
  "src/exception-handlers.cpp"
  "src/initialize-hardware.c"
  "src/vectors_stm32h743xx.c"
)

target_compile_definitions(device-stm32h743zi-interface INTERFACE
  "${xpack_device_compile_definition}"
)

target_compile_options(device-stm32h743zi-interface INTERFACE
  # None.
)

target_link_libraries(device-stm32h743zi-interface INTERFACE
  micro-os-plus::stm32cubemx
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(device-stm32h743zi-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

add_library(micro-os-plus::device ALIAS device-stm32h743zi-interface)
message(VERBOSE "> micro-os-plus::device -> device-stm32h743zi-interface")

# -----------------------------------------------------------------------------
