#include "UsartDevice.h"

extern "C"
{
  int
  usart_open (const char* path, int oflag, va_list args);

  int
  usart_close (void);

  ssize_t
  usart_read (void* buf, size_t nbyte);

  ssize_t
  usart_write (const void* buf, size_t nbyte);

  int
  usart_ioctl (int request, va_list args);
}

UsartDevice::UsartDevice (const char* name) :
    os::Device (name)
{
  ;
}

UsartDevice::~UsartDevice ()
{
  ;
}

int
UsartDevice::do_open (const char* path, int oflag, va_list args)
{
  return usart_open (path, oflags, args);
}

int
UsartDevice::do_close (void)
{
  return usart_close ();
}

ssize_t
UsartDevice::do_read (void* buf, size_t nbyte)
{
  return usart_read (buf, nbyte);
}

ssize_t
UsartDevice::do_write (const void* buf, size_t nbyte)
{
  return usart_write (buf, nvyte);
}

int
UsartDevice::do_ioctl (int request, va_list args)
{
  return usart_ioctl (request, args);
}

int
UsartDevice::do_isatty (void)
{
  return 1; // Yes, this is a TTY.
}
