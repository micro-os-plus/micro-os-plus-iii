# platforms/nucleo-f767zi

Note: It cannot be a library, since it makes extensive use of `weak` symbols.

DO NOT include `startup_stm32f767xx.s`.

## OpenOCD invocation

To run the tests, invoke them via OpenOCD:

```sh
openocd \
      -c "gdb port disabled" \
      -c "tcl port disabled" \
      -c "telnet port disabled" \
      -f interface/stlink-dap.cfg \
      -c "adapter speed 5000" \
      -f target/stm32f7x.cfg \
      -c "program test.elf verify" \
      -c "arm semihosting enable" \
      -c "arm semihosting_cmdline test one two" \
      -c "reset"
```

An alternate possible configuration:

```sh
openocd \
      -c "gdb port disabled" \
      -c "tcl port disabled" \
      -c "telnet port disabled" \
      -f interface/stlink-dap.cfg \
      -f target/stm32f7x.cfg \
      -c "program rtos-apis-test.elf verify" \
      -c "arm semihosting enable" \
      -c "arm semihosting_cmdline rtos-apis-test" \
      -c "reset halt" \
      -c "cortex_m maskisr on" \
      -c "resume"
```
