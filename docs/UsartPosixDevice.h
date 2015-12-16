#include "posix-io/PosixDevice.h"

class UsartPosixDevice : public os::PosixDevice
{
public:
  UsartPosixDevice (const char* name);

  virtual
  ~UsartPosixDevice ();

  virtual int
  doOpen (const char* path, int oflag, va_list args);

  virtual int
  doClose (void);

  virtual ssize_t
  doRead (void* buf, size_t nbyte);

  virtual ssize_t
  doWrite (const void* buf, size_t nbyte);

  virtual int
  doIoctl (int request, va_list args);

  virtual int
  doIsatty ();

private:

  // Add member variables, if needed
};
