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

## Clarify the behaviour of thread signal flags

Use a simplified behaviours, inspired by POSIX, but without queues and handlers. 

## Define the behaviour of Event Flags, similar to that of thread signal flags

Add an Event_flags object.

## Split the implementation code in separate objects

Each object (thread, mutex, semaphore, etc) is defined in its separate file.

## Define separate `port/os-inlines.h` 

All port implementations are static inlines, included on demand in the object implementation files.

## Implement for FreeRTOS

Define each function in `port/os-inlines.h` using FreeRTOS primitives.

## Use the Keil RTOS validator to make the implementation compliant

- realise that osThreadDef() instances mean the macro should reserve space for multiple threads
- realise that individual thread defs can be reused after thread terminates, even for multiple instances
- add stack to thread definitions

## Implement the portable synchronisation objects

All synchronisation objects were implemented. To be noted the waiting lists, 
which have their nodes allocated on each waiting thread stack.

## Preliminary definition for the scheduler API

Based on the experience with the FreeRTOS port, it looks safer to split `suspend()` into
`prepare_suspend()` and `reschedule()`, and call the first one in a critical
region together with linking the thread to the waiting lists.

## Write a stress test for the semaphore implementation

April 2016 - The test uses a hardware timer to generate interrupts at various rates, and
the ISR stores a word into an array and notifies a semaphore. The thread 
waits for the semaphore and checks the presence of the value in the array.

It is a more automated (and much faster) version of the copy/paste test using 
the usart input to a monitor.

## Implement a 64-bits cooperative scheduler on OS X

April 2016 - A cooperative scheduler, based on POSIX makecontext()/swapcontext() was written for OS X and is fully functional, passing the ARM CMSIS RTOS validator.

It is part of the synthetic/posix-arch-xpack.

## Implement the Cortex-M scheduler

May 2016 - A full preemptive scheduler for Cortex-M3/4 is fully functional. It passes the ARM CMSIS RTOS validator and the semaphore stress test.

The core context switching routine is written in C++, with two inlined insertions in asm(), used to save/restore the registers.

The scheduler is part of the portable/micro-os-plus-xpack.

It does not come from nowhere, it is heavily based on the experience with the previous µOS++ versions.












