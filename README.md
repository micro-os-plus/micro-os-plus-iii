# CMSIS++

An xPack with CMSIS++, a proposal for a future CMSIS, written in C++ and POSIX compliant.

## Package

[CMSIS++](https://github.com/micro-os-plus/cmsis-plus) package details:

* type: [xcdl](http://xcdl.github.io)
* yotta name: `ilg-cmsis-plus`
* yotta repo: http://yotta.mbed.com/#/module/ilg-cmsis-plus/0.1.1
* git repo: https://github.com/micro-os-plus/cmsis-plus.git
* homepage: https://github.com/micro-os-plus/cmsis-plus
* latest archive: https://github.com/micro-os-plus/cmsis-plus/archive/xpack.zip

## Version

* 0.1.1

## Releases

* none

## Package maintainers

* [Liviu Ionescu](http://liviusdotnet.worldpress.com) [&lt;ilg@livius.net&gt;](mailto:ilg@livius.net)

## Content contributors

* [Liviu Ionescu](http://liviusdotnet.worldpress.com) [&lt;ilg@livius.net&gt;](mailto:ilg@livius.net)

## Support

* [forum](http://www.element14.com/community/groups/gnu-arm-eclipse)
* [bugs & enhancements](https://github.com/micro-os-plus/cmsis-plus/issues)

## Dependencies

* none

## Keywords

* arm
* cmsis
* cortex
* c++
* rtos
* drivers
* scheduler
* posix

## License

* [spdx](http://spdx.org/licenses/): MIT

--- 
## Purpose

The CMSIS++ package is a proposal for a future CMSIS version, intended for C++ embedded applications, but with very good support for C. 

The specifications are more or less complete for CMSIS++ RTOS, and work in progress for the other components, so any contributions are highly appreciated.

CMSIS++ is currently used by µOS++ III, but has no dependencies on it, and can be used in any other C++ embedded project.

## CMSIS++ or POSIX++?

The first attempt was to design the C++ API based on the ARM CMSIS RTOS v1.x C API, but the ARM API had too many [shortcommings](https://github.com/ARM-software/CMSIS/issues/64), and the idea was abandonned. 

The second attempt tried to stay as close as possible to the POSIX requirements, and now CMSIS++ RTOS is more or less a C++ rewrite of the POSIX pthread library, so it might very well be called **POSIX++**.

## Project highlights

The project highlights are:

- the core RTOS is based on a C++ API (namespace os::rtos), defined in [<cmsis-plus/rtos/os.h>](https://github.com/micro-os-plus/cmsis-plus/blob/xpack/include/cmsis-plus/rtos/os.h)

- exactly the same functionality is provided in C, with a C wrapper, for those who prefer to use plain C; the C API is defined in [<cmsis-plus/rtos/os-c-api.h>](https://github.com/micro-os-plus/cmsis-plus/blob/xpack/include/cmsis-plus/rtos/os-c-api.h)

- the actual ARM CMSIS API v1.x is also implemented in the C wrapper; it is defined in [<cmsis_os.h>](https://github.com/micro-os-plus/cmsis-plus/blob/xpack/include/cmsis-plus/legacy/cmsis_os.h) (functional, just passed the CMSIS RTOS validation, using the [FreeRTOS port](https://github.com/xpacks/freertos/blob/xpack/cmsis-plus/include/cmsis-plus/rtos/port/os-inlines.h))

- on top of the core C++ API, the ISO standard thread library is fully implemented, and this is the recommended API for future applications; the definitions are available form [<cmsis-plus/estd/*>](https://github.com/micro-os-plus/cmsis-plus/tree/xpack/include/cmsis-plus/estd)

- all RTOS objects are based on the core C++ objects, and objects created in C++ can be used in C and similarly objects created in C can be used in C++ (there is no mystery here, they represent exactly the same objects)

- the CMSIS++ RTOS specification go one step further then the original CMSIS, it also suggest a portable scheduler API, and a set of portable synchronisation primitives, calling the portable scheduler; the portable synchronisation objects can be considered a 'reference implementation', however CMSIS++ does not mandate the use of these objects, as it is quite easy to forward all calls to the underlaying implementation (a fully functional version running on top of FreeRTOS is available)

- there is also a reference scheduler (called µOS++ III); the implementation is highly portable, it runs in 32-bits and 64-bits environments but is specifically tailored for Cortex-M cores; it also runs on synthetic POSIX platforms, for example as a user mode POSIX process, on OS X and GNU/Linux. 

- CMSIS++ also includes a POSIX IO interface (namespace os::posix), bringing together access to terminal devices, files and sockets, via a unified and standard API (functional in a separate project, will be updated and moved here shortly)

- CMSIS++ Drivers are the response to CMSIS Drivers, but designed in C++ (namespace os::drivers), and with a C API on top of them (CMSIS++ serial, USB Device and USB Host already defined and partly implemented)

- the CMSIS++ packaging solution will extend and complement CMSIS Packs with **xPack/XCDL** packs

## Documentation

* [CMSIS++ / µOS++ IIIe](http://micro-os-plus.github.io)
* [CMSIS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/)
* the original ARM CMSIS documentation is available form 
[keil.com](http://www.keil.com/pack/doc/CMSIS/General/html/index.html)

## License

CMSIS++ is provided free of charge under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## Related projects

The current CMSIS++ code tries to be as portable as possible.

Specific implementations are available from separate projects:

- the Cortex-M port of the scheduler is packed with [µOS++ IIIe xPack](https://github.com/micro-os-plus/micro-os-plus-iii)
- the synthetic POSIX port of the scheduler is packed with [POSIX arch xPack](https://github.com/micro-os-plus/posix-arch)

## Tests

* tests/rtos - simple test to exercise the CMSIS++ RTOS C++ API, the C API and the ISO C++ API
* tests/mutex-stress - a stress test with 10 threads fighting for a mutex
* tests/sema-stress - a stress test posting to a semaphore from a high frequency interrupt.
* tests/gcc - compile test with host GCC compiler

The ARM CMSIS RTOS validator is available from a [separate project](https://github.com/xpacks/arm-cmsis-rtos-validator).



--- 
