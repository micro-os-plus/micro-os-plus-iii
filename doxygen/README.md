# Generating the µOS++ Reference pages

The reference pages are generated with Doxygen.

Warning: The current procedure is schematic and needs more work.

The destination folder is hard coded as `OUTPUT_DIRECTORY` in `config.doxyfile`.

```console
$ cd /Users/ilg/Library/xPacks/ilg/micro-os-plus-iii.git/doxygen
$ axbb
$ doxygen --version
1.8.14
$ doxygen config.doxyfile
```
