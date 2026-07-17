# Instrumentation Test

This test exercises several SEGGER System View use cases.

It runs currently on the nucleo-f411re board.

- possibly disable some tests in `cmake/global-definitions.cmake`; keep `ENABLE_RTOS_APIS_TEST`
- in `platforms/nucleo-f411re/cmake/definitions.cmake` enable `ENABLE_INSTRUMENTATION_TEST`
- `xpm run build --config nucleo-f411re-cmake-debug -C tests`

There are two tests:

- `build/nucleo-f411re-cmake-debug/platform-bin/instrumentation-test.elf` - simple 
- `build/nucleo-f411re-cmake-debug/platform-bin/rtos-apis-instrumentation-test.elf` - all API calls

Power up the board, connect the J-Link.

## Ozone

Start Ozone, and open one of:

- `platforms/nucleo-f411re/instrumentation.jdebug`
- `platforms/nucleo-f411re/rtos-api-test-instrumentation.jdebug`

Tools -> Debug Settings

- STM32F411RE
- SWD: 4 MHz
- USB

## SystemView

Copy `SYSVIEW_micro-os-plus-iii.txt` to SystemView (as admin):

- `micro-os-plus.github/xPacks/xPacks-3rd-party/segger-system-view-xpack.git/descriptions/SYSVIEW_micro-os-plus-iii.txt`
- `/Applications/SEGGER/SystemView_V410b/Description`

Start SystemView (Continue under SPL) and open project

- `platforms/nucleo-f411re/instrumentation.SVPrj`
