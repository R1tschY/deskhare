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

#include "../applications/startmenusource.h"

#include <shlobj.h>
#include <windows.h>
#include <exception>
#include <QDebug>
#include <QDirIterator>
#include <QSqlError>
#include <QSqlQuery>
#include "../matches/applicationmatch.h"

#include "../../../libdeskhare/matches/applicationmatch.h"
#include "../../../libdeskhare/platforms/windows/shell/shelllink.h"
#include "../../../libdeskhare/platforms/windows/shell/utils.h"
#include "../../../libdeskhare/query.h"

#ifdef SQLITECPP_ENABLE_ASSERT_HANDLER
namespace SQLite
{
// definition of the assertion handler enabled when SQLITECPP_ENABLE_ASSERT_HANDLER is defined in the project (CMakeList.txt)
void assertion_failed(const char* apFile, const long apLine, const char* apFunc, const char* apExpr, const char* apMsg)
{
    // Print a message to the standard error output stream, and abort the program.
    qDebug() << apFile << ":" << apLine << ":" << " error: assertion failed (" << apExpr << ") in " << apFunc << "() with message \"" << apMsg << "\"\n";
    std::abort();
}
}
#endif

namespace Deskhare {

enum StartMenuDatabaseColumns
{
  SMDBC_Name,
  SMDBC_Description,
  SMDBC_Path,
  SMDBC_WorkingDir,
  SMDBC_Origin,
  SMDBC_Arguments,
  SMDBC_ShowCmd,
  SMDBC_IconPath,
  SMDBC_IconId,
};

StartMenuSource::StartMenuSource()
{
  db_ = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), QStringLiteral("StartMenuSource"));
  if (!db_.isValid())
    throw std::runtime_error("QSQLITE database driver is invalid.");

  db_.setDatabaseName("StartMenuCache.db");
  if (!db_.open())
  {
    qDebug() << "Cannot open start menu cache database:" << db_.lastError();
    throw std::runtime_error("Cannot open start menu cache database.");
  }

  update();
}

bool StartMenuSource::canHandleQuery(const Query& query)
{
  return query.hasCategory(Query::Category::App); // TODO: db entries > 0
}

std::vector<MatchResult> StartMenuSource::search(const Query& query)
{
  if (query.getSearchString().isEmpty())
    return std::vector<MatchResult>(); // TODO recent things

  std::vector<MatchResult> result;

  QSqlQuery q(db_);
  q.prepare(QStringLiteral("SELECT * FROM apps WHERE name LIKE ? || '%'"));
  q.addBindValue(query.getSearchString());
  q.exec();
  while (q.next())
  {
    result.emplace_back(
      std::unique_ptr<ApplicationMatch>(new ApplicationMatch(
        q.value(SMDBC_Name).toString(),
        q.value(SMDBC_Description).toString(),
        q.value(SMDBC_Path).toString(),
        q.value(SMDBC_Arguments).toString(),
        q.value(SMDBC_WorkingDir).toString()
      )),
      MatchScore::Excellent);
  }

  return result;
}

void searchDir(
    const QString& path,
    QSqlQuery& q,
    const QStringList& name_filters,
    Windows::ShellLink& link)
{
  QDirIterator it(
        path,
        name_filters,
        QDir::Files | QDir::Readable,
        QDirIterator::Subdirectories);
  while (it.hasNext())
  {
      link.open(it.next());
      QString description;
      QString path;
      try {
        link.getPath(path);
        link.getDescription(description);
        qDebug() << QStringLiteral("Entry: ")
               << it.fileInfo().baseName()
               << description
               << path;
      }
      catch (...)
      {
        qDebug() << QStringLiteral("Entry: ")
               << it.fileName()
               << QStringLiteral("(ERROR)");
      }

      q.bindValue(QStringLiteral(":name"), it.fileInfo().baseName());
      q.bindValue(QStringLiteral(":description"), description);
      q.bindValue(QStringLiteral(":path"), path);
      q.bindValue(QStringLiteral(":origin"), it.filePath());
      q.exec();
  }
}

void StartMenuSource::update()
{
  QSqlQuery drop_query(QStringLiteral("DROP TABLE IF EXISTS apps"), db_);
  if (!drop_query.exec())
  {
    qDebug() << "drop table failed:" << drop_query.lastError().text();
    throw std::runtime_error("drop table failed");
  }

  QSqlQuery create_query(db_);
  bool success = create_query.prepare(QStringLiteral(
    "CREATE TABLE apps ("
    "  name        TEXT NOT NULL,"
    "  description TEXT,"
    "  path        TEXT NOT NULL,"
    "  working_dir TEXT,"
    "  origin      TEXT NOT NULL,"
    "  arguments   TEXT,"
    "  show_cmd    INT,"
    "  icon_path   TEXT,"
    "  icon_id     INT" // TODO: icon BLOB
    ")"));
  if (!success)
  {
    qDebug() << "create table failed:" << create_query.lastError();
    throw std::runtime_error("create table failed");
  }

  if (!create_query.exec())
  {
    qDebug() << "create table failed:" << create_query.lastError();
    throw std::runtime_error("create table failed");
  }

  QStringList name_filters;
  name_filters.append(QStringLiteral("*.lnk"));

  Windows::ShellLink link;

  QSqlQuery q(db_);
  q.prepare(QStringLiteral(
    "INSERT INTO apps (name, description, path, origin) VALUES ("
    "  :name, :description, :path, :origin"
    ")"));

  searchDir(Windows::getKnownFolderPath(FOLDERID_Programs), q, name_filters, link);
  searchDir(Windows::getKnownFolderPath(FOLDERID_CommonPrograms), q, name_filters, link);
}

} // namespace QuickStarter
