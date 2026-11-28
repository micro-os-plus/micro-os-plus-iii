# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# build-helper/templates/common/_micro-os-plus/tests/cmake/project-definitions-liquid.cmake
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

# -----------------------------------------------------------------------------
# Project specific definitions.

set (xpack_dependencies_tests_folders)

set (XPACK_ENABLE_RTOS_APIS_TEST true)
set (XPACK_ENABLE_UNIT_TEST true)
set (XPACK_ENABLE_MUTEX_STRESS_TEST true)
set (XPACK_ENABLE_CMSIS_OS_VALIDATOR_TEST true)

# Each test has its own library. Add only those enabled.
if (XPACK_ENABLE_RTOS_APIS_TEST)
  list (APPEND xpack_dependencies_tests_folders
        "${CMAKE_SOURCE_DIR}/sources/rtos-apis"
  )
endif ()

if (XPACK_ENABLE_UNIT_TEST)
  list (APPEND xpack_dependencies_tests_folders
        "${CMAKE_SOURCE_DIR}/sources/unit"
  )
endif ()

if (XPACK_ENABLE_MUTEX_STRESS_TEST)
  list (APPEND xpack_dependencies_tests_folders
        "${CMAKE_SOURCE_DIR}/sources/mutex-stress"
  )
endif ()

if (XPACK_ENABLE_CMSIS_OS_VALIDATOR_TEST)
  list (APPEND xpack_dependencies_tests_folders
        "${CMAKE_SOURCE_DIR}/sources/cmsis-os-validator"
  )
endif ()

# -----------------------------------------------------------------------------

set (XPACK_ENABLE_REPORT_SIZE true)
set (XPACK_ENABLE_CREATE_HEX false)
set (XPACK_ENABLE_CREATE_LISTING false)

# -----------------------------------------------------------------------------
