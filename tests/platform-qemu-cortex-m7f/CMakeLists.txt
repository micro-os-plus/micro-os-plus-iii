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

message(VERBOSE "Processing 'tests/platform-qemu-cortex-m7f'...")

# -----------------------------------------------------------------------------

if(NOT DEFINED xpack_platform_compile_definition)
  message(FATAL_ERROR "Define xpack_platform_compile_definition in platform*/cmake/globals.cmake")
endif()

# set(xpack_create_listing true)
# set(xpack_create_hex true)

# -----------------------------------------------------------------------------
# Define the platform library.

add_library(platform-qemu-cortex-m7f-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(platform-qemu-cortex-m7f-interface INTERFACE
  "include"
)

target_sources(platform-qemu-cortex-m7f-interface INTERFACE
  # None.
)

target_compile_definitions(platform-qemu-cortex-m7f-interface INTERFACE
  "${xpack_platform_compile_definition}"
)

target_compile_options(platform-qemu-cortex-m7f-interface INTERFACE
  # None.
)

target_link_libraries(platform-qemu-cortex-m7f-interface INTERFACE
  micro-os-plus::iii-cortexm
  micro-os-plus::device
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(platform-qemu-cortex-m7f-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

add_library(micro-os-plus::platform ALIAS platform-qemu-cortex-m7f-interface)
message(VERBOSE "> micro-os-plus::platform -> platform-qemu-cortex-m7f-interface")

# =============================================================================
# Define the tests executables.

if("${CMAKE_HOST_SYSTEM_NAME}" STREQUAL "Windows")
  set(extension ".cmd")
endif()

# -----------------------------------------------------------------------------

if (ENABLE_RTOS_APIS_TEST)

  add_executable(rtos-apis-test)
  set_target_properties(rtos-apis-test PROPERTIES OUTPUT_NAME "rtos-apis-test")

  target_compile_definitions(rtos-apis-test PRIVATE
    OS_USE_SEMIHOSTING_SYSCALLS
    OS_USE_TRACE_SEMIHOSTING_STDOUT
  )

  # The compile options were defined globally.
  target_compile_options(rtos-apis-test PRIVATE
    # None.
  )

  target_link_options(rtos-apis-test PRIVATE
    -Wl,-Map,platform-bin/rtos-apis-test-map.txt
  )

  target_link_libraries(rtos-apis-test PRIVATE
    # Test library.
    test::rtos-apis

    # Tested library.
    micro-os-plus::iii

    # Portable dependencies.
    xpacks::chan-fatfs

    # Platform specific dependencies.
    micro-os-plus::platform
  )

  xpack_add_cross_custom_commands("rtos-apis-test")

  message(VERBOSE "A> rtos-apis-test")

  add_test(
    NAME "rtos-apis-test"

    COMMAND qemu-system-arm${extension}
      --machine mps2-an500
      --cpu cortex-m7
      --kernel rtos-apis-test.elf
      --nographic
      -d unimp,guest_errors
      --semihosting-config enable=on,target=native,arg=rtos-apis-test
      # --semihosting-config arg=--verbose
    )

endif ()

# -----------------------------------------------------------------------------

if (ENABLE_MUTEX_STRESS_TEST)

  add_executable(mutex-stress-test)
  set_target_properties(mutex-stress-test PROPERTIES OUTPUT_NAME "mutex-stress-test")

  target_compile_definitions(mutex-stress-test PRIVATE
    OS_USE_SEMIHOSTING_SYSCALLS
    OS_USE_TRACE_SEMIHOSTING_STDOUT
  )

  # The compile options were defined globally.
  target_compile_options(mutex-stress-test PRIVATE
    # None.
  )

  target_link_options(mutex-stress-test PRIVATE
    -Wl,-Map,platform-bin/mutex-stress-test-map.txt
  )

  target_link_libraries(mutex-stress-test PRIVATE
    # Test library.
    test::mutex-stress

    # Tested library.
    micro-os-plus::iii

    # Platform specific dependencies.
    micro-os-plus::platform
  )

  xpack_add_cross_custom_commands("mutex-stress-test")

  message(VERBOSE "A> mutex-stress-test")

  add_test(
    NAME "mutex-stress-test"

    COMMAND qemu-system-arm${extension}
      --machine mps2-an500
      --cpu cortex-m7
      --kernel mutex-stress-test.elf
      --nographic
      -d unimp,guest_errors
      --semihosting-config enable=on,target=native,arg=mutex-stress-test
      # --semihosting-config arg=--verbose
    )

endif ()

# -----------------------------------------------------------------------------


if (ENABLE_CMSIS_OS_VALIDATOR_TEST)

  add_executable(cmsis-os-validator-test)
  set_target_properties(cmsis-os-validator-test PROPERTIES OUTPUT_NAME "cmsis-os-validator-test")

  target_compile_definitions(cmsis-os-validator-test PRIVATE
    OS_USE_SEMIHOSTING_SYSCALLS
    OS_USE_TRACE_SEMIHOSTING_STDOUT
  )

  # The compile options were defined globally.
  target_compile_options(cmsis-os-validator-test PRIVATE
    # None.
  )

  target_link_options(cmsis-os-validator-test PRIVATE
    -Wl,-Map,platform-bin/cmsis-os-validator-test-map.txt
  )

  target_link_libraries(cmsis-os-validator-test PRIVATE
    # Test library.
    test::cmsis-os-validator

    # Tested library.
    micro-os-plus::iii

    # Portable dependencies.
    xpacks::arm-cmsis-os-validator

    # Platform specific dependencies.
    micro-os-plus::platform
  )

  xpack_add_cross_custom_commands("cmsis-os-validator-test")

  message(VERBOSE "A> cmsis-os-validator-test")

  add_test(
    NAME "cmsis-os-validator-test"

    COMMAND qemu-system-arm${extension}
      --machine mps2-an500
      --cpu cortex-m7
      --kernel cmsis-os-validator-test.elf
      --nographic
      -d unimp,guest_errors
      --semihosting-config enable=on,target=native,arg=cmsis-os-validator-test
      # --semihosting-config arg=--verbose
    )

endif ()

# -----------------------------------------------------------------------------
