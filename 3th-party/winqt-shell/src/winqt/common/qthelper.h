/// \file qthelper.h

#pragma once

#include <QString>

namespace WinQt {

inline const wchar_t* winStringView(const QString& str)
{
  return reinterpret_cast<const wchar_t*>(str.utf16());
}

}  // namespace WinQt
