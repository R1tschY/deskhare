/// \file comobject.h

#pragma once

#include <qstring.h>
#include <windows.h>
#include "memory.h"

namespace WinQt {

// fwd decls

template<typename Interface>
class Idl_;

// helper

class Result
{
public:
  Result() = default;

  Result(HRESULT result)
  : value_(result)
  { }

  operator HRESULT() const
  {
    return value_;
  }

  bool failed() const { return value_ < 0; }
  bool succeeded() const { return value_ >= 0; }
  explicit operator bool() const { return succeeded(); }

  QString toString() const;

private:
  HRESULT value_ = S_OK;
};

// Unknown

/// \brief wrapper for IUnknown
///
/// a reference counting object
class Unknown
{
public:
  explicit Unknown() = default;

  explicit Unknown(IUnknown* ptr)
  : ptr_(ptr)
  { }

  Unknown(const Unknown& other)
  : ptr_(other.ptr_)
  {
    if (ptr_)
      ptr_->AddRef();
  }

  Unknown& operator=(const Unknown& other)
  {
    reset(other.ptr_);

    if (ptr_)
      ptr_->AddRef();

    return *this;
  }

  Unknown(Unknown&& other)
  : ptr_(other.ptr_)
  {
    other.ptr_ = nullptr;
  }

  Unknown& operator=(Unknown&& other)
  {
    reset(other.ptr_);
    other.ptr_ = nullptr;
    return *this;
  }

  ~Unknown()
  {
    if (ptr_)
      ptr_->Release();
  }

  // access

  bool isNull() const { return ptr_ == nullptr; }
  bool empty() const { return isNull(); }
  explicit operator bool() const { return !empty(); }

  IUnknown* get() { return ptr_; }

  // interface access

  template<typename T>
  inline Idl_<T> queryInterface(Result* result = nullptr) const;

  template<typename T>
  inline Result queryInterface(Idl_<T>& iterface_) const;

  // modify

  void reset(IUnknown* ptr)
  {
    if (ptr_)
      ptr_->Release();

    ptr_ = ptr;
  }

protected:
  IUnknown* get() const { return ptr_; }

private:
  IUnknown* ptr_ = nullptr;
};

// Idl

/// \brief typed interface
template<typename Interface>
class Idl_ : public Unknown
{
public:
  Idl_() = default;

  explicit Idl_(Interface* ptr)
  : Unknown(ptr)
  { }

  Interface* get() { return static_cast<Interface*>(Unknown::get()); }
  Interface* operator->() { return get(); }

  Result create(
    const IID& clsid,
    DWORD clsContext,
    const IID& iid
  )
  {
    Interface* obj;
    HRESULT result = ::CoCreateInstance(
      clsid, nullptr, clsContext, iid, reinterpret_cast<void**>(&obj)
    );
    reset(obj);

    return Result(result);
  }

protected:
  Interface* get() const { return static_cast<Interface*>(get()); }
};


// inline impl

template<typename T>
inline Idl_<T> Unknown::queryInterface(Result* result) const
{
  T* obj = nullptr;
  HRESULT hresult = ptr_->QueryInterface(
    __uuidof(T),
    reinterpret_cast<void**>(&obj)
  );

  if (result)
    *result = Result(hresult);

  return Idl_<T>(obj);
}

template<typename T>
inline Result Unknown::queryInterface(Idl_<T>& iterface_) const
{
  T* obj = nullptr;
  HRESULT result = ptr_->QueryInterface(
    __uuidof(T),
    reinterpret_cast<void**>(&obj)
  );
  iterface_.reset(obj);
  return result;
}

} // namespace WinQt
