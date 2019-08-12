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

#include "unixdesktopbookmarks.h"

#include <QStandardPaths>
#include <libdeskhare/matches/filematch.h>

namespace Deskhare {

UnixDesktopBookmarks::UnixDesktopBookmarks()
{
  index();
}

void UnixDesktopBookmarks::index()
{
  clearMatches();

  indexGeneric();
}

void UnixDesktopBookmarks::indexGeneric()
{
  addPath(
    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
    QStandardPaths::displayName(QStandardPaths::DocumentsLocation),
    QStringList());
}

void UnixDesktopBookmarks::indexGtk3()
{
}

void UnixDesktopBookmarks::indexGtk3Server()
{
}

void UnixDesktopBookmarks::addPath(const QString& path, const QString& title,
  const QStringList& keywords)
{
  if (foundPaths_.contains(path))
    return;

  //addMatch(std::make_shared<LocalFileMatch>(path));
}

} // namespace Deskhare
