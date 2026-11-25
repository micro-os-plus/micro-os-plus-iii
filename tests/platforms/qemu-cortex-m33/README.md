# platforms/qemu-cortex-m33

Support files for building application to run on the QEMU "mps2-an505"
emulated board.

## Include folders

The following folders should be passed to the compiler during the build:

- none

## Source files

The source files to be added to user projects are:

- none

## Memory range

The applications are built for the following memory range:

- FLASH: 0x1000_0000-0x101F_FFFF (2 MB)
- RAM: 0x1020_0000-0x103F_FFFF (2 MB)
- stack: 0x1040_0000

The heap and stack are set automatically in `_startup()` to the values
returned by `SEMIHOSTING_SYS_HEAPINFO`.

## QEMU invocation

To run tests, pass the ELF file and the arguments:

```sh
qemu-system-arm --machine mps2-an505 --cpu cortex-m33 --nographic -d unimp,guest_errors --kernel "unit-test.elf" --semihosting-config enable=on,target=native,arg=test
```

For debug sessions start QEMU in GDB server mode by passing both `-s -S`:

```sh
qemu-system-arm --machine mps2-an505 --cpu cortex-m33 --nographic -d unimp,guest_errors -s -S --semihosting-config enable=on,target=native,arg=test
```

## Links

- [QEMU Arm](https://www.qemu.org/docs/master/system/target-arm.html)
- [virt](https://www.qemu.org/docs/master/system/arm/virt.html)
