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

# Platform specific definitions.

# -----------------------------------------------------------------------------

message(VERBOSE "Including tests/platforms/${PLATFORM_NAME}/cmake/definitions.cmake...")

# -----------------------------------------------------------------------------

# Required in devices-qemu-cortexm.
set(xpack_device_compile_definition "MICRO_OS_PLUS_DEVICE_QEMU_CORTEX_M3")

set(xpack_platform_compile_definition "MICRO_OS_PLUS_PLATFORM_QEMU_CORTEX_M3")

set(xpack_device_linker_script_file_name "mem-mps2-an385.ld")

# -----------------------------------------------------------------------------
