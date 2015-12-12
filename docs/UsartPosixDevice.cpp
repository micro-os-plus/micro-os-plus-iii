#include "UsartPosixDevice.h"

extern "C"
{
  int
  usart_open (const char *path, int oflag, va_list args);

  int
  usart_close (void);

  ssize_t
  usart_read (void *buf, size_t nbyte);

  ssize_t
  usart_write (const void *buf, size_t nbyte);

  int
  usart_ioctl (int request, va_list args);
}

UsartPosixDevice::UsartPosixDevice (const char* name) :
    os::PosixDevice (name)
{
  ;
}

UsartPosixDevice::~UsartPosixDevice ()
{
  ;
}

int
UsartPosixDevice::doOpen (const char *path, int oflag, va_list args)
{
  return usart_open (path, oflags, args);
}

int
UsartPosixDevice::doClose (void)
{
  return usart_close ();
}

ssize_t
UsartPosixDevice::doRead (void *buf, size_t nbyte)
{
  return usart_read (buf, nbyte);
}

ssize_t
UsartPosixDevice::doWrite (const void *buf, size_t nbyte)
{
  return usart_write (buf, nvyte);
}

int
UsartPosixDevice::doIoctl (int request, va_list args)
{
  return usart_ioctl (request, args);
}

int
UsartPosixDevice::doIsatty (void)
{
  return 1; // Yes, this is a TTY.
}
