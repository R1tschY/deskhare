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

#ifndef WINSTRING_REF_H
#define WINSTRING_REF_H

#include <memory>
#include <type_traits>
#include <windows.h>

namespace Windows {


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

//
// GeneralHandle

#define WINDOWS_DEFINE_HANDLE_DELETER(func) \
  template<typename PointerT> \
  struct func##Functor { \
    typedef PointerT pointer; \
    void operator()(PointerT handle) { \
      func(handle); \
    } \
  }

#define WINDOWS_HANDLE_DELETER(func) func##Functor

#define WINDOWS_DEFINE_GENERIC_HANDLE_TYPE(name, deleter) \
  WINDOWS_DEFINE_HANDLE_DELETER(deleter); \
  template<typename T> \
  using name = ::Windows::GeneralHandle<T, WINDOWS_HANDLE_DELETER(deleter)>;

#define WINDOWS_DEFINE_HANDLE_TYPE(name, type, deleter) \
  WINDOWS_DEFINE_HANDLE_DELETER(deleter); \
  using name = ::Windows::GeneralHandle<type, WINDOWS_HANDLE_DELETER(deleter)>;


template<
  typename PointerT,
  template<typename> class DeleterFuncT
>
using GeneralHandle = std::unique_ptr<PointerT, DeleterFuncT<PointerT> >;

//
// Handle

struct HandleDeleter {
  typedef HANDLE pointer;

  void operator()(HANDLE ptr)
  { CloseHandle(ptr); }
};

using Handle = std::unique_ptr<HANDLE, HandleDeleter>;

struct HandleExDeleter {
  typedef HANDLE pointer;

  void operator()(HANDLE ptr)
  { if (ptr != INVALID_HANDLE_VALUE) CloseHandle(ptr); }
};

using HandleEx = std::unique_ptr<HANDLE, HandleExDeleter>;

struct FindHandleDeleter {
  typedef HANDLE pointer;

  void operator()(HANDLE ptr)
  { if (ptr != INVALID_HANDLE_VALUE) FindClose(ptr); }
};

using FindHandle = std::unique_ptr<HANDLE, FindHandleDeleter>;

} // namespace Windows

#endif // WINSTRING_REF_H
