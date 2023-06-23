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

if(NOT DEFINED xpack_platform_compile_definition)
  message(FATAL_ERROR "Define xpack_platform_compile_definition in platform*/cmake/globals.cmake")
endif()

# -----------------------------------------------------------------------------
# Define the platform library.

add_library(platform-native-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(platform-native-interface INTERFACE
  # None
)

target_sources(platform-native-interface INTERFACE
  # None.
)

target_compile_definitions(platform-native-interface INTERFACE
  "${xpack_platform_compile_definition}"
)

target_compile_options(platform-native-interface INTERFACE
  # None.
)

target_link_libraries(platform-native-interface INTERFACE
  micro-os-plus::iii-posix-arch
  xpack-3rd-party::libucontext
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(platform-native-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

add_library(micro-os-plus::platform ALIAS platform-native-interface)
message(VERBOSE "> micro-os-plus::platform -> platform-native-interface")

# =============================================================================

if (CMAKE_SYSTEM_NAME STREQUAL "Linux")

  # On Lnux, get the actual libraries paths by asking the compiler.
  execute_process(
    COMMAND "${CMAKE_CURRENT_SOURCE_DIR}/scripts/get-libraries-paths.sh" ${CMAKE_CXX_COMPILER}
    OUTPUT_VARIABLE RPATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message(VERBOSE "RPATH=${RPATH}")

  add_link_options (
    -Wl,-rpath=${RPATH}
  )

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
