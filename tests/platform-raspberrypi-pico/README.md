# platform-raspberrypi-pico

Support files for building Cortex-M0+ tests to run on the
Raspberry Pico board.

## Prerequisites

- [Raspberry Pico H](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html)
- [Raspberry Pi Debug Probe](https://www.raspberrypi.com/products/debug-probe/)

The **Pico H** is a newer version, with a small 3 pin connector soldered,
which can be directly connected to the Debug Probe, without any
custom wiring.

The **Debug Probe** is a cheap SWD (Serial Wire Debug) probe, implementing
CMSIS-DAP; for best results, this probe requires a recent OpenOCD (>= 0.12),
the current Raspberry OpenOCD fork (0.11.x) is too verbose for the use with
semihosting.

An alternate setup can be a pair of Pico's (preferably the initial model,
without the debug connector soldered) and some custom wiring, with one
Pico running the [Picoprobe](https://github.com/raspberrypi/picoprobe)
firmware.

## Include folders

The following folders should be passed to the compiler during the build:

- `cmsis-plus/platform.h`

## Source files

The source files to be added to user projects are:

- `bs2_default_padded_checksummed.S`

## Memory range

The applications are built for the following memory range:

- FLASH: 0x1000_0000-0x001F_FFFF (2 MB)
- RAM: 0x2000_0000-0x2003_FFFF (256 KB)
- stack: 0x2004_0000

## Eclipse OpenOCD launcher

Main tab
    C/C++ Application: absolute path to `app.elf`

Debugger tab
    OpenOCD Setup
        Executable path: absolute path to `openocd`
        Config options:
            `-f interface/cmsis-dap.cfg`
            `-c "adapter speed 5000"`
            `-f target/rp2040.cfg`
    GDB Client Setup
        Executable path: absolute path to `arm-none-eabi-gdb`
        Commands:
            `set mem inaccessible-by-default off`
            `set remotetimeout 4000`

Startup tab
    Initialization commands
        `monitor arm semihosting_cmdline test one two`
    Set breakpoint at: `_start`

## OpenOCD invocation

To run tests, invoke them via OpenOCD:

```sh
openocd \
      -c "tcl_port disabled" \
      -c "telnet_port disabled" \
      -f interface/cmsis-dap.cfg \
      -c "adapter speed 5000" \
      -f target/rp2040.cfg \
      -c "program test.elf verify" \
      -c "arm semihosting enable" \
      -c "arm semihosting_cmdline test one two" \
      -c "reset"
```

## Links

- [Pico SDK](https://www.raspberrypi.com/documentation/pico-sdk/)
