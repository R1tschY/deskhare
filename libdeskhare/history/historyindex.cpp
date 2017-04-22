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

#include "historyindex.h"

#include <QSqlQuery>
#include <QFileInfo>
#include <QString>
#include <QDateTime>
#include <QSqlError>
#include <QLoggingCategory>
#include <QSqlDriver>
#include <cpp-utils/scope.h>


#include "epochtime.h"


namespace Deskhare {

namespace {
Q_LOGGING_CATEGORY(logger, "deskhare.HistoryIndex")
} // namespace

HistoryIndex::HistoryIndex()
: SqliteIndex("history", 0)
{ }

void HistoryIndex::update(const QString& uri, time_t time)
{
  if (uri.isEmpty())
    return;

  dataBase().transaction();
  scope(exit) { dataBase().commit(); };

  auto stats = getStats(uri);
  auto delta_t = time - std::get<0>(stats);
  if (delta_t < Epoch::min * 15)
  {
    // old event or event not counted, because of near existing event
    return;
  }

  QSqlQuery query(dataBase());
  query.prepare(QLatin1String(
    "REPLACE INTO history (uri, last_used, use_count) VALUES (?, ?, ?)"));
  query.addBindValue(uri);
  query.addBindValue(qlonglong(time));
  query.addBindValue(qulonglong(std::get<1>(stats) + 1));
  if (!query.exec())
  {
    qCCritical(logger) << "Update failed:" << query.lastError();
  }
}

void HistoryIndex::clear()
{
  QSqlQuery clearQuery(QLatin1String("DELETE FROM history"), dataBase());
  if (!clearQuery.last())
  {
    qCCritical(logger) << "Cannot clear table:" <<
      clearQuery.lastError().text();
  }
}

std::tuple<time_t, std::size_t> HistoryIndex::getStats(const QString& uri)
{
  if (uri.isEmpty())
    return std::tuple<time_t, std::size_t>{ 0, 0 };

  QSqlQuery sqlQuery(dataBase());
  sqlQuery.prepare(QStringLiteral(
      "SELECT last_used, use_count FROM history WHERE uri = ?"));
  sqlQuery.bindValue(0, uri);
  if (!sqlQuery.exec())
  {
    qCCritical(logger) << "search failed:"
      << sqlQuery.lastError();
    return std::tuple<time_t, std::size_t>{ 0, 0 };
  }

  if (!sqlQuery.first())
    return std::tuple<time_t, std::size_t>{ 0, 0 };

  return std::tuple<time_t, std::size_t>{
    sqlQuery.value(0).toLongLong(), sqlQuery.value(1).toULongLong()
  };
}

bool HistoryIndex::create()
{
  QSqlQuery create_query(dataBase());
  if (!create_query.exec(QStringLiteral(
    "CREATE TABLE history("
    "  uri TEXT PRIMARY KEY,"
    "  last_used DATETIME NOT NULL DEFAULT 0,"
    "  use_count INT NOT NULL DEFAULT 0"
    ")")))
  {
    qCCritical(logger) << "create table failed:" << create_query.lastError();
    return false;
  }

  return true;
}

bool HistoryIndex::upgrade(int currentFormatVersion)
{
  return false;
}

} // namespace Deskhare
