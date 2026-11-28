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

# Define a list of libraries dependencies.

# -----------------------------------------------------------------------------

message (
  VERBOSE
  "Including 'tests/platforms/${PLATFORM_NAME}/cmake/dependencies-libraries.cmake'..."
)

# -----------------------------------------------------------------------------

# More libraries in addition to common ones (common-options and platform).
# Included in `cmake/project-functions.cmake`.

# The tested library, linked to all tests.
set (xpack_dependencies_libraries micro-os-plus::iii)

# -----------------------------------------------------------------------------
