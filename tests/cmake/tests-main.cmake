# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# build-helper/templates/common/_micro-os-plus/tests/cmake/tests-main-liquid.cmake
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

# Bare-metal executables have the .elf extension.
if (CMAKE_SYSTEM_NAME STREQUAL "Generic")
  set (CMAKE_EXECUTABLE_SUFFIX ".elf")
endif ()

# -----------------------------------------------------------------------------
# Non-target specific definitions.

# The globals must be included in this scope, before creating any targets. The
# compile options, symbols and include folders apply to all compiled sources,
# from all libraries.
if ("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
  set (extension ".cmd")
endif ()

# Define functions like add_cross_test_executable().
include ("cmake/common-functions.cmake")

# Define `micro-os-plus::common-options` with the compile & link options common
# to all platforms.
include ("cmake/common-options-library.cmake")

# -----------------------------------------------------------------------------
# Dependencies.

# Optionally, override the project definitions with platform specific values.
if (EXISTS
    "${CMAKE_SOURCE_DIR}/platforms/${PLATFORM_NAME}/cmake/definitions.cmake"
)
  message (VERBOSE "") # Separator for readability
  include ("platforms/${PLATFORM_NAME}/cmake/definitions.cmake")
endif ()

message (VERBOSE "") # Separator for readability
# Set `xpack_dependencies_folders` with the platform specific dependencies.
include ("platforms/${PLATFORM_NAME}/cmake/dependencies-folders.cmake")

foreach (folder ${xpack_dependencies_folders})
  file (RELATIVE_PATH relative_folder "${CMAKE_SOURCE_DIR}" "${folder}")
  message (VERBOSE "  ${relative_folder}")
endforeach ()

# Iterate the platform dependencies and `add_subdirectory()`.
message (VERBOSE "") # Separator for readability
message (VERBOSE "Adding dependencies libraries...")
xpack_add_dependencies_subdirectories (
  "${xpack_dependencies_folders}" "dependencies-bin"
)

message (VERBOSE "") # Separator for readability
# Define the generic `micro-os-plus::architecture` alias.
include ("platforms/${PLATFORM_NAME}/cmake/architecture-alias.cmake")

message (VERBOSE "") # Separator for readability
# Set `xpack_dependencies_libraries` with the platform specific libraries.
include ("platforms/${PLATFORM_NAME}/cmake/dependencies-libraries.cmake")
foreach (library ${xpack_dependencies_libraries})
  message (VERBOSE "  ${library}")
endforeach ()

# -----------------------------------------------------------------------------

# Add the platform library. The artefacts are created in the
# `platform-bin` folder.
message (VERBOSE "") # Separator for readability
message (VERBOSE "Adding platform library...")
message (VERBOSE "") # Separator for readability
add_subdirectory ("platforms/${PLATFORM_NAME}" "platform-bin")

# -----------------------------------------------------------------------------

# Add the project library, defined one level above.
message (VERBOSE "") # Separator for readability
message (VERBOSE "Adding top library...")
message (VERBOSE "") # Separator for readability
add_subdirectory (".." "top-bin")

# -----------------------------------------------------------------------------

# Iterate the folders from `project-definitions.cmake` and `add_subdirectory()`.
message (VERBOSE "") # Separator for readability
message (VERBOSE "Adding tests libraries...")
xpack_add_dependencies_subdirectories (
  "${xpack_dependencies_tests_folders}" "tests-bin"
)

# -----------------------------------------------------------------------------

# With all libraries processed, include the artefacts and tests definitions.
message (VERBOSE "") # Separator for readability
message (VERBOSE "Adding tests artefacts...")
message (VERBOSE "") # Separator for readability
include ("platforms/${PLATFORM_NAME}/cmake/artefacts.cmake")

# -----------------------------------------------------------------------------

message (VERBOSE "") # Separator for readability

# -----------------------------------------------------------------------------
