# POSIX IO Tests

These are generally unit tests, but some tests exercise more than one class.

Both the C++ & C API are tested, although not exactly all C++ functions are 
directly called, since most of them are anyway called by the C API.

## descriptors-manager

Test the `FileDescriptorsManager` class, that manages the file descriptors, as
indices in an array of pointers to objects.

## device

Test the `CharDevice` class, that implements the POSIX read/write API.

## pool

Test the `Pool` class, that manages a pool of File or Socket objects.

## file

Test the `File` and `FileSystem` classes, that implement the POSIX file 
related functions.

## directory

Test the `Directory` class, that implements the POSIX directory related functions (opendir(), readdir(), etc).

## socket

Test the `Socket` class, that implements the POSIX socket API.