/// \file iconlocation.h

#pragma once

#include <QString>

class QPixmap;

namespace WinQt {

enum class IconSizeCategory {
  Small, Large
};

class IconLocation
{
public:
  IconLocation() = default;

  explicit IconLocation(const QString& path, int index)
  : path_(path), index_(index)
  { }

  // state

  bool isNull() const { return path_.isEmpty(); }

  // access / modify

  int index() const { return index_; }
  void setIndex(int index) { index_ = index; }

  const QString& path() const { return path_; }
  void setPath(const QString& path) { path_ = path; }

  // actions

  QPixmap resolve(IconSizeCategory cat = IconSizeCategory::Large) const;

  // convert

  /**
   * Convert to string representation <path>:<index>.
   */
  QString toString() const;

  /**
   * Parse from string representation <path>:<index>.
   */
  static IconLocation fromString(const QString& iconLoc);

private:
  QString path_;
  int index_ = 0;
};

} // namespace WinQt

