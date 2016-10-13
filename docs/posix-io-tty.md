# POSIX TTY support

## Reference

- [Open Group](http://pubs.opengroup.org/onlinepubs/9699919799/)
- [General Terminal Interface](http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap11.html)

Functions

- [`tcsetattr()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcsetattr.html)
- [`tcgetattr()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcgetattr.html)
- [`tcsendbreak()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcsendbreak.html)
- [`tcdrain()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcdrain.html)
- [`tcflow()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcflow.html)
- [`tcflush()`](http://pubs.opengroup.org/onlinepubs/9699919799/functions/tcflush.html)

In UNIX it is possible to register a device with two names, one with modem signals (`/dev/ttyS0`), and one without (`/dev/cua0`).

Suggestion:

- for the given device name, register two devices, `/dev/xyz` and `/dev/xyx-local` (this is currently not possible with the current intrusive lists).
- add a new parameter to the constructor, to set the CLOCAL flag


