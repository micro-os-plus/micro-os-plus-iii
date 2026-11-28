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

# Platform specific definitions.

# -----------------------------------------------------------------------------

message (
  VERBOSE
  "Including 'tests/platforms/${PLATFORM_NAME}/cmake/definitions.cmake'..."
)

# -----------------------------------------------------------------------------

# Required by the platform `device` folder.
set (xpack_device_compile_definition "MICRO_OS_PLUS_DEVICE_STM32F411XE")

# Create the listing files.
set (XPACK_ENABLE_CREATE_LISTING true)

# Platform specific tests.
set (XPACK_ENABLE_BLINKY_TEST true)
list (APPEND xpack_dependencies_tests_folders
      "${CMAKE_SOURCE_DIR}/sources/blinky"
)
set (XPACK_ENABLE_INSTRUMENTATION_TEST true)
list (APPEND xpack_dependencies_tests_folders
      "${CMAKE_SOURCE_DIR}/sources/instrumentation"
)

# -----------------------------------------------------------------------------
