# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2022-2025 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
#  obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

message(VERBOSE "Processing 'tests/platforms/qemu-cortex-m3'...")

# -----------------------------------------------------------------------------

# set(xpack_create_listing true)
# set(xpack_create_hex true)

# -----------------------------------------------------------------------------
include("cmake/platform-library.cmake")

# -----------------------------------------------------------------------------

# Define the tests executables.
function(add_test_executable name)
  add_executable(${name})

  set_target_properties(${name}

    PROPERTIES
    OUTPUT_NAME "${name}"
  )

  target_compile_definitions(${name} PRIVATE
    OS_USE_TRACE_SEMIHOSTING_STDOUT
  )

  # No other common compile or link options, all were defined in the
  # libraries and will be inherited.
  target_link_options(${name} PRIVATE
    -Wl,-Map,platform-bin/${name}-map.txt

    # -v
  )

  xpack_add_cross_custom_commands(${name})
endfunction()

# -----------------------------------------------------------------------------
if(ENABLE_RTOS_APIS_TEST)
  add_test_executable(rtos-apis-test)

  target_link_libraries(rtos-apis-test PRIVATE

    micro-os-plus::common-options

    # Current test library.
    test::rtos-apis

    # Tested library.
    micro-os-plus::iii

    # Portable dependencies.
    xpacks::chan-fatfs

    # Platform specific dependencies.
    micro-os-plus::platform
  )

  message(VERBOSE "A> rtos-apis-test")

  add_test(
    NAME "rtos-apis-test"

    COMMAND qemu-system-arm${extension}
    --machine mps2-an385
    --cpu cortex-m3
    --kernel rtos-apis-test.elf
    --nographic
    -d unimp,guest_errors
    --semihosting-config enable=on,target=native,arg=rtos-apis-test

    # --semihosting-config arg=--verbose
  )
endif()

# -----------------------------------------------------------------------------
if(ENABLE_MUTEX_STRESS_TEST)
  add_test_executable(mutex-stress-test)

  target_link_libraries(mutex-stress-test PRIVATE

    micro-os-plus::common-options

    # Current test library.
    test::mutex-stress

    # Tested library.
    micro-os-plus::iii

    # Platform specific dependencies.
    micro-os-plus::platform
  )

  message(VERBOSE "A> mutex-stress-test")

  add_test(
    NAME "mutex-stress-test"

    COMMAND qemu-system-arm${extension}
    --machine mps2-an385
    --cpu cortex-m3
    --kernel mutex-stress-test.elf
    --nographic
    -d unimp,guest_errors
    --semihosting-config enable=on,target=native,arg=mutex-stress-test

    # --semihosting-config arg=--verbose
  )
endif()

# -----------------------------------------------------------------------------
if(ENABLE_CMSIS_OS_VALIDATOR_TEST)
  add_test_executable(cmsis-os-validator-test)

  target_link_libraries(cmsis-os-validator-test PRIVATE

    micro-os-plus::common-options

    # Current test library.
    test::cmsis-os-validator

    # Tested library.
    micro-os-plus::iii

    # Portable dependencies.
    xpacks::arm-cmsis-os-validator

    # Platform specific dependencies.
    micro-os-plus::platform
  )

  message(VERBOSE "A> cmsis-os-validator-test")

  add_test(
    NAME "cmsis-os-validator-test"

    COMMAND qemu-system-arm${extension}
    --machine mps2-an385
    --cpu cortex-m3
    --kernel cmsis-os-validator-test.elf
    --nographic
    -d unimp,guest_errors
    --semihosting-config enable=on,target=native,arg=cmsis-os-validator-test

    # --semihosting-config arg=--verbose
  )
endif()

# -----------------------------------------------------------------------------
