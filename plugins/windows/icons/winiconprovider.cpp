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

#include "winiconprovider.h"

#include <QIcon>
#include <QDebug>

#include <winqt/shell/iconlocation.h>

namespace Deskhare {

WinIconProvider::WinIconProvider()
{
  fillLocations();
}

float WinIconProvider::getPriorityIndex()
{
  return 10;
}

QIcon WinIconProvider::iconFromTheme(const QString& name)
{
  qDebug() << "request" << name;
  if (cache_.contains(name))
  {
    qDebug() << "in cache" << name;
    return *cache_.object(name);
  }

  QIcon* icon = resolve(name);
  QIcon result = *icon;
  cache_.insert(name, icon);
  return result;
}

QString WinIconProvider::getDescription() const
{
  return "Windows icon provider.";
}

QIcon* WinIconProvider::resolve(const QString& name) const
{
  qDebug() << "resolve" << name;
  auto locationsIter = icon_locations_.find(name);
  if (locationsIter == icon_locations_.end()) {
    qDebug() << "not found" << name;
    return new QIcon();
  }

  QIcon* result = nullptr;
  const auto locations = *locationsIter;
  for (auto& location : locations)
  {
    qDebug() << "search for" << name << "@" << location;
    auto iconLoc = WinQt::IconLocation::fromString(location);
    if (!iconLoc.isNull())
    {
      auto pixmap = iconLoc.resolve();
      if (!pixmap.isNull())
      {
        qDebug() << "got" << name << "@" << location;
        result = new QIcon(iconLoc.resolve());
        break;
      }
    }
  }

  return result ? result : new QIcon();
}

void WinIconProvider::fillLocations()
{
  icon_locations_.insert("accessories-calculator", {"calc.exe:0"});
}

} // namespace Deskhare

