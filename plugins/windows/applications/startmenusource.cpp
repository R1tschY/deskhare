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

#include "startmenusource.h"

#include <shlobj.h>
#include <windows.h>
#include <exception>
#include <QDebug>
#include <QDirIterator>
#include <QDateTime>
#include <QLoggingCategory>

#include <cpp-utils/algorithm/container.h>
#include <libdeskhare/query.h>
#include <libdeskhare/resultset.h>
#include <libdeskhare/filesystem/diriterator.h>
#include <winqt/shell/knownfolder.h>
#include <winqt/shell/shelllink.h>
#include <winqt/common/comobject.h>
#include <winqt/common/env.h>


namespace Deskhare {

using namespace WinQt;

static Q_LOGGING_CATEGORY(logger, "deskhare.win.appsource")

StartMenuSource::StartMenuSource(const PluginContext& ctx)
{
  if (!index_.open(SqliteIndex::createIndexPath("winappindex.db")))
    return;

  if (index_.getLastIndexing().daysTo(QDateTime::currentDateTime()) > 7)
    index();
}

bool StartMenuSource::canHandleQuery(const Query& query)
{
  return index_.isOpen() && !query.getSearchString().isEmpty()
    && query.hasCategory(Query::Category::App);
}

void StartMenuSource::search(const Query& query, ResultSet& results)
{
  Q_ASSERT(index_.isOpen());

  if (query.getSearchString().isEmpty())
    return; // TODO recent things

  results.sendMatches(
    index_.search(query.getSearchString())
  );
}

QString StartMenuSource::getDescription() const
{
  return QLatin1String("Windows start menu applications.\n"
    "\n"
    "Access to applications in the start menu and on the desktop.");
}

void StartMenuSource::index()
{
  qCInfo(logger) << "Indexing applications ...";

  if (!index_.isOpen())
    return;

  auto transaction = index_.createTransaction();
  index_.clear();

  WinQt::ShellLink shell_link;
  if (!shell_link.okay())
  {
    qCCritical(logger)
      << "failed to create ShellLink object:"
      << shell_link.lastResult().toString();
  }

  int i = 0;
  for (const GUID& folderId : {FOLDERID_Programs, FOLDERID_CommonPrograms})
  {
    auto dir = WinQt::getKnownFolderPath(folderId);
    cpp::for_each(DirRange(
      dir, QStringList(QLatin1String("*.lnk")),
      QDir::Files | QDir::Readable,
      QDirIterator::Subdirectories),
    [&](const QDirIterator& file)
    {
      i += 1;
      if (i & 0xF == 0)
        qCInfo(logger) << "Indexed" << i << "applications so far ...";

      WinQt::Result res = shell_link.load(file.filePath());
      if (!res)
      {
        qCWarning(logger)
          << "Cannot open shell link"
          << res.toString()
          << file.filePath();
        return;
      }

      QString path = shell_link.path();
      QFileInfo pathInfo(path);
      QString fileName = pathInfo.fileName();
      if (!path.endsWith(QLatin1String("exe"))
          || fileName.startsWith(QLatin1String("unins"), Qt::CaseInsensitive)
          || fileName.compare(
              QLatin1String("msiexec.exe"), Qt::CaseInsensitive) == 0
          || fileName.contains(QLatin1String("uninstall"), Qt::CaseInsensitive)
          )
        return;

      IconLocation iconLoc = shell_link.iconLocation();
      if (iconLoc.isNull())
      {
        iconLoc.setPath(path);
        iconLoc.setIndex(0);
      }

      index_.addApplication(
        file.fileInfo().completeBaseName(),
        shell_link.description(),
        file.filePath(),
        iconLoc
      );
    });
  }

  index_.setLastIndexing(QDateTime::currentDateTime());

  qCInfo(logger) << "Indexed" << i << "applications";
}

} // namespace QuickStarter
