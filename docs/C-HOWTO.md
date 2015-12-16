## C API

Althoght the POSIX IO layer is largly written in C++, it obviouly 
has a plain C interface, with prototypes strictly adhering to the 
POSIX API.

## `__posix_` prefix

To allow tests, the native function names are prefixed with `__posix_`,
like:

```
extern "C" int __attribute__((weak))
__posix_open (const char* path, int oflag, ...);
```

By default this prefix is removed by a set of macros like:

```
#define __posix_open open
```

## `OS_BOOL_PREFIX_POSIX_SYSCALLS`

For test purposes, these macros can be disabled, and the function names remain prefixed with `__posix_`.

Do not define this macro in normal applications.

## Using drivers in C

Assuming the POSIX syscalls were redirected, the use of drivers is standard, with the only mention that by default devices are mapped under `/dev/`.

```
int fd;
fd = open("/dev/usart", mode);
...
int ret;
ret = read(fd, buf, nbyte);
...
ret = write(fd, buf, nbyte);
...
ret = close(fd);
```

## Defining drivers with C implementation

Although a small C++ glue code is required to allow registration to the central drivers registry, it is perfectly possible to integrate drivers with C implementations.

For this, define C functions with prototypes identical to the `doXxx()` functions in the `PosixIo.h`.

For example, for an USART driver, define:

```
	int
    usart_open (const char* path, int oflag, va_list args);

    int
    usart_close (void);

    ssize_t
    usart_read (void* buf, size_t nbyte);

    ssize_t
    usart_write (const void* buf, size_t nbyte);

    int
    usart_ioctl (int request, va_list args);
    
    ...

```

### Variadic implementation

To facilitate the implementation of the variable args list defined in POSIX, open() uses a third parameter of type `va_list`. If you want to extract more arguments, use specific variadic macros:

```
int mode;
mode = va_arg(args, int);
```

### C++ glue code

To link the implementation to the POSIX IO subsystem, it is necessary to define an object derived from `os::PosixDevice` and implement the `doXxx()` functions.

For an complete example, please check the sample `UsartPosixDevice` class in `docs`.

### Further improvements

Once you become more confident with C++, you can try to eliminate the intermediate `usart_` layer and move its functionality directly to the C++ class.

