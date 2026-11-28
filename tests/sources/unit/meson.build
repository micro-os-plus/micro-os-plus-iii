# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# sources/unit/xcdl-test.jsonc
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

# This file is intended to be used by applications via:
#
# `subdir('sources/unit')`
#
# This creates dependencies that can be linked to a target with:
#
# `dependencies: [test_unit_dependency],`

# Note: the Meson configuration is provided only as a proof of concept;
# for production it might need some refinements.

# -----------------------------------------------------------------------------

message('Processing \'tests/sources/unit/\'...')

# -----------------------------------------------------------------------------

# Meson's DSL does not provide introspection methods on dependency objects
# to retrieve their properties after creation. The `_local_*` variables
# therefore serve a dual purpose: populating the dependency object and
# iterating over its components for the `message()` calls at the end.

# -----------------------------------------------------------------------------

# The source code for the unit tests.

_local_compile_args = [] # Common C/C++ args.
_local_compile_c_args = [] # C only args.
_local_compile_cpp_args = [] # C++ only args.
_local_include_directories = []
_local_sources = []
_local_compile_definitions = []
_local_dependencies = []
_local_link_args = []
_local_link_with = []

_local_include_directories += [
  'include',
]

_local_sources += [
  'src/main.cpp',
  'src/test-c-api.c',
  'src/test-memory.cpp',
  'src/test-rtos.cpp',
]

# The dependencies are single objects; enclose them in brackets,
# to append them as elements of the list.
_local_dependencies += [
  micro_os_plus_iii_dependency,
]

# The dependencies' C/C++ args are lists; add them without brackets,
# to concatenate their elements rather than nest the lists.
_local_compile_c_args += micro_os_plus_iii_dependency_compile_c_args
_local_compile_cpp_args += micro_os_plus_iii_dependency_compile_cpp_args

# https://mesonbuild.com/Reference-manual_functions.html#declare_dependency
test_unit_dependency = declare_dependency(
  include_directories: include_directories(_local_include_directories),
  compile_args: _local_compile_args + _local_compile_definitions,
  sources: files(_local_sources),
  dependencies: _local_dependencies,
  link_args: _local_link_args,
  link_with: _local_link_with,
)

# Meson dependencies cannot differentiate between C and C++ compile args;
# they are therefore exposed as separate variables alongside the dependency.
test_unit_dependency_compile_c_args = _local_compile_c_args
test_unit_dependency_compile_cpp_args = _local_compile_cpp_args

foreach name : _local_include_directories
  message('+ -I ' + name)
endforeach
foreach name : _local_sources + _local_compile_definitions + _local_compile_args + _local_compile_c_args + _local_compile_cpp_args + _local_link_args
  message('+ ' + name)
endforeach
message('> test_unit_dependency')

# -----------------------------------------------------------------------------
