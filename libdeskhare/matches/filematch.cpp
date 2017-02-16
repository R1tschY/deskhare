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
#include "filematch.h"

#include <QFileInfo>
#include <QDir>
#include <QStringList>

#include "../shell/fileiconservice.h"

namespace Deskhare {

static QStringList getPathComponents(const QFileInfo& fileInfo)
{
  QStringList pathparts;
  pathparts.append(fileInfo.fileName());

  QDir dir = fileInfo.absoluteDir();
  while (!dir.isRoot())
  {
    pathparts.prepend(dir.dirName());
    dir.cdUp();
  }
  pathparts.prepend(dir.dirName());

  return pathparts;
}

LocalFileMatch::LocalFileMatch(const QString& filePath, const FileIconProvider& icons)
: Match("file://" + filePath), fileInfo_(filePath), icon_(icons.icon(fileInfo_))
{
  Q_ASSERT(fileInfo_.isAbsolute());

  description_ = getPathComponents(fileInfo_).join(QStringLiteral(" › "));
}

QString LocalFileMatch::getDescription() const
{
  return description_;
}

QString LocalFileMatch::getTitle() const
{
  return fileInfo_.fileName();
}

QIcon LocalFileMatch::getIcon() const
{
  return icon_;
}

} // namespace Deskhare
