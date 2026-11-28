# -----------------------------------------------------------------------------
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

# Define the generic architecture library as an alias to the architecture
# specific one.

# -----------------------------------------------------------------------------

message (
  VERBOSE
  "Including 'tests/platforms/${PLATFORM_NAME}/cmake/architecture-alias.cmake'..."
)

# -----------------------------------------------------------------------------

# Included from `cmake/tests-main.cmake`, after the dependencies folders are
# added (the architecture specific target must already exist) and before the
# platform library, which links to the generic name.

# https://cmake.org/cmake/help/v3.20/command/add_library.html#alias-libraries
add_library (
  micro-os-plus::architecture ALIAS micro-os-plus-iii-posix-arch-interface
)
message (
  VERBOSE
  "> micro-os-plus::architecture -> micro-os-plus-iii-posix-arch-interface"
)

# -----------------------------------------------------------------------------
