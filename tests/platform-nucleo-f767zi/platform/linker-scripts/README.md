# linker-scripts

The scripts used in the build are

- `mem.ld`
- `sections-flash.ld`

The sections are mainly from the µOS++, with some content moved from
flash to RAM, since accessing flash is quite expensive.

The `memmap_default_orig.ld` was copied from the Pico SDK, as a reference.
