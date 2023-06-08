# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2022 Liviu Ionescu
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/MIT/.
#
# -----------------------------------------------------------------------------

# This file creates the platform specific test executables.

message(VERBOSE "Processing 'tests/platform-native'...")

# -----------------------------------------------------------------------------

set(xpack_platform_compile_definition "PLATFORM_NATIVE")

# -----------------------------------------------------------------------------
# Define the platform library.

add_library(platform-native-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(platform-native-interface INTERFACE
  "include-platform"
)

target_sources(platform-native-interface INTERFACE
  # None.
)

target_compile_definitions(platform-native-interface INTERFACE
  "${xpack_platform_compile_definition}"
  "_XOPEN_SOURCE=700L"
)

target_compile_options(platform-native-interface INTERFACE
  # None.
)

target_link_libraries(platform-native-interface INTERFACE
  micro-os-plus::iii-posix-arch
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
    OUTPUT_VARIABLE LD_LIBRARY_PATH
    OUTPUT_STRIP_TRAILING_WHITESPACE
  )
  message(VERBOSE "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}")

endif()

if (ENABLE_RTOS_APIS_TEST)

  add_executable(rtos-apis-test)
  set_target_properties(rtos-apis-test PROPERTIES OUTPUT_NAME "rtos-apis-test")

  target_compile_definitions(rtos-apis-test PRIVATE
    # None.
  )

  # The compile options were defined globally.
  target_compile_options(rtos-apis-test PRIVATE
    # None.
  )

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html
  target_link_options(rtos-apis-test PRIVATE
    $<$<PLATFORM_ID:Windows>:-static>
    $<$<AND:$<C_COMPILER_ID:GNU>,$<PLATFORM_ID:Darwin>>:-static-libgcc>
    $<$<AND:$<C_COMPILER_ID:GNU>,$<PLATFORM_ID:Darwin>>:-static-libstdc++>
    $<$<PLATFORM_ID:Darwin>:-Wl,-dead_strip>
    $<$<PLATFORM_ID:Linux,Windows>:-Wl,--gc-sections>
    $<$<PLATFORM_ID:Linux,Windows>:-Wl,-Map,platform-bin/${name}-map.txt>
    $<$<AND:$<C_COMPILER_ID:Clang>,$<PLATFORM_ID:Linux>>:-fuse-ld=lld>
    # -v
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

  if(LD_LIBRARY_PATH)
    set_property(
      TEST "rtos-apis-test"
      PROPERTY ENVIRONMENT "LD_LIBRARY_PATH=${LD_LIBRARY_PATH}"
    )
  endif()

endif()

# -----------------------------------------------------------------------------
