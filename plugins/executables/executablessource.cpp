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

#include "executablessource.h"

#include <QLoggingCategory>
#include <QSqlError>

#include <libdeskhare/query.h>
#include <libdeskhare/resultset.h>
#include <libdeskhare/filesystem/diriterator.h>
#include <libdeskhare/utils/getenvtext.h>
#include <cpp-utils/algorithm/container.h>
#include <cpp-utils/algorithm/erase.h>

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.executables.source")

static
QStringList getSearchPaths()
{
  QStringList searchPaths;

  const QStringList rawPaths = getEnvText("PATH").split(
    QDir::listSeparator(), QString::SkipEmptyParts);
  searchPaths.reserve(rawPaths.size());
  for (const QString &rawPath : rawPaths)
  {
    QString cleanPath = QDir::cleanPath(rawPath);
    if (cleanPath.size() > 1 && cleanPath.endsWith(QLatin1Char('/')))
      cleanPath.truncate(cleanPath.size() - 1);

    if (QDir(cleanPath).exists())
      searchPaths.push_back(cleanPath);
  }

  return searchPaths;
}

static
QStringList getSearchExtensions()
{
#ifdef Q_OS_WIN
  QStringList pathExt = getEnvText("PATHEXT")
    .toLower().split(QLatin1Char(';'), QString::SkipEmptyParts);

  if (!pathExt.contains(QLatin1String(".exe")))
  {
    return QStringList()
      << QLatin1String(".exe") << QLatin1String(".com")
      << QLatin1String(".bat") << QLatin1String(".cmd");
  }

  cpp::erase_if(
    pathExt,
    [](const QString& ext){ return ext.length() < 2 || !ext.startsWith('.'); });

  return pathExt;
#else
  return QStringList();
#endif
}

ExecutablesSource::ExecutablesSource(const PluginContext& ctx)
: context_(ctx)
{
  if (!db_.open(SqliteIndex::createIndexPath("executables.db")))
    return;
};

bool ExecutablesSource::canHandleQuery(const Query& query)
{
  return db_.isOpen() && query.hasCategory(Query::Category::App)
    && !query.getSearchString().isEmpty();
}

void ExecutablesSource::search(const Query& query, ResultSet& results)
{
  auto matches = db_.search(
    query.getSearchString(), *context_.getFileIconProvider());
  results.sendMatches(matches);
}

QString ExecutablesSource::getDescription() const
{
  return QLatin1String("Executables in the PATH.");
}

void ExecutablesSource::index()
{
  qCInfo(logger) << "Indexing files ...";

  if (!db_.isOpen())
    return;

  auto transaction = db_.createTransaction();
  db_.clear();

  int i = 0;
  for (const QString& dir : getSearchPaths())
  {
    // TODO: improve performance: create addFiles and use on batches of files
    cpp::for_each(DirRange(dir, QDir::Files | QDir::Executable),
    [&](const QDirIterator& file)
    {
      i += 1;
      db_.addFile(file.fileInfo());
    });

    qCDebug(logger) << "Indexed" << i << "executables so far ...";
  }

  db_.setLastIndexing(QDateTime::currentDateTime());

  qCInfo(logger) << "Indexed" << i << "executables";
}

} // namespace Deskhare

