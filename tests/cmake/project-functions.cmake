# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# build-helper/templates/common/_micro-os-plus/tests/cmake/project-functions-liquid.cmake
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

# This file defines project specific functions used by the platforms.

# -----------------------------------------------------------------------------

message (VERBOSE "Including 'tests/cmake/project-functions.cmake'...")

# -----------------------------------------------------------------------------

# `xpack_dependencies_libraries` is defined in each platform
# `cmake/dependencies-libraries.cmake` file.

function (target_link_native_test_libraries name)
  target_link_libraries (
    ${name}
    PRIVATE # The compile & link options common to all platforms.
            micro-os-plus::common-options
            # Library with the current test.
            ${ARGN}
            ${xpack_dependencies_libraries}
            # Platform dependency.
            micro-os-plus::platform # bring device & architecture too
  )

  if (NOT ARGN)
    message (
      FATAL_ERROR
        "target_link_native_test_libraries: at least one library required"
    )
  endif ()
endfunction ()

# -----------------------------------------------------------------------------

# `xpack_dependencies_libraries` is defined in each platform
# `cmake/dependencies-libraries.cmake` file.

function (target_link_cross_tests_libraries name)
  target_link_libraries (
    ${name}
    PRIVATE # The compile & link options common to all platforms.
            micro-os-plus::common-options
            # Library with the current test.
            ${ARGN}
            ${xpack_dependencies_libraries}
            # Platform specific dependencies.
            micro-os-plus::platform # bring device & architecture too
  )

  if (NOT ARGN)
    message (
      FATAL_ERROR
        "target_link_cross_tests_libraries: at least one library required"
    )
  endif ()
endfunction ()

# -----------------------------------------------------------------------------
