# platform-qemu-cortex-m4f

Support files for building application to run on the QEMU "mps2-an386"
emulated board.

## Include folders

The following folders should be passed to the compiler during the build:

- none

## Source files

The source files to be added to user projects are:

- none

## Memory range

The applications are built for the following memory range:

- FLASH: 0x0000_0000-0x007F_FFFF (8 MB)
- RAM: 0x2000_0000-0x207F_FFFF (8 MB)
- HEAP: 0x6000_0000-0x60FF_FFFF (16 MB)
- stack: 0x6100_0000

The heap and stack are set automatically in `_startup()` to the values
returned by `SEMIHOSTING_SYS_HEAPINFO`.

## QEMU invocation

To run tests, pass the ELF file and the arguments:

```sh
qemu-system-arm --machine mps2-an386 --cpu cortex-m4 --nographic -d unimp,guest_errors --kernel "unit-test.elf" --semihosting-config enable=on,target=native,arg=test
```

For debug sessions start QEMU in GDB server mode by passing both `-s -S`:

```sh
qemu-system-arm --machine mps2-an386 --cpu cortex-m4 --nographic -d unimp,guest_errors -s -S --semihosting-config enable=on,target=native,arg=test
```

## Links

- [QEMU Arm](https://www.qemu.org/docs/master/system/target-arm.html)
- [virt](https://www.qemu.org/docs/master/system/arm/virt.html)
