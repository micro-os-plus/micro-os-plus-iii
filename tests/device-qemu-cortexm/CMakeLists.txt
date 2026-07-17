# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2022-2025 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

message (VERBOSE "Processing 'tests/device-qemu-cortexm'...")

if (NOT DEFINED xpack_device_compile_definition)
  message (
    FATAL_ERROR
      "Define xpack_device_compile_definition in platform/${PLATFORM_NAME}/cmake/definitions.cmake"
  )
endif ()

# -----------------------------------------------------------------------------
# Define the device library.
add_library (device-qemu-cortexm-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------
target_include_directories (device-qemu-cortexm-interface INTERFACE "include")

target_sources (
  device-qemu-cortexm-interface
  INTERFACE "src/system-cortexm.c" "src/initialize-hardware.c"
            "src/vectors-cortexm.c" "src/exception-handlers.cpp"
)

target_compile_definitions (
  device-qemu-cortexm-interface INTERFACE "${xpack_device_compile_definition}"
)

target_compile_options (device-qemu-cortexm-interface INTERFACE # None.
)

# Validate.
if (NOT DEFINED xpack_device_linker_script_file_name)
  message (
    FATAL_ERROR
      "Define xpack_device_linker_script_file_name in platforms/${PLATFORM_NAME}/cmake/dependencies-folders.cmake"
  )
endif ()

target_link_options (
  device-qemu-cortexm-interface
  INTERFACE
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/${xpack_device_linker_script_file_name}
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/sections-flash.ld
)

target_link_libraries (
  device-qemu-cortexm-interface INTERFACE xpacks::arm-cmsis
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists (device-qemu-cortexm-interface)
endif ()

# -----------------------------------------------------------------------------
# Aliases.
add_library (micro-os-plus::device ALIAS device-qemu-cortexm-interface)
message (VERBOSE "> micro-os-plus::device -> device-qemu-cortexm-interface")

# -----------------------------------------------------------------------------
