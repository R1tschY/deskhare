/// \file comobject.cpp

#include "comobject.h"

#include <winbase.h>
#include <string>

#include "memory.h"

namespace WinQt {

QString Result::toString() const
{
  wchar_t* buffer;
  FormatMessageW(
      FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM
      | FORMAT_MESSAGE_IGNORE_INSERTS,
      nullptr,
      value_,
      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),  // TODO: use QLocale
      (wchar_t*)&buffer,
      0,
      nullptr);
  LocalPtr<wchar_t> bufferGuard(buffer);

  return QString::fromWCharArray(buffer);
}

} // namespace WinQt

