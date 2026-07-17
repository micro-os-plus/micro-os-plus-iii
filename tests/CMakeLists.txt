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

# -----------------------------------------------------------------------------
# Preamble.

# https://cmake.org/cmake/help/v3.20/
cmake_minimum_required (VERSION 3.20)

project (micro-os-plus-micro-os-plus-iii-tests DESCRIPTION "µOS++ IIIe tests")

# -----------------------------------------------------------------------------
# Project wide setup.

# Enable the languages used in the tests.
enable_language (C)
enable_language (CXX)
enable_language (ASM)

# Specify the C/C++ standards.
set (CMAKE_C_STANDARD 11)
set (CMAKE_C_STANDARD_REQUIRED ON)
set (CMAKE_C_EXTENSIONS OFF)
set (CMAKE_CXX_STANDARD 20)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------

# Must be called from the project root folder.
enable_testing ()

# -----------------------------------------------------------------------------

# Protect against in-source builds.
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message (
    FATAL_ERROR
      "In-source builds are not supported. Please use a separate folder for build."
  )
endif ()

# The build-helper is a devDependency, available only after `xpm install`.
list (APPEND CMAKE_MODULE_PATH
      "${CMAKE_CURRENT_SOURCE_DIR}/xpacks/@micro-os-plus/build-helper/cmake"
)
include ("micro-os-plus-build-helper")

xpack_display_greetings ("${CMAKE_CURRENT_SOURCE_DIR}/../package.json")

# -----------------------------------------------------------------------------

# Global definitions.
include ("cmake/global-definitions.cmake")

# -----------------------------------------------------------------------------

# Include the main CMake script, common to all projects.
include ("cmake/tests-main.cmake")

# -----------------------------------------------------------------------------
