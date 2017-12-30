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
#include <QDebug>
#include <XdgDirs>
#include <cpp-utils/algorithm/range.h>

#include "xdginfocache.h"

namespace Deskhare {

XdgApplicationDesktopFile::XdgApplicationDesktopFile()
{ }

XdgApplicationDesktopFile::XdgApplicationDesktopFile(
  const XdgDesktopFile& other)
: XdgDesktopFile(other), gettextDomain_(
  other.value("X-Ubuntu-Gettext-Domain").toString().toUtf8()
)
{ }

bool XdgApplicationDesktopFile::load(const QString& fileName)
{
  bool result = XdgDesktopFile::load(fileName);

  gettextDomain_ = value("X-Ubuntu-Gettext-Domain").toString().toUtf8();
  ::bind_textdomain_codeset(gettextDomain_.data(), "UTF-8");

  return result;
}

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
  QVariant v = value(key);

  // Ubuntu extention
  if (!gettextDomain_.isEmpty() && v.type() == QVariant::String)
  {
    return QString::fromUtf8(
      ::dgettext(gettextDomain_.data(), v.toString().toUtf8().data())
    );
  }

  QString lKey = localizedKey(locale, key);
  if (!lKey.isEmpty())
    return value(lKey);

  return v;
}

QString XdgApplicationDesktopFile::localizedKey(const LinuxLocale& locale,
  const QString& key) const
{
  const auto locKeys = getInfoCache().getLocaleKeys(locale);

  QString result;
  for (const auto& locKey : locKeys)
  {
    result = key + locKey;
    if (contains(result))
      return result;
  }

  return key;
}

QString XdgApplicationDesktopFile::id() const
{
  const QFileInfo f(fileName());

  QString id = f.absoluteFilePath();

  const auto appDirs = getInfoCache().getApplicationDirs();
  auto appDir = cpp::range::find_if(
    appDirs,
    [&](auto& dataDir) { return id.startsWith(dataDir); }
  );
  if (!appDir)
  {
    return QString();
  }

  // remove data dir and /
  id.remove(0, appDir.begin()->size() + 1);

  // replace / with -
  id.replace('/', '-');
  return id;
}

} // namespace Deskhare
