## RTC Driver

- read hardware regs
- write hw regs
- write alarm (array)
- read alarm
- capabilities : nr. of alarms

## Cortex-M code in the portable core

The core is meant to contain only portable code, with all the
architecture-specific code in the port repositories. However, a
significant part of the Cortex-M startup and diagnostics layer lives in
the core, guarded by `__ARM_ARCH_PROFILE == 'M'`:

- `src/startup/exception-handlers.c` and
  `include/cmsis-plus/cortexm/exception-handlers.h` - the vector table,
  the weak `Reset_Handler()`, and the fault handlers;
- `src/startup/initialize-hardware.c` and `src/startup/reset-hardware.c`;
- `src/startup/startup.cpp` (`<cmsis_device.h>`, `__enable_irq()`);
- `src/libc/stdlib/exit.c` (debugger breakpoint);
- `src/diag/trace-itm.cpp` and `src/diag/trace-segger-rtt.cpp`;
- `src/semihosting/c-syscalls-semihosting.cpp` and
  `include/cmsis-plus/arm/semihosting.h` (Arm semihosting, selected by
  `__arm__` and the profile).

This is a design issue: these files are not part of the port contract
(`port/os-decls.h`, `port/os-c-decls.h`, `port/os-inlines.h`,
`os-core.cpp`), and the AArch32, AArch64, and RISC-V ports will need
their own startup and fault code.

Moving this code into the Cortex-M port (and defining a startup
contract for the other ports) would change the build of existing
applications, which now get it from the core, so it is a breaking
change. It is deferred to a future major release; until then, new
Cortex-M code (for example the stack limit checks) may be added to
these files, but must be guarded as described in `.claude/project.md`,
and should be kept small and isolated, to ease the later move.
