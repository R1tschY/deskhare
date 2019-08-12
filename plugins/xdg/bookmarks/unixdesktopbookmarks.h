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

#include <QSet>
#include <QCoreApplication>

#include <libdeskhare/sources/genericsourcebase.h>

class QString;
class QStringList;

namespace Deskhare {

/// \brief
class UnixDesktopBookmarks: public GenericSourceBase
{
  Q_DECLARE_TR_FUNCTIONS(UnixDesktopBookmarks)
public:
  UnixDesktopBookmarks();

private:
  QSet<QString> foundPaths_;

  void index();

  void indexGeneric();
  void indexGtk3();
  void indexGtk3Server();

  void addPath(
    const QString& path, const QString& title, const QStringList& keywords);
};

} // namespace Deskhare
