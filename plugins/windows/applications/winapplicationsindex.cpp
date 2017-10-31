/// \file winapplicationsindex.cpp

#include "winapplicationsindex.h"

#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QLoggingCategory>
#include <QDateTime>
#include <QFileIconProvider>

#include <libdeskhare/match.h>
#include <libdeskhare/matches/genericmatch.h>
#include <winqt/shell/iconlocation.h>

#include "winapplicationaction.h"

namespace Deskhare {

using namespace WinQt;

static Q_LOGGING_CATEGORY(logger, "deskhare.win.appindex")

WinApplicationsIndex::WinApplicationsIndex()
: SqliteIndex(QLatin1String("winApplications"), 0)
{ }

WinApplicationsIndex::~WinApplicationsIndex() = default;


QDateTime WinApplicationsIndex::getLastIndexing()
{
  QSqlQuery query(dataBase());
  query.prepare(
    QLatin1String("SELECT time FROM times WHERE id = 'last_indexing'"));
  if (!query.exec())
  {
    qCCritical(logger) << "Failed to read last indexing of app index:"
      << query.lastError();
    return QDateTime();
  }
  if (!query.next())
    return QDateTime();

  return QDateTime::fromMSecsSinceEpoch(query.value(0).toLongLong() * 1000);
}

void WinApplicationsIndex::setLastIndexing(const QDateTime& time)
{
  QSqlQuery query(dataBase());
  query.prepare(
    QLatin1String("UPDATE times SET time=? WHERE id = 'last_indexing'"));
  query.bindValue(0, time.toMSecsSinceEpoch() / 1000);
  if (!query.exec())
  {
    qCCritical(logger) << "Failed to set last indexing of app index:"
      << query.lastError();
  }
}

bool WinApplicationsIndex::create()
{
  auto transaction = createTransactionGuard();

  QSqlQuery create_query(dataBase());
  if (!create_query.exec(QLatin1String(
    "CREATE VIRTUAL TABLE apps USING fts4("
    "  name, description, path, iconLoc, "
    "  notindexed=path, notindexed=description, notindexed=iconLoc"
    ")")))
  {
    qCCritical(logger) << "create app index table failed:"
      << create_query.lastError();
    transaction.rollback();
    return false;
  }

  if (!create_query.exec(QLatin1String(
    "CREATE TABLE times ("
    "  id   TEXT NOT NULL,"
    "  time INTEGER NOT NULL"
    ")")))
  {
    qCCritical(logger) << "create app index times table failed:"
      << create_query.lastError();
    transaction.rollback();
    return false;
  }
  if (!create_query.exec(QLatin1String(
    "INSERT INTO times VALUES ('last_indexing', 0)")))
  {
    qCCritical(logger)
      << "inserting last indexing in app index table failed:"
      << create_query.lastError();
    transaction.rollback();
    return false;
  }

  return true;
}

void WinApplicationsIndex::addApplication(const QString& name,
  const QString& description, const QString& path,
  const WinQt::IconLocation& iconLoc)
{
  QSqlQuery query(dataBase());
  query.prepare(QLatin1String(
    "INSERT INTO apps(name, description, path, iconLoc) VALUES (?, ?, ?, ?)"));
  query.bindValue(0, name);
  query.bindValue(1, description);
  query.bindValue(2, path);
  query.bindValue(3, iconLoc.toString());
  if (!query.exec())
  {
    qCCritical(logger) << "insert app in index table failed:"
      << query.lastError();
  }
}

std::vector<std::shared_ptr<Match> > WinApplicationsIndex::search(
  const QString& query)
{
  std::vector<std::shared_ptr<Match>> matches;

  QSqlQuery sqlquery(dataBase());
  sqlquery.prepare(QLatin1String(
    "SELECT name, description, path, iconLoc FROM apps WHERE name MATCH ?"));
  sqlquery.bindValue(0, QString(query + '*'));  // TODO: escape query
  if (!sqlquery.exec())
  {
    qCCritical(logger) << "search app in index failed:"
      << sqlquery.lastError();
    return matches;
  }

  while (sqlquery.next())
  {
    qCDebug(logger)
      << sqlquery.value(3).toString()
      << IconLocation::fromString(sqlquery.value(3).toString()).toString();

    auto name = sqlquery.value(0).toString();
    matches.emplace_back(std::make_shared<GenericMatch>(
      name,
      sqlquery.value(1).toString(),
      // TODO: use cache (QPixmapCache)
      IconLocation::fromString(sqlquery.value(3).toString()).resolve(),
      "xdg-app://" + name,
      MatchScore::Excellent,
      std::make_shared<WinApplicationAction>(sqlquery.value(2).toString(), 0)
    ));
  }

  return matches;
}

bool WinApplicationsIndex::upgrade(int currentFormatVersion)
{
  return false;
}

void WinApplicationsIndex::clear()
{
  QSqlQuery clearQuery(QLatin1String("DELETE FROM apps"), dataBase());
  if (!clearQuery.last())
  {
    qCCritical(logger) << "Cannot clear apps table:" <<
      clearQuery.lastError().text();
  }
}

} // namespace Deskhare
