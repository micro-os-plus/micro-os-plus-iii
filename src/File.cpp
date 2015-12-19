/*
 * This file is part of the µOS++ distribution.
 *   (https://github.com/micro-os-plus)
 * Copyright (c) 2015 Liviu Ionescu.
 *
 * µOS++ is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, version 3.
 *
 * µOS++ is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "posix-io/File.h"
#include "posix-io/FileSystem.h"
#include "posix-io/MountManager.h"
#include <cerrno>

// ----------------------------------------------------------------------------

namespace os
{
  namespace posix
  {
    // ------------------------------------------------------------------------

    File::File ()
    {
      fType = Type::FILE;
      fFileSystem = nullptr;
    }

    File::~File ()
    {
      fFileSystem = nullptr;
    }

  } /* namespace posix */
} /* namespace os */

// ----------------------------------------------------------------------------
