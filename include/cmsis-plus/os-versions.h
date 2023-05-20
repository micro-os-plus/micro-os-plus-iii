/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2016 Liviu Ionescu.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CMSIS_PLUS_OS_VERSIONS_H_
#define CMSIS_PLUS_OS_VERSIONS_H_

// ----------------------------------------------------------------------------

// According to Semantic Versioning 2.0.0 (http://semver.org),
// given a version number MAJOR.MINOR.PATCH, increment the:
//
// MAJOR version when you make incompatible API changes,
// MINOR version when you add functionality in a backwards-compatible manner,
// PATCH version when you make backwards-compatible bug fixes,
// PRE_RELEASE is a hyphen and a series of dot separated identifiers,
// BUILD a plus sign and a series of dot separated identifiers.

// ----------------------------------------------------------------------------

#define OS_MACRO_SHARP(x)               #x
#define OS_MACRO_STRINGIFY(x)           OS_MACRO_SHARP(x)

// ----------------------------------------------------------------------------

// µOS++ version.
#define OS_INTEGER_RTOS_IMPL_VERSION_MAJOR 6
#define OS_INTEGER_RTOS_IMPL_VERSION_MINOR 3
#define OS_INTEGER_RTOS_IMPL_VERSION_PATCH 17
#define OS_STRING_RTOS_IMPL_VERSION_PRE_RELEASE ""
// #define OS_STRING_RTOS_IMPL_VERSION_PRE_RELEASE "-beta"
#define OS_STRING_RTOS_IMPL_VERSION_BUILD ""

#define OS_INTEGER_RTOS_IMPL_YEAR 2023
#define OS_STRING_RTOS_IMPL_YEAR OS_MACRO_STRINGIFY(OS_INTEGER_RTOS_IMPL_YEAR)

#define OS_STRING_RTOS_IMPL_VERSION \
  OS_MACRO_STRINGIFY(OS_INTEGER_RTOS_IMPL_VERSION_MAJOR) "." \
  OS_MACRO_STRINGIFY(OS_INTEGER_RTOS_IMPL_VERSION_MINOR) "." \
  OS_MACRO_STRINGIFY(OS_INTEGER_RTOS_IMPL_VERSION_PATCH) \
  OS_STRING_RTOS_IMPL_VERSION_PRE_RELEASE \
  OS_STRING_RTOS_IMPL_VERSION_BUILD

// ----------------------------------------------------------------------------

/**
 * @ingroup cmsis-plus-app-config-info
 * @{
 */

/**
 * @brief Tell the world that µOS++ is in use.
 * @details
 * Macro to inform the application build that µOS++
 * is in use.
 */
 #define OS_USE_MICRO_OS_PLUS

/**
 * @def OS_IS_CROSS_BUILD
 * @brief Tell the world that this is a cross build.
 * @details
 * Currently only macOS and GNU/Linux are identified as native builds,
 * all the other are considered cross builds.
 */

/**
 * @def OS_HAS_STD_THREADS
 * @brief Tell the world that `std::tread` is available.
 * @details
 * For cross builds, standard thread definitions (available in the
 * `os::estd::` namespace) are also aliased or redefined in the `std::`
 * namespace.
 */

#if !defined(__APPLE__) && !defined(__linux__)
#define OS_IS_CROSS_BUILD
#if !defined(_GLIBCXX_HAS_GTHREADS)
#define OS_HAS_STD_THREADS
#endif
#endif

/**
 * @}
 */

// ----------------------------------------------------------------------------

#endif /* CMSIS_PLUS_OS_VERSIONS_H_ */
