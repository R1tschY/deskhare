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

#include "xbelreader.h"

namespace Deskhare {

using Ascii = QLatin1String;

XbelReader::XbelReader() = default;

bool XbelReader::read(QIODevice* device)
{
  xml_.setDevice(device);

  if (xml_.readNextStartElement())
  {
    if (xml_.name() == Ascii("xbel")
      && xml_.attributes().value(Ascii("version")) == Ascii("1.0"))
    {
      readXbel();
    } else {
      xml_.raiseError(tr("The file is not an XBEL version 1.0 file."));
    }
  }

  return !xml_.error();
}

QVector<XbelBookmark> XbelReader::bookmarks() const
{
  return bookmarks_;
}

void XbelReader::readXbel()
{
  while (xml_.readNextStartElement()) {
    if (xml_.name() == Ascii("folder"))
      readFolder();
    else if (xml_.name() == Ascii("bookmark"))
      readBookmark();
    else
      xml_.skipCurrentElement();
  }
}

void XbelReader::readBookmark()
{
  bookmarks_.push_back(XbelBookmark());
  auto& back = bookmarks_.back();

  auto attrs = xml_.attributes();
  back.added = readDateTime(attrs.value(Ascii("added")));
  back.modified = readDateTime(attrs.value(Ascii("modified")));
  back.visited = readDateTime(attrs.value(Ascii("visited")));
  back.href = QUrl::fromUserInput(attrs.value(Ascii("href")).toString());

  while (xml_.readNextStartElement())
  {
    if (xml_.name() == Ascii("title"))
      back.title = xml_.readElementText();
    else
      xml_.skipCurrentElement();
  }
}

void XbelReader::readFolder()
{
  while (xml_.readNextStartElement()) {
    if (xml_.name() == Ascii("folder"))
      readFolder();
    else if (xml_.name() == Ascii("bookmark"))
      readBookmark();
    else
      xml_.skipCurrentElement();
  }
}

QDateTime XbelReader::readDateTime(const QStringRef& dateTime)
{
  if (dateTime.isEmpty())
    return QDateTime();

  return QDateTime::fromString(dateTime.toString(), Qt::ISODate);
}

} // namespace Deskhare

