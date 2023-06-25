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

# This file adds the platform dependencies.

set(xpack_dependencies_folders
  # Project dependencies.
  # The BINARY_DIR is the `build/<config>` folder.
  "${CMAKE_BINARY_DIR}/xpacks/@micro-os-plus/micro-os-plus-iii-cortexm"
  "${CMAKE_BINARY_DIR}/xpacks/@xpacks/arm-cmsis"

  # The SOURCE_DIR is the `tests` folder.
  "${CMAKE_SOURCE_DIR}/device-qemu-cortexm"
  "${CMAKE_SOURCE_DIR}/xpacks/@xpacks/arm-cmsis-rtos-validator"
  "${CMAKE_SOURCE_DIR}/xpacks/@xpacks/chan-fatfs"

  # Build configuration dependencies.
  # None.
)

xpack_add_dependencies_subdirectories("${xpack_dependencies_folders}" "xpacks-bin")

# -----------------------------------------------------------------------------
