//
// deskhare - cross-platform quick launcher
// Copyright (C) 2017 Richard Liebscher
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "sharedlock.h"

#include <QSystemSemaphore>

namespace Deskhare {

SharedLock::SharedLock(const QString& id)
: mutex_(id)
{
#ifndef Q_OS_WIN32
  QSystemSemaphore semaphore(id + "-mutex", 1);
  semaphore.acquire();

  // on linux/unix shared memory is not freed upon crash
  // so if there is any trash from previous instance, clean it
  QSharedMemory nix_fix(id);
  if (nix_fix.attach())
    nix_fix.detach();
#endif

  if (mutex_.attach())
  {
    hasLock_ = false;
  }
  else
  {
    mutex_.create(1);
    hasLock_ = true;
  }

#ifndef Q_OS_WIN32
  semaphore.release();
#endif
}

} // namespace Deskhare
