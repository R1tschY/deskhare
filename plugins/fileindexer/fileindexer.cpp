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

#include "fileindexer.h"

#include <QDebug>
#include <QStandardPaths>
#include <QDirIterator>
#include <QSqlError>
#include <QSqlQuery>
#include <QLoggingCategory>
#include <QFile>
#include <QDateTime>
#include <cpp-utils/algorithm/container.h>

#include <libdeskhare/filesystem/diriterator.h>
#include <libdeskhare/indexes/sqliteindex.h>
#include <libdeskhare/match.h>
#include <libdeskhare/matches/filematch.h>
#include <libdeskhare/query.h>
#include <libdeskhare/source.h>

Q_LOGGING_CATEGORY(fileIndexer, "deskhare.fileindexer")

namespace Deskhare {

class FileIndex : public SqliteIndex
{
public:
  FileIndex()
  : SqliteIndex("fileIndex", 1)
  { }

  bool addFile(QFileInfo file)
  {
    Q_ASSERT(file.isAbsolute());

    QSqlQuery query(dataBase());
    query.prepare(
      QStringLiteral("INSERT INTO files (name, path) VALUES (:name, :path)"));
    query.bindValue(":name", file.fileName());
    query.bindValue(":path", file.path());
    if (!query.exec())
    {
      qCCritical(fileIndexer) << "insert file in index table failed:"
        << query.lastError();
      return false;
    }

    return true;
  }

  std::vector<std::unique_ptr<LocalFileMatch>> search(const QString& query,
    const FileIconProvider& icon_provider)
  {
    std::vector<std::unique_ptr<LocalFileMatch>> matches;

    QString search_query_string;
    search_query_string =
      QStringLiteral("'%'")
      + QStringLiteral("|| ? || '%'").repeated(query.length());

    QSqlQuery sqlquery(dataBase());
    sqlquery.prepare(QStringLiteral(
      "SELECT * FROM files WHERE name LIKE ") + search_query_string);
    for (QChar c : query)
      sqlquery.addBindValue(c);
    if (!sqlquery.exec())
    {
      qCCritical(fileIndexer) << "search file in index failed:"
        << sqlquery.lastError();
      return matches;
    }

    while (sqlquery.next())
    {
      matches.emplace_back(std::make_unique<LocalFileMatch>(
        sqlquery.value("path").toString() + "/" + sqlquery.value("name").toString(),
        icon_provider
      ));
    }

    return matches;
  }

  QDateTime getLastIndexing()
  {
    QSqlQuery query(dataBase());
    query.prepare(
      QStringLiteral("SELECT time FROM times WHERE id = 'last_indexing'"));
    if (!query.exec())
    {
      qCCritical(fileIndexer) << "Failed to read last indexing of file index:"
        << query.lastError();
      return QDateTime();
    }
    if (!query.next())
      return QDateTime();

    return QDateTime::fromMSecsSinceEpoch(query.value(0).toLongLong() * 1000);
  }

  void setLastIndexing(QDateTime time)
  {
    QSqlQuery query(dataBase());
    query.prepare(
      QStringLiteral("UPDATE times SET time=:time WHERE id = 'last_indexing'"));
    query.bindValue(":time", time.toMSecsSinceEpoch() / 1000);
    if (!query.exec())
    {
      qCCritical(fileIndexer) << "Failed to set last indexing of file index:"
        << query.lastError();
    }
  }

private:
  bool create()
  {
    QSqlQuery create_query(dataBase());
    if (!create_query.exec(QStringLiteral(
      "CREATE TABLE files ("
      "  name        TEXT NOT NULL,"
      "  path        TEXT NOT NULL"
      ")")))
    {
      qCCritical(fileIndexer) << "create file index table failed:"
        << create_query.lastError();
      return false;
    }
    if (!create_query.exec(QStringLiteral(
      "CREATE TABLE times ("
      "  id   TEXT NOT NULL,"
      "  time INTEGER NOT NULL"
      ")")))
    {
      qCCritical(fileIndexer) << "create file index table failed:"
        << create_query.lastError();
      return false;
    }
    if (!create_query.exec(QStringLiteral(
      "INSERT INTO times VALUES ('last_indexing', 0)")))
    {
      qCCritical(fileIndexer) << "create file index table failed:"
        << create_query.lastError();
      return false;
    }

    return true;
  }

  bool upgrade(int currentFormatVersion)
  {
    return false;
  }
};

class FileIndexSource: public Source
{
public:
  FileIndexSource(const PluginContext& ctx);
  ~FileIndexSource();

  bool canHandleQuery(const Query& query) override;
  MatchResults search(const Query& query) override;

private:
  std::unique_ptr<FileIndex> db_;
  PluginContext context_;

  void index();
};

FileIndexSource::FileIndexSource(const PluginContext& ctx)
: db_(std::make_unique<FileIndex>())
{
  context_ = ctx;

  if (!db_->open("fileindex.db"))
    return;

  if (db_->getLastIndexing().daysTo(QDateTime::currentDateTime()) > 7)
  {
    index();
    db_->setLastIndexing(QDateTime::currentDateTime());
  }
}

FileIndexSource::~FileIndexSource() = default;

bool FileIndexSource::canHandleQuery(const Query& query)
{
  return db_->isOpen() && !query.getSearchString().isEmpty()
    && query.hasCategories(Query::Categories::File);
}


MatchResults FileIndexSource::search(const Query& query)
{
  Q_ASSERT(!query.getSearchString().isEmpty());
  Q_ASSERT(db_->isOpen());

  auto results = db_->search(query.getSearchString(),
    *context_.getFileIconProvider());
  return MatchResults(
    std::make_move_iterator(results.begin()),
    std::make_move_iterator(results.end()));
}

void FileIndexSource::index()
{
  if (!db_->isOpen())
    return;

  qCInfo(fileIndexer) << "Indexing files ...";

  int i = 0;
  auto videos = QStandardPaths::standardLocations(
    QStandardPaths::MoviesLocation);
  foreach (QString dir, videos)
  {
    cpp::for_each(DirRange(dir,
      QDir::Files | QDir::Readable,
      QDirIterator::Subdirectories),
    [&](const QDirIterator& file)
    {
      i += 1;
      db_->addFile(file.fileInfo());
    });
  }

  qCInfo(fileIndexer) << "Indexed" << i << "files";
}

// FileIndexerPlugin

std::unique_ptr<Source> FileIndexerPlugin::getSource(const PluginContext& ctx)
{
  return std::make_unique<FileIndexSource>(ctx);
}

QString FileIndexerPlugin::getSourceDescription()
{
  return QString(
    "File indexer plugin.\n"
    "\n"
    "Index files in local folders."
  );
}

} // namespace Deskhare
