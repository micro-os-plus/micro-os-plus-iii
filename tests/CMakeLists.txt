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
## Project wide setup ##

enable_language(C)
enable_language(CXX)
enable_language(ASM)

# Specify the C/C++ standards.
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate the compile_commands.json file to feed the indexer.
# Highly recommended, to help IDEs construct the index.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Enable this to see the dependency graph.
# set_property(GLOBAL PROPERTY GLOBAL_DEPENDS_DEBUG_MODE 1)

# -----------------------------------------------------------------------------

if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "In-source builds are not supported. Please use a separate folder for build.")
endif()

# The build-helper is a devDependency, available only after `xpm install`.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/xpacks/@micro-os-plus/build-helper/cmake")
include("micro-os-plus-build-helper")

# Parse the top package.json for the version.
xpack_display_greetings("${CMAKE_CURRENT_SOURCE_DIR}/../package.json")

# -----------------------------------------------------------------------------

# CMAKE_BUILD_TYPE: Debug, Release, RelWithDebInfo, MinSizeRel

set(ENABLE_RTOS_APIS_TEST true)
set(ENABLE_MUTEX_STRESS_TEST true)
set(ENABLE_CMSIS_OS_VALIDATOR_TEST true)

# -----------------------------------------------------------------------------

# Bare-metal executables have the .elf extension.
if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
  set(CMAKE_EXECUTABLE_SUFFIX ".elf")
endif()

# -----------------------------------------------------------------------------
## Non-target specific definitions ##

# The globals must be included in this scope, before creating any targets.
# The compile options, symbols and include folders apply to all
# compiled sources, from all libraries.
include("cmake/globals.cmake")
include("platform-${PLATFORM_NAME}/cmake/globals.cmake")

xpack_display_global_lists()

# -----------------------------------------------------------------------------
## Dependencies ##

include("platform-${PLATFORM_NAME}/cmake/dependencies.cmake")

# Add the main library from the root folder.
message(VERBOSE "Adding top library...")
add_subdirectory(".." "top-bin")

# -----------------------------------------------------------------------------

if(ENABLE_RTOS_APIS_TEST)
  add_subdirectory("rtos-apis")
endif()

if(ENABLE_MUTEX_STRESS_TEST)
  add_subdirectory("mutex-stress")
endif()

# -----------------------------------------------------------------------------
## The platform specific path, where the tests are defined ##

# Add the platform specific configuration.
# The result will be in the `platform-bin` folder.
add_subdirectory("platform-${PLATFORM_NAME}" "platform-bin")

# -----------------------------------------------------------------------------
