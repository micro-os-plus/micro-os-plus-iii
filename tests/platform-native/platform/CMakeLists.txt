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

message(VERBOSE "Processing 'tests/platform-native/platform'...")

# -----------------------------------------------------------------------------

if(NOT DEFINED xpack_platform_compile_definition)
  message(FATAL_ERROR "Define xpack_platform_compile_definition in platform*/cmake/common.cmake")
endif()

# -----------------------------------------------------------------------------
# Define the platform library.

add_library(platform-native-interface INTERFACE EXCLUDE_FROM_ALL)

# -----------------------------------------------------------------------------

target_include_directories(platform-native-interface INTERFACE
  "include"
)

target_sources(platform-native-interface INTERFACE
  # None.
)

target_compile_definitions(platform-native-interface INTERFACE
  "${xpack_platform_compile_definition}"
  # $<$<NOT:$<C_COMPILER_ID:Clang,AppleClang>>:_POSIX_C_SOURCE=200809L>
  _POSIX_C_SOURCE=200809L
)

set(xpack_platform_common_args
  -Werror
  # Apple clang 13 does not support -Wunused-but-set-variable
  # $<$<PLATFORM_ID:Darwin>:-Wno-unknown-warning-option>
  $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-unknown-warning-option>
  $<$<PLATFORM_ID:Darwin>:-Wno-missing-include-dirs>
  # -g
)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Windows")
  list(APPEND xpack_platform_common_args
    $<$<C_COMPILER_ID:Clang,AppleClang>:-Wno-used-but-marked-unused>
  )
endif()

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang")
  # https://libcxx.llvm.org/UsingLibcxx.html
  list(APPEND xpack_platform_common_args
    $<$<COMPILE_LANGUAGE:CXX>:-stdlib=libc++>
  )
endif()

# https://cmake.org/cmake/help/v3.20/variable/CMAKE_LANG_COMPILER_ID.html
# message("${CMAKE_C_COMPILER_ID} ${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_PROCESSOR}")
# Unfortunatelly in a container it shows aarch64 instead of armv7l.

# -flto seems ok now with clang too, but on Linux it requires -fuse-ld=lld
list(APPEND xpack_platform_common_args
  $<$<CONFIG:Release,MinSizeRel>:-flto>
)

# Possible workaround for the bug in recent CLT for Ventura 13.6.
# However it is preferable to downgrade to CLT 14.3.1.
# https://download.developer.apple.com/Developer_Tools/Command_Line_Tools_for_Xcode_14.3.1/Command_Line_Tools_for_Xcode_14.3.1.dmg

# 0  0x104d87f43  __assert_rtn + 64
# 1  0x104c89f43  ld::AtomPlacement::findAtom(unsigned char, unsigned long long, ld::AtomPlacement::AtomLoc const*&, long long&) const + 1411
# 2  0x104ca6431  ld::InputFiles::SliceParser::parseObjectFile(mach_o::Header const*) const + 19745
# 3  0x104cb6b71  ld::InputFiles::parseAllFiles(void (ld::AtomFile const*) block_pointer)::$_7::operator()(unsigned long, ld::FileInfo const&) const + 657
# 4  0x7ff806725066  _dispatch_client_callout2 + 8
# 5  0x7ff806736e09  _dispatch_apply_invoke + 213
# 6  0x7ff806725033  _dispatch_client_callout + 8
# 7  0x7ff8067350f6  _dispatch_root_queue_drain + 683
# 8  0x7ff806735768  _dispatch_worker_thread2 + 170
# 9  0x7ff8068c2c0f  _pthread_wqthread + 257
# ld: Assertion failed: (resultIndex < sectData.atoms.size()), function findAtom, file Relocations.cpp, line 1336.
# collect2: error: ld returned 1 exit status

list(APPEND xpack_platform_common_args
  # $<$<AND:$<C_COMPILER_ID:GNU>,$<PLATFORM_ID:Darwin>>:-no-pie>
)

# With clang 12 & CLT 14.3.1:
# /Library/Developer/CommandLineTools/SDKs/MacOSX14.0.sdk/usr/include/c++/v1/cstdlib:144:9: error: no member named 'at_quick_exit' in the global namespace
# using ::at_quick_exit _LIBCPP_USING_IF_EXISTS;
#       ~~^
# /Library/Developer/CommandLineTools/SDKs/MacOSX14.0.sdk/usr/include/c++/v1/cstdlib:145:9: error: no member named 'quick_exit' in the global namespace
# using ::quick_exit _LIBCPP_USING_IF_EXISTS;
#       ~~^
# 2 errors generated.

target_compile_options(platform-native-interface INTERFACE
  ${xpack_platform_common_args}
)

# When `-flto` is used, the compile options must be passed to the linker too.
target_link_options(platform-native-interface INTERFACE
  -v
  ${xpack_platform_common_args}

  $<$<PLATFORM_ID:Windows>:-static>

  # On Linux no need for statics, -rpath is fine.
  # On macOS, GCC 11 gets confused
  # dyld[72401]: Symbol not found: (__ZNKSt3_V214error_category10_M_messageB5cxx11Ei)
  $<$<AND:$<C_COMPILER_ID:GNU>,$<PLATFORM_ID:Darwin>>:-static-libgcc>
  $<$<AND:$<C_COMPILER_ID:GNU>,$<PLATFORM_ID:Darwin>>:-static-libstdc++>

  $<$<PLATFORM_ID:Darwin>:-Wl,-dead_strip>
  $<$<PLATFORM_ID:Linux,Windows>:-Wl,--gc-sections>
)

if("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang" )
  # https://clang.llvm.org/docs/Toolchain.html#compiler-runtime
  target_link_options (platform-native-interface INTERFACE

    $<$<AND:$<PLATFORM_ID:Linux>,$<COMPILE_LANGUAGE:CXX>>:-stdlib=libc++>
    $<$<PLATFORM_ID:Linux>:-rtlib=compiler-rt>
    $<$<PLATFORM_ID:Linux>:-lunwind>
    $<$<PLATFORM_ID:Linux>:-fuse-ld=lld>
  )
endif()

target_link_libraries(platform-native-interface INTERFACE
  micro-os-plus::common
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

# -----------------------------------------------------------------------------
