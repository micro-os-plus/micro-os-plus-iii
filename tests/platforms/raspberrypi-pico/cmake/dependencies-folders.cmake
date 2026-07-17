# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2022-2025 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
#  obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

# Define a list of folders where the platform dependencies are located.

# -----------------------------------------------------------------------------

message(VERBOSE "Including tests/platforms/${PLATFORM_NAME}/cmake/dependencies-folders.cmake...")

# -----------------------------------------------------------------------------
set(xpack_dependencies_folders

  "${CMAKE_SOURCE_DIR}/rtos-apis-test"
  "${CMAKE_SOURCE_DIR}/mutex-stress-test"
  "${CMAKE_SOURCE_DIR}/cmsis-os-validator-test"

  # Project dependencies.
  # The BINARY_DIR is the `build/<config>` folder.
  "${CMAKE_BINARY_DIR}/xpacks/@micro-os-plus/micro-os-plus-iii-cortexm"

  # Pico SDK.
  "${CMAKE_BINARY_DIR}/xpacks/@xpack-3rd-party/raspberrypi-pico-sdk"

  # The SOURCE_DIR is the `tests` folder.
  "${CMAKE_SOURCE_DIR}/xpacks/@xpacks/arm-cmsis-rtos-validator"
  "${CMAKE_SOURCE_DIR}/xpacks/@xpacks/chan-fatfs"

  # Build configuration dependencies.
  # None.
)

# -----------------------------------------------------------------------------
