# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2023-2025 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

# Platform specific definitions.

# -----------------------------------------------------------------------------

message (VERBOSE
         "Including tests/platforms/${PLATFORM_NAME}/cmake/definitions.cmake..."
)

# -----------------------------------------------------------------------------

# Required in devices-qemu-cortexm.
set (xpack_device_compile_definition "MICRO_OS_PLUS_DEVICE_STM32F767XX")

set (xpack_platform_compile_definition "MICRO_OS_PLUS_PLATFORM_NUCLEO_F767ZI")

# -----------------------------------------------------------------------------
