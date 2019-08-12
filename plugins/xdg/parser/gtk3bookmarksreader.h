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

#pragma once

#include <QString>
#include <QUrl>
#include <QVector>

class QIODevice;

namespace Deskhare {

struct Gtk3Bookmark {
  QString title;
  QUrl href;
};

/// \brief
class Gtk3BookmarksReader
{
public:
  bool read(QIODevice* device);

  QVector<Gtk3Bookmark> bookmarks() const;

private:
  QVector<Gtk3Bookmark> bookmarks_;

  void addBookmark(QUrl href, QString title);
};

} // namespace Deskhare
