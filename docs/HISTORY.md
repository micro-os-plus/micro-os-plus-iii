# Implementation steps

## Rewrite Keil CMSIS in C++

The first step towards CMSIS++ was to rewrite the original cmsis_os.h in C++,
resulting the initial `cmsis-plus/rtos/os.h`.

## Implement CMSIS based on the C++ definitions

Using the new C++ definitions, an implementation of CMSIS was added.

To be able to include the C++ definitions, the implementation file
must be in C++ (`cmsis_os.cpp`), but from the functional point of view,
it defines the clasical C functions (as `extern "C"`).

## Add extensions

Since the purpose of the CMSIS++ was to provide a better user experience, 
from the first moment it was clear that the new CMSIS specification will
include different definitions.

The initial approach was to extend the current API with new functions
suffixed with `Ex`, like `osThreadCreateEx`.

All these definitions were grouped in a new header file `cmsis_os_ex.h`,
that first included the original `cmsis_os.h` file, then added new
definitions.

However, as the C++ API grows and settles, the number of `Ex` functions 
is expected to grow and make this approach less convenient, with a transition
to a completely new C API planned, to match the C++ calls.

## Add ISO 14882 C++ standard support

Support for ISO and POSIX standards was probably the most important 
requirement for the new CMSIS++.

Implementation of the `thread`, `mutex`, `condition_variable` and `chrono`
features brought not only compatibility with the standards, but also caused
changes to the native C++ APIs, to better and conveniently support
the implementation of the standard features.

## Define two native clocks (Systick\_clock & Realtime\_clock)

Based on the standard definition of system clocks, to the initial SysTick 
clock (with ticks granularity) was added a new RTC based clock (with seconds
granularity).

 



