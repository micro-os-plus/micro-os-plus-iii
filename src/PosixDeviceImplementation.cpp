/*
 * PosixDeviceImplementation.cpp
 *
 *  Created on: Dec 10, 2015
 *      Author: ilg
 */

#include "posix-io/PosixDeviceImplementation.h"
#include <cassert>
#include <cstring>

namespace os
{

  // --------------------------------------------------------------------------

  PosixDeviceImplementation::PosixDeviceImplementation (const char* name)
  {
    fName = name;
  }

  PosixDeviceImplementation::~PosixDeviceImplementation ()
  {
    fName = nullptr;
  }

  // --------------------------------------------------------------------------

  bool
  PosixDeviceImplementation::matchName (const char* name) const
  {
    assert(name != nullptr);
    assert(fName != nullptr);

    return (std::strcmp (name, fName) == 0);
  }

} /* namespace os */

// ----------------------------------------------------------------------------
