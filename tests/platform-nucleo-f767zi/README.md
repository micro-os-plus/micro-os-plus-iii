# platform-nucleo-f767zi

Note: It cannot be a library, since it make extensive use of `weak` symbols.

DO NOT include `startup_stm32f767xx.s`.

## OpenOCD invocation

To run tests, invoke them via OpenOCD:

```sh
openocd \
      -c "gdb_port disabled" \
      -c "tcl_port disabled" \
      -c "telnet_port disabled" \
      -f interface/stlink.cfg \
      -c "adapter speed 5000" \
      -f target/stm32f7x.cfg \
      -c "program test.elf verify" \
      -c "arm semihosting enable" \
      -c "arm semihosting_cmdline test one two" \
      -c "reset"
```
