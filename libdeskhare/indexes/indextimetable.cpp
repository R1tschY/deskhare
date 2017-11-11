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

#include "indextimetable.h"

#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QLoggingCategory>

#include "sqliteindex.h"

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.indextimetable")

IndexTimeTable::IndexTimeTable(SqliteIndex& index)
: index_(index)
{ }

QDateTime IndexTimeTable::getLastIndexing()
{
  QSqlQuery query(index_.dataBase());
  query.prepare(
    QLatin1String("SELECT time FROM times WHERE id = 'last_indexing'"));
  if (!query.exec())
  {
    qCCritical(logger) << "Failed to read last indexing of file index:"
      << query.lastError();
    return QDateTime();
  }
  if (!query.next())
    return QDateTime();

  return QDateTime::fromMSecsSinceEpoch(query.value(0).toLongLong() * 1000);
}

void IndexTimeTable::setLastIndexing(const QDateTime& time)
{
  QSqlQuery query(index_.dataBase());
  query.prepare(
    QLatin1String("UPDATE times SET time=? WHERE id = 'last_indexing'"));
  query.bindValue(0, time.toMSecsSinceEpoch() / 1000);
  if (!query.exec())
  {
    qCCritical(logger) << "Failed to set last indexing of file index:"
      << query.lastError();
  }
}

bool IndexTimeTable::create()
{
  auto transaction = index_.createTransactionGuard();

  QSqlQuery create_query(index_.dataBase());
  if (!create_query.exec(QLatin1String(
    "CREATE TABLE times ("
    "  id   TEXT NOT NULL,"
    "  time INTEGER NOT NULL"
    ")")))
  {
    qCCritical(logger) << "create file index times table failed:"
      << create_query.lastError();
    transaction.rollback();
    return false;
  }
  if (!create_query.exec(QLatin1String(
    "INSERT INTO times VALUES ('last_indexing', 0)")))
  {
    qCCritical(logger) << "inserting last indexing in file index table failed:"
      << create_query.lastError();
    transaction.rollback();
    return false;
  }

  return true;
}

} // namespace Deskhare
