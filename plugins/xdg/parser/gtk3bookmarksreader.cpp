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

#include "gtk3bookmarksreader.h"

#include <QTextStream>
#include <QIODevice>

namespace Deskhare {

bool Gtk3BookmarksReader::read(QIODevice* device)
{
  QTextStream in(device);
  in.setCodec("UTF-8");

  QString line;
  while (in.readLineInto(&line)) {
    auto firstSpace = line.indexOf(QChar::Space);
    if (firstSpace < 0) {
      addBookmark(QUrl::fromUserInput(line), QString());
    } else {
      addBookmark(
        QUrl::fromUserInput(line.left(firstSpace)), line.mid(firstSpace + 1));
    }
  }

  return true;
}

QVector<Gtk3Bookmark> Gtk3BookmarksReader::bookmarks() const
{
  return bookmarks_;
}

void Gtk3BookmarksReader::addBookmark(QUrl href, QString title)
{
  if (!href.isValid())
    return;

  bookmarks_.push_back(Gtk3Bookmark{title.trimmed(), href});
}

} // namespace Deskhare

