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
  add_cross_test_executable (rtos-apis-test)
  target_link_cross_tests_libraries (rtos-apis-test test::rtos-apis)

  # Portable dependencies.
  target_link_libraries (rtos-apis-test PRIVATE xpacks::chan-fatfs)

  target_compile_definitions (
    rtos-apis-test PRIVATE OS_USE_TRACE_SEMIHOSTING_STDOUT
  )

  xpack_display_target_lists (rtos-apis-test)
  message (VERBOSE "A> rtos-apis-test")

  add_test (
    NAME "rtos-apis-test"
    COMMAND
      openocd${extension} -c "gdb port disabled" -c "tcl port disabled" -c
      "telnet port disabled" -f interface/stlink-dap.cfg -f target/stm32f7x.cfg
      -c "program rtos-apis-test.elf verify" -c "arm semihosting enable" -c
      "arm semihosting_cmdline rtos-apis-test" -c "reset"
  )

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------

if (XPACK_ENABLE_UNIT_TEST)
  add_cross_test_executable (unit-test)
  target_link_cross_tests_libraries (unit-test test::unit)

  target_compile_definitions (unit-test PRIVATE OS_USE_TRACE_SEMIHOSTING_STDOUT)

  xpack_display_target_lists (unit-test)
  message (VERBOSE "A> unit-test")

  add_test (
    NAME "unit-test"
    COMMAND
      openocd${extension} -c "gdb port disabled" -c "tcl port disabled" -c
      "telnet port disabled" -f interface/stlink-dap.cfg -f target/stm32f7x.cfg
      -c "program unit-test.elf verify" -c "arm semihosting enable" -c
      "arm semihosting_cmdline unit-test" -c "reset"
  )

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------

if (XPACK_ENABLE_MUTEX_STRESS_TEST)
  add_cross_test_executable (mutex-stress-test)
  target_link_cross_tests_libraries (mutex-stress-test test::mutex-stress)

  target_compile_definitions (
    mutex-stress-test PRIVATE OS_USE_TRACE_SEMIHOSTING_STDOUT
  )

  xpack_display_target_lists (mutex-stress-test)
  message (VERBOSE "A> mutex-stress-test")

  add_test (
    NAME "mutex-stress-test"
    COMMAND
      openocd${extension} -c "gdb port disabled" -c "tcl port disabled" -c
      "telnet port disabled" -f interface/stlink-dap.cfg -f target/stm32f7x.cfg
      -c "program mutex-stress-test.elf verify" -c "arm semihosting enable" -c
      "arm semihosting_cmdline mutex-stress-test" -c "reset"
  )

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------

if (XPACK_ENABLE_CMSIS_OS_VALIDATOR_TEST)
  add_cross_test_executable (cmsis-os-validator-test)
  target_link_cross_tests_libraries (
    cmsis-os-validator-test test::cmsis-os-validator
  )

  # Portable dependencies.
  target_link_libraries (
    cmsis-os-validator-test PRIVATE xpacks::arm-cmsis-os-validator
  )

  target_compile_definitions (
    cmsis-os-validator-test PRIVATE OS_USE_TRACE_SEMIHOSTING_STDOUT
  )

  xpack_display_target_lists (cmsis-os-validator-test)
  message (VERBOSE "A> cmsis-os-validator-test")

  add_test (
    NAME "cmsis-os-validator-test"
    COMMAND
      openocd${extension} -c "gdb port disabled" -c "tcl port disabled" -c
      "telnet port disabled" -f interface/stlink-dap.cfg -f target/stm32f7x.cfg
      -c "program cmsis-os-validator-test.elf verify" -c
      "arm semihosting enable" -c
      "arm semihosting_cmdline cmsis-os-validator-test" -c "reset"
  )

  message (VERBOSE "") # Separator for readability
endif ()

# -----------------------------------------------------------------------------
