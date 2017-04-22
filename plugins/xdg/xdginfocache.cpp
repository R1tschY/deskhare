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

#include "xdginfocache.h"

#include <XdgDirs>

namespace Deskhare {

XdgInfoCache::XdgInfoCache()
{
  initLocale();
  initApplicationDirs();
  initLocaleKeys();
}

void XdgInfoCache::initApplicationDirs()
{
  appDirs_ = XdgDirs::dataDirs("/applications");

  QString appHome = XdgDirs::dataHome(false) + "/applications";
  if (!appDirs_.contains(appHome))
    appDirs_.append(appHome);
}

void XdgInfoCache::initLocale()
{
  locale_ = LinuxLocale(LC_MESSAGES);
}

QStringList XdgInfoCache::getLocaleKeys(const LinuxLocale& locale) const
{
  if (locale == locale_)
    return localeKeys_;
  else
    return findLocaleKeys(locale);
}

QStringList XdgInfoCache::findLocaleKeys(const LinuxLocale& locale)
{
  QStringList result;

  if (!locale.modifier().isEmpty() && !locale.country().isEmpty())
  {
    result.append(QString(
      '[' % locale.language() % '_' % locale.modifier() % '@' %
      locale.modifier() % ']'
    ));
  }

  if (!locale.country().isEmpty())
  {
    result.append(QString(
      '[' % locale.language() % '_' % locale.country() % ']'
    ));
  }

  if (!locale.modifier().isEmpty())
  {
    result.append(QString(
      '[' % locale.language() % '@' % locale.modifier() % ']'
    ));
  }

  result.append(QString('[' % locale.language() % ']'));

  return result;
}

void XdgInfoCache::initLocaleKeys()
{
  localeKeys_ = findLocaleKeys(locale_);
}

const XdgInfoCache& getInfoCache()
{
  static XdgInfoCache infoCache;
  return infoCache;
}

} // namespace Deskhare

