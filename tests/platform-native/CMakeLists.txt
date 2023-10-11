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

# This file creates the platform specific test executables.

message(VERBOSE "Processing 'tests/platform-native'...")

# -----------------------------------------------------------------------------

add_subdirectory("platform")

# -----------------------------------------------------------------------------

if (CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")

  # On non-Windows, get the actual libraries paths by asking the compiler.
  execute_process(
    COMMAND "${CMAKE_SOURCE_DIR}/xpacks/@micro-os-plus/build-helper/dev-scripts/get-libraries-paths.sh" ${CMAKE_CXX_COMPILER}
    OUTPUT_VARIABLE cxx_library_path
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  string(REPLACE ":" ";" cxx_library_path_list ${cxx_library_path})

  set(rpath_options_list)
  foreach(RPATH IN LISTS cxx_library_path_list)
    cmake_path(SET normalized_path NORMALIZE ${RPATH})
    list(APPEND rpath_options_list "-Wl,-rpath,${normalized_path}")
  endforeach()

  cmake_path(GET CMAKE_CXX_COMPILER FILENAME cxx_filename)
  message(STATUS "${cxx_filename} RPATH_LIST: ${rpath_options_list}")

endif()

# -----------------------------------------------------------------------------

if (ENABLE_RTOS_APIS_TEST)

  add_executable(rtos-apis-test)
  set_target_properties(rtos-apis-test PROPERTIES OUTPUT_NAME "rtos-apis-test")

  target_compile_definitions(rtos-apis-test PRIVATE
    # Use buffered write with caution, it occasionally hangs.
    # OS_USE_TRACE_POSIX_FWRITE_STDOUT
    OS_USE_TRACE_POSIX_STDOUT
  )

  # The compile options were defined globally.
  target_compile_options(rtos-apis-test PRIVATE
    # None.
  )

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html
  target_link_options(rtos-apis-test PRIVATE
    $<$<PLATFORM_ID:Linux,Windows>:-Wl,-Map,platform-bin/rtos-apis-test-map.txt>
    $<$<PLATFORM_ID:Linux,Darwin>:${rpath_options_list}>
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

  message(VERBOSE "A> rtos-apis-test")

  add_test(
    NAME "rtos-apis-test"
    COMMAND rtos-apis-test
  )

endif()

# -----------------------------------------------------------------------------

if (ENABLE_MUTEX_STRESS_TEST)

  add_executable(mutex-stress-test)
  set_target_properties(mutex-stress-test PROPERTIES OUTPUT_NAME "mutex-stress-test")

  target_compile_definitions(mutex-stress-test PRIVATE
    # Use buffered write with caution, it occasionally hangs.
    # OS_USE_TRACE_POSIX_FWRITE_STDOUT
    OS_USE_TRACE_POSIX_STDOUT
  )

  # The compile options were defined globally.
  target_compile_options(mutex-stress-test PRIVATE
    # None.
  )

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html
  target_link_options(mutex-stress-test PRIVATE
    $<$<PLATFORM_ID:Linux,Windows>:-Wl,-Map,platform-bin/mutex-stress-test-map.txt>
    $<$<PLATFORM_ID:Linux,Darwin>:${rpath_options_list}>
  )

  target_link_libraries(mutex-stress-test PRIVATE
    # Test library.
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

if (ENABLE_CMSIS_OS_VALIDATOR_TEST)

  add_executable(cmsis-os-validator-test)
  set_target_properties(cmsis-os-validator-test PROPERTIES OUTPUT_NAME "cmsis-os-validator-test")

  target_compile_definitions(cmsis-os-validator-test PRIVATE
    # Use buffered write with caution, it occasionally hangs.
    # OS_USE_TRACE_POSIX_FWRITE_STDOUT
    OS_USE_TRACE_POSIX_STDOUT
  )

  # The compile options were defined globally.
  target_compile_options(cmsis-os-validator-test PRIVATE
    # None.
  )

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html
  target_link_options(cmsis-os-validator-test PRIVATE
    $<$<PLATFORM_ID:Linux,Windows>:-Wl,-Map,platform-bin/cmsis-os-validator-test-map.txt>
    $<$<PLATFORM_ID:Linux,Darwin>:${rpath_options_list}>
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

  message(VERBOSE "A> cmsis-os-validator-test")

  add_test(
    NAME "cmsis-os-validator-test"
    COMMAND cmsis-os-validator-test
  )

endif()

# -----------------------------------------------------------------------------
