# µOS++ IIIe

An xPack with the third version of µOS++, a POSIX inspired open source system, written in C++

## Package

[µOS++ IIIe](https://github.com/micro-os-plus/micro-os-plus-iii) package details:

* type: [xcdl](http://xcdl.github.io)
* name: `ilg/micro-os-plus-iii`
* git repo: https://github.com/micro-os-plus/micro-os-plus-iii.git
* homepage: https://github.com/micro-os-plus/micro-os-plus-iii
* latest archive: https://github.com/micro-os-plus/micro-os-plus-iii/archive/xpack.zip

## Version

* 6.3.11

## Releases

* none

## Package maintainers

* [Liviu Ionescu](http://liviusdotnet.worldpress.com) [&lt;ilg@livius.net&gt;](mailto:ilg@livius.net)

## Content contributors

* [Liviu Ionescu](http://liviusdotnet.worldpress.com) [&lt;ilg@livius.net&gt;](mailto:ilg@livius.net)

## Support

* [forum](http://www.element14.com/community/groups/gnu-arm-eclipse)
* [bugs & enhancements](https://github.com/micro-os-plus/micro-os-plus-iii/issues)

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

The **µOS++ IIIe** _(micro oh ɛs plus plus third edition)_ project is the third iteration of µOS++, a POSIX-like, portable, open source, royalty-free, multi-tasking real-time operating system intended for 32/64-bits embedded applications, written in C++. The APIs are documented in the [µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/).

**POSIX++** is another point of view of the project; with most system APIs using the POSIX semantics, but being written in C++, the **µOS++ IIIe** APIs can also be seen as a C++ version of POSIX, thus the name **POSIX++**.

## Status

The µOS++ project is fully functional for Cortex-M devices and also runs on synthetic POSIX platforms (like macOS and GNU/Linux).

## Related projects

The current µOS++ code tries to be as portable as possible.

Specific implementations are available from separate projects:

- the Cortex-M port of the scheduler is packed as [µOS++ IIIe Cortex-M xPack](https://github.com/micro-os-plus/micro-os-plus-iii-cortexm)
- the synthetic POSIX port of the scheduler is packed as [POSIX arch xPack](https://github.com/micro-os-plus/posix-arch)

## Build Configuration

To include µOS++ in a project, in addition to one of the port specific project, consider the following details:

### Include folders

- `include` 
 
### Source folders

- `src` 

### Symbols

See [µOS++ Application Config](http://micro-os-plus.github.io/reference/cmsis-plus/group__cmsis-plus-app-config.html).

## Documentation

* [µOS++ IIIe](http://micro-os-plus.github.io)
* [µOS++ reference](http://micro-os-plus.github.io/reference/cmsis-plus/)
* the original ARM CMSIS documentation is available form 
[keil.com](http://www.keil.com/pack/doc/CMSIS/General/html/index.html)

## Demo projects

Examples on how to structure projects using µOS++, including integration with CubeMX for STM devices, can be found in the separate [GitHub project](https://github.com/micro-os-plus/eclipse-demo-projects).

## Tests

* tests/rtos - simple test to exercise the µOS++ RTOS C++ API, the C API and the ISO C++ API
* tests/mutex-stress - a stress test with 10 threads fighting for a mutex
* tests/sema-stress - a stress test posting to a semaphore from a high frequency interrupt.
* tests/gcc - compile test with host GCC compiler

The ARM CMSIS RTOS validator is available from a [separate project](https://github.com/xpacks/arm-cmsis-rtos-validator).

To run these tests, several Eclipse projects are available in a separate [GitHub project](https://github.com/micro-os-plus/eclipse-test-projects).

## License

µOS++ is provided free of charge under the terms of the [MIT License](https://opensource.org/licenses/MIT).

## GCC 6.2 patches

Up to including GCC 6.2, the `allocator_traits<>` template had a misplaced static_assert, preventing µOS++ to compile.

The final workaround was to add a `rebind` template to the allocator, but before this an alternate workaround was to patch the `bits/alloc_traits.h` file.

```
# Note: EOF is quoted to prevent substitutions here.
$ cat <<'__EOF__' > /tmp/alloc_traits_patch.txt
--- bits/alloc_traits.h	2017-02-14 19:17:10.000000000 +0200
+++ bits/alloc_traits_patched.h	2017-02-14 19:25:39.000000000 +0200
@@ -184,8 +184,8 @@
       template<typename _Tp>
 	using rebind_traits = allocator_traits<rebind_alloc<_Tp>>;
 
-      static_assert(!is_same<rebind_alloc<value_type>, __undefined>::value,
-	  "allocator defines rebind or is like Alloc<T, Args>");
+//      static_assert(!is_same<rebind_alloc<value_type>, __undefined>::value,
+//	  "allocator defines rebind or is like Alloc<T, Args>");
 
     private:
       template<typename _Alloc2>
__EOF__

# Update the --directory= to point to your location.
$ sudo patch --backup --unified --verbose --strip=0 \
--input=/tmp/alloc_traits_patch.txt \
--directory=/Users/ilg/opt/gcc-arm-none-eabi-6_2-2016q4/arm-none-eabi/include/c++/6.2.1
```


--- 
