//
// deskhare
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

#include <QString>
#include <QStringList>

#include "linuxlocale.h"

namespace Deskhare {

/// \brief
class XdgInfoCache
{
public:
  XdgInfoCache();

  /// \brief list of dirs with desktop-files
  ///
  /// \return $XDG_DATA_DIRS + $XDG_DATA_HOME append with "/applications"
  QStringList getApplicationDirs() const { return appDirs_; }

  LinuxLocale getLocale() const { return locale_; }
  QStringList getLocaleKeys() const { return localeKeys_; }
  QStringList getLocaleKeys(const LinuxLocale& locale) const;

private:
  QStringList appDirs_;
  void initApplicationDirs();
  
  LinuxLocale locale_;
  void initLocale();

  QStringList localeKeys_;
  static QStringList findLocaleKeys(const LinuxLocale& locale);
  void initLocaleKeys();
};

const XdgInfoCache& getInfoCache();

} // namespace Deskhare
