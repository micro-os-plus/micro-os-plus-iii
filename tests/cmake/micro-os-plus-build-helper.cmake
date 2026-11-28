# -----------------------------------------------------------------------------
# DO NOT EDIT! Automatically generated from template file:
# build-helper/templates/common/_micro-os-plus/tests/cmake/micro-os-plus-build-helper-liquid.cmake
#
# This file is part of the µOS++ project (https://micro-os-plus.github.io/).
# Copyright (c) 2021-2026 Liviu Ionescu. All rights reserved.
#
# Permission to use, copy, modify, and/or distribute this software for any
# purpose is hereby granted, under the terms of the MIT license.
#
# If a copy of the license was not distributed with this file, it can be
# obtained from https://opensource.org/licenses/mit.
#
# -----------------------------------------------------------------------------

# This file includes various helper functions that may be used in in CMake
# scripts. All functions are prefixed with `xpack_`, used as a namespace.

# -----------------------------------------------------------------------------

message (VERBOSE "Including 'micro-os-plus-build-helper.cmake'...")

# -----------------------------------------------------------------------------

# Called from tests/CMakeLists.txt.
function (xpack_display_greetings)

  if (${ARGC} GREATER_EQUAL 1)
    # If there are arguments, use the first one as path to package.json.
    set (package_json_path "${ARGV0}")
  else ()
    if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/package.json")
      set (package_json_path "${CMAKE_CURRENT_SOURCE_DIR}/package.json")
    elseif (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../package.json")
      set (package_json_path "${CMAKE_CURRENT_SOURCE_DIR}/../package.json")
    else ()
      message (FATAL_ERROR "Path to package.json required")
    endif ()
  endif ()

  message (VERBOSE "CMake version: ${CMAKE_VERSION}")
  message (VERBOSE
           "Compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}"
  )

  xpack_get_package_name_and_version ("${package_json_path}")
  message (VERBOSE "package.name: ${PACKAGE_JSON_NAME}")
  message (VERBOSE "package.version: ${PACKAGE_JSON_VERSION}")

  message (VERBOSE "Platform name: ${PLATFORM_NAME}")
  # CMAKE_BUILD_TYPE: Debug, Release, RelWithDebInfo, MinSizeRel
  message (VERBOSE "Build type: ${CMAKE_BUILD_TYPE}")
  message (VERBOSE "Project path: ${CMAKE_SOURCE_DIR}")
  message (VERBOSE "Build path: ${CMAKE_BINARY_DIR}")
  message (VERBOSE "Module path: ${CMAKE_MODULE_PATH}")

  message (VERBOSE "CMAKE_C_COMPILER_ID: ${CMAKE_C_COMPILER_ID}")
  message (VERBOSE "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
  message (VERBOSE "CMAKE_SYSTEM_PROCESSOR: ${CMAKE_SYSTEM_PROCESSOR}")

endfunction ()

# -----------------------------------------------------------------------------

# DEPRECATED! Use xpack_add_dependencies_subdirectories().
macro (xpack_glob_add_subdirectories from_path base_bin_path)

  # message(VERBOSE "[xpack_glob_add_subdirectories ${from_path}
  # ${base_bin_path}]")
  file (
    GLOB children
    LIST_DIRECTORIES true
    "${from_path}/*"
  )
  # message(VERBOSE "children ${children}")
  foreach (child ${children})
    # message(VERBOSE "${child}")
    if (IS_DIRECTORY "${child}")
      if (EXISTS "${child}/CMakeLists.txt")
        file (RELATIVE_PATH child_relative_path ${CMAKE_SOURCE_DIR} ${child})
        message (VERBOSE "Adding '${child_relative_path}'...")
        get_filename_component (child_folder_name ${child} NAME)
        add_subdirectory ("${child}" "${base_bin_path}/${child_folder_name}")
        list (APPEND XPACK_ADDED_DEPENDENCIES "${child}")
      else ()
        get_filename_component (child_folder_name ${child} NAME)
        file (
          GLOB garandchildren
          LIST_DIRECTORIES true
          "${child}/*"
        )
        foreach (grandchild ${garandchildren})
          # message(VERBOSE "${grandchild}")
          if (IS_DIRECTORY "${grandchild}")
            if (EXISTS "${grandchild}/CMakeLists.txt")
              file (RELATIVE_PATH grandchild_relative_path ${CMAKE_SOURCE_DIR}
                    ${grandchild}
              )
              message (VERBOSE "Adding '${grandchild_relative_path}'...")
              get_filename_component (grandchild_folder_name ${grandchild} NAME)
              add_subdirectory (
                "${grandchild}"
                "${base_bin_path}/${child_folder_name}/${grandchild_folder_name}"
              )
              list (APPEND XPACK_ADDED_DEPENDENCIES "${grandchild}")
            endif ()
          endif ()
        endforeach ()
      endif ()
    endif ()
  endforeach ()

endmacro ()

# -----------------------------------------------------------------------------

macro (xpack_add_dependencies_subdirectories dependencies_folders base_bin_path)

  # message(VERBOSE "${dependencies_folders}")
  foreach (dependency_folder ${dependencies_folders})
    # message(VERBOSE "${dependency_folder}")
    if (EXISTS "${dependency_folder}/CMakeLists.txt")
      file (RELATIVE_PATH relative_path ${CMAKE_SOURCE_DIR}
            ${dependency_folder}
      )
      # message (VERBOSE "Adding '${relative_path}'...")
      get_filename_component (folder_name ${dependency_folder} NAME)
      message (VERBOSE "") # Separator for readability
      add_subdirectory (
        "${dependency_folder}" "${base_bin_path}/${folder_name}"
      )
      list (APPEND XPACK_ADDED_DEPENDENCIES "${dependency_folder}")
    else ()
      message (FATAL_ERROR "Missing ${dependency_folder}/CMakeLists.txt")
    endif ()
  endforeach ()

endmacro ()

# -----------------------------------------------------------------------------

# Extract the three semver numbers from the input string. The second argument is
# a string with the variable name in the parent scope. Assume the string
# contains a full semver (https://semver.org).

function (xpack_parse_semver version_in variable_name)

  # Ignore the possible pre-release part.
  string (REGEX REPLACE "^\([0-9]+\.[0-9]+\.[0-9]+\).*$" "\\1" semver
                        "${version_in}"
  )

  # Pass the three numbers semver to the caller.
  set (
    "${variable_name}"
    "${semver}"
    PARENT_SCOPE
  )

  string (REGEX REPLACE "^\([0-9]+\)\.\([0-9]+\)\.\([0-9]+\)$" "\\1"
                        semver_major "${semver}"
  )
  string (REGEX REPLACE "^\([0-9]+\)\.\([0-9]+\)\.\([0-9]+\)$" "\\2"
                        semver_minor "${semver}"
  )
  string (REGEX REPLACE "^\([0-9]+\)\.\([0-9]+\)\.\([0-9]+\)$" "\\3"
                        semver_patch "${semver}"
  )

  # Pass the separate semver components to the caller.
  set (
    "${variable_name}_MAJOR"
    "${semver_major}"
    PARENT_SCOPE
  )
  set (
    "${variable_name}_MINOR"
    "${semver_minor}"
    PARENT_SCOPE
  )
  set (
    "${variable_name}_PATCH"
    "${semver_patch}"
    PARENT_SCOPE
  )

endfunction ()

macro (_xpack_parse_package_json_semver package_json_path)

  # Read the whole file into memory.
  file (READ "${package_json_path}" package_json_content)

  string (JSON package_json_name GET "${package_json_content}" "name")
  message (VERBOSE "package.name: ${package_json_name}")

  # Parse JSON and get the version property.
  string (JSON package_json_version_in GET "${package_json_content}" "version")
  message (VERBOSE "package.version: ${package_json_version_in}")

  xpack_parse_semver ("${package_json_version_in}" "PACKAGE_JSON_VERSION")

endmacro ()

function (xpack_get_package_name_and_version package_json_path)

  # Read the whole file into memory.
  file (READ "${package_json_path}" package_json_content)

  string (JSON package_json_name GET "${package_json_content}" "name")
  set (
    PACKAGE_JSON_NAME
    "${package_json_name}"
    PARENT_SCOPE
  )

  # Parse JSON and get the version property.
  string (JSON package_json_version_in GET "${package_json_content}" "version")
  set (
    PACKAGE_JSON_VERSION
    "${package_json_version_in}"
    PARENT_SCOPE
  )

endfunction ()

# -----------------------------------------------------------------------------

# Define a list of compiler options to enable as many warnings as possible.
macro (xpack_set_all_compiler_warnings variable_name)

  set (
    ${variable_name}
    -Wall
    # Nope. It complains about #include_next, and #pragma does not disable it,
    # at least on GCC 7. -Wpedantic
  )

  # https://cmake.org/cmake/help/v3.20/manual/cmake-generator-expressions.7.html
  # https://cmake.org/cmake/help/v3.20/variable/CMAKE_LANG_COMPILER_ID.html
  if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")

    # message(VERBOSE "GCC ${CMAKE_C_COMPILER_VERSION}")

    if ("${CMAKE_C_COMPILER_VERSION}" VERSION_LESS "7.0.0")
      message (FATAL_ERROR "GNU GCC older than 7.x not supported.")
    endif ()

    message (VERBOSE "Adding GCC warnings...")

    if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER_EQUAL "7.0.0")

      # message(VERBOSE "Adding GCC 7 warnings...")

      list (
        APPEND
        ${variable_name}
        # ---------------------------------------------------------------------
        # Common GNU C & C++.
        -Waggregate-return
        -Wcast-align
        -Wcast-qual
        -Wconversion
        -Wdouble-promotion
        -Wduplicated-branches
        -Wduplicated-cond
        -Wextra
        -Wfloat-conversion
        -Wfloat-equal
        -Wformat-nonliteral
        -Wformat-overflow=2
        -Wformat-security
        -Wformat-signedness
        -Wformat-truncation=2
        -Wformat-y2k
        -Wformat=2
        -Wlogical-op
        -Wmissing-declarations
        # Older GCC may include non-existent SDK folders.
        $<$<NOT:$<PLATFORM_ID:Darwin>>:-Wmissing-include-dirs>
        -Wnull-dereference
        -Wpacked
        -Wpadded
        -Wpointer-arith
        -Wredundant-decls
        -Wshadow
        -Wshift-overflow=2
        -Wsign-conversion
        -Wswitch-default
        -Wswitch-enum
        -Wundef
        -Wuninitialized
        -Wvla
        # ---------------------------------------------------------------------
        # GNU C only.
        $<$<COMPILE_LANGUAGE:C>:-Wbad-function-cast>
        $<$<COMPILE_LANGUAGE:C>:-Wc++-compat>
        $<$<COMPILE_LANGUAGE:C>:-Wduplicate-decl-specifier>
        $<$<COMPILE_LANGUAGE:C>:-Wmissing-prototypes>
        $<$<COMPILE_LANGUAGE:C>:-Wnested-externs>
        $<$<COMPILE_LANGUAGE:C>:-Wold-style-definition>
        $<$<COMPILE_LANGUAGE:C>:-Wstrict-prototypes>
        # ---------------------------------------------------------------------
        # GNU C++ only. It inherits the "cxx11" ABI tag for 'std::string'.
        # $<$<COMPILE_LANGUAGE:CXX>:-Wabi-tag>
        $<$<COMPILE_LANGUAGE:CXX>:-Wctor-dtor-privacy>
        $<$<COMPILE_LANGUAGE:CXX>:-Wnoexcept>
        $<$<COMPILE_LANGUAGE:CXX>:-Wnon-virtual-dtor>
        $<$<COMPILE_LANGUAGE:CXX>:-Wold-style-cast>
        $<$<COMPILE_LANGUAGE:CXX>:-Woverloaded-virtual>
        $<$<COMPILE_LANGUAGE:CXX>:-Wplacement-new=2>
        $<$<COMPILE_LANGUAGE:CXX>:-Wregister>
        $<$<COMPILE_LANGUAGE:CXX>:-Wsign-promo>
        $<$<COMPILE_LANGUAGE:CXX>:-Wstrict-null-sentinel>
        $<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-final-methods>
        $<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-final-types>
        $<$<COMPILE_LANGUAGE:CXX>:-Wsuggest-override>
        $<$<COMPILE_LANGUAGE:CXX>:-Wuseless-cast>
        $<$<COMPILE_LANGUAGE:CXX>:-Wzero-as-null-pointer-constant>
      )

    endif ()

    if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER_EQUAL "8.0.0")

      # message(VERBOSE "Adding GCC 8 warnings...")

      list (APPEND ${variable_name} $<$<COMPILE_LANGUAGE:CXX>:-Wextra-semi>)

    endif ()

    if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER_EQUAL "9.0.0")

      # message(VERBOSE "Adding GCC 9 warnings...")

      list (APPEND ${variable_name} # None so far.
      )

    endif ()

    if ("${CMAKE_C_COMPILER_VERSION}" VERSION_GREATER_EQUAL "10.0.0")

      # message(VERBOSE "Adding GCC 10 warnings...")

      list (
        APPEND
        ${variable_name}
        # ---------------------------------------------------------------------
        # Common GNU C & C++.
        -Warith-conversion
        # ---------------------------------------------------------------------
        # GNU C++ only.
        $<$<COMPILE_LANGUAGE:CXX>:-Wcomma-subscript>
        $<$<COMPILE_LANGUAGE:CXX>:-Wredundant-tags>
        $<$<COMPILE_LANGUAGE:CXX>:-Wvolatile>
      )

      # RISC-V 10.1 fails with `note: replace the class-key with 'struct'`.
      if (NOT "${CMAKE_C_COMPILER_VERSION}" VERSION_EQUAL "10.1.0")

        list (APPEND ${variable_name}
              $<$<COMPILE_LANGUAGE:CXX>:-Wmismatched-tags>
        )

      endif ()

    endif ()

  elseif ("${CMAKE_C_COMPILER_ID}" MATCHES ".*Clang")

    message (VERBOSE "Adding clang warnings...")

    list (
      APPEND ${variable_name}
      # For clang things are much easier.
      -Weverything $<$<C_COMPILER_ID:ARMClang>:-Wno-poison-system-directories>
    )

  endif ()

endmacro ()

# -----------------------------------------------------------------------------

function (xpack_glob_cxx variable_name sources_folder_path)

  set (local_sources)
  file (GLOB local_sublist CONFIGURE_DEPENDS "${sources_folder_path}/*.c*")
  list (APPEND local_sources ${local_sublist})
  file (GLOB local_sublist CONFIGURE_DEPENDS "${sources_folder_path}/*.S")
  list (APPEND local_sources ${local_sublist})

  set (
    ${variable_name}
    ${local_sources}
    PARENT_SCOPE
  )

endfunction ()

function (xpack_glob_recurse_cxx variable_name sources_folder_path)

  set (local_sources)
  file (GLOB_RECURSE local_sublist CONFIGURE_DEPENDS
        "${sources_folder_path}/*.c*"
  )
  list (APPEND local_sources ${local_sublist})
  file (GLOB_RECURSE local_sublist CONFIGURE_DEPENDS
        "${sources_folder_path}/*.S"
  )
  list (APPEND local_sources ${local_sublist})

  set (
    ${variable_name}
    ${local_sources}
    PARENT_SCOPE
  )

endfunction ()

function (xpack_display_relative_paths files_list relative_to_path)

  foreach (file_path IN LISTS files_list)
    file (RELATIVE_PATH file_relative_path "${relative_to_path}" "${file_path}")
    message (VERBOSE "+ ${file_relative_path}")
  endforeach ()

endfunction ()

# -----------------------------------------------------------------------------

# https://cmake.org/cmake/help/v3.20/manual/cmake-properties.7.html#properties-on-targets
# https://cmake.org/cmake/help/v3.20/command/get_target_property.html

function (xpack_display_target_lists target)

  set (relative_to_path "${CMAKE_CURRENT_SOURCE_DIR}")
  get_property (
    name
    TARGET ${target}
    PROPERTY NAME
  )
  message (VERBOSE "> ${name}")

  get_property (
    include_paths
    TARGET ${target}
    PROPERTY INTERFACE_INCLUDE_DIRECTORIES
  )

  if (include_paths)
    foreach (file_path IN LISTS include_paths)
      if ("${file_path}" MATCHES "^\\$<")
        message (VERBOSE "+ih -I ${file_path}")
        continue ()
      endif ()
      if (NOT IS_ABSOLUTE "${file_path}")
        set (file_path "${CMAKE_CURRENT_SOURCE_DIR}/${file_path}")
      endif ()
      file (RELATIVE_PATH file_relative_path "${CMAKE_SOURCE_DIR}"
            "${file_path}"
      )
      message (VERBOSE "+ih -I ${file_relative_path}")
    endforeach ()
  endif ()

  get_property (
    include_paths
    TARGET ${target}
    PROPERTY INCLUDE_DIRECTORIES
  )

  if (include_paths)
    foreach (file_path IN LISTS include_paths)
      if ("${file_path}" MATCHES "^\\$<")
        message (VERBOSE "+h -I ${file_path}")
        continue ()
      endif ()
      if (NOT IS_ABSOLUTE "${file_path}")
        set (file_path "${CMAKE_CURRENT_SOURCE_DIR}/${file_path}")
      endif ()
      file (RELATIVE_PATH file_relative_path "${CMAKE_SOURCE_DIR}"
            "${file_path}"
      )
      message (VERBOSE "+h -I ${file_relative_path}")
    endforeach ()
  endif ()

  get_property (
    sources_paths
    TARGET ${target}
    PROPERTY INTERFACE_SOURCES
  )

  if (sources_paths)
    foreach (file_path IN LISTS sources_paths)
      if ("${file_path}" MATCHES "^\\$<")
        message (VERBOSE "+is ${file_path}")
        continue ()
      endif ()
      if (NOT IS_ABSOLUTE "${file_path}")
        set (file_path "${CMAKE_CURRENT_SOURCE_DIR}/${file_path}")
      endif ()
      file (RELATIVE_PATH file_relative_path "${CMAKE_SOURCE_DIR}"
            "${file_path}"
      )
      message (VERBOSE "+is ${file_relative_path}")
    endforeach ()
  endif ()

  get_property (
    sources_paths
    TARGET ${target}
    PROPERTY SOURCES
  )

  if (sources_paths)
    foreach (file_path IN LISTS sources_paths)
      if ("${file_path}" MATCHES "^\\$<")
        message (VERBOSE "+s ${file_path}")
        continue ()
      endif ()
      if (NOT IS_ABSOLUTE "${file_path}")
        set (file_path "${CMAKE_CURRENT_SOURCE_DIR}/${file_path}")
      endif ()
      file (RELATIVE_PATH file_relative_path "${CMAKE_SOURCE_DIR}"
            "${file_path}"
      )
      message (VERBOSE "+s ${file_relative_path}")
    endforeach ()
  endif ()

  get_property (
    compile_definitions
    TARGET ${target}
    PROPERTY INTERFACE_COMPILE_DEFINITIONS
  )

  if (compile_definitions)
    foreach (def IN LISTS compile_definitions)
      message (VERBOSE "+id -D ${def}")
    endforeach ()
  endif ()

  get_property (
    compile_definitions
    TARGET ${target}
    PROPERTY COMPILE_DEFINITIONS
  )

  if (compile_definitions)
    foreach (def IN LISTS compile_definitions)
      message (VERBOSE "+d -D ${def}")
    endforeach ()
  endif ()

  get_property (
    compile_options
    TARGET ${target}
    PROPERTY INTERFACE_COMPILE_OPTIONS
  )

  if (compile_options)
    foreach (opt IN LISTS compile_options)
      message (VERBOSE "+ic ${opt}")
    endforeach ()
  endif ()

  get_property (
    compile_options
    TARGET ${target}
    PROPERTY COMPILE_OPTIONS
  )

  if (compile_options)
    foreach (opt IN LISTS compile_options)
      message (VERBOSE "+c ${opt}")
    endforeach ()
  endif ()

  get_property (
    link_libraries
    TARGET ${target}
    PROPERTY INTERFACE_LINK_LIBRARIES
  )

  if (link_libraries)
    foreach (lib IN LISTS link_libraries)
      message (VERBOSE "+iL ${lib}")
    endforeach ()
  endif ()

  get_property (
    link_libraries
    TARGET ${target}
    PROPERTY LINK_LIBRARIES
  )

  if (link_libraries)
    foreach (lib IN LISTS link_libraries)
      message (VERBOSE "+L ${lib}")
    endforeach ()
  endif ()

  get_property (
    link_options
    TARGET ${target}
    PROPERTY INTERFACE_LINK_OPTIONS
  )

  if (link_options)
    foreach (opt IN LISTS link_options)
      message (VERBOSE "+il ${opt}")
    endforeach ()
  endif ()

  get_property (
    link_options
    TARGET ${target}
    PROPERTY LINK_OPTIONS
  )

  if (link_options)
    foreach (opt IN LISTS link_options)
      message (VERBOSE "+l ${opt}")
    endforeach ()
  endif ()

endfunction ()

# -----------------------------------------------------------------------------

# https://cmake.org/cmake/help/v3.20/manual/cmake-properties.7.html#properties-of-global-scope
# https://cmake.org/cmake/help/v3.20/command/get_property.html

function (xpack_display_global_lists)

  set (relative_to_path "${CMAKE_CURRENT_SOURCE_DIR}")

  get_directory_property (include_paths INCLUDE_DIRECTORIES)

  if (include_paths)
    foreach (file_path IN LISTS include_paths)
      file (RELATIVE_PATH file_relative_path "${relative_to_path}"
            "${file_path}"
      )
      message (VERBOSE ".G+ -I ${file_relative_path}")
    endforeach ()
  endif ()

  get_directory_property (compile_definitions COMPILE_DEFINITIONS)

  if (compile_definitions)
    foreach (def IN LISTS compile_definitions)
      message (VERBOSE ".G+ -D ${def}")
    endforeach ()
  endif ()

  get_directory_property (compile_options COMPILE_OPTIONS)

  if (compile_options)
    foreach (opt IN LISTS compile_options)
      message (VERBOSE ".G+ ${opt}")
    endforeach ()
  endif ()

endfunction ()

# -----------------------------------------------------------------------------

function (xpack_add_cross_custom_commands target_name)

  # TODO use add_custom_target()
  # https://cmake.org/cmake/help/v3.20/command/add_custom_command.html
  add_custom_command (
    TARGET ${target_name}
    POST_BUILD
    COMMAND ${CMAKE_SIZE} --format=berkeley "$<TARGET_FILE:${target_name}>"
  )

  if (xpack_create_hex)
    add_custom_command (
      TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_OBJCOPY} -O ihex "$<TARGET_FILE:${target_name}>"
              "$<TARGET_FILE:${target_name}>.hex"
    )
  endif ()

  if (xpack_create_listing)
    add_custom_command (
      TARGET ${target_name}
      POST_BUILD
      COMMAND ${CMAKE_OBJDUMP} --source --all-headers --demangle --line-numbers
              --wide "$<TARGET_FILE:${target_name}>" > ${target_name}-list.txt
      VERBATIM
    )
  endif ()

endfunction ()

# -----------------------------------------------------------------------------
