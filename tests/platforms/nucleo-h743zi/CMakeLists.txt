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

message (VERBOSE "Processing 'tests/platforms/nucleo-h743zi'...")

# -----------------------------------------------------------------------------

# set(xpack_create_listing true) set(xpack_create_hex true)

# -----------------------------------------------------------------------------
add_subdirectory ("stm32h743cubemx")

# TODO: update to separate device-stm32h7-extras, when ready.
add_subdirectory ("device")

include ("cmake/platform-library.cmake")

# -----------------------------------------------------------------------------

# Define the tests executables.
function (add_test_executable name)
  add_executable (${name})

  set_target_properties (${name} PROPERTIES OUTPUT_NAME "${name}")

  target_compile_definitions (${name} PRIVATE OS_USE_TRACE_SEMIHOSTING_STDOUT)

  # No other common compile or link options, all were defined in the libraries
  # and will be inherited.
  target_link_options (
    ${name} PRIVATE -Wl,-Map,platform-bin/${name}-map.txt # -v
  )

  xpack_add_cross_custom_commands (${name})
endfunction ()

# -----------------------------------------------------------------------------
if (ENABLE_RTOS_APIS_TEST)
  add_test_executable (rtos-apis-test)

  target_link_libraries (
    rtos-apis-test
    PRIVATE micro-os-plus::common-options
            # Current test library.
            test::rtos-apis
            # Tested library.
            micro-os-plus::iii
            # Portable dependencies.
            xpacks::chan-fatfs
            # Platform specific dependencies.
            micro-os-plus::platform
  )

  message (VERBOSE "A> rtos-apis-test")

  add_test (
    NAME "rtos-apis-test"
    COMMAND
      openocd${extension}
      # -d3
      -c "gdb port disabled" -c "tcl port disabled" -c "telnet port disabled"
      -f interface/stlink-dap.cfg -f target/stm32h7x.cfg -c
      "program rtos-apis-test.elf verify" -c "arm semihosting enable" -c
      "arm semihosting_cmdline rtos-apis-test" -c "reset"
  )
endif ()

# -----------------------------------------------------------------------------
if (ENABLE_MUTEX_STRESS_TEST)
  add_test_executable (mutex-stress-test)

  target_link_libraries (
    mutex-stress-test
    PRIVATE micro-os-plus::common-options
            # Current test library.
            test::mutex-stress
            # Tested library.
            micro-os-plus::iii
            # Platform specific dependencies.
            micro-os-plus::platform
  )

  message (VERBOSE "A> mutex-stress-test")

  add_test (
    NAME "mutex-stress-test"
    COMMAND
      openocd${extension} -c "gdb port disabled" -c "tcl port disabled" -c
      "telnet port disabled" -f interface/stlink-dap.cfg -f target/stm32h7x.cfg
      -c "program mutex-stress-test.elf verify" -c "arm semihosting enable" -c
      "arm semihosting_cmdline mutex-stress-test" -c "reset"
  )
endif ()

# -----------------------------------------------------------------------------
if (ENABLE_CMSIS_OS_VALIDATOR_TEST)
  add_test_executable (cmsis-os-validator-test)

  target_link_libraries (
    cmsis-os-validator-test
    PRIVATE micro-os-plus::common-options
            # Current test library.
            test::cmsis-os-validator
            # Tested library.
            micro-os-plus::iii
            # Portable dependencies.
            xpacks::arm-cmsis-os-validator
            # Platform specific dependencies.
            micro-os-plus::platform
  )

  message (VERBOSE "A> cmsis-os-validator-test")

  add_test (
    NAME "cmsis-os-validator-test"
    COMMAND
      openocd${extension} -c "gdb port disabled" -c "tcl port disabled" -c
      "telnet port disabled" -f interface/stlink-dap.cfg -f target/stm32h7x.cfg
      -c "program cmsis-os-validator-test.elf verify" -c
      "arm semihosting enable" -c
      "arm semihosting_cmdline cmsis-os-validator-test" -c "reset"
  )
endif ()

# -----------------------------------------------------------------------------
