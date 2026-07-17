# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2023-2025 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

message (VERBOSE "Processing 'tests/platforms/${PLATFORM_NAME}/device'...")

# -----------------------------------------------------------------------------
if (NOT DEFINED xpack_device_compile_definition)
  message (
    FATAL_ERROR
      "Define xpack_device_compile_definition in platform/${PLATFORM_NAME}/cmake/definitions.cmake"
  )
endif ()

# -----------------------------------------------------------------------------
# Define the device library.
add_library (device-stm32f767xx-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------
target_include_directories (device-stm32f767xx-interface INTERFACE "include")

target_sources (
  device-stm32f767xx-interface
  INTERFACE "src/exception-handlers.cpp" "src/initialize-hardware.c"
            "src/vectors-stm32f767xx.c"
)

target_compile_definitions (
  device-stm32f767xx-interface INTERFACE "${xpack_device_compile_definition}"
)

target_compile_options (device-stm32f767xx-interface INTERFACE # None.
)

target_link_options (
  device-stm32f767xx-interface INTERFACE
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/mem.ld
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/sections-flash.ld
)

target_link_libraries (
  device-stm32f767xx-interface INTERFACE micro-os-plus::stm32cubemx
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists (device-stm32f767xx-interface)
endif ()

# -----------------------------------------------------------------------------
# Aliases.
add_library (micro-os-plus::device ALIAS device-stm32f767xx-interface)
message (VERBOSE "> micro-os-plus::device -> device-stm32f767xx-interface")

# -----------------------------------------------------------------------------
