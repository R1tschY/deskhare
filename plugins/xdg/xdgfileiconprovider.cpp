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

#include "xdgfileiconprovider.h"

#include <qfileiconprovider.h>
#include <qmimedatabase.h>
#include <QFileInfo>
#include <xdgthumbnails.h>

namespace Deskhare {

QIcon XdgFileIconProvider::icon(const QFileInfo& info) const
{
  if (info.isFile())
  {
    // thumbnail
    QString thumbnail =
      XdgThumbnails::getThumbnailFile(QUrl::fromLocalFile(info.filePath()));

    if (QFileInfo::exists(thumbnail))
      return QIcon(thumbnail);


    QMimeType mime = mimeDb_.mimeTypeForFile(info);
    QIcon icon = QIcon::fromTheme(mime.iconName());
    if (!icon.isNull())
      return icon;

    icon = QIcon::fromTheme(mime.genericIconName());
    if (!icon.isNull())
      return icon;
  }

  return QFileIconProvider::icon(info);
}


float XdgFileIconProviderPlugin::getPriorityIndex()
{
  return 10;
}

const QFileIconProvider& XdgFileIconProviderPlugin::getFileIconProvider()
{
  return fileIcons_;
}

QIcon XdgFileIconProviderPlugin::iconFromTheme(const QString& name)
{
  return QIcon::fromTheme(name);
}

QString XdgFileIconProviderPlugin::getDescription() const
{
  return "Xdg file icon provider.\n"
    "\n"
    "Search for thumbnails and get file icon based on MIME information in "
    "Freedesktop.org compatible systems.";
}

} // namespace Deskhare

