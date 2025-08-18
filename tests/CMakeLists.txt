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

# -----------------------------------------------------------------------------
## Preamble ##

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required(VERSION 3.20)

project(
  micro-os-plus-micro-os-plus-iii-tests
  DESCRIPTION "µOS++ IIIe tests"
)

# Must be called from the project root folder.
enable_testing()

# -----------------------------------------------------------------------------

# Global definitions.
include("cmake/global-definitions.cmake")

# -----------------------------------------------------------------------------
## Tests ##

if(ENABLE_RTOS_APIS_TEST)
  add_subdirectory("rtos-apis")
endif()

if(ENABLE_MUTEX_STRESS_TEST)
  add_subdirectory("mutex-stress")
endif()

if(ENABLE_CMSIS_OS_VALIDATOR_TEST)
  add_subdirectory("cmsis-os-validator")
endif()

# -----------------------------------------------------------------------------

# Include the main CMake script, common to all projects.
include("cmake/tests-main.cmake")

# -----------------------------------------------------------------------------
