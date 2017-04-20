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

#include "plugin.h"

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
#include <libdeskhare/resultset.h>

#include "fileindex.h"

Q_LOGGING_CATEGORY(fileIndexer, "deskhare.fileindexer")

namespace Deskhare {

class FileIndexSource: public Source
{
public:
  FileIndexSource(const PluginContext& ctx);
  ~FileIndexSource();

  bool canHandleQuery(const Query& query) override;
  void search(const Query& query, ResultSet& results) override;

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
    index();
}

FileIndexSource::~FileIndexSource() = default;

bool FileIndexSource::canHandleQuery(const Query& query)
{
  return db_->isOpen() && !query.getSearchString().isEmpty()
    && query.hasCategory(Query::Categories::File);
}


void FileIndexSource::search(const Query& query, ResultSet& results)
{
  Q_ASSERT(!query.getSearchString().isEmpty());
  Q_ASSERT(db_->isOpen());

  auto matches = db_->search(query.getSearchString(), *context_.getFileIconProvider());
  results.sendMatches(matches);
}

void FileIndexSource::index()
{
  qCInfo(fileIndexer) << "Indexing files ...";

  if (!db_->isOpen())
    return;

  db_->clear();

  int i = 0;
  auto videos = QStandardPaths::standardLocations(
    QStandardPaths::MoviesLocation); // TODO: editable list
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

  db_->setLastIndexing(QDateTime::currentDateTime());

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
