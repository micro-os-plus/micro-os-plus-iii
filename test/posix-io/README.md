# POSIX IO Tests

These are generally unit tests, but some tests exercise more than one class.

Both the C++ & C API are tested, although not exactly all C++ functions are 
directly called, since most of them are anyway called by the C API.

## descriptors-manager

Test the `FileDescriptorsManager` class, that manages the file descriptors, as
indices in an array of pointers to objects.

## device

Test the `device_char` class, that implements the POSIX read/write API.

## pool

Test the `pool` class, that manages a pool of file or socket objects.

## file

Test the `file` and `file_system` classes, that implement the POSIX file 
related functions.

## directory

Test the `directory` class, that implements the POSIX directory related functions (opendir(), readdir(), etc).

## socket

Test the `socket` class, that implements the POSIX socket API.