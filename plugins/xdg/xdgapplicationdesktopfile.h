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

#include "linuxlocale.h"

#include <QString>
#include <xdgdesktopfile.h>

namespace Deskhare {

/// \brief
class XdgApplicationDesktopFile : protected XdgDesktopFile
{
public:
  XdgApplicationDesktopFile();
  XdgApplicationDesktopFile(const XdgDesktopFile& other);

  using XdgDesktopFile::load;
  using XdgDesktopFile::value;
  using XdgDesktopFile::contains;
  using XdgDesktopFile::fileName;
  using XdgDesktopFile::isSuitable;
  using XdgDesktopFile::startDetached;
  using XdgDesktopFile::expandExecString;

  QVariant localizedValue(const QString& key, const QVariant& defaultValue = QVariant()) const
  {
    return localizedValue(LinuxLocale(LC_MESSAGES), key, defaultValue);
  }

  QVariant localizedValue(const LinuxLocale& locale, const QString& key,
    const QVariant& defaultValue = QVariant()) const;

  QString name() const
  {
    return localizedValue(QStringLiteral("Name")).toString();
  }

  QString comment() const
  {
    return localizedValue(QStringLiteral("Comment")).toString();
  }

  QString iconName() const
  {
    return value(QStringLiteral("Icon")).toString();
  }

  QIcon icon() const;

  QString robustName() const;

private:
  QString localizedKey(const LinuxLocale& locale, const QString& key) const;
  static QIcon iconFallback();
};

} // namespace Deskhare
