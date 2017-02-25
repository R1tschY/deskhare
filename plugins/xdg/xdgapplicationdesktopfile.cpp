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

#include "xdgapplicationdesktopfile.h"

#include <libintl.h>
#include <QFileInfo>
#include <QIcon>

namespace Deskhare {

XdgApplicationDesktopFile::XdgApplicationDesktopFile()
{ }

XdgApplicationDesktopFile::XdgApplicationDesktopFile(
  const XdgDesktopFile& other)
: XdgDesktopFile(other)
{ }

QString XdgApplicationDesktopFile::robustName() const
{
  auto result = name();
  if (!result.isEmpty())
    return result;

  return QFileInfo(fileName()).completeBaseName();
}

QIcon XdgApplicationDesktopFile::icon() const
{
  QString icoName = iconName();
  if (!icoName.isEmpty())
  {
    QIcon result;

    QFileInfo fileInfo(icoName);
    if (fileInfo.isAbsolute())
      result = QIcon(icoName);
    else
      result = QIcon::fromTheme(icoName);

    return (!result.isNull()) ? result : iconFallback();
  }

  // fallback
  return QIcon::fromTheme(QStringLiteral("application-x-executable"));
}

QIcon XdgApplicationDesktopFile::iconFallback()
{
  return QIcon::fromTheme(QStringLiteral("application-x-executable"));
}

QVariant XdgApplicationDesktopFile::localizedValue(const LinuxLocale& locale,
  const QString& key, const QVariant& defaultValue) const
{
  QString lKey = localizedKey(locale, key);

  if (!lKey.isEmpty())
    return value(lKey);

  QVariant v = value(key);
  if (v.type() != QVariant::String)
    return v;

  // Ubuntu extention
  QVariant domain = value("X-Ubuntu-Gettext-Domain");
  if (!domain.isNull())
  {
    auto domainUtf8 = domain.toString().toUtf8();
    ::bind_textdomain_codeset(domainUtf8.data(), "UTF-8");
    return QString::fromUtf8(
      ::dgettext(domainUtf8.data(), v.toString().toUtf8().data())
    );
  }

  return v;
}

QString XdgApplicationDesktopFile::localizedKey(const LinuxLocale& locale,
  const QString& key) const
{
  QString result;

  if (!locale.modifier().isEmpty() && !locale.country().isEmpty())
  {
    result = key % '[' % locale.language() % '_' % locale.modifier() % '@'
      % locale.modifier() % ']';
    if (contains(result))
      return result;
  }

  if (!locale.country().isEmpty())
  {
    result = key % '[' % locale.language() % '_' % locale.country() % ']';
    if (contains(result))
      return result;
  }

  if (!locale.modifier().isEmpty())
  {
    result = key % '[' % locale.language() % '@' % locale.modifier() % ']';
    if (contains(result))
      return result;
  }

  result = key % '[' % locale.language() % ']';
  if (contains(result))
    return result;

  return QString();
}

} // namespace Deskhare
