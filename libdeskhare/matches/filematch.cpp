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
#include <QLoggingCategory>
#include <QDesktopServices>

#include "../actions/openurlaction.h"
#include "../shell/iconservice.h"

Q_LOGGING_CATEGORY(localFileMatch, "deskhare.LocalFileMatch")

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


LocalFileMatch::LocalFileMatch(const QString& filePath, float score)
: LocalFileMatch(QFileInfo(filePath), score)
{ }

LocalFileMatch::LocalFileMatch(const QFileInfo& fileInfo, float score)
: Match(
    fileInfo.fileName(),
    getPathComponents(fileInfo).join(QStringLiteral(" › ")),
    IconService::fileIcon(fileInfo),
    "file://" + fileInfo.filePath(),
    score
  )
{
  Q_ASSERT(fileInfo.isAbsolute());
}

std::shared_ptr<Action> LocalFileMatch::getDefaultAction() const
{
  return std::make_shared<OpenUrlAction>(MatchScore::Highest);
}

} // namespace Deskhare
