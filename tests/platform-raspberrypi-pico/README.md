# platform-raspberrypi-pico

Support files for building Cortex-M0+ application to run on the
Raspberry Pico board.

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
