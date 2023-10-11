# platform-native

Support files for building application to run as native processes
on GNU/Linux and macOS.

The platform specific include header is currently empty:

- `platform/include/micro-os-plus/platform.h`

## RPATH

To accommodate for the custom library path, ask the compiler
for the actual path and use `-Wl,-rpath` during link.

For older GCC (like 11) it is still necessary to use -static-* on macOS.
