# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2022-2026 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

# Define a list of folders where the platform dependencies are located.

# -----------------------------------------------------------------------------

message (
  VERBOSE
  "Including 'tests/platforms/${PLATFORM_NAME}/cmake/dependencies-folders.cmake'..."
)

# -----------------------------------------------------------------------------

# All the dependencies folders will be included in `tests-main.cmake` with
# `add_subdirectory()`.

# The SOURCE_DIR is the `tests` folder; the BINARY_DIR is the `build/<config>`
# folder.

set (
  xpack_dependencies_folders
  #
  # Portable dependencies.
  "${CMAKE_SOURCE_DIR}/xpacks/@xpack-3rd-party/arm-cmsis-rtos-validator"
  "${CMAKE_SOURCE_DIR}/xpacks/@xpack-3rd-party/chan-fatfs"
  #
  # Platform specific dependencies.
  "${CMAKE_BINARY_DIR}/xpacks/@micro-os-plus/micro-os-plus-iii-cortexm"
  "${CMAKE_BINARY_DIR}/xpacks/@xpack-segger/rtt"
  "${CMAKE_BINARY_DIR}/xpacks/@xpack-segger/system-view"
)

# -----------------------------------------------------------------------------
