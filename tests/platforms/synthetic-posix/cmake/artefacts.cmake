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

# Create the platform specific test executables.

# -----------------------------------------------------------------------------

message (VERBOSE
         "Including 'tests/platforms/${PLATFORM_NAME}/cmake/artefacts.cmake'..."
)

# -----------------------------------------------------------------------------

if (XPACK_ENABLE_RTOS_APIS_TEST)
  add_native_test_executable (rtos-apis-test)
  target_link_native_test_libraries (rtos-apis-test test::rtos-apis)

  # Portable dependencies.
  target_link_libraries (rtos-apis-test PRIVATE xpacks::chan-fatfs)

  target_compile_definitions (
    rtos-apis-test
    PRIVATE # Use buffered write with caution, it occasionally hangs.
            # OS_USE_TRACE_POSIX_FWRITE_STDOUT
            OS_USE_TRACE_POSIX_STDOUT
  )

  xpack_display_target_lists (rtos-apis-test)
  message (VERBOSE "A> rtos-apis-test")

  add_test (NAME "rtos-apis-test" COMMAND rtos-apis-test)
  message (VERBOSE "T> rtos-apis-test")

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------

if (XPACK_ENABLE_UNIT_TEST)
  add_native_test_executable (unit-test)
  target_link_native_test_libraries (unit-test test::unit)

  target_compile_definitions (
    unit-test
    PRIVATE # Use buffered write with caution, it occasionally hangs.
            # OS_USE_TRACE_POSIX_FWRITE_STDOUT
            OS_USE_TRACE_POSIX_STDOUT
  )

  xpack_display_target_lists (unit-test)
  message (VERBOSE "A> unit-test")

  add_test (NAME "unit-test" COMMAND unit-test)
  message (VERBOSE "T> unit-test")

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------

if (XPACK_ENABLE_MUTEX_STRESS_TEST)
  add_native_test_executable (mutex-stress-test)
  target_link_native_test_libraries (mutex-stress-test test::mutex-stress)

  target_compile_definitions (
    mutex-stress-test
    PRIVATE # Use buffered write with caution, it occasionally hangs.
            # OS_USE_TRACE_POSIX_FWRITE_STDOUT
            OS_USE_TRACE_POSIX_STDOUT
  )

  xpack_display_target_lists (mutex-stress-test)
  message (VERBOSE "A> mutex-stress-test")

  add_test (NAME "mutex-stress-test" COMMAND mutex-stress-test)
  message (VERBOSE "T> mutex-stress-test")

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------

if (XPACK_ENABLE_CMSIS_OS_VALIDATOR_TEST)
  add_native_test_executable (cmsis-os-validator-test)
  target_link_native_test_libraries (
    cmsis-os-validator-test test::cmsis-os-validator
  )

  # Portable dependencies.
  target_link_libraries (
    cmsis-os-validator-test PRIVATE xpacks::arm-cmsis-os-validator
  )

  target_compile_definitions (
    cmsis-os-validator-test
    PRIVATE # Use buffered write with caution, it occasionally hangs.
            # OS_USE_TRACE_POSIX_FWRITE_STDOUT
            OS_USE_TRACE_POSIX_STDOUT
  )

  xpack_display_target_lists (cmsis-os-validator-test)
  message (VERBOSE "A> cmsis-os-validator-test")

  add_test (NAME "cmsis-os-validator-test" COMMAND cmsis-os-validator-test)
  message (VERBOSE "T> cmsis-os-validator-test")

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------
