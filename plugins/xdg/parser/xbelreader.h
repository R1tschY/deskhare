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

#include <QVector>
#include <QDateTime>
#include <QUrl>
#include <QString>
#include <QXmlStreamReader>
#include <QCoreApplication>

class QIODevice;

namespace Deskhare {

struct XbelBookmark {
  QDateTime added;
  QDateTime modified;
  QDateTime visited;
  QString title;
  QUrl href;
};

/// \brief read bookmarks from a xbel file
class XbelReader
{
  Q_DECLARE_TR_FUNCTIONS(XbelReader)
public:
  XbelReader();

  bool read(QIODevice* device);

  QVector<XbelBookmark> bookmarks() const;
  bool hasError() const { return xml_.hasError(); }
  QString errorString() const { return xml_.errorString(); }

private:
  QXmlStreamReader xml_;
  QVector<XbelBookmark> bookmarks_;

  void readXbel();
  void readBookmark();
  void readFolder();

  QDateTime readDateTime(const QStringRef& dateTime);
};

} // namespace Deskhare
