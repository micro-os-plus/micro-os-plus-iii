# -----------------------------------------------------------------------------
#
# This file is part of the µOS++ distribution.
#   (https://github.com/micro-os-plus/)
# Copyright (c) 2023 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software
# for any purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can
# be obtained from https://opensource.org/licenses/mit/.
#
# -----------------------------------------------------------------------------

message(VERBOSE "Processing 'tests/platform-nucleo-f767zi/platform'...")

# -----------------------------------------------------------------------------

if(NOT DEFINED xpack_platform_compile_definition)
  message(FATAL_ERROR "Define xpack_platform_compile_definition in platform*/cmake/globals.cmake")
endif()

# -----------------------------------------------------------------------------
# Define the platform library.

add_library(platform-nucleo-f767zi-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(platform-nucleo-f767zi-interface INTERFACE
  "include"
)

target_sources(platform-nucleo-f767zi-interface INTERFACE
  # None
)

target_compile_definitions(platform-nucleo-f767zi-interface INTERFACE
  "${xpack_platform_compile_definition}"
  # _POSIX_C_SOURCE=200809L
  # For S_IREAD
  _GNU_SOURCE
)

# Global definitions.
# add_compile_definitions()
# include_directories()

set(xpack_platform_common_args
  -mcpu=cortex-m7
  -mthumb
  # -mfloat-abi=soft
  -mfloat-abi=hard

  # -fno-move-loop-invariants

  # Embedded builds gnerally must be warning free, but PICO SDK does not allow this.
  # -Werror

  # -flto fails to run on QEMU.
  # $<$<CONFIG:Release>:-flto>
  # $<$<CONFIG:MinSizeRel>:-flto>

  $<$<CONFIG:Debug>:-fno-omit-frame-pointer>

  # ... libs-c/src/stdlib/exit.c:132:46
  # $<$<CXX_COMPILER_ID:GNU>:-Wno-missing-attributes>

  # $<$<COMPILE_LANGUAGE:C>:-fxxx>

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html?highlight=compile_language#genex:COMPILE_LANGUAGE
  # $<$<COMPILE_LANGUAGE:CXX>:-fno-exceptions>
  # $<$<COMPILE_LANGUAGE:CXX>:-fno-rtti>
  # $<$<COMPILE_LANGUAGE:CXX>:-fno-use-cxa-atexit>
  $<$<COMPILE_LANGUAGE:CXX>:-fno-threadsafe-statics>
)

target_compile_options(platform-nucleo-f767zi-interface INTERFACE
  ${xpack_platform_common_args}
)

# When `-flto` is used, the compile options must be passed to the linker too.
target_link_options(platform-nucleo-f767zi-interface INTERFACE
  -v
  ${xpack_platform_common_args}

  -nostartfiles
  # --specs=rdimon.specs -Wl,--start-group -lgcc -lc -lc -lm -lrdimon -Wl,--end-group

  # Force the linker to keep the interrupt vectors which otherwise
  # are not referred from anywhere.
  # -u_interrupt_vectors

  # nano has no exceptions.
  # -specs=nano.specs

  -Wl,--gc-sections

  -Wl,--no-warn-rwx-segment

  # Including files from other packages is not very nice, but functional.
  # Use absolute paths, otherwise set -L.
  # -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/memmap_default.ld
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/mem.ld
  -T${CMAKE_CURRENT_SOURCE_DIR}/linker-scripts/sections-flash.ld
)

target_link_libraries(platform-nucleo-f767zi-interface INTERFACE
  micro-os-plus::common
  micro-os-plus::iii-cortexm
  micro-os-plus::device
)

if (COMMAND xpack_display_target_lists)
  xpack_display_target_lists(platform-nucleo-f767zi-interface)
endif()

# -----------------------------------------------------------------------------
# Aliases.

add_library(micro-os-plus::platform ALIAS platform-nucleo-f767zi-interface)
message(VERBOSE "> micro-os-plus::platform -> platform-nucleo-f767zi-interface")

# -----------------------------------------------------------------------------
