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

#include "fileindex.h"

#include <QSqlQuery>
#include <QFileInfo>
#include <QString>
#include <QDateTime>
#include <QSqlError>

#include <cpp-utils/algorithm/container.h>
#include <libdeskhare/match.h>
#include <libdeskhare/matches/filematch.h>
#include <libdeskhare/query.h>
#include <libdeskhare/resultset.h>

#include "logging.h"

namespace Deskhare {

FileIndex::FileIndex()
: SqliteIndex(QLatin1String("fileIndex"), 2),
  index_time_table_(*this)
{ }

void FileIndex::clear()
{
  QSqlQuery clearQuery(dataBase());
  if (!clearQuery.exec(QLatin1String("DELETE FROM executables")))
  {
    qCCritical(fileIndexLogger) << "Cannot clear files table:" <<
      clearQuery.lastError().text();
  }
}

bool FileIndex::addFile(const QFileInfo& file)
{
  Q_ASSERT(file.isAbsolute());

  QSqlQuery query(dataBase());
  query.prepare(
    QLatin1String("INSERT INTO files(name, path) VALUES (?, ?)"));
  query.bindValue(0, file.fileName());
  query.bindValue(1, file.filePath());
  if (!query.exec())
  {
    qCCritical(fileIndexLogger) << "insert file in index table failed:"
      << query.lastError();
    return false;
  }

  return true;
}

std::vector<std::shared_ptr<Match>> FileIndex::search(
  const QString& query,
  const FileIconProvider& icon_provider)
{
  std::vector<std::shared_ptr<Match>> matches;

  QSqlQuery sqlquery(dataBase());
  sqlquery.prepare(QLatin1String(
    "SELECT path FROM files WHERE name MATCH ?"));
  sqlquery.bindValue(0, QString(query + '*'));  // TODO: escape query
  if (!sqlquery.exec())
  {
    qCCritical(fileIndexLogger) << "search file in index failed:"
      << sqlquery.lastError();
    return matches;
  }

  while (sqlquery.next())
  {
    matches.emplace_back(std::make_shared<LocalFileMatch>(
      sqlquery.value(0).toString(),
      icon_provider,
      MatchScore::Good
    ));
  }

  return matches;
}

bool FileIndex::create()
{
  auto transaction = createTransactionGuard();

  QSqlQuery create_query(dataBase());
  if (!create_query.exec(QLatin1String(
    "CREATE VIRTUAL TABLE files USING fts4("
    "  name, path, notindexed=path"
    ")")))
  {
    qCCritical(fileIndexLogger) << "create file index table failed:"
      << create_query.lastError();
    transaction.rollback();
    return false;
  }

  return index_time_table_.create();
}

bool FileIndex::upgrade(int currentFormatVersion)
{
  return false;
}

} // namespace Deskhare
