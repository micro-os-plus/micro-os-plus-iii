# CMSIS++

An xPack with CMSIS++, a C++ version of CMSIS.

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

## License

* [spdx](http://spdx.org/licenses/): MIT

--- 
## Purpose

The CMSIS++ package is a proposal for a future CMSIS version, intended for C++ embedded applications, but with complete support for C.

The specifications are currently work in progress, and any contributions are highly appreciated.

CMSIS++ is currently used by µOS++ III, but has no dependencies on it, and can be used in any other C++ embedded project.

## Project highlights

The project highlights are:

- the core RTOS is based on a C++ API (namespace os::rtos), defined in [<cmsis-plus/rtos/os.h>](https://github.com/micro-os-plus/cmsis-plus/blob/xpack/include/cmsis-plus/rtos/os.h)

- exactly the same functionality is provided in C, with a C wrapper, for those who prefer to use plain C; the C API is defined in [<cmsis-plus/rtos/os-c-api.h>](https://github.com/micro-os-plus/cmsis-plus/blob/xpack/include/cmsis-plus/rtos/os-c-api.h)

- the actual CMSIS API is also implemented in the C wrapper; it is defined in [<cmsis_os.h>](https://github.com/micro-os-plus/cmsis-plus/blob/xpack/include/cmsis-plus/legacy/cmsis_os.h) (functional, just passed the CMSIS RTOS validation, using the [FreeRTOS port](https://github.com/xpacks/freertos/blob/xpack/cmsis-plus/include/cmsis-plus/rtos/port/os-inlines.h))

- on top of the core C++ API, the ISO standard thread library is fully implemented, and this is the recommended API for future applications; the definitions are available form [<cmsis-plus/iso/*>](https://github.com/micro-os-plus/cmsis-plus/tree/xpack/include/cmsis-plus/iso)

- all RTOS objects are based on the core C++ objects, and objects created in C++ can be used in C and similarly objects created in C can be used in C++ (there is no mystery here, they represent exactly the same objects)

- the C++ API was originally derived from the original CMSIS RTOS C API, but was adjusted to match the POSIX requirements as close as possible, so now it is generally a C++ version of the POSIX pthread library

- the CMSIS++ RTOS specification will go one step further then the original CMSIS, it will also suggest a portable scheduler API, and a set of portable synchronisation primitives, calling the portable scheduler; the portable synchronisation objects can be considered a 'reference implementation', however CMSIS++ does not mandate the use of these objects, it is quite easy to forward all calls to the underlaying implementation, as the current version using FreeRTOS does (partly implemented, to be completed soon)

- there will also be a reference scheduler implementation (called µOS++ III), that will also be highly portable; it will run in 32-bits and 64-bits environments but will be specifically tailored for Cortex-M cores; it'll also run on synthetic POSIX platforms, for example as a user mode POSIX process, on OS X and GNU/Linux, possibly on Windows if MinGW-w64 will allow an easy port. (will be based on prior µOS++ versions, which are fully functional)

- CMSIS++ also includes a POSIX IO interface (namespace os::posix), bringing together access to terminal devices, files and sockets, via a unified and standard API (functional in a separate project, will be updated and moved here shortly)

- CMSIS++ Drivers are the response to CMSIS Drivers, but designed in C++ (namespace os::drivers), and with a C API on top of them (CMSIS++ serial, USB Device and USB Host already defined and partly implemented)

- the CMSIS++ packaging solution will extend and complement CMSIS Packs with **xPack/XCDL** packs

## Documentation

TODO: create a CMSIS++ Doxygen site and add link here.

For reference, the original ARM CMSIS documentation is available form 
[keil.com](http://www.keil.com/pack/doc/CMSIS/General/html/index.html).

## Tests

* tests/gcc - compile test with host GCC compiler
* tests/eabi-gcc - cross compile test with arm-none-eabi-gcc



--- 
