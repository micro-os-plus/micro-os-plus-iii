# CMake Tips and tricks

> DO NOT EDIT! Automatically generated from template file:
> build-helper/templates/common/_micro-os-plus/tests/cmake/TIPS-AND-TRICKS-liquid.md

Extracts from Craig Scott's "Professional CMake - A Practical Guide".

## Folders

### CMAKE_SOURCE_DIR

The top-most directory of the source tree (i.e. where the top-most
CMakeLists.txt file resides).
This variable never changes its value.

- <https://cmake.org/cmake/help/latest/variable/CMAKE_SOURCE_DIR.html>

### CMAKE_CURRENT_SOURCE_DIR

Note: use CMAKE_CURRENT_LIST_DIR.

The directory of the CMakeLists.txt file currently being processed by
CMake. It is updated each time a new file is processed as a result of
an add_subdirectory() call and is restored back again when processing
of that directory is complete.

- <https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_SOURCE_DIR.html>

### CMAKE_CURRENT_LIST_DIR

Analogous to CMAKE_CURRENT_SOURCE_DIR except it will be updated when
processing the included file. This is the variable to use where the
directory of the current file being processed is required, no matter
how it has been added to the build. It will always hold an absolute path.

The CMAKE_CURRENT_LIST_DIR variable is generally going to be a better
choice than CMAKE_CURRENT_SOURCE_DIR.

- <https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_LIST_DIR.html>

### CMAKE_CURRENT_LIST_FILE

Always gives the name of the file currently being processed. It always
holds an absolute path to the file, not just the file name.

### CMAKE_CURRENT_FUNCTION_LIST_DIR

When executing code inside a function(), this variable contains the full directory of the listfile that defined the current function.

- <https://cmake.org/cmake/help/latest/variable/CMAKE_CURRENT_FUNCTION_LIST_DIR.html>

## Other

### TARGET_FILE

This will yield the absolute path and file name of the target’s binary,
including any file prefix and suffix if relevant for the platform
(e.g .exe, .dylib). For Unix-based platforms where shared libraries
typically have version details in their file name, these will also
be included.

### TARGET_FILE_NAME

Same as TARGET_FILE but without the path (i.e. it provides just the
file name part).

### TARGET_FILE_DIR

Same as TARGET_FILE but without the file name. This is the most robust
way to obtain the directory in which the final executable or library
is built. It’s value is different for different build configurations
when using a multi configuration generator like Xcode or Visual Studio.

## Generator expressions

### `$<CONFIG>`

Evaluates to the build type. Use this in preference to the
CMAKE_BUILD_TYPE variable since that variable is not used on multi
configuration project generators like Xcode or Visual Studio.

### `$<PLATFORM_ID>`

Identifies the platform for which the target is being built. This
can be useful in cross-compiling situations, especially where a build
may support multiple platforms (e.g. device and simulator builds).
This generator expression is closely related to the CMAKE_SYSTEM_NAME
variable and projects should consider whether using that variable
would be simpler in their specific situation.

### `$<LOWER_CASE:...>`, `$<UPPER_CASE:...>`

Any content can be converted to lower or upper case via these expressions.

### Language specific options

```cmake
$<$<COMPILE_LANGUAGE:C>:-Wbad-function-cast>
$<$<COMPILE_LANGUAGE:CXX>:-Wnoexcept>
```

### Toolchain specific options

```cmake
$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-ffunction-sections>
$<$<C_COMPILER_ID:GNU,Clang,AppleClang>:-fdata-sections>
$<$<C_COMPILER_ID:GNU>:"-Wl,--gc-sections">
$<$<C_COMPILER_ID:Clang,AppleClang>:"-Wl,-dead_strip">
```

Common IDs:

- GNU
- AppleClang
- Clang
- MSVC

Full list:

- <https://cmake.org/cmake/help/v3.20/variable/CMAKE_LANG_COMPILER_ID.html>

## GCC With 32-bit Target On 64-bit Host

Possibly in the toolchain file:

```cmake
set(CMAKE_C_FLAGS_INIT   -m32)
set(CMAKE_CXX_FLAGS_INIT -m32)
set(CMAKE_EXE_LINKER_FLAGS_INIT -m32)
```

## function() vs macro()

- <https://levelup.gitconnected.com/cmake-variable-scope-f062833581b7>

`function()` creates a new child scope, `macro()` uses the same scope.

To modify parent variables use:

```cmake
set(name value PARENT_SCOPE)
```

## add_subdirectory() vs include()

- <https://levelup.gitconnected.com/cmake-variable-scope-f062833581b7>

`add_subdirectory()` creates a new child scope, `include()` uses the same scope.

To modify parent variables use:

```cmake
set(name value PARENT_SCOPE)
```

## Recommended Practices

When using single configuration generators like Makefiles or Ninja,
consider using multiple build directories, one for each build type
of interest.

Projects should generally avoid using the CMAKE_CROSSCOMPILING variable
for any of its logic.
