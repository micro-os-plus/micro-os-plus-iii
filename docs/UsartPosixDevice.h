#include "posix-io/Device.h"

class UsartDevice : public os::posix::Device
{
public:
  UsartDevice (const char* name);

  virtual
  ~UsartDevice ();

  virtual int
  do_open (const char* path, int oflag, va_list args);

  virtual int
  do_close (void);

  virtual ssize_t
  do_read (void* buf, size_t nbyte);

  virtual ssize_t
  do_write (const void* buf, size_t nbyte);

  virtual int
  do_ioctl (int request, va_list args);

  virtual int
  do_isatty ();

private:

  // Add member variables, if needed
};
