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

#pragma once

#include <winbase.h>
#include <memory>

namespace WinQt {

//
// Local memory

namespace Detail {
struct LocalDeleter {
  void operator()(void* ptr) {
    LocalFree(ptr);
  }
};
} // namespace Detail

template<typename T>
using LocalPtr = std::unique_ptr<T, Detail::LocalDeleter>;

// OLE

struct OLEDeleter {
  template<typename T>
  void operator()(T* ptr) {
    CoTaskMemFree(ptr);
  }
};

template<typename T>
using OLEPtr = std::unique_ptr<T, OLEDeleter>;

// COM

struct COMDeleter {
  template<typename T>
  void operator()(T* ptr) {
    ptr->Release();
  }
};

template<typename T>
using COMPtr = std::unique_ptr<T, COMDeleter>;

} // namespace WinQt
