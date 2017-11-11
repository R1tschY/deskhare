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

#include "executablesindex.h"

#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <QLoggingCategory>
#include <QProcess>
#include <QDir>

#include <cpp-utils/algorithm/container.h>
#include <libdeskhare/match.h>
#include <libdeskhare/matches/filematch.h>
#include <libdeskhare/query.h>
#include <libdeskhare/resultset.h>
#include <libdeskhare/shell/fileiconservice.h>

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.executables.index")

ExecutableMatch::ExecutableMatch(
  const QString& commandLine,
  const QString& exePath,
  const QIcon& icon,
  float score)
: Action(
    commandLine, // TODO: escape chars
    tr("Call executable"),
    icon,
    QLatin1String("run://") + commandLine,
    score),
  exePath_(exePath)
{ }

bool ExecutableMatch::canHandleMatch(const Match& match) const
{
  return true;
}

void ExecutableMatch::execute(const Match& target) const
{
  QProcess::startDetached(exePath_, QStringList(), QDir::homePath());
}


ExecutablesIndex::ExecutablesIndex()
: SqliteIndex(QLatin1String("executables"), 0),
  index_time_table_(*this)
{ }

void ExecutablesIndex::clear()
{
  QSqlQuery clearQuery(dataBase());
  if (!clearQuery.exec(QLatin1String("DELETE FROM executables")))
  {
    qCCritical(logger) << "Cannot clear executables table:" <<
      clearQuery.lastError().text();
  }
}

bool ExecutablesIndex::addFile(const QFileInfo& file)
{
  Q_ASSERT(file.isAbsolute());

  QSqlQuery query(dataBase());
  query.prepare(
    QLatin1String("INSERT INTO executables(name, path) VALUES (?, ?)"));
  query.bindValue(0, file.baseName());
  query.bindValue(1, file.filePath());
  if (!query.exec())
  {
    qCCritical(logger) << "insert executables in index table failed:"
      << query.lastError();
    return false;
  }

  return true;
}

std::vector<std::shared_ptr<Match>> ExecutablesIndex::search(
  const QString& query,
  const FileIconProvider& icon_provider)
{
  std::vector<std::shared_ptr<Match>> matches;

  QSqlQuery sqlquery(dataBase());
  sqlquery.prepare(QLatin1String(
    "SELECT path FROM executables WHERE name MATCH ?"));
  sqlquery.bindValue(0, QString(query + '*'));  // TODO: escape query
  if (!sqlquery.exec())
  {
    qCCritical(logger) << "search executables in index failed:"
      << sqlquery.lastError();
    return matches;
  }

  QFileInfo fileInfo;
  while (sqlquery.next())
  {
    fileInfo.setFile(sqlquery.value(0).toString());

    matches.emplace_back(std::make_shared<ExecutableMatch>(
      fileInfo.baseName(),
      fileInfo.filePath(),
      icon_provider.icon(fileInfo.filePath()),
      MatchScore::BelowAverage
    ));
  }

  return matches;
}

bool ExecutablesIndex::create()
{
  auto transaction = createTransactionGuard();

  QSqlQuery create_query(dataBase());
  if (!create_query.exec(QLatin1String(
    "CREATE VIRTUAL TABLE executables USING fts4("
    "  name, path, notindexed=path"
    ")")))
  {
    qCCritical(logger) << "create executables index table failed:"
      << create_query.lastError();
    transaction.rollback();
    return false;
  }

  return index_time_table_.create();
}

bool ExecutablesIndex::upgrade(int currentFormatVersion)
{
  return false;
}

} // namespace Deskhare
