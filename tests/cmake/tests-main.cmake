# -----------------------------------------------------------------------------
# DO NOT EDIT!
# Automatically generated from build-helper/templates.
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
# Project wide setup #

# Enable the languages used in the tests.
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

# Protect against in-source builds.
if(CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
  message(FATAL_ERROR "In-source builds are not supported. Please use a separate folder for build.")
endif()

# The build-helper is a devDependency, available only after `xpm install`.
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/xpacks/@micro-os-plus/build-helper/cmake")
include("micro-os-plus-build-helper")

xpack_display_greetings("${CMAKE_CURRENT_SOURCE_DIR}/../package.json")

# -----------------------------------------------------------------------------

# Bare-metal executables have the .elf extension.
if(CMAKE_SYSTEM_NAME STREQUAL "Generic")
  set(CMAKE_EXECUTABLE_SUFFIX ".elf")
endif()

# -----------------------------------------------------------------------------
# Non-target specific definitions #

# The globals must be included in this scope, before creating any targets.
# The compile options, symbols and include folders apply to all
# compiled sources, from all libraries.
if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
  set(extension ".cmd")
endif()

# Define `micro-os-plus::common-options` with the compile & link options
# common to all platforms.
include("cmake/common-options.cmake")

# Set `xpack_dependencies_folders` with the platform specific dependencies.
include("platforms/${PLATFORM_NAME}/cmake/dependencies-folders.cmake")

# Define `micro-os-plus::platform` with the platform definitions,
# common to all tests.
include("platforms/${PLATFORM_NAME}/cmake/platform-options.cmake")

# Iterate the platform dependencies and `add_subdirectory()`.
xpack_add_dependencies_subdirectories("${xpack_dependencies_folders}" "xpacks-bin")

# -----------------------------------------------------------------------------

# Add the project library, defined one level above.
message(VERBOSE "Adding top library...")
add_subdirectory(".." "top-bin")

# -----------------------------------------------------------------------------
# Platform specifics #

# Add the platform specific targets and tests.
# For consistency, the binaries are created in the `platform-bin` folder.
add_subdirectory("platforms/${PLATFORM_NAME}" "platform-bin")

# -----------------------------------------------------------------------------
