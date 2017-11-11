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

#include "sqliteindex.h"

#include <QDebug>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDriver>
#include <QFile>
#include <QStandardPaths>
#include <QFileInfo>
#include <QDir>
#include <QLoggingCategory>

namespace Deskhare {

static Q_LOGGING_CATEGORY(logger, "deskhare.SqliteIndex")

SqliteIndex::SqliteIndex(const QString& indexName, int formatVersion)
: formatVersion_(formatVersion)
{
  db_ = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), indexName);
  if (!db_.isValid())
    qCCritical(logger) << "QSQLITE database driver is invalid.";
}

bool SqliteIndex::open(const QString& filePath)
{
  QFileInfo filePathInfo(filePath);
  QDir dirInfo(filePathInfo.path());
  if (!dirInfo.exists())
  {
    bool success = dirInfo.mkpath(".");
    if (!success)
    {
      qCCritical(logger) << "Cannot create directory for index database"
        << db_.connectionName() << ":" << filePathInfo.path();
      return recreate(true);
    }
  }

  db_.setDatabaseName(filePath);
  if (!db_.open())
  {
    qCCritical(logger) << "Cannot open index database" << db_.connectionName()
      << ":" << db_.lastError().text();
    return recreate(true);
  }

  if (isEmpty())
  {
    return doCreate();
  }

  int curVersion = getCurrentFormatVersion();
  if (curVersion < 0)
    return recreate(true);

  if (curVersion > formatVersion_)
  {
    qCCritical(logger) << "Recreate index database" << db_.connectionName()
      << "because database version" << curVersion
      << "is bigger than current version";
    return recreate();
  }

  if (curVersion < formatVersion_)
  {
    // upgrade needed
    if (!doUpgrade(curVersion))
      return recreate();
  }

  return true;
}

int SqliteIndex::getCurrentFormatVersion()
{
  // get format version of table
  QSqlQuery version_query(db_);
  if (!version_query.exec(
    QStringLiteral("SELECT * FROM versions WHERE id == 'format'")))
  {
    qCCritical(logger) << "Getting current database format version failed:"
      << version_query.lastError().text();
    return -1;
  }
  if (!version_query.first())
  {
    qCCritical(logger) << "Getting current database format version failed:"
      << version_query.lastError().text();
    return -1;
  }
  return version_query.value("version").toInt();
}

bool SqliteIndex::setCurrentFormatVersion()
{
  auto transaction = createTransaction();

  if (isEmpty())
  {
    QSqlQuery create_query(db_);
    if (!create_query.exec(QStringLiteral("CREATE TABLE versions ("
      "  id   TEXT NOT NULL,"
      "  version INTEGER NOT NULL"
      ")")))
    {
      qCCritical(logger) << "Create database version table failed:"
        << create_query.lastError().text();
      return false;
    }

    QSqlQuery version_query(db_);
    version_query.prepare(
      QStringLiteral("INSERT INTO versions VALUES ('format', :version)"));
    version_query.bindValue(":version", formatVersion_);
    if (!version_query.exec())
    {
      qCCritical(logger) << "Setting database format version failed:"
        << version_query.lastError().text();
      return false;
    }
    return true;
  }

  QSqlQuery version_query(db_);
  version_query.prepare(QStringLiteral("UPDATE versions SET version=:version WHERE id == 'format'"));
  version_query.bindValue(":version", formatVersion_);
  if (!version_query.exec())
  {
    qCCritical(logger) << "Setting database format version failed:"
      << version_query.lastError().text();
    return false;
  }
  return true;
}

bool SqliteIndex::recreate(bool useFallback)
{
  if (db_.isOpen())
    db_.close();

  if (db_.databaseName().isEmpty() || db_.databaseName() == ":memory:")
  {
    qCCritical(logger) << "Giving up to open index" << db_.connectionName() <<
      "because of too many errors.";
    return false;
  }

  if (useFallback)
  {
    qCWarning(logger) << "Using in-memory database for index"
      << db_.connectionName() << "because of too many errors.";
    db_.setDatabaseName(":memory:");
  }

  if (!db_.databaseName().isEmpty() && db_.databaseName() != ":memory:")
  {
    QFile dbfile(db_.databaseName());
    if (dbfile.exists())
    {
      dbfile.remove();
    }
    // TODO: create parent folders
  }

  return open(db_.databaseName());
}

bool SqliteIndex::doUpgrade(int currentFormatVersion)
{
  qCInfo(logger) << "Upgrading index database" << db_.connectionName()
    << "from" << currentFormatVersion << "to" << formatVersion_;

  if (!upgrade(currentFormatVersion))
  {
    qCWarning(logger) << "Upgrade of index database" << db_.connectionName()
      << "failed";
    return false;
  }

  return setCurrentFormatVersion();
}

bool SqliteIndex::doCreate()
{
  qCInfo(logger) << "Creating index database" << db_.connectionName();

  if (!create())
  {
    qCWarning(logger) << "Creating of index database" << db_.connectionName()
      << "failed";
    return recreate(true);
  }

  return setCurrentFormatVersion();
}

QString SqliteIndex::createIndexPath(const QString& fileName)
{
  auto dir = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
  return dir + "/" + fileName;
}

void SqliteIndex::printDbDriverFeatures()
{
  auto* driver = db_.driver();

  std::initializer_list<std::pair<QSqlDriver::DriverFeature, const char*>> features = {
    { QSqlDriver::Transactions, "QSqlDriver::Transactions" },
    { QSqlDriver::QuerySize, "QSqlDriver::QuerySize" },
    { QSqlDriver::BLOB, "QSqlDriver::BLOB" },
    { QSqlDriver::Unicode, "QSqlDriver::Unicode" },
    { QSqlDriver::PreparedQueries, "QSqlDriver::PreparedQueries" },
    { QSqlDriver::NamedPlaceholders, "QSqlDriver::NamedPlaceholders" },
    { QSqlDriver::PositionalPlaceholders, "QSqlDriver::PositionalPlaceholders" },
    { QSqlDriver::LastInsertId, "QSqlDriver::LastInsertId" },
    { QSqlDriver::BatchOperations, "QSqlDriver::BatchOperations" },
    { QSqlDriver::SimpleLocking, "QSqlDriver::SimpleLocking" },
    { QSqlDriver::LowPrecisionNumbers, "QSqlDriver::LowPrecisionNumbers" },
    { QSqlDriver::EventNotifications, "QSqlDriver::EventNotifications" },
    { QSqlDriver::FinishQuery, "QSqlDriver::FinishQuery" },
    { QSqlDriver::MultipleResultSets, "QSqlDriver::MultipleResultSets" },
    { QSqlDriver::CancelQuery, "QSqlDriver::CancelQuery" },
  };

  QSqlQuery query(db_);
  if (query.exec(QStringLiteral("select sqlite_version()")))
  {
    if (query.first())
      qCDebug(logger) << "Sqlite version =" << query.value(0).toString();
    else
      qCDebug(logger) << "Sqlite version = <failure:"
        << query.lastError() << ">";
  }

  for (auto& feature : features)
  {
    qCDebug(logger)
      << feature.second << "=" << driver->hasFeature(feature.first);
  }
}

bool SqliteIndex::upgrade(int currentFormatVersion)
{
  return false;
}

bool SqliteIndex::isEmpty() const
{
  QSqlQuery version_query(db_);

  if (!version_query.exec(
    QStringLiteral("SELECT name FROM sqlite_master WHERE type='table' AND name='versions'")))
  {
    qCCritical(logger)
      << "Internal error: cannot read table informations in index "
      << db_.connectionName() << ":" << version_query.lastError().text();
    throw std::runtime_error("Internal error: cannot read table informations");
  }

  return !version_query.first();
}

QString SqliteIndex::escape(QChar c)
{
  switch(c.toLatin1())
  {
  case '%':
    return QStringLiteral("\\%");

  case '\'':
    return QStringLiteral("''");
  }
  return c;
}

QString SqliteIndex::escapeLike(QChar c, QChar escapeChar)
{
  switch(c.toLatin1())
  {
  case '%':
    return QString(escapeChar) + '%';

  case '_':
    return QString(escapeChar) + '_';

  case '\'':
    return QStringLiteral("''");
  }
  return c;
}


} // namespace Deskhare
