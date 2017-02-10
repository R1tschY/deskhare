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

#include <plugins/xdg/xdgfileiconprovider.h>
#include <xdgmimetype.h>

namespace Deskhare {

XdgFileIconProvider::XdgFileIconProvider()
{
}

QIcon XdgFileIconProvider::icon(const QFileInfo& info) const
{
  QMimeType mimetype = mime_database_.mimeTypeForFile(info.filePath());
  qDebug() << mimetype.iconName() << QIcon::themeName() << mimetype.genericIconName();

  QIcon icon = QIcon::fromTheme(mimetype.iconName());
  if (!icon.isNull())
    return icon;

  icon = QIcon::fromTheme(mimetype.genericIconName());
  if (!icon.isNull())
    return icon;

  return QFileIconProvider::icon(info);
}

} // namespace Deskhare
