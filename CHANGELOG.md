# Change & release log

Releases in reverse chronological order.

Please check
[GitHub](https://github.com/micro-os-plus/micro-os-plus-iii-/issues)
and close existing issues and pull requests.

## 2023-07-14

* v7.0.0
* 0d39158 tests/package.json: update devDependencies
* abcf6f4 package.json: minXpm 0.16.2

## 2023-07-13

* 39e0a50 tests/rtos-apis: cleanup workarounds
* b8480e5 cmake update for cubemx
* d862e0b package-lock.json update
* e841261 update cubemx code
* d8bf213 rename stm32f767cubemx
* 25d385b rename stm32f411cubemx
* 0fa468d READMEs updates
* 0de5218 app platform-nucleo-h743zi

## 2023-07-12

* 1d3e7ff package.json: add nucleo-f411re
* 0463f2d rtos-apis: add \n workaround
* 29e6f91 platform-nucleo-f767 update to cube 6.9.0
* 1cf5c82 add platform-nucleo-f411re
* 2f0f47e tests: separate platform sub-folder
* 137a8d6 rtos-apis main.c semihosting test commented out
* da25cd3 platform-nucleo-f767zi README update
* d672e21 openocd gdb_port disabled

## 2023-07-11

* feab5be package.json: 7.0.0-pre.2
* 95eb4d2 silence struct stat warnings

## 2023-07-05

* 1562b11 temporarily enable release test for f7676
* c5d2066 cmake cosmetics

## 2023-07-04

* 5460752 tests README update
* 6ff0f5e tests READMEs
* ae3315e package.json: fix pico link-deps
* 7ee1cde package.json: separate openocd-dependencies
* 06c0443 package.json: separate arm-cmsis-dependencies
* 1faa181 package.json: remove BuildVerbose
* 0d8e9c0 cmake cosmetics
* 622440b add platform-nucleo-f767zi
* 4d046e5 exit.c: OS_ENABLE_BKPT_ON_EXIT
* 67b84c9 assert.c: use OS_INCLUDE_ASSERT_FAILED
* 0a4a524 .vscode/settings.json: ignoreWords

## 2023-07-03

* f90828a add stm32cubemx initial content
* b823423 package.json: min 0.16.2, devDeps in new format
* fa469a5 README update
* 08485d5 README update
* eed10e9 tests/package.json: add xpm run test
* c295557 remove Eclipse files
* b03c306 README update
* 02838f1 move test to deprecated
* 0264f31 tests: uniform OS_TRACE_*
* e19082d add OS_TRACE_RTOS_SYSCLOCK_TICK_BRACES

## 2023-07-01

* 6dcbec8 sections-flash.ld: no vtable, .rodata
* 4e99e35 tests: prefix device & platform with MICRO_OS_PLUS
* 2e6cfb4 add raspberry pico platform
* a45ea79 .vscode/settings.json: ignoreWords
* 8fed17d tests/CMakeLists.txt: re-enable all tests
* 7a3af2a startup.cpp: start with empty line
* db903f8 test-iso-api.cpp: OS_INCLUDE_RTC_TEST

## 2023-06-30

* ecd75ef */platform.h: use ARM_CMSIS_VALIDATOR_IRQHandler
* 84ec251 os-hooks.h: silence warning
* 2359c3c cosmetics
* f773760 explicit include os-app-config.h
* 35406e0 use micro-os-plus::common instead of globals

## 2023-06-26

* 4801002 add platform-qemu-cortex-m3
* 671ee0f package-lock.json: update
* b8abacc cmsis-os-validator: disable INTERRUPT_PRIORITY (4)
* 09a2f28 cmsis_os.h: silence warnings
* 5ddd438 CMakeLists.txt cosmetics
* c60efc4 CMakeLists.txt add subdir "cmsis-os-validator"
* a09d110 globals.cmake cosmetics
* d877272 add tests READMEs
* 9692618 vectors-cortexm.c: add WDT_IRQHandler
* 6b640ba add platform.h

## 2023-06-25

* a66b37f CMakeLists: add include/cmsis-plus/legacy
* 7de8255 add cmsis-os-validator
* a3358ee silence -Wc++-compat
* 4d8b70f #if defined(OS_USE_OS_APP_CONFIG_H)
* 7a91b74 #elif defined(__APPLE__) || defined(__linux__)

## 2023-06-23

* 3fefc02 tests/rtos-api: OS_TRACE_RTOS_SYSCLOCK_TICK
* ad7ab37 mutex-stress: OS_INCLUDE_LIBUCONTEXT
* 2c021aa tests/CMakeLists: remove "tests-bin"
* 930884b add mutex-stress
* 826f8ba platform-m0: fix qemu machine & cpu
* c2f8bbc platform-cortex:  xpack_add_cross_custom_commands
* ef949f5 platform-native: move *_POSIX_STDOUT to CMake
* 6abbcdf platform-m7: move link options to globals
* 841ca41 platform-m0: move link options to globals
* a47a242 platform-native: move link options to globals

## 2023-06-22

* 957e694 tests/README update
* 3a77ffd tests/README update
* b317a58 reset-hardware.c: include diag/trace.h
* 950a47a update copyright notice
* bf7f3d3 add OS_DISABLE_RESET_HARDWARE
* 7e6f87e tests/README update
* 39c167b package.json: skip gcc tests Apple Silicon macOS
* e4e279d tests/README: add failures
* 1f69d51 #define OS_INCLUDE_LIBUCONTEXT
* 5560698 revert to OS_USE_TRACE_POSIX_STDOUT

## 2023-06-21

* 816a320 tests/README update
* f1200fe .vscode/settings.json: ignoreWords
* 9620a1d README-MAINTAINER update
* d4b594f package.json:  "7.0.0-pre.1"
* aac9c87 tests/README.md updates
* 538c2ef tests CMakeLists: fix typos
* cfe510a test-cpp-api.cpp: silence warning
* 6d51060 test-cpp-api.cpp: comment out struct
* 3eea085 rtos-apis/os-app-config.h:  INTERRUPT_PRIORITY
* 7c4fd5a globals.cmake cleanups
* abc9b65 add platform-qemu-cortex-* tests
* 5d5a4cc platform-native: fix map.txt
* 8367cb7 platform-native: move PLATFORM-NATIVE to globals
* 3b41d85 platform-native: clean unused includes
* 70dad18 exception-handlers.c: init SCB->VTOR
* 2ca9340 exception-handlers.s: enable FP
* 669fb75 exception-handlers.c: MSR msp
* 42b47e5 comment out redundant struct
* 2d085be reset-hardware.c: __builtin_unreachable()
* 573b1cb reset-hardware.c: no redundant NVI_SystemReset()
* aea9aa8 startup.cpp: use C++ casts
* 54a052c startup.cpp: remove redundant _start() definition
* 6ef4e46 add #pragma to silence gcc warnings
* 149c94b use __asm__
* c03454a semihosting.h: add __cplusplus extern "C"

## 2023-06-18

* 4b60deb package.json: remove devDeps & deep-clean
* f0fa5b3 .vscode/settings.json: xpack & npm exclude
* 80b6c28 MIT link in lower case

## 2023-06-16

* e69d7f6 tests/package.json: remove npm scripts
* 713bc7b .vscode/settings.json: exclude tests
* d1c5cfc README-MAINTAINER update
* 4835904 cmake update for tests sub-folder
* e06f693 .vscode/settings.json: xpack.exclude
* 4bf3d6f add tests/LICENSE
* 1229e4f move package.json to tests/package.json
* 4521dbb README-MAINTAINER update
* 1e7a2aa cmake: disable -rtlib=compiler-rt, it fails on arm
* 70da846 cmake: -flto on Release on all platforms

## 2023-06-15

* 35537b2 cmake: use -Wl,-rpath instead of LD_LIBRARY_PATH
* 37bb3bf README-MAINTAINER update
* 5edf99e cmake: add -lunwind for clang
* 82ef64d cmake: use -rtlib=compiler-rt
* b1b36a6 test: comment out fflush()
* 625dca1 os-version 7.0.0-beta
* 5889133 cmake: -fuse-ld=lld only on non Apple
* 6cab4ab cmake: -stdlib=libc++ only on non Apple
* 330b3d1 os-app-config.h: use _FWRITE_STDOUT
* 60ac088 native/cmake: -stdlib=libc++ only for C++
* c56015b cmake: -stdlib=libc++ for clang
2023-06-14 * 53e4f4d README-MAINTAINER update

## 2023-06-13

* 69e346e README-MAINTAINER update
* dd2e698 README-MAINTAINER update
* 440acc8 .vscode/settings.json: ignoreWords
* 2bb94e3 add README-MAINTAINER.md
* 4adad52 platform-native: add libucontext
* 7f6824f package.json: add @xpack-3rd-party/libucontext

## 2023-06-11

* 9facdb5 .vscode/settings.json: ignoreWords
* fecf169 remove trailing dots in messages
* 9d4afc2 os-clocks.cpp: use {t t} in os_systick_handler()
* 34abf78 test-cpp-api.cpp: move blk definition down
* 3bc89ca os-app-config.h: enlarge stack size for posix
* 989d669 globals.cmake: commented out -g
* d88677d #84 os-thread.h: always abort() on stack underflow

## 2023-06-09

* 8a61d7a add launch.json
* db84bd4 .vscode/settings.json: displayFormat hex
* 3acaa18 os-sched.h: update {C C} {U U}
* 128197a tests: silence warning
* 508c403 tests: update thread_state[]
* b3681be tests: add fflush(stdout)
* 72d9ea5 tests cosmetics, remove trailing dot

## 2023-06-08

* 0209dc3 libcpp/new.cpp: operator delete() with #pragma
* 7fd8c36 estd/thread: fix clang warning
* 2c82f26 platform-native: use LD_LIBRARY_PATH
* 37f7625 add get-libraries-paths.sh
* 115a918 add #pragma to silence warnings
* 68b3399 .vscode/settings.json: ignoreWords
* 92de959 package.json: add actions for all toolchains
* 4c0b3a4 platform-native/cmake/global.cmake warnings
* 4f6e9f2 cosmetics
* 219ea63 driver/usb-*: add @todo
* 3748139 estd/memory_resource:  redundant __throw_bad_alloc()
* 36e6015 posix-io/types.h: redundant os_startup_initialize_args
* 8d9fe1b os-decls.h: remove redundant os_main();
* eb8ce9c os-sched.h: remove redundant cpu_cycles()
* d4043b1 os.h: include os-c-api.h
* a94c843 new.cpp: remove duplicate operator delete()
* f8e049f block-device.cpp: add static_cast<>
* 5762200 block-device.cpp: add default switch case
* aea6bf7 os-idle.cpp: duplicate os_rtos_idle_actions()
* 80f7d3f os-mempool.cpp: add static_cast<>
* e17500a os-thread: fix exception missing reference
* 7ed1e25 tests/rtos-apis: add casts
* 3e31197 comment out redundant struct and class
* 920d244 silence warnings with #pragma
* 39a79e7 #83: make os::posix::io::type an enum class
* e8c16a2 #82: rename get_file_system()

## 2023-06-07

* a073233 silence clang warnings
* 3f2262c remove empty statements to silence clang

## 2023-06-06

* fecfe0c clang -Wno-unknown-warning-option
* d47e114 update for micro-os-plus-iii-posix-arch
* f89d19f package.json: add del-cli devDependence
* da91a82 package.json: add xpack actions & build configs
* 90e8d2a add CMakeLists.txt micro-os-plus::iii
* 598d429 .gitignore
* fe4a0a7 att tests with platform-native
* accb633 cosmetics: separator lines
* 93bea58 fix extra ; (formatter bug)
* f192d75 usart-wrapper.h: remove duplicate members
* 99a6d82 add missing override
* 7cb3154 fix trailing comma in enums
* 39ebf4f silence clang warnings
* 8cfaea6 fix doxygen clang warnings

## 2023-06-05

* da1b5e2 os-thread.cpp: fix typo
* eb8cd21 #81: update is_constructed() documentation
* d4b9c92 add CHANGELOG.md
* 6a05683 .npmignore updates
* 84c3c93 package.json: updates

## 2023-06-01

* 0506af2 assert.c: os_irq_critical_enter ()
* e63f613 os-thread.cpp: doxy update
* 2b72a1f #81: remove func_ from reuse logic checks
* 75eedc7 #81: rename th_enable_assert_reuse
* 2e41ede #81: add th_assert_reuse to .c code
* 8b5e375 #79: !os_irq_in_handler_mode() in assert.c
* 5467d62 #79: prefix message with \n

## 2023-05-31

* ddcebba #81: add thread::is_constructed()
* 62e0234 #81: OS_INTEGER_RTOS_REUSE_MAGIC on release too
* 3ce1dad #81: rename th_assert_reuse
* d4757e8 modules.dox: fix typo
* ff7b795 #81: make assert logic more robust
* 21139c0 #81: check th_check_reuse
* 9fc7791 #81: add thread attribute::th_check_reuse
* a88e891 #81: add OS_INTEGER_RTOS_REUSE_MAGIC
* 9795a01 #81: add thread state::initializing
* 0920f5d #80: avoid already gone message

## 2023-05-30

* b7ec1e4 #79: display current thread on asserts
* 9a9d4f3 doxygen: If the _attr_ attributes...
* 9c0ead8 #79: display current thread on asserts
* 2b1cc45 #78: add support to clear statistics
* ba9d62b test-iso-api.cpp: skip std::thread tests
* ee51b65 thread_internal.h: silence cast warning

## 2023-05-29

* 0cb1659 os-c-wrapper.cpp: @ref syntax
* 8031082 exit.c: doxygen metadata @param code
* 1c446ae doxygen metadata: @par returns Nothing
* 9592083 main.md: update

## 2023-05-28

* 9d9ff03 logo.png: make transparent
* 669d3d4 main.md: change svg
* fe9c10a main.md: cosmetics
* 0e9bb2d main.md: remove card link
* f7a3ccb add package.json
* b02af6d main.md: use cards
* 3060ec1 doxygen/pages: rename .md
* 8b10118 main.markdown: update
* 9e6fb75 doxygen/pages: rename .dox

## 2023-05-25

* 4aae11c README update
* 435c858 .vscode/settings.json: ignoreWords
* 77abaec launchers update for gnumcueclipse
* 1c09a40 add tests/README.md
* e13752c test/sema-stress: increase stacks
* 2c9992e test/sema-stress: fix extern
* 16a442b thread: silence unary_function<>
* 7c8baff add OS_INCLUDE_STD_THREADS
* d34b2fb os-c-wrapper.cpp: silence cast-function-type
* 21562c6 os-c-wrapper.cpp: silence sign-compare
* 17a8598 net-stack.cpp: silence -Wnonnull
* fb38d93 os-decls.h: comment out constructors/operators
* c0277b6 io.h: silence socket shadow
* d04c71c c-syscalls-aliases-standard.h: fix __posix_tcdrain
* 2fe30f2 add override
* c68687f trace.cpp: fix trace_vprintf(...,  va_list args)

## 2023-05-21

* d3ab18d os-versions.h:6.3.18-beta
* 81f2ab2 prepare 6.3.17
* 0869a4b os-version.h: remove -beta
* 77b2b44 os-memory.h: disable doxy for operator()
* 5910137 c-syscalls: disable doxygen

## 2023-05-08

* 2f1a3b8 #75: cannot clear members in destructor

## 2023-05-04

* 46e2836 #76: Exclude socket definitions used by LWIP

## 2023-05-03

* 9ecdf5d #75: Revert statically initialised lists to BSS
* a03bcaa os-version.h: bump 6.3.17-beta
* 466c8d6 prepare v6.3.16
* 1b1fcf0 #74: add os_thread_kill()

## 2021-01-13

* 9581626 remove package.json, it is not an xPack
* 8fc1277 README: revert to pre-xPack status

## 2020-12-23

* ce4a741 module-rtos.doxygen: fix typos

## 2020-11-20

* 8243a00 README: license badge
* 1e118cd Merge commit '021db18b01147cb2441aef9e7f49ae06de00aca1' into xpack
* 021db18 cleanups for first xPack release
* 9a6f9ac cleanups for first xPack release

## 2020-10-12

* 096dfc3 Merge pull request #57 from LnnrtS/fix_exit_no_return
* 7b6b984 missing noreturn attribute for _exit fixing to remove compiler warning

## 2020-10-06

* 9508c96 Merge pull request #55 from LnnrtS/topic-50
* 63655cd messages of message_queue need to be at least the size of a pointer. Check as early as possible for this requirement to not fail with a less specific assert().

## 2018-11-21

* a6b4410 [#48] posix-io/char-device.h: add override
* 4b5e21b os-version.h: bump 6.3.16-beta

## 2018-11-19

* 1ea30f6 prepare v6.3.15

## 2018-11-09

* e8bea4f [#47] stack::available() comment (size in bytes)

## 2018-10-25

* e73681f README: doxygen version notice
* d9fd328 [#46]: protected mutex, do not kep ownership if error
* 87c694d os-version.h: 6.3.15-beta
* bfdd9c3 [#45]: rtos/os-core.cpp: check locked()

## 2018-08-15

* 3b61444 os-thread.cpp: explain TODO in inherited

## 2018-08-13

* 689399c os-thread.cpp: priority_inherited() doxygen

## 2018-07-04

* da2aa42 reorder changes in reverse

## 2018-07-03

* 0d58744 change-log: add #44 bug fix
* 27b2f34 change-log: prepare v6.3.14
* 0d770b6 README: prepare 6.3.14
* fff03f4 os-version.h: remove -beta
* 107b1d5 [#44] os_main.cpp: add os_main_thread cast

## 2018-07-02

* f61c709 test/rtos: enable all Chan tests
* 6d7c88d test/rtos: fix block device ram addressing
* f00c652 memory: rename internal_align_

## 2018-06-29

* f974153 posix-io/file-descriptors: use const reserved__
* 975676f posix-io/file-descriptors: add used()
* 7ee4702 posix-io/file-descriptors: ren allocate/deallocate

## 2018-04-15

* 6742761 tests/rtos: use posix::opendir()
* 5ad09cb tests/rtos: use posix::statvfs
* 5b0624d posix/dirent.h: add comments & link to POSIX
* be67fba posix: directory close() add trace message
* 308b5d4 posix: fix file_system statvfs friend
* 4ae4998 posix: directory memset dir_entry
* 5a9d24f utils: lists.cpp explain link asserts
* cd0fed6 test/rtos: skip some tests on mac
* e60146f libc/stdlib: timegm.c add cast
* 7f2ec8b test/rtos: simplify my_block
* d5dadf5 test/rtos: remove is_opened()
* 102785f posix: adjust path skips extra '/'
* 7f6025e posix: static opendir rewritten

## 2018-04-14

* 41ab8cd test/rtos: skip some tests that fail on apple
* aad63d5 test/rtos: more chan test cases
* 3a2a6d7 [#41] add timegm()
* e7931eb posix: remove is_open from vmount

## 2018-04-12

* db1c64e tests/rtos: increase dynamic size to 14 KiB
* 54fcbc1 cosmetics
* fd34905 memory: add class name to trace output
* b584330 memory/first_fit_top: add do_align()
* 589b030 test/rtos: disable fat32 test
* 10a8908 test/rtos: main stack 4000
* 6692414 posix: deallocate in file_system_impl destructor
* 5b7e61b add preliminary chan test
* d0e8721 posix: more validations
* c68d2bb posix: explicit clear() for static lists
* f81ba5f posix: check is_opened()
* 2b75771 posix: add is_opened() to io
* 3a25773 posix: add more overrides

## 2018-04-10

* 6e56da7 posix: make file_system_ a reference
* 0eb705f posix: add friend class with self classes
* 2b9e889 posix: fix sloppy references (offset, open_count)
* e82bf00 posix: remove self references

## 2018-04-06

* 84eb384 [#40] posix: add statvfs()/fstatvfs()
* cfbcf1d posix/file-system: make most impl abstract
* 210cd39 posix/termios.h: update for POSIX

## 2018-04-05

* e609eb3 fix new clang 9.1.0 warnings
* 4a91f23 posix: standard check for POSIX uses unistd.h
* 3b19242 [#36] posix: add tc*() to aliases

## 2018-03-31

* 6399cc2 posix: BLOCK_DEVICE, CHAR_DEVICE

## 2018-03-30

* 648c3d4 doxygen/README update
* 3b65d35 rtos: use OS_IS_CROSS_BUILD
* 158613d rtos: use OS_HAS_STD_THREADS
* 641c1de [#39] add OS_IS_CROSS_BUILD & OS_HAS_STD_THREADS
* 4941c73 [#38] abi note for gcc 7.1

## 2018-03-29

* 286115c test/rtos: activate posix on macos
* d2d63d0 memory: init_once_default_resource()
* d522b30 doxygen/custom.css: update h2.memtitle
* e2f141c update doxygen 6.3.14 configuration
* 6720071 update doxygen meta-data

## 2018-03-28

* 8a83788 test/rtos: add simple posix io test
* 61d6741 test/rtos: main.cpp print errno
* cb5ac98 test/rtos: use explicit estd and separate std
* 870eedf posix: cosmetics
* 2ca525c posix/block-device: add sync
* ca06f15 posix: silence warnings
* 5706d74 posix/block-device: blknum_t as size_t
* c77afa0 posix/tty: add tcdrain()
* cf7ebee posix/c-syscalls-posix: gettimeofday cast
* e3e3a15 posix: preliminary socket, net-stack, net-interface
* 0c806e1 memory/first-fit-top: fix warnings
* 9c549f1 estd: thread in both std & estd; thread::id; hash
* d64177f estd: mutex & condition_variable aliased to std
* 8826516 rtos: add comments before asserts
* 5754380 estd: update guard macros
* 5246710 posix/sys: add pragma GCC system_header

## 2018-03-27

* 36d569c doxygen: add OS_INCLUDE_ATEXIT_STATIC
* d5829df [#15] libs/atexit: add OS_INCLUDE_ATEXIT_STATIC
* cd23f16 memory/first-fit-top: cosmetics & reformat
* ca34c5d Merge pull request #19 from imatic01/alignment

## 2018-03-26

* 73193aa [#17] fix _LITE_EXIT redefinition
* 84df980 [#37] add tcdrain() to POSIX I/O
* 4690fa4 [#35] add missing _fini()
* 711eccf [#36] add tty functions to the POSIX C API
* a77d4da posix: update type_ field
* 6fc7512 posix: rename char_device, block_device
* de67234 rtos/thread: auto*

## 2018-03-25

* 9d97646 posix: add variadic constructors to file_system
* e877b6d posix: make tty & char template constructor variadic
* e26f464 posix: cosmetics
* 3bc43f4 posix/tty: fix parent tty, not char
* 3177994 posix: #ifdef trace
* d7382d4 posix/device-char: add defaults do_lseek, do_sync
* 3882a68 posix-io: add impl() to all class templates
* 31091ee posix-io: use OS_USE_OS_APP_CONFIG_H
* 3371f44 posix: migrate some estd to std

## 2018-03-24

* e75d26e doxygen: OS_TRACE_UTILS_LISTS
* 47f9737 rtos: reformat
* 72af49a rtos/thread: unlock dangling mutexes
* a89276e rtos/mutex: cosmetics
* 07ce43a rtos/mutex: mark_owner_dead() no need to resume
* 36320a4 rtos/mutex: always link & count
* 97a25c1 rtos/mutex: add internal_unlock_()

## 2018-03-23

* 13653c1 [#34] move thread, mutex & condition_variable to std::
* 04bc13b doxygen: OS_USE_NAMESPACE_ESTD
* 6cf11fb [#33] rtos: update condition_variable
* 7326621 [#32] rtos: remove locks from estd::mutex
* b08908f [#31] estd/mutex: fix try_lock()
* bb31fe7 [#30] rtos: relax mutex ownership
* a6ec159 doxygen: OS_DISABLE_CORTEXM_SET_MSP_VIA_VTOR
* da3e4dd [#29] Add thread top try/catch for exceptions

## 2018-03-22

* 53a24cf [#28] disable local atexit() if exceptions

## 2018-03-21

* fb36fad posix-io: conditional compile for trace
* 3b88ddb posix-io: add support for lockers
* 32cbef2 [#27] Display main stack range in startup
* 5e6301a [#26] Make the os_main() year configurable
* da612e7 [#25] intrusive_list: add unlink_*()

## 2018-03-13

* 3d65970 posix-io: move mount manager inside file_system
* bf2ed3f posix-io/directory: implement
* 6fd590c posix-io: trace::printf()

## 2018-03-11

* 9fc9828 posix-io: fix open/vopen
* b521e29 posix-io: fix mount/umount
* 8c88c79 posix-io: change sync() to void
* efeeae2 posix-io/device: trace::printf() messages
* 14243dc posix-io/file-system: add device_block&

## 2018-03-10

* 6f25fd9 posix-io/device: add sync()
* 87e94ca posix-io: add physical block size
* c793d9a posix-io/device-char: cleanups
* f7ccdc8 posix-io: do_istty() true only in tty
* a7a02d9 posix-io: trace::printf() with class name

## 2018-03-09

* 8ae9f45 posix-io/io: remove unused variable
* 574d7ad posix-io/device-blocks: validate blknum+nblocks
* 8ba63db posix-io/file-descriptors-manager: size + 3
* 85008ad posix-io/file: add do_isatty() returning false
* 5d604444 posix-io/file-system: add mount/umount, empty
* 6ee02b6 posix-io: add device-block-partition
* acabcf3 posix-io: device-char derived from device
* ffb3cfc posix-io: use single device-registry
* a82157a posix-io: add trace messages
* 81000ad posix-io: format

## 2018-03-06

* cdf2a9b [#24] make io more abstract
* 2cd0279 [#23] Add posix::device_block
* 4e9f27b [#22] Move lseek() from posix::file to posix::io
* 4a59281 [#21] Extend intrusive_list
* 5d9f37f [#20] Add sys/ioctl.h

## 2017-10-17

* 4aec4d3 Added arbitrary power of two alignment for first-fit-top and lifo allocators.

## 2017-10-11

* 76b3692 doxygen/pages/os-app-config: remove duplicate

## 2017-10-09

* b3d5e79 [#16] libc/_sbrk.c: unsigned int linker symbols

## 2017-10-07

* 79425dc v6.3.14-beta

## 2017-10-06

* 4c02efe rtos/os_core: use tiny_thread
* ff51dc0 rtos/thread: move errno_ first

## 2017-08-29

* 410445a posix-io/c-syscalls-posix: fix warning

## 2017-08-26

* 3f18238 v6.3.13
* 31a824f posix/*.h: update guard macro
* 49923bd posix-io/tty.h: update termios.h path
* 8b7f946 add posix-io/tty class
* 5b15e3e add posix/termios.h
* 797a6f4 posix-io/c-syscalls-posix.cpp: gettimeofday()

## 2017-08-24

* 2c93251 v6.3.13-beta
* a56ed2a v6.3.12
* 38634b9 replace CMSIS++ references with µOS++

## 2017-08-23

* e45781f Merge branch 'xpack' into develop
* 090af4c Merge pull request #13 from sam-bristow/xpack
* 13b674e Fix typos in documentation
* 4147d23 Fix spelling error in pool API

## 2017-07-06

* 4ad092a remove not functional badge
* 7478074 Merge pull request #9 from codacy-badger/codacy-badge
* 32f6fe0 Add codacy badge
* 8e39ae2 Merge pull request #12 from armint/listfix
* 5896ac4 Fix operators ++ and -- in list iterator

## 2017-07-03

* 7816567 #11, diag/trace-semihosting.cpp

## 2017-02-14

* 953a0a5 rtos/os-memory.h: allocator_traits workaround

## 2017-02-10

* 9f370f7 os-app-config.h: define OS_USE_TRACE_POSIX_STDOUT
* d879da3 Merge branch 'xpack' into develop
* ec2d48b doxy/config-common.doxyfile: get rid of ##
* 9ff81c1 .gitignore: all .vscode

## 2017-02-08

* 3b41642 doxygen: quiet
* 63b72c1 doxygen: HAVE_DOT = NO
* ad5d937 doxygen: update STRIP_FROM_PATH
* 3bcefc5 doxygen: add travis config

## 2017-01-22

* a5cff17 Add Codacy badge
* 90d73ef os-version.h: 6.3.12-beta
* b886c51 v6.3.11

## 2017-01-01

* 434db36 os-mqueue.cpp: remove wrong assert from *_receive()

## 2016-12-30

* fb26930 os-mqueue.cpp: remove wrong assert from receive()

## 2016-11-28

* b891078 Merge branch 'xpack' into develop
* 7b3ded6 update README version 6.3.10

## 2016-11-27

* ce74e3c os-version.h: 6.3.11-beta
* 5b6c586 v6.3.10
* 057951e startup/initialize-hardware.c: use __ARM_FP

## 2016-11-15

* f7577d8 first-fit-top: fix free list chunk init
* 80adae2 os-clocks: add check_timestamps after update
* 19d8ed6 os-main: greeting version
* a51bd2d os-version.h: 6.3.10-beta

## 2016-10-13

* f980730 v6.3.9
* 21fb0d4 more posix-io docs
* 92da4ec xpack.json: version 6.3.8
* 57708d3 packet title "µOS++ IIIe"
* 9bf8d68 update README from xpack.json
* 64dd844 use USE_FREERTOS in os-app-config.h

## 2016-10-12

* aa3eeaa rename micro-os-plus-iii-xpack
* dd8f9e8 doxy output folder absolute
* e81431e rename /test/
* 78ed7dd tests: add subfolders

## 2016-10-11

* 19d01ae Merge branch 'xpack' of https://github.com/micro-os-plus/cmsis-plus into xpack
* bae15d9 rtos/os-clock: add update_for_slept_time()

## 2016-10-08

* 4b050d0 xpacks-helper: add _test_xpack()
* eb4583a xpacks-helper: remove do_check_micro_os_plus
* 74519a4 rtos/os-main.cpp: display interrupts stack size
* fa0b5af diag/trace.cpp: puts accepts empty string
* 9951756 add xpacks-helper.sh

## 2016-10-07

* 98a0a8c rtos/os-thread: assert interrupts stack size

## 2016-10-05

* afe4c37 v6.3.8
* 885b3c0 utils/lists: add doxy grouping

## 2016-10-04

* 937c9ee posix-io: use lists for device-char-registry
* bcab8ec posix-driver: add trace in constructors
* 27a9489 driver: add trace in constructors
* fa26c9a mode base lists to utils
* 169f4d2 posix-driver: update to lowercase
* a6863e8 move syscall aliases to include/posix-io
* 2f89de5 posix-io & posix-driver: fill-in the rule of five
* eed8f98 doxy modules: add POSIX I/O C++ and C
* 7ffd0c3 config.doxygen: add POSIX++ to name
* 4127dfc c-posix-syscalls: parenthesis

## 2016-10-03

* d6846ba posix-io & posix-driver: add doxy grouping
* ff4020a modules.doxygen: split into multiple files
* c0e3029 c-posix-syscalls: use namespace os::
* 26d2681 posix-io: syscall aliases
* bff9181 c-posix-syscalls.cpp moved to posix-io
* a266701 posix-io: refurbish to all lowercase
* 2d76cd5 posix-io: rename classes to lower case

## 2016-10-02

* ada3137 posix-driver: remove not used CmsisUsartCharDevice
* c8a0950 posix-driver: rename circular_buffer_bytes
* 3a75c01 posix-driver: add circular-buffer template
* 66fb393 rename folder '*driver' (singular)
* 1b5871e drivers: fix clang build
* e19a93c posix-io: reindent
* 61be6e0 update posix-drivers paths & macros
* e3cfa33 Add 'posix-drivers/' from commit '0a72a2cf7c38fd748a598025b74ac81f436fa1b7'

## 2016-10-01

* 0a72a2c update posix-io paths
* 36ef410 update posix-io paths
* 260abcc add vscode.mac.command
* 7950707 integrate posix-io into main tree
* 983d2b3 Add 'posix-io/' from commit '7a15bc8c54a7225d7ea8060688f856ceeec9489b'
* 668d207 diag/trace.h: fix pragma warning
* 7a15bc8 update licence to MIT
