//
// xdg-thumbnails-qt
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

#include "xdgthumbnails.h"

#include <cstdlib>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>

/**
 * Get $XDG_CACHE_HOME or $HOME/.cache
 */
static QString getXdgCacheDir()
{
  static QString dir = [](){
    auto xdgCacheDir = QFile::decodeName(qgetenv("XDG_CACHE_HOME"));
    return (!xdgCacheDir.isEmpty())
      ? xdgCacheDir
      : (QDir::homePath() + "/.cache");
  }();
  return dir;
}

/**
 * Create canonical uri representation.
 */
static QByteArray encodeUri(const QUrl& uri)
{
  QByteArray result = uri.toEncoded();

  // Is needed because `;[]` are not encoded by Qt. But GNOME does. What does
  // KDE do?
  result.replace(';', "%3B");
  result.replace('[', "%5B");
  result.replace(']', "%5D");
  return result;
}

QString XdgThumbnails::getThumbnailFile(
  const QUrl& fileUrl, XdgThumbnail::ImageSize size)
{
  if (!fileUrl.isValid() || fileUrl.isRelative())
    return { };

  auto hash = QCryptographicHash::hash(
    encodeUri(fileUrl),
    QCryptographicHash::Md5
  );

  if (size == XdgThumbnail::Large)
  {
    return getXdgCacheDir() + "/thumbnails/large/" + hash.toHex() + ".png";
  }
  else if (size == XdgThumbnail::Normal)
  {
    return getXdgCacheDir() + "/thumbnails/normal/" + hash.toHex() + ".png";
  }
  else
  {
    Q_ASSERT(false);
    return { };
  }
}
