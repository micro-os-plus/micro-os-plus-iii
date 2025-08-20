# platforms/nucleo-f411re

Note: It cannot be a library, since it makes extensive use of `weak` symbols.

DO NOT include `startup_stm32f411xe.s`.

## OpenOCD invocation

To run the tests, invoke them via OpenOCD:

```sh
openocd \
      -c "gdb port disabled" \
      -c "tcl port disabled" \
      -c "telnet port disabled" \
      -f interface/stlink-dap.cfg \
      -c "adapter speed 5000" \
      -f target/stm32f4x.cfg \
      -c "program test.elf verify" \
      -c "arm semihosting enable" \
      -c "arm semihosting_cmdline test one two" \
      -c "reset"
```
