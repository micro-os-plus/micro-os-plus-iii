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

# This file creates the platform specific test executables.

message(VERBOSE "Processing 'tests/platforms/native'...")

# -----------------------------------------------------------------------------

# set(xpack_create_listing true)

# -----------------------------------------------------------------------------
include("cmake/platform-library.cmake")

# -----------------------------------------------------------------------------
function(add_test_executable name)
  add_executable(${name})

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html
  # The link options were defined in `platform-native-interface`.
  target_link_options(${name} PRIVATE
    $<$<PLATFORM_ID:Linux,Windows>:-Wl,-Map,platform-bin/${name}-map.txt>
    $<$<PLATFORM_ID:Linux,Darwin>:${rpath_options_list}>

    # -v
  )

  if(xpack_create_listing)
    add_custom_command(TARGET ${name} POST_BUILD

      # --all-headers -> Invalid/Unsupported object file format
      COMMAND ${CMAKE_OBJDUMP} --source --demangle --line-numbers --wide "$<TARGET_FILE:${name}>" > ${name}-list.txt
      VERBATIM
    )
  endif()
endfunction()

# -----------------------------------------------------------------------------
if(ENABLE_RTOS_APIS_TEST)
  add_test_executable(rtos-apis-test)

  target_compile_definitions(rtos-apis-test PRIVATE

    # Use buffered write with caution, it occasionally hangs.
    # OS_USE_TRACE_POSIX_FWRITE_STDOUT
    OS_USE_TRACE_POSIX_STDOUT
  )

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
    COMMAND rtos-apis-test
  )
endif()

# -----------------------------------------------------------------------------
if(ENABLE_MUTEX_STRESS_TEST)
  add_test_executable(mutex-stress-test)

  target_compile_definitions(mutex-stress-test PRIVATE

    # Use buffered write with caution, it occasionally hangs.
    # OS_USE_TRACE_POSIX_FWRITE_STDOUT
    OS_USE_TRACE_POSIX_STDOUT
  )

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
    COMMAND mutex-stress-test
  )
endif()

# -----------------------------------------------------------------------------
if(ENABLE_CMSIS_OS_VALIDATOR_TEST)
  add_test_executable(cmsis-os-validator-test)

  target_compile_definitions(cmsis-os-validator-test PRIVATE

    # Use buffered write with caution, it occasionally hangs.
    # OS_USE_TRACE_POSIX_FWRITE_STDOUT
    OS_USE_TRACE_POSIX_STDOUT
  )

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html
  target_link_options(cmsis-os-validator-test PRIVATE
    $<$<PLATFORM_ID:Linux,Windows>:-Wl,-Map,platform-bin/cmsis-os-validator-test-map.txt>
    $<$<PLATFORM_ID:Linux,Darwin>:${rpath_options_list}>
  )

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
    COMMAND cmsis-os-validator-test
  )
endif()

# -----------------------------------------------------------------------------
