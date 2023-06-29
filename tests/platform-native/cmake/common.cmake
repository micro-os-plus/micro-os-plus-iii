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

# This file defines the global compiler settings that apply to all targets.
# Must be added with `include()` in the `tests` scope.

message(VERBOSE "Including platform-native common definitions...")

# -----------------------------------------------------------------------------

set(xpack_platform_compile_definition "PLATFORM_NATIVE")

# -----------------------------------------------------------------------------
